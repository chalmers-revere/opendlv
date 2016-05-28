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

#include "opendavinci/odcore/strings/StringToolbox.h"

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
  m_closestObject(),
  m_secondClosestObject(),
  m_mostInterestingObject(),
  m_desiredAzimuth(0.0f),
  m_cruiseSpeed(0.0),
  m_desiredAngularSize(30.0f),
  m_speed(0.0),
  m_isAutonomous(false),
  m_platoonId(),
  m_currentLane(),
  m_isTail(),
  m_hasMerged(false),
  m_isInitialized(false),
  m_scenarioIsReady(false),
  m_isLeader(false),
  m_scenarioType(),
  m_hasSetupBeenRun(false)
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

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    if (m_hasSetupBeenRun) {
      odcore::data::TimeStamp timestamp;


      opendlv::perception::ObjectDesiredAngularSize angularsize(0.075f, -1);
      odcore::data::Container objectContainer0(angularsize);
      getConference().send(objectContainer0);      

      if (m_scenarioType == "platooning") {
        opendlv::knowledge::Insight scenarioOut(timestamp, "mergeScenario");
        odcore::data::Container objectContainer1(scenarioOut);
        getConference().send(objectContainer1);
      }

      if (m_isInitialized) {
        if (m_hasMerged) {
          m_mostInterestingObject = m_closestObject;
        }
        else {
          m_mostInterestingObject = m_secondClosestObject;
        }

        std::vector<std::string> properties = m_mostInterestingObject.getListOfProperties();
        if (properties.empty()) {
          std::cout << "ERROR: WHAT THE FML" << std::endl;
        }
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(0), ' ');

        if (strVector.at(0) == "Station") {
          opendlv::knowledge::Insight mioOut(timestamp, "mioId=" + strVector.at(2));
          odcore::data::Container objectContainerMio(mioOut);
          getConference().send(objectContainerMio);
        }
        else {
          std::cout << "ERROR: mostInterestingObject had no station ID?..." << std::endl;
        }
      }

      opendlv::knowledge::Insight laneOut(timestamp, "currentLane=" + m_currentLane);
      odcore::data::Container objectContainerLane(laneOut);
      getConference().send(objectContainerLane);

      if (m_hasMerged) {
        m_isTail = "0";
      }
      opendlv::knowledge::Insight tailOut(timestamp, "isTail=" + m_isTail);
      odcore::data::Container objectContainer5(tailOut);
      getConference().send(objectContainer5);

      opendlv::knowledge::Insight platoonOut(timestamp, "platoonId=" + m_platoonId);
      odcore::data::Container objectContainer6(platoonOut);
      getConference().send(objectContainer6);





      if (m_scenarioIsReady) {
        opendlv::sensation::DesiredOpticalFlow desired(m_cruiseSpeed);
        odcore::data::Container objectContainerDesiredOpticalFlow(desired);
        getConference().send(objectContainerDesiredOpticalFlow);


        if (m_isLeader) {
          m_desiredAzimuth = 0.0f;
        }
        else {
          m_desiredAzimuth = m_mostInterestingObject.getDirection().getAzimuth();
        }
        
        opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
        opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
        odcore::data::Container objectContainerDirection(desiredDirection);
        getConference().send(objectContainerDirection);


        m_desiredAngularSize = 10 + 0.9f*m_speed;
        int16_t objectId = m_mostInterestingObject.getObjectId();
        opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      }

    }




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

  if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    opendlv::perception::Environment receivedEnvironment =
        a_container.getData<opendlv::perception::Environment>();

    std::vector<opendlv::perception::Object> objects = receivedEnvironment.getListOfObjects();

    if (objects.size() < 2) {
      std::cout << "ERROR: rule.cpp only received info about " << objects.size() << " objects..." << std::endl;
    }

    float pi = 3.1415926535f;
    opendlv::perception::Object closestObject;
    float closestDistance = 99999;
    opendlv::perception::Object secondClosestObject;
    float secondClosestDistance = 99999;

    for (uint32_t i=0; i<objects.size(); i++) {
      opendlv::perception::Object currentObject = objects.at(i);

      float distance = currentObject.getDistance();
      float azimuth = currentObject.getDirection().getAzimuth();
      std::vector<std::string> properties = currentObject.getListOfProperties();
      if (properties.empty()) {
        continue;
      }
      std::vector<std::string> strVector = 
          odcore::strings::StringToolbox::split(properties.at(0), ' ');
      if (azimuth > -pi/2.0f && azimuth < pi/2.0f && strVector.at(0) == "Station") {
        // Object is in front of us and ha station ID

        if (distance < closestDistance) {
          secondClosestDistance = closestDistance;
          secondClosestObject = closestObject;

          closestDistance = distance;
          closestObject = currentObject;
        }
        else if (distance < secondClosestDistance) {
          secondClosestDistance = distance;
          secondClosestObject = currentObject;
        }
      }
    }

    if (fabs(secondClosestObject.getDirection().getAzimuth()) < fabs(closestObject.getDirection().getAzimuth())) {
      // This is expected
    } 
    else {
      std::cout << "UNEXPECTED: closestObject has smaller azimuth than secondClosestObject..." <<  std::endl;
    }

    m_closestObject = closestObject;
    m_secondClosestObject = secondClosestObject;
    m_isInitialized = true;
  }



  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    m_speed = (float) speedKph / 3.6f;
  }




  odcore::data::TimeStamp timestamp;

  if (a_container.getDataType() == opendlv::knowledge::Insight::ID()) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();


    std::string activeScenario = "A"; // We start in the left lane
    //std.:string activeScenario = "B"; // We start in the right lane

    std::vector<std::string> strVector = 
        odcore::strings::StringToolbox::split(whatInsight, '=');

    if (strVector.size() > 1) {
      if (strVector[0] == "cruiseSpeed") {
        m_cruiseSpeed = std::stof(strVector[1]);


      }
    } else {

      if (whatInsight == "scenarioReady") {
        m_scenarioIsReady = true;
      }


      if (whatInsight == "mergeRequest") {


      
      }



      if (whatInsight == "isLeader" && m_platoonId == "1" /* scenario A */) {
        m_isLeader = true;

        std::vector<std::string> properties = m_closestObject.getListOfProperties();
        if (properties.empty()) {
          std::cout << "ERROR: closestObject did not have any properties" << std::endl;
        }
        strVector = odcore::strings::StringToolbox::split(properties.at(0), ' ');

        opendlv::knowledge::Insight forwardIdInsight(timestamp, "forwardId=" + strVector.at(2));
        odcore::data::Container objectContainerForwardId(forwardIdInsight);
        getConference().send(objectContainerForwardId);
      }

      if (whatInsight == "safeToMerge" && m_platoonId == "1" /* scenario A */) {

        opendlv::knowledge::Insight mergeFlagOut(timestamp, "mergeFlag=1");
        odcore::data::Container objectContainerMergeFlag(mergeFlagOut);
        getConference().send(objectContainerMergeFlag);

        opendlv::knowledge::Insight leadFlagOut(timestamp, "isHead=0");
        odcore::data::Container objectContainerLeadFlag(leadFlagOut);
        getConference().send(objectContainerLeadFlag);

        opendlv::knowledge::Insight platoonOut(timestamp, "platoonId=2");
        odcore::data::Container objectContainerPlatoonOut(platoonOut);
        getConference().send(objectContainerPlatoonOut);


        m_hasMerged = true;
        m_isLeader = false;
        m_currentLane = "1";

      }

      if (whatInsight == "mergeFinished" && activeScenario == "A") {
        // Keep the same pace as FWD MIOR
      }
    }

