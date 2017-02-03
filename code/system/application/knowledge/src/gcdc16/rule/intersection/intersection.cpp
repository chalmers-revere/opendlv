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
#include <limits>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include <odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h>

#include "opendavinci/odcore/strings/StringToolbox.h"

#include "gcdc16/rule/intersection/intersection.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace intersection {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
Intersection::Intersection(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(
  a_argc, a_argv, "knowledge-gcdc16-rule-intersection"),
  m_scenarioType(""),
  m_enableLaneFollowing(),
  m_runScenario(false),
  m_crossingSafeAngle(0.2f),
  m_desiredGroundSpeed(),
  m_desiredGroundSpeedMax(),
  m_previousAzimuthFollowed(),
  m_lateralDiscardDistance(300.0f),
  m_longitudinalDiscardDistance(50.0f),
  m_timeToCollision(10.0f)
{
}

Intersection::~Intersection()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Intersection::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  } 
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Intersection::ActOnEnvironment(opendlv::perception::Environment &a_environment)
{
  odcore::data::TimeStamp now;
  odcore::data::TimeStamp environmentValidUntil = a_environment.getValidUntil();
  if (now.toMicroseconds() - environmentValidUntil.toMicroseconds() > 0) {
    return;
  }

  auto listOfObjects = a_environment.getListOfObjects();
  if (listOfObjects.size() > 0) {
    ActOnMio(listOfObjects);
  }
  
  auto listOfSurfaces = a_environment.getListOfSurfaces();
  if (listOfSurfaces.size() > 0) {
    ActOnLane(listOfSurfaces);
  }
}

void Intersection::ActOnMio(std::vector<opendlv::perception::Object> &a_listOfObjects)
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

        float speed_new;
        
        if (m_scenarioType == "midLeftTurnCrossingPath") {

          if (longitudinal > 0.0f && azimuth < m_crossingSafeAngle) {
            speed_new = std::min(m_desiredGroundSpeedMax, longitudinal / m_timeToCollision);
            std::cout << "MIO in front and is CROSSING! New speed: " << speed_new << std::endl;
          } else {
            speed_new = m_desiredGroundSpeedMax;
            std::cout << "MIO is in safe place. New speed: " << speed_new << std::endl;
          }

        } else if (m_scenarioType == "mioLeftTurnToLead") {
            speed_new = std::min(m_desiredGroundSpeedMax, longitudinal / m_timeToCollision);
            std::cout << "MIO is leading. New speed: " << speed_new << std::endl;
        }
 
        if (speed_new < 0.0f) {
          speed_new = 0.0f;
        }
       
        m_desiredGroundSpeed = speed_new;
      }
    }
  }
}

void Intersection::ActOnLane(std::vector<opendlv::perception::Surface> &a_listOfSurfaces)
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
    m_previousAzimuthFollowed = azimuth_new;
    ControlDirectionOfMovement(azimuth_new);
  }
}

void Intersection::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID())) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    if (insight.getInsight() == "scenarioReady") {
      m_runScenario = true;
    }
  } else if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    opendlv::perception::Environment message = a_container.getData<opendlv::perception::Environment>();
    ActOnEnvironment(message);
  } else if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    float speed = static_cast<float>(speedKph / 3.6);

    ControlGroundSpeed(speed);
  }
}

void Intersection::ControlGroundSpeed(float a_speed)
{
  odcore::data::TimeStamp now;
  if (m_runScenario) {
    opendlv::perception::StimulusGroundSpeed sgs(now, m_desiredGroundSpeed, a_speed);
    odcore::data::Container container(sgs);
    getConference().send(container);
  }
}

void Intersection::ControlDirectionOfMovement(float a_azimuth)
{
  odcore::data::TimeStamp now;
  if (m_runScenario) {
    opendlv::model::Direction direction(a_azimuth, 0.0f);
    opendlv::perception::StimulusDirectionOfMovement sdom(now, direction, opendlv::model::Direction(0.0f, 0.0f));
    odcore::data::Container container(sdom);
    getConference().send(container);
  }
}

void Intersection::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_crossingSafeAngle = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection.crossingSafeAngle"));
  m_desiredGroundSpeedMax = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection.desiredGroundSpeed"));
  m_lateralDiscardDistance = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection.lateralDiscardDistance"));
  m_longitudinalDiscardDistance = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection.longitudinalDiscardDistance"));
  m_timeToCollision = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection.timeToCollision"));
  bool isMioCrossingPath = kv.getValue<bool>("knowledge-gcdc16-rule-intersection.isMioCrossingPath");
  m_enableLaneFollowing = kv.getValue<bool>("knowledge-gcdc16-rule-intersection.enableLaneFollowing");
  bool forceScenarioStart = kv.getValue<bool>("knowledge-gcdc16-rule-intersection.forceScenarioStart");

  if (isMioCrossingPath) {
    m_scenarioType = "mioLeftTurnCrossingPath";
  } else {
    m_scenarioType = "mioLeftTurnToLead";
  }
  std::cout << "SCENARIO: " << m_scenarioType << std::endl;

  if (forceScenarioStart) {
    m_runScenario = true;
  }
}

void Intersection::tearDown()
{
}

} // intersection
} // rule
} // gcdc16
} // knowledge
} // opendlv
