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

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "gcdc16/rule/rule.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
Rule::Rule(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(
	a_argc, a_argv, "knowledge-gcdc16-rule"),
  m_object(),
  m_desiredAzimuth(0.0f),
  m_isAutonomous(false)
	//standstillDistance(6), //TODO: Get actual value at GCDC in meters
	//headway(1), //TODO: Get actual value at GCDC in seconds
	//minimumEuclideanDistance(5) //TODO: Get actual value at GCDC in meters
{
}

Rule::~Rule()
{
}
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Rule::body()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string mio1 = kv.getValue<std::string>("knowledge-gcdc16-rule.mio_1");
  std::string mio2 = kv.getValue<std::string>("knowledge-gcdc16-rule.mio_2");
  std::string backwardId = kv.getValue<std::string>("knowledge-gcdc16-rule.backward_id");
  std::string initialLane = kv.getValue<std::string>("knowledge-gcdc16-rule.initial_lane");
  std::string isTail = kv.getValue<std::string>("knowledge-gcdc16-rule.is_tail");
  std::string platoonId = kv.getValue<std::string>("knowledge-gcdc16-rule.platoon_id");

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING){
      odcore::data::TimeStamp timestamp;

      opendlv::sensation::DesiredOpticalFlow desired(35.0f/3.6f);
      odcore::data::Container objectContainer0(desired);
      getConference().send(objectContainer0);
      std::cout << "Debug: " << std::endl;


      opendlv::knowledge::Insight scenarioOut(timestamp, "mergeScenario");
      odcore::data::Container objectContainer1(scenarioOut);
      getConference().send(objectContainer1);

      opendlv::knowledge::Insight mioOut(timestamp, "mio=" + mio1);
      odcore::data::Container objectContainer2(mioOut);
      getConference().send(objectContainer2);

      opendlv::knowledge::Insight backwardOut(timestamp, "backwardId=" + backwardId);
      odcore::data::Container objectContainer3(backwardOut);
      getConference().send(objectContainer3);

      opendlv::knowledge::Insight laneOut(timestamp, "initialLane=" + initialLane);
      odcore::data::Container objectContainer4(laneOut);
      getConference().send(objectContainer4);

      opendlv::knowledge::Insight tailOut(timestamp, "isTail=" + isTail);
      odcore::data::Container objectContainer5(tailOut);
      getConference().send(objectContainer5);

      opendlv::knowledge::Insight platoonOut(timestamp, "platoonId=" + platoonId);
      odcore::data::Container objectContainer6(platoonOut);
      getConference().send(objectContainer6);

  } 
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
   
}

/**
* Receives .
* Sends .
*/
/*
double Rule::getDistances(double hostVelocity)
{
	double desiredDistance = standstillDistance + headway * hostVelocity;
	//double safeDistance = scalingFactor * desiredDistance;
	//returnVector[0] = desiredDistance;
	//returnvector[1] = safeDistance;
	return desiredDistance;
}

bool Rule::euclideanDistance(double measuredDistance)
{
	double distanceError = measuredDistance - minimumEuclideanDistance;
	if (distanceError >= 0) {
		return true;
	}
	return false;

}
*/

void Rule::nextContainer(odcore::data::Container &a_container)
{
  
 /* if (a_container.getDataType() == opendlv::proxy::ControlState::ID()) {
    opendlv::proxy::ControlState isAutonomous = a_container.getData<opendlv::proxy::ControlState>();

    bool autonomous = isAutonomous.getIsAutonomous();
    if (autonomous) {
      m_isAutonomous = true;
    }
  } else if (m_isAutonomous && a_container.getDataType() == opendlv::knowledge::Insight::ID()) {

    opendlv::knowledge::Insight unpackedObject =
    a_container.getData<opendlv::knowledge::Insight>();
    std::string insightMessage = unpackedObject.getInsight();
  }
      //TODO: STOM, MergeFlag, Ask about Intention messages, distancetravelledCZ
      //TODO: use rsuEvent -> merging should commence
} */

    if (a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t id = unpackedObject.getObjectId();

    if (id == -1) {
      return;
    }

    opendlv::model::Direction direction = unpackedObject.getDirection();
    float azimuth = direction.getAzimuth();

    if (std::abs(azimuth) < 0.22f) {
      if (m_object == nullptr) {
        m_object.reset(new opendlv::perception::Object(unpackedObject));
      } else {
        if (unpackedObject.getDistance() < m_object->getDistance())
          m_object.reset(new opendlv::perception::Object(unpackedObject));
      }
    }
    m_desiredAzimuth = m_object->getDirection().getAzimuth();
    opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
    opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
    odcore::data::Container objectContainer(desiredDirection);
    getConference().send(objectContainer);
    }
  }






void Rule::setUp()
{
}

void Rule::tearDown()
{
}

} // rule
} // gcdc16
} // knowledge
} // opendlv
