/**
 * Copyright (C) 2017 Ola Benderius
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <cmath>
#include <cstdio>

#include <iostream>

#include <opendavinci/odcore/base/Thread.h>
#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>

#include <opendlv/data/environment/Line.h>
#include <opendlv/data/environment/Obstacle.h>
#include <opendlv/data/environment/Polygon.h>

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>
#include <odvdopendlvstandardmessageset/GeneratedHeaders_ODVDOpenDLVStandardMessageSet.h>
#include <odvdvehicle/generated/opendlv/proxy/ActuationRequest.h>

#include "vehicle.hpp"
#include "vehiclefollower.hpp"

namespace opendlv {
namespace logic {
namespace legacy {

VehicleFollower::VehicleFollower(int32_t const &a_argc, char **a_argv)
  : TimeTriggeredConferenceClientModule(a_argc, a_argv, 
      "logic-legacy-vehiclefollower"),
  m_referenceMutex(),
  m_egoVehicle(),
  m_targetVehicle(),
  m_steeringWheelAngle()
{
}

VehicleFollower::~VehicleFollower()
{
}

void VehicleFollower::setUp()
{
  double const latitude = getKeyValueConfiguration().getValue<double>(
      "global.reference.WGS84.latitude");
  double const longitude = getKeyValueConfiguration().getValue<double>(
      "global.reference.WGS84.longitude");
  opendlv::data::environment::WGS84Coordinate m_wgs84Reference(latitude, 
      longitude);
  m_egoVehicle = std::unique_ptr<Vehicle>(new Vehicle(m_wgs84Reference));
  m_targetVehicle = std::unique_ptr<Vehicle>(new Vehicle(m_wgs84Reference));
}

void VehicleFollower::tearDown()
{
}
    
void VehicleFollower::nextContainer(odcore::data::Container &a_container) {
  if (a_container.getDataType() == opendlv::data::environment::WGS84Coordinate::ID()) {

    uint32_t const egoVehicleId = getKeyValueConfiguration().getValue<uint32_t>(
        "logic-legacy-vehiclefollower.ego-vehicle-id");
    uint32_t const targetVehicleId = getKeyValueConfiguration().getValue<uint32_t>(
        "logic-legacy-vehiclefollower.target-vehicle-id");
    
    auto wgs84Position = a_container.getData<opendlv::data::environment::WGS84Coordinate>();
    
    if (a_container.getSenderStamp() == egoVehicleId) {
     // double distanceFromReferenceLimit = 
     //   getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.distance_from_reference_limit");

      m_egoVehicle->updatePosition(wgs84Position);
      //double distanceFromReference = m_egoVehicle->updatePosition(wgs84Position);
    /*  if (distanceFromReference > distanceFromReferenceLimit) {
        odcore::base::Lock l(m_referenceMutex);
        m_egoVehicle->updateReference(wgs84Position);
        m_targetVehicle->updateReference(wgs84Position);

        if (isVerbose()) {
          std::cout << "[" << getName() << "]: " << "Reference updated." << std::endl;
        }
      }*/
    } else if (a_container.getSenderStamp() == targetVehicleId) {
      odcore::base::Lock l(m_referenceMutex);
      m_targetVehicle->updatePosition(wgs84Position);
    } 
  } else if (a_container.getDataType() == opendlv::proxy::GroundSpeedReading::ID()) {
    
    uint32_t const egoVehicleId = getKeyValueConfiguration().getValue<uint32_t>(
        "logic-legacy-vehiclefollower.ego-vehicle-id");
    uint32_t const targetVehicleId = getKeyValueConfiguration().getValue<uint32_t>(
        "logic-legacy-vehiclefollower.target-vehicle-id");

    auto groundSpeed = a_container.getData<opendlv::proxy::GroundSpeedReading>();
    double speed = groundSpeed.getGroundSpeed();

    if (a_container.getSenderStamp() == egoVehicleId) {
      m_egoVehicle->updateSpeed(speed);
    } else if (a_container.getSenderStamp() == targetVehicleId) {
      m_targetVehicle->updateSpeed(speed);
    }
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode VehicleFollower::body()
{

  // Send some zeros first to fix acceleration.
  opendlv::proxy::ActuationRequest initAr;
  initAr.setAcceleration(0.0);
  initAr.setSteering(0.0);
  initAr.setIsValid(true);

  odcore::data::Container initC(initAr);
  getConference().send(initC);

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    if (!m_egoVehicle->isUpdated() || !m_targetVehicle->isUpdated()) {
      if (isVerbose()) {
        if (!m_egoVehicle->isUpdated()) {
          std::cout << "Ego vehicle position and heading is not updated." << std::endl;
        } else if (!m_targetVehicle->isUpdated()) {
          std::cout << "Target vehicle position and heading is not updated." << std::endl;
        }
      }
      continue;
    }

    double const dt = 1.0 / static_cast<double>(getFrequency());

    opendlv::data::environment::EgoState egoState = m_egoVehicle->getEgoState();
    opendlv::data::environment::EgoState targetEgoState = 
      m_targetVehicle->getEgoState();

    // Get our current positions and aim point.
    opendlv::data::environment::Point3 egoPosition = egoState.getPosition();
    opendlv::data::environment::Point3 targetPosition = targetEgoState.getPosition();
    opendlv::data::environment::Point3 aimPoint = targetPosition - egoPosition;
    double const heading = egoState.getRotation().getAngleXY();

    double aimPointAngle = aimPoint.getAngleXY() - heading;
    while (aimPointAngle < -cartesian::Constants::PI) {
      aimPointAngle += 2.0 * cartesian::Constants::PI;
    }
    while (aimPointAngle > cartesian::Constants::PI) {
      aimPointAngle -= 2.0 * cartesian::Constants::PI;
    }

    double const aimPointGain = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.aim_point_gain");
    double const steeringFilterCoefficient = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.steering_filter_coefficient");
    double const steeringWheelAngleUnfiltered = aimPointGain * aimPointAngle;

    double const steeringWheelAngleRate = steeringFilterCoefficient * 
      (steeringWheelAngleUnfiltered - m_steeringWheelAngle);
    m_steeringWheelAngle += steeringWheelAngleRate * dt;


    m_steeringWheelAngle = (m_steeringWheelAngle > 3.0 * cartesian::Constants::PI) ? 9.3 : m_steeringWheelAngle;
    m_steeringWheelAngle = (m_steeringWheelAngle < -3.0 * cartesian::Constants::PI) ? -9.3 : m_steeringWheelAngle;

    if (isVerbose()) {
      std::cout << "[" << getName() << "]: " << "Longitudinal control: Aim point angle = " << aimPointAngle << ", steering wheel angle: " << m_steeringWheelAngle << std::endl;
    }

    {
      // Visualize steering control.
      opendlv::data::environment::Line l(egoPosition, targetPosition);
      opendlv::data::environment::Point3 perpendicularPoint = l.getPerpendicularPoint(targetPosition);

      opendlv::data::environment::Polygon p;
      p.add(egoPosition);
      p.add(targetPosition);
      p.add(perpendicularPoint);

      opendlv::data::environment::Obstacle o(1, opendlv::data::environment::Obstacle::UPDATE);
      o.setPolygon(p);
      odcore::data::Container c = odcore::data::Container(o);
      getConference().send(c);
      
      c = odcore::data::Container(egoState);
      getConference().send(c);
    }

    // Get speeds and TTC.
    double const egoSpeed = m_egoVehicle->getSpeed();
    double const targetSpeed = m_targetVehicle->getSpeed();
    double const distance = aimPoint.lengthXY();
    //double const timeToCollision = distance / (egoSpeed - targetSpeed);

    
    double const desiredDistance = 
      getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.desired_distance");
    double const desiredDistanceCorrectionTime = 
      getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.desired_distance_correction_time");
    double const distanceError = distance - desiredDistance;


    double const desiredDistanceCorrectionSpeed = targetSpeed + 
      distanceError / desiredDistanceCorrectionTime;

    
  //  double const maxSpeed = 
  //    getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.maximum_speed");
  //  double const desiredSpeed = (desiredDistanceCorrectionSpeed < 0.0) ? 0.0 :
  //    (desiredDistanceCorrectionSpeed > maxSpeed) ? maxSpeed : desiredDistanceCorrectionSpeed;

    double speedError = desiredDistanceCorrectionSpeed - egoSpeed;

    double accelerationRequest;
    if (speedError > 0.0) {
      double const pidK = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.acceleration_k");
      double const accelerationLimit = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.acceleration_limit");

      accelerationRequest = pidK * speedError;
      accelerationRequest = (accelerationRequest > accelerationLimit) ? accelerationLimit : accelerationRequest;
    } else {
      double const pidK = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.deceleration_k");
      double const decelerationLimit = getKeyValueConfiguration().getValue<double>("logic-legacy-vehiclefollower.deceleration_limit");

      accelerationRequest = pidK * speedError;
      accelerationRequest = (accelerationRequest < decelerationLimit) ? decelerationLimit : accelerationRequest;
    }

    if (isVerbose()) {
      std::cout << "[" << getName() << "]: " << "Longitudinal control: current speed = " << egoSpeed << ", speed error = " << speedError << ", distance error = " << distanceError << ", acceleration request = " << accelerationRequest << std::endl;
    }

    // Create vehicle control data.
    opendlv::proxy::ActuationRequest ar;
    ar.setAcceleration(static_cast<float>(accelerationRequest));
    ar.setSteering(static_cast<float>(m_steeringWheelAngle));
    ar.setIsValid(true);

    odcore::data::Container c = odcore::data::Container(ar);
    getConference().send(c);
  }

  // Stop vehicle.
  opendlv::proxy::ActuationRequest ar;
  ar.setAcceleration(-2.0);
  ar.setSteering(0.0);
  ar.setIsValid(true);

  odcore::data::Container c(ar);
  getConference().send(c);

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

}
}
}
