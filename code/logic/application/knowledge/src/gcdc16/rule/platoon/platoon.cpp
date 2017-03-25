/**
 * Copyright (C) 2015 Chalmers REVERE
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opendavinci/odcore/strings/StringToolbox.h"

#include "gcdc16/rule/platoon/platoon.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace platoon {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
Platoon::Platoon(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(
  a_argc, a_argv, "knowledge-gcdc16-platoon"),
  m_steerTowardsMio(false),
  m_desiredGroundSpeed(),
  m_desiredGroundSpeedMax(),
  m_previousAzimuthFollowed(),
  m_lateralDiscardDistance(300.0f),
  m_longitudinalDiscardDistance(50.0f),
  m_steerTowardsMioThreshold(0.1f),
  m_timeToCollision(10.0f)
{
}

Platoon::~Platoon()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Platoon::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  } 
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
   
}

void Platoon::ActOnEnvironment(opendlv::perception::Environment &a_environment)
{
  m_steerTowardsMio = false;

  odcore::data::TimeStamp now;
  odcore::data::TimeStamp environmentValidUntil = a_environment.getValidUntil();
  if (now.toMicroseconds() - environmentValidUntil.toMicroseconds() > 0) {
    return;
  }

  auto listOfObjects = a_environment.getListOfObjects();
  if (listOfObjects.size() > 0) {
    ActOnMio(listOfObjects);
  }
 
  if (!m_steerTowardsMio) { 
    auto listOfSurfaces = a_environment.getListOfSurfaces();
    if (listOfSurfaces.size() > 0) {
      ActOnLane(listOfSurfaces);
    }
  }
}

void Platoon::ActOnMio(std::vector<opendlv::perception::Object> &a_listOfObjects)
{
  for (auto object : a_listOfObjects) {
    for (auto property : object.getListOfProperties()) {
      if (property == "Station Id: 1") {
        auto direction = object.getDirection();
        float azimuth = direction.getAzimuth();
        float distance = object.getDistance();
        
        std::cout << "The MIO was found in azimuth " << azimuth << " at a distance of " << distance << " m." << std::endl;

        float longitudinal = cos(azimuth) * distance;
        float lateral = sin(azimuth) * distance;

        std::cout << "  - Longitudinal: " << longitudinal << std::endl;
        std::cout << "  - Lateral: " << lateral << std::endl;

        if (std::abs(longitudinal) > m_longitudinalDiscardDistance || std::abs(lateral) > m_lateralDiscardDistance) {
          return;
        }

        float speed_new = std::min(m_desiredGroundSpeedMax, longitudinal / m_timeToCollision);
        std::cout << "MIO is leading. New speed: " << speed_new << std::endl;

        if (std::abs(azimuth) < m_steerTowardsMioThreshold) {
          m_steerTowardsMio = true;
          std::cout << "Steer towards MIO." << std::endl;
    
          m_previousAzimuthFollowed = azimuth;
          ControlDirectionOfMovement(azimuth);
        }
 
        if (speed_new < 0.0f) {
          speed_new = 0.0f;
        }
       
        m_desiredGroundSpeed = speed_new;
      }
    }
  }
}

void Platoon::ActOnLane(std::vector<opendlv::perception::Surface> &a_listOfSurfaces)
{
  float azimuth_pre = m_previousAzimuthFollowed;
  float azimuth_new = std::numeric_limits<float>::max();

 // int32_t i = 0;
  for (auto surface : a_listOfSurfaces) {
    auto listOfEdges = surface.getListOfEdges();

    if (listOfEdges.size() == 4) {
      opendlv::model::Cartesian3 topLeftCorner = listOfEdges[0];
      opendlv::model::Cartesian3 topRightCorner = listOfEdges[3];

      float x_m = 0.5f * (topLeftCorner.getX() + topRightCorner.getX());
      float y_m = 0.5f * (topLeftCorner.getY() + topRightCorner.getY());

      float azimuth = atan2(y_m, x_m); 

    //  std::cout << "Azimuth of surface #" << i << ": " << azimuth << std::endl; 
    //  i++;

      if (std::abs(azimuth_pre - azimuth) < std::abs(azimuth_pre - azimuth_new)) {
        azimuth_new = azimuth;
      }
    }
  }

  if (azimuth_new < std::numeric_limits<float>::max()) {
    std::cout << "Steer towards lane " << azimuth_new << std::endl;
    m_previousAzimuthFollowed = azimuth_new;
    ControlDirectionOfMovement(azimuth_new);
  }
}

void Platoon::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    opendlv::perception::Environment message = a_container.getData<opendlv::perception::Environment>();
    ActOnEnvironment(message);
  } else if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto vehicleSpeed = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = vehicleSpeed.getPropulsionShaftVehicleSpeed();
    float speed = static_cast<float>(speedKph / 3.6);

    ControlGroundSpeed(speed);
  }
}

void Platoon::ControlGroundSpeed(float a_speed)
{
  odcore::data::TimeStamp now;
  opendlv::perception::StimulusGroundSpeed sgs(now, m_desiredGroundSpeed, a_speed);
  odcore::data::Container container(sgs);
  getConference().send(container);
}

void Platoon::ControlDirectionOfMovement(float a_azimuth)
{
  odcore::data::TimeStamp now;
  opendlv::model::Direction direction(a_azimuth, 0.0f);
  opendlv::perception::StimulusDirectionOfMovement sdom(now, direction, opendlv::model::Direction(0.0f, 0.0f));
  odcore::data::Container container(sdom);
  getConference().send(container);
}

void Platoon::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_desiredGroundSpeedMax = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-platoon.desiredGroundSpeed"));
  m_lateralDiscardDistance = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-platoon.lateralDiscardDistance"));
  m_longitudinalDiscardDistance = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-platoon.longitudinalDiscardDistance"));
  m_timeToCollision = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-platoon.timeToCollision"));
  m_steerTowardsMioThreshold = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-platoon.steerTowardsMioThreshold"));
}

void Platoon::tearDown()
{
}

} // platoon
} // rule
} // gcdc16
} // knowledge
} // opendlv
