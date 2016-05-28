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
  m_intersection_mostInterestingObject(0),
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
  m_hasSetupBeenRun(false),
  m_isCreatingGap(false)
	//standstillDistance(6), ,//TODO: Get actual value at GCDC in meters
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

      if (m_scenarioType == "mergeScenario") {
        opendlv::knowledge::Insight scenarioOut(timestamp, "mergeScenario");
        odcore::data::Container objectContainer1(scenarioOut);
        getConference().send(objectContainer1);

        bodyMergeScenario();
      }

/*
<<<<<<< HEAD
      std::cout << "DEBUG 6" << std::endl;
      std::cout << "DEBUG: m_currentLane - " << m_currentLane << std::endl;
      // opendlv::knowledge::Insight laneOut(timestamp, "currentLane=" + m_currentLane);
      // odcore::data::Container objectContainerLane(laneOut);
      // getConference().send(objectContainerLane);

      std::cout << "DEBUG 7" << std::endl;
      if (m_hasMerged) {
        m_isTail = "0";
      }
      opendlv::knowledge::Insight tailOut(timestamp, "isTail=" + m_isTail);
      odcore::data::Container objectContainer5(tailOut);
      getConference().send(objectContainer5);

      std::cout << "DEBUG 8" << std::endl;
      opendlv::knowledge::Insight platoonOut(timestamp, "platoonId=" + m_platoonId);
      odcore::data::Container objectContainer6(platoonOut);
      getConference().send(objectContainer6);



      std::cout << "DEBUG 9" << std::endl;


      //if (m_scenarioIsReady)
      {
          m_cruiseSpeed = 10.0;
        opendlv::sensation::DesiredOpticalFlow desired(m_cruiseSpeed);
        odcore::data::Container objectContainerDesiredOpticalFlow(desired);
        getConference().send(objectContainerDesiredOpticalFlow);

        
        std::cout << "DEBUG 10 \n Cruise speeed " << m_cruiseSpeed << std::endl;

        if (m_isCreatingGap) {

          std::vector<std::string> properties = m_closestObject.getListOfProperties();
          if (properties.size() > 2) {
            std::vector<std::string> strVector = 
                odcore::strings::StringToolbox::split(properties.at(1), ' ');
            if (strVector.size() == 3) {
              std::stringstream sstr;
              sstr << strVector.at(2);
              float vehicleLength;
              sstr >> vehicleLength;

              float gapSize = m_secondClosestObject.getDistance();
              float gapLimit = 1.4f*(10 + 0.9f*m_speed) + vehicleLength;
              if (gapSize > gapLimit) {
                opendlv::knowledge::Insight stomInsight(timestamp, "safeToMerge=1");
                odcore::data::Container stomContainer(stomInsight);
                getConference().send(stomContainer);
              }
            }
            else {
              std::cout << "Error: Object properties did not contain correct vehicle length" << std::endl;
              // Error
            }
          }
          else {
            std::cout << "Error: Object did not have enough properties to contain vehicle length" << std::endl;
            // Error
          }

        }

        if (m_isLeader) {
          m_desiredAzimuth = 0.0f;
          m_desiredAngularSize = 10 + 0.9f*m_speed;
          int16_t objectId = m_closestObject.getObjectId();
          opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
          odcore::data::Container objectContainerDistance(desiredAngularSize);
          getConference().send(objectContainerDistance);
        } else if (m_isCreatingGap) {
          m_desiredAzimuth = m_secondClosestObject.getDirection().getAzimuth();
          m_desiredAngularSize = 10 + 0.9f*m_speed;
          int16_t objectId = m_closestObject.getObjectId();
          opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
          odcore::data::Container objectContainerDistance(desiredAngularSize);
          getConference().send(objectContainerDistance);
        } else {
          m_desiredAzimuth = m_mostInterestingObject.getDirection().getAzimuth();
          m_desiredAngularSize = 10 + 0.9f*m_speed;
          int16_t objectId = m_mostInterestingObject.getObjectId();
          opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
          odcore::data::Container objectContainerDistance(desiredAngularSize);
          getConference().send(objectContainerDistance);
        }
        
        std::cout << "DEBUG 11" << std::endl;
        opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
        opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
        
        odcore::data::Container objectContainerDirection(desiredDirection);
        getConference().send(objectContainerDirection);

=======
*/
      if (m_scenarioType == "intersectionScenario") {
        opendlv::knowledge::Insight scenarioOut(timestamp, "intersectionScenario");
        odcore::data::Container objectContainer1(scenarioOut);
        getConference().send(objectContainer1);
//>>>>>>> feature.rulegcdc

        bodyIntersectionScenario();
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
  if (!m_hasSetupBeenRun) {
    return;
  }


  if (m_scenarioType == "mergeScenario") {

    receivedContainerMergeScenario(a_container);
  }

  if (m_scenarioType == "intersectionScenario") {

    receivedContainerIntersectionScenario(a_container);
  } 

}