/*
  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    m_speed = speedKph / 3.6;
  }


  if (a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t id = unpackedObject.getObjectId();

    if (id == -1) {
      return;
    }

    opendlv::model::Direction direction = unpackedObject.getDirection();
    float azimuth = direction.getAzimuth();

    if (std::abs(azimuth) < 0.22f) { //TODO: mio front left or front right
      if (m_object == nullptr) {
        m_object.reset(new opendlv::perception::Object(unpackedObject));
      } else {
        if (unpackedObject.getDistance() < m_object->getDistance())
          m_object.reset(new opendlv::perception::Object(unpackedObject));
      }
    }*/

    float mioBearing = m_object->getDirection().getAzimuth();
    float mioRangeRate = -1.0f;
    float mioRange = m_object->getDistance();
    float mioTimeHeadway = mioRange / static_cast<float>(m_speed);

    opendlv::knowledge::Insight mioBearingInsight(timestamp, "mioBearing=" + std::to_string(mioBearing));
    odcore::data::Container mioBearingContainer(mioBearingInsight);
    getConference().send(mioBearingContainer);

    opendlv::knowledge::Insight mioRangeInsight(timestamp, "mioRange=" + std::to_string(mioRange));
    odcore::data::Container mioRangeContainer(mioRangeInsight);
    getConference().send(mioRangeContainer);

    opendlv::knowledge::Insight mioRangeRateInsight(timestamp, "mioRangeRate=" + std::to_string(mioRangeRate));
    odcore::data::Container mioRangeRateContainer(mioRangeRateInsight);
    getConference().send(mioRangeRateContainer);

    opendlv::knowledge::Insight mioTimeHeadwayInsight(timestamp, "mioTimeHeadway=" + std::to_string(mioTimeHeadway));
    odcore::data::Container mioTimeHeadwayContainer(mioTimeHeadwayInsight);
    getConference().send(mioTimeHeadwayContainer);
 


    //m_desiredAzimuth = m_object->getDirection().getAzimuth();
    //opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
    //opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
    //odcore::data::Container objectContainer(desiredDirection);
    //getConference().send(objectContainer);

  } 

}


  
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





void Rule::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_currentLane = kv.getValue<std::string>("knowledge-gcdc16-rule.initial_lane");
  m_isTail = kv.getValue<std::string>("knowledge-gcdc16-rule.is_tail");
  m_platoonId = kv.getValue<std::string>("knowledge-gcdc16-rule.platoon_id");
  m_scenarioType = kv.getValue<std::string>("knowledge-gcdc16-rule.scenario");
  m_hasSetupBeenRun = true;
}

void Rule::tearDown()
{
}

} // rule
} // gcdc16
} // knowledge
} // opendlv