void Rule::receivedContainerMergeScenario(odcore::data::Container &a_container)
{

  std::cout << "DEBUG 42" << std::endl;

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
    bool foundSomething = false;

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
      if (azimuth > -pi/2.0f && azimuth < pi/2.0f && !strVector.empty() && strVector.at(0) == "Station") {
        // Object is in front of us and ha station ID

        if (distance < closestDistance) {
          secondClosestDistance = closestDistance;
          secondClosestObject = closestObject;

          closestDistance = distance;
          closestObject = currentObject;
          foundSomething = true;
        }
        else if (distance < secondClosestDistance) {
          secondClosestDistance = distance;
          secondClosestObject = currentObject;
          foundSomething = true;
        }
      }
    }

    if (fabs(secondClosestObject.getDirection().getAzimuth()) < fabs(closestObject.getDirection().getAzimuth())) {
      // This is expected
    } 
    else {
      std::cout << "UNEXPECTED: closestObject has smaller azimuth than secondClosestObject..." <<  std::endl;
    }

    if (foundSomething) {
      m_closestObject = closestObject;
      m_secondClosestObject = secondClosestObject;
      m_isInitialized = true;
    }
  }

  std::cout << "DEBUG 52" << std::endl;


  //m_speed = 7; // hardcoded for testing

  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    m_speed = (float) speedKph / 3.6f;
  }


      std::cout << "DEBUG6 2" << std::endl;


  odcore::data::TimeStamp timestamp;

  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();



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


      if (whatInsight == "mergeRequest" && m_platoonId == "2") {
        
        std::vector<std::string> properties = m_closestObject.getListOfProperties();
        if (properties.empty()) {
          std::cout << "ERROR: closestObject did not have any properties" << std::endl;
        } else {
          strVector = odcore::strings::StringToolbox::split(properties.at(0), ' ');

          if (!strVector.empty()) {
            opendlv::knowledge::Insight forwardIdInsight(timestamp, "forwardId=" + strVector.at(2));
            odcore::data::Container objectContainerForwardId(forwardIdInsight);
            getConference().send(objectContainerForwardId);
          }
        }
        m_isCreatingGap = true;      
      }



      if (whatInsight == "isLeader" && m_platoonId == "1" /* scenario A */) {
        m_isLeader = true;

        std::vector<std::string> properties = m_closestObject.getListOfProperties();
        if (properties.empty()) {
          std::cout << "ERROR: closestObject did not have any properties" << std::endl;
        } else {
          strVector = odcore::strings::StringToolbox::split(properties.at(0), ' ');

          if (!strVector.empty()) {
            opendlv::knowledge::Insight forwardIdInsight(timestamp, "forwardId=" + strVector.at(2));
            odcore::data::Container objectContainerForwardId(forwardIdInsight);
            getConference().send(objectContainerForwardId);
          }
        }
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

    }

      std::cout << "DEBUG 72" << std::endl;
/*
  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    m_speed = speedKph / 3.6;
  }


  if (a_container.getDataType() == (opendlv::perception::Object::ID() + 300)) {
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

      
 


    //m_desiredAzimuth = m_object->getDirection().getAzimuth();
    //opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
    //opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
    //odcore::data::Container objectContainer(desiredDirection);
    //getConference().send(objectContainer);

  }
}

void Rule::receivedContainerIntersectionScenario(odcore::data::Container &a_container)
{
  std::cout << "receivedContainerIntersectionScenario" << std::endl;

  odcore::data::TimeStamp timestamp;

  if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    std::cout << "received environment object" << std::endl;
    opendlv::perception::Environment receivedEnvironment =
        a_container.getData<opendlv::perception::Environment>();
    std::cout << "Debug 1" << std::endl;

    std::vector<opendlv::perception::Object> objects = receivedEnvironment.getListOfObjects();

    std::cout << "Debug 2" << std::endl;
    if (objects.size() < 1) {
      std::cout << "ERROR: rule.cpp only received info about " << objects.size() << " objects..." << std::endl;
    }

    std::cout << "Debug 3" << std::endl;
    float closestDistance = 99999;
    opendlv::perception::Object* closestObject = 0;

    std::cout << "Debug 4" << std::endl;
    for (uint32_t i=0; i<objects.size(); i++) {
      opendlv::perception::Object object = objects.at(i);
      std::vector<std::string> properties = object.getListOfProperties();

      std::cout << "Debug 4.1" << std::endl;
      if (properties.size() > 1) { 
        std::cout << "Debug 4.2" << std::endl;
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(0), ' ');
        std::cout << "Debug 4.3" << std::endl;
        if (strVector.size() == 3 && strVector.at(0) == "Station") {
          std::cout << "Debug 4.4" << std::endl;
          std::stringstream sstr;
          sstr << strVector.at(2);
          int stationId;
          sstr >> stationId;
          std::cout << "Debug 4.5" << std::endl;
          float distanceToObject = object.getDistance();

          if (stationId < 100 && distanceToObject < closestDistance) {
            std::cout << "Debug 4.6" << std::endl;
            closestDistance = distanceToObject;
            closestObject = &object; 
          }
        }
            std::cout << "Debug 4.7" << std::endl;
      }
            std::cout << "Debug 4.8" << std::endl;
    }

    std::cout << "Debug 5" << std::endl;

    if (closestObject != 0) {
      // We have found a closest object with stationID smaller than 100
      m_intersection_mostInterestingObject = closestObject;

    }
  }
  std::cout << "Debug 6" << std::endl;

  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();


    if (whatInsight == "scenarioReady") {
      opendlv::sensation::DesiredOpticalFlow desired(30/3.6f);
      odcore::data::Container objectContainerDesiredOpticalFlow(desired);
      getConference().send(objectContainerDesiredOpticalFlow);

    }
  }
  std::cout << "Debug 7" << std::endl;

}

void Rule::bodyMergeScenario()
{
  odcore::data::TimeStamp timestamp;
  std::cout << "DEBUG 2" << std::endl;

  opendlv::perception::ObjectDesiredAngularSize angularsize(0.075f, -1);
  odcore::data::Container objectContainer0(angularsize);
  getConference().send(objectContainer0);      

  std::cout << "DEBUG 3" << std::endl;

  std::cout << "DEBUG 4" << std::endl;
  if (m_isInitialized) {
    if (m_hasMerged) {
      m_mostInterestingObject = m_closestObject;
    }
    else {
      m_mostInterestingObject = m_secondClosestObject;
    }

    std::cout << "DEBUG 5" << std::endl;
    std::vector<std::string> properties = m_mostInterestingObject.getListOfProperties();
    if (properties.empty()) {
      std::cout << "getSize_ListOfSources(): " << m_mostInterestingObject.getSize_ListOfSources() << std::endl;
      for (auto qq:properties) {
        std::cout << "qq " << qq << std::endl;
      }
      std::cout << "ERROR: WHAT THE FML" << std::endl;
    }
    std::vector<std::string> strVector = 
        odcore::strings::StringToolbox::split(properties.at(0), ' ');

    if (strVector.size() > 0 && strVector[0] == "Station") {
      opendlv::knowledge::Insight mioOut(timestamp, "mioId=" + strVector[2]);
      odcore::data::Container objectContainerMio(mioOut);
      getConference().send(objectContainerMio);
    }
    else {
      std::cout << "ERROR: mostInterestingObject had no station ID?..." << std::endl;

    }
  }

  std::cout << "DEBUG 6" << std::endl;
  std::cout << "DEBUG: m_currentLane - " << m_currentLane << std::endl;
  // opendlv::knowledge::Insight laneOut(timestamp, "currentLane=" + m_currentLane);
  // odcore::data::Container objectContainerLane(laneOut);
  // getConference().send(objectContainerLane);

  std::cout << "DEBUG 7" << std::endl;
  if (m_hasMerged) {
    m_isTail = "0";
  }
  opendlv::knowledge::Insight tailOut(timestamp, "isTail=" + m_isTail);
  odcore::data::Container objectContainer5(tailOut);
  getConference().send(objectContainer5);

  std::cout << "DEBUG 8" << std::endl;
  opendlv::knowledge::Insight platoonOut(timestamp, "platoonId=" + m_platoonId);
  odcore::data::Container objectContainer6(platoonOut);
  getConference().send(objectContainer6);



  std::cout << "DEBUG 9" << std::endl;


  if (m_scenarioIsReady) {
    opendlv::sensation::DesiredOpticalFlow desired(m_cruiseSpeed);
    odcore::data::Container objectContainerDesiredOpticalFlow(desired);
    getConference().send(objectContainerDesiredOpticalFlow);

    
    std::cout << "DEBUG 10" << std::endl;

    if (m_isCreatingGap) {

      std::vector<std::string> properties = m_closestObject.getListOfProperties();
      if (properties.size() > 2) {
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(1), ' ');
        if (strVector.size() == 3) {
          std::stringstream sstr;
          sstr << strVector.at(2);
          float vehicleLength;
          sstr >> vehicleLength;

          float gapSize = m_secondClosestObject.getDistance();
          float gapLimit = 1.4f*(10 + 0.9f*m_speed) + vehicleLength;
          if (gapSize > gapLimit) {
            opendlv::knowledge::Insight stomInsight(timestamp, "safeToMerge=1");
            odcore::data::Container stomContainer(stomInsight);
            getConference().send(stomContainer);
          }
        }
        else {
          std::cout << "Error: Object properties did not contain correct vehicle length" << std::endl;
          // Error
        }
      }
      else {
        std::cout << "Error: Object did not have enough properties to contain vehicle length" << std::endl;
        // Error
      }

    }

    if (m_isLeader) {
      m_desiredAzimuth = 0.0f;
      m_desiredAngularSize = 10 + 0.9f*m_speed;
      int16_t objectId = m_closestObject.getObjectId();
      opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      odcore::data::Container objectContainerDistance(desiredAngularSize);
      getConference().send(objectContainerDistance);
    } else if (m_isCreatingGap) {
      m_desiredAzimuth = m_secondClosestObject.getDirection().getAzimuth();
      m_desiredAngularSize = 10 + 0.9f*m_speed;
      int16_t objectId = m_closestObject.getObjectId();
      opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      odcore::data::Container objectContainerDistance(desiredAngularSize);
      getConference().send(objectContainerDistance);
    } else {
      m_desiredAzimuth = m_mostInterestingObject.getDirection().getAzimuth();
      m_desiredAngularSize = 10 + 0.9f*m_speed;
      int16_t objectId = m_mostInterestingObject.getObjectId();
      opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      odcore::data::Container objectContainerDistance(desiredAngularSize);
      getConference().send(objectContainerDistance);
    }
    
    std::cout << "DEBUG 11" << std::endl;
    opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
    opendlv::sensation::DesiredDirectionOfMovement desiredDirection(objectDirection);
    
    odcore::data::Container objectContainerDirection(desiredDirection);
    getConference().send(objectContainerDirection);


    std::cout << "DEBUG 12" << std::endl;
  }



  float mioBearing = m_mostInterestingObject.getDirection().getAzimuth();
  float mioRangeRate = 100000.0f;
  float mioRange = m_mostInterestingObject.getDistance();
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

  opendlv::knowledge::Insight mioTimeHeadwayInsight(timestamp, "timeHeadway=" + std::to_string(mioTimeHeadway));
  odcore::data::Container mioTimeHeadwayContainer(mioTimeHeadwayInsight);
  getConference().send(mioTimeHeadwayContainer);


}

void Rule::bodyIntersectionScenario()
{

  bool debuggingSpeed = false;
  if (debuggingSpeed) {

    std::cout << "DEBUGGING SPEED!!!!" << std::endl;

    opendlv::sensation::DesiredOpticalFlow desired(10.0f);
    odcore::data::Container objectContainerDesiredOpticalFlow(desired);
    getConference().send(objectContainerDesiredOpticalFlow);


    return;
  }


  std::cout << "bodyIntersectionScenario" << std::endl;
  odcore::data::TimeStamp timestamp;
  if (m_intersection_mostInterestingObject != 0) {

    float mioBearing = m_mostInterestingObject.getDirection().getAzimuth();
    float mioRangeRate = 100000.0f;
    float mioRange = m_mostInterestingObject.getDistance();
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

    opendlv::knowledge::Insight mioTimeHeadwayInsight(timestamp, "timeHeadway=" + std::to_string(mioTimeHeadway));
    odcore::data::Container mioTimeHeadwayContainer(mioTimeHeadwayInsight);
    getConference().send(mioTimeHeadwayContainer);


    float euclideanDistance = 15;

    opendlv::perception::ObjectDesiredAngularSize angularsize((euclideanDistance + 5), -1);
    odcore::data::Container objectContainer0(angularsize);
    getConference().send(objectContainer0); 

    
  }





}


  
 /* if (a_container.getDataType() == opendlv::proxy::ControlState::ID()) {
    opendlv::proxy::ControlState isAutonomous = a_container.getData<opendlv::proxy::ControlState>();

    bool autonomous = isAutonomous.getIsAutonomous();
    if (autonomous) {
      m_isAutonomous = true;
    }
  } else if (m_isAutonomous && a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {

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
