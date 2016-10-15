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

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opendavinci/odcore/strings/StringToolbox.h"

#include "gcdc16/rule/intersectionleft/intersectionleft.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace intersectionleft {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
IntersectionLeft::IntersectionLeft(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(
  a_argc, a_argv, "knowledge-gcdc16-intersectionleft"),
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
  m_hasSetupBeenRun(false),
  m_isCreatingGap(false)
  //standstillDistance(6), ,//TODO: Get actual value at GCDC in meters
  //headway(1), //TODO: Get actual value at GCDC in seconds
  //minimumEuclideanDistance(5) //TODO: Get actual value at GCDC in meters
{
}

IntersectionLeft::~IntersectionLeft()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode IntersectionLeft::body()
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

      if (m_scenarioType == "intersectionScenario") {
        opendlv::knowledge::Insight scenarioOut(timestamp, "intersectionScenario");
        odcore::data::Container objectContainer1(scenarioOut);
        getConference().send(objectContainer1);

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
double IntersectionLeft::getDistances(double hostVelocity)
{
  double desiredDistance = standstillDistance + headway * hostVelocity;
  //double safeDistance = scalingFactor * desiredDistance;
  //returnVector[0] = desiredDistance;
  //returnvector[1] = safeDistance;
  return desiredDistance;
}

bool IntersectionLeft::euclideanDistance(double measuredDistance)
{
  double distanceError = measuredDistance - minimumEuclideanDistance;
  if (distanceError >= 0) {
    return true;
  }
  return false;

}
*/

void IntersectionLeft::nextContainer(odcore::data::Container &a_container)
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


void IntersectionLeft::receivedContainerMergeScenario(odcore::data::Container &a_container)
{


  if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    opendlv::perception::Environment receivedEnvironment =
        a_container.getData<opendlv::perception::Environment>();


    std::vector<opendlv::perception::Object> objects = receivedEnvironment.getListOfObjects();

    if (objects.size() < 2) {
      std::cout << "ERROR: intersectionleft.cpp only received info about " << objects.size() << " objects..." << std::endl;
      return;
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
        // Object is in front of us and has a station ID

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
      else if (!strVector.empty() && strVector.at(0) == "Station") {
        std::cout << "Found vehicle WITH stationID (" << properties.at(0)  << ") that is NOT in front of us." << std::endl;
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



  //m_speed = 7; // hardcoded for testing

  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    double speedKph = propulsion.getPropulsionShaftVehicleSpeed();
    m_speed = (float) speedKph / 3.6f;
  }




  odcore::data::TimeStamp timestamp;

  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();



    std::vector<std::string> strVector = 
        odcore::strings::StringToolbox::split(whatInsight, '=');

    if (strVector.size() > 1) {
      if (strVector[0] == "cruiseSpeed") {
        // assuming recieving is km/h
        m_cruiseSpeed = std::stof(strVector[1]) / 3.6f;


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

          if (strVector.size() > 2) {
            opendlv::knowledge::Insight forwardIdInsight(timestamp, "forwardId=" + strVector.at(2));
            odcore::data::Container objectContainerForwardId(forwardIdInsight);
            getConference().send(objectContainerForwardId);
          }
          else {
            std::cout << "ERROR: closestObject had no valid station ID?..." << std::endl;
          }
        }
      }

      if (whatInsight == "createDistance" && m_platoonId == "2") {
        m_isCreatingGap = true;
      }




      if (whatInsight == "isLeader" && m_platoonId == "1" /* scenario A */) {
        m_isLeader = true;

        std::vector<std::string> properties = m_closestObject.getListOfProperties();
        if (properties.empty()) {
          std::cout << "ERROR: closestObject did not have any properties" << std::endl;
        } else {
          strVector = odcore::strings::StringToolbox::split(properties.at(0), ' ');

          if (strVector.size() > 2) {
            opendlv::knowledge::Insight forwardIdInsight(timestamp, "forwardId=" + strVector.at(2));
            odcore::data::Container objectContainerForwardId(forwardIdInsight);
            getConference().send(objectContainerForwardId);
          }
          else {
            std::cout << "ERROR: closestObject had no valid station ID?..." << std::endl;
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

  }
}

void IntersectionLeft::bodyMergeScenario()
{
  odcore::data::TimeStamp timestamp;

  float timeHeadway = 0.9f;

  //opendlv::perception::ObjectDesiredAngularSize angularsize(0.075f, -1);
  //odcore::data::Container objectContainer0(angularsize);
  //getConference().send(objectContainer0);      

  if (m_isInitialized) {
    if (m_hasMerged) {
      m_mostInterestingObject = m_closestObject;
    }
    else {
      m_mostInterestingObject = m_secondClosestObject;
    }

    std::vector<std::string> properties = m_mostInterestingObject.getListOfProperties();
    if (properties.empty()) {
      std::cout << "getSize_ListOfSources(): " << m_mostInterestingObject.getSize_ListOfSources() << std::endl;
      // for (auto qq:properties) {
      //   std::cout << "qq " << qq << std::endl;
      // }
      std::cout << "ERROR: WHAT THE FML" << std::endl;
    }
    else {
      std::vector<std::string> strVector = 
          odcore::strings::StringToolbox::split(properties.at(0), ' ');

      if (strVector.size() > 2 && strVector[0] == "Station") {
        opendlv::knowledge::Insight mioOut(timestamp, "mioId=" + strVector[2]);
        odcore::data::Container objectContainerMio(mioOut);
        getConference().send(objectContainerMio);
      }
      else {
        std::cout << "ERROR: mostInterestingObject had no valid station ID?..." << std::endl;
      }
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
 /*   opendlv::knowledge::DesiredOpticalFlow desired(m_cruiseSpeed);
    odcore::data::Container objectContainerDesiredOpticalFlow(desired);
    getConference().send(objectContainerDesiredOpticalFlow);
*/

    if (m_isCreatingGap) {

      std::vector<std::string> properties = m_closestObject.getListOfProperties();
      if (properties.size() > 2) {
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(1), ' ');
        if (strVector.size() == 3) {
          std::stringstream sstr;
          sstr << strVector.at(2);
          float vehicleLength = 5; // vehicle length of closest vehicle
          try {
            sstr >> vehicleLength;
          }
          catch(...) {
            // if we can't cast to float, set default to 5 meters
            vehicleLength = 5;
          }

          // THIS ASSUMES THAT THE SECOND CLOSEST OBJECT EXISTS
          float gapSize = m_secondClosestObject.getDistance();
          float gapLimit = 1.5f*(10 + timeHeadway*m_speed) + vehicleLength; 
          if (gapSize > gapLimit) {
            opendlv::knowledge::Insight stomInsight(timestamp, "safeToMerge=1");
            odcore::data::Container stomContainer(stomInsight);
            getConference().send(stomContainer);
          }
        }
        else {
          // Error
          std::cout << "Error: Object properties did not contain correct vehicle length" << std::endl;
        }
      }
      else {
        // Error
        std::cout << "Error: Object did not have enough properties to contain vehicle length" << std::endl;
      }

    }

    if (m_isLeader) {
      // m_desiredAzimuth = 0.0f; // could be dangerous, really want to follow road.
      // m_desiredAngularSize = 10 + timeHeadway*m_speed; // desired distance
      // int16_t objectId = m_closestObject.getObjectId();
      // opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      // odcore::data::Container objectContainerDistance(desiredAngularSize);
      // getConference().send(objectContainerDistance);
    } else if (m_isCreatingGap) {
      // m_desiredAzimuth = m_secondClosestObject.getDirection().getAzimuth();
      // m_desiredAngularSize = 10 + timeHeadway*m_speed; // desired distance
      // int16_t objectId = m_closestObject.getObjectId();
      // opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      // odcore::data::Container objectContainerDistance(desiredAngularSize);
      // getConference().send(objectContainerDistance);
    } else {
      // m_desiredAzimuth = m_mostInterestingObject.getDirection().getAzimuth();
      // m_desiredAngularSize = 10 + timeHeadway*m_speed; // desired distance
      // int16_t objectId = m_mostInterestingObject.getObjectId();
      // opendlv::perception::ObjectDesiredAngularSize desiredAngularSize(m_desiredAngularSize, objectId);
      // odcore::data::Container objectContainerDistance(desiredAngularSize);
      // getConference().send(objectContainerDistance);
    }
    
    /*
    opendlv::model::Direction objectDirection(m_desiredAzimuth, 0.0f);
    opendlv::knowledge::DesiredDirectionOfMovement desiredDirection(objectDirection);
    odcore::data::Container objectContainerDirection(desiredDirection);
    getConference().send(objectContainerDirection);
    */
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

void IntersectionLeft::receivedContainerIntersectionScenario(odcore::data::Container &a_container)
{

  odcore::data::TimeStamp timestamp;

  if (a_container.getDataType() == opendlv::perception::Environment::ID()) {
    std::cout << "received environment object" << std::endl;
    opendlv::perception::Environment receivedEnvironment =
        a_container.getData<opendlv::perception::Environment>();

    std::vector<opendlv::perception::Object> objects = receivedEnvironment.getListOfObjects();

    if (objects.size() < 1) {
      std::cout << "ERROR: intersectionleft.cpp only received info about " << objects.size() << " objects..." << std::endl;
      return;
    }

    float closestDistance = 99999;
    bool foundObject = false;
    opendlv::perception::Object closestObject;

    for (uint32_t i=0; i<objects.size(); i++) {
      opendlv::perception::Object object = objects.at(i);
      std::vector<std::string> properties = object.getListOfProperties();

      if (properties.size() > 1) { 
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(0), ' ');
        if (strVector.size() == 3 && strVector.at(0) == "Station") {
          std::stringstream sstr;
          sstr << strVector.at(2);

          try {
            int stationId;
            sstr >> stationId;
            float distanceToObject = object.getDistance();

            if (stationId < 100 && distanceToObject < closestDistance) {
              closestDistance = distanceToObject;
              closestObject = object; 
              foundObject = true;
            }
          }
          catch(...) {
            // well, we didn't manage to cast to int... so don't do anything
            std::cout << "ERROR: failed to cast stationID to int" << std::endl;
          }
        }
      }
    }

    if (foundObject) {
      // We have found a closest object with stationID smaller than 100
      m_mostInterestingObject = closestObject;

    }
  }

  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();


    if (whatInsight == "scenarioReady") {
      // set desired speed of 30 km/h
      /*
      opendlv::knowledge::DesiredOpticalFlow desired(30/3.6f);
      odcore::data::Container objectContainerDesiredOpticalFlow(desired);
      getConference().send(objectContainerDesiredOpticalFlow);
      */
    }
  }

}


void IntersectionLeft::bodyIntersectionScenario()
{
  /*
  bool debuggingSpeed = false;
  if (debuggingSpeed) {

    std::cout << "DEBUGGING SPEED!!!!" << std::endl;

    opendlv::sensation::DesiredOpticalFlow desired(10.0f);
    odcore::data::Container objectContainerDesiredOpticalFlow(desired);
    getConference().send(objectContainerDesiredOpticalFlow);


    return;
  }
  */

  odcore::data::TimeStamp timestamp;


  bool intersectionToLeft = true; // THIS MUST BE SET



  if (intersectionToLeft) {

    if (m_mostInterestingObject.getListOfProperties().size() > 0) {

      std::vector<std::string> properties = m_mostInterestingObject.getListOfProperties();
      if (properties.empty()) {
        std::cout << "getSize_ListOfSources(): " << m_mostInterestingObject.getSize_ListOfSources() << std::endl;
        // for (auto qq:properties) {
        //   std::cout << "qq " << qq << std::endl;
        // }
        std::cout << "ERROR: WHAT THE FML" << std::endl;
      }
      else {
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(0), ' ');

        if (strVector.size() > 2 && strVector[0] == "Station") {
          opendlv::knowledge::Insight mioOut(timestamp, "mioId=" + strVector[2]);
          odcore::data::Container objectContainerMio(mioOut);
          getConference().send(objectContainerMio);
        }
        else {
          std::cout << "ERROR: mostInterestingObject had no valid station ID?..." << std::endl;
        }
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


      // float euclideanDistance = 15;

      // opendlv::perception::ObjectDesiredAngularSize angularsize((euclideanDistance + 5), m_mostInterestingObject.getObjectId());
      // odcore::data::Container objectContainer0(angularsize);
      // getConference().send(objectContainer0); 

      
    }

  }
  else { // if intersection is to the right

    if (m_mostInterestingObject.getListOfProperties().size() > 0) {

      std::vector<std::string> properties = m_mostInterestingObject.getListOfProperties();
      if (properties.empty()) {
        std::cout << "getSize_ListOfSources(): " << m_mostInterestingObject.getSize_ListOfSources() << std::endl;
        // for (auto qq:properties) {
        //   std::cout << "qq " << qq << std::endl;
        // }
        std::cout << "ERROR: WHAT THE FML" << std::endl;
      }
      else {
        std::vector<std::string> strVector = 
            odcore::strings::StringToolbox::split(properties.at(0), ' ');

        if (strVector.size() > 2 && strVector[0] == "Station") {
          opendlv::knowledge::Insight mioOut(timestamp, "mioId=" + strVector[2]);
          odcore::data::Container objectContainerMio(mioOut);
          getConference().send(objectContainerMio);
        }
        else {
          std::cout << "ERROR: mostInterestingObject had no valid station ID?..." << std::endl;
        }
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



      if (mioBearing > 0.1f) { // ~ 5 degrees to left

        // spoofing the hell out of act
        // opendlv::perception::ObjectDesiredAngularSize angularsize(20, 5798); // secret id with act.cpp
        // odcore::data::Container objectContainer0(angularsize);
        // getConference().send(objectContainer0); 
      }
      else {

        // mio OCV vehicle is to the right, we should care about it
        // float euclideanDistance = 15;
        // opendlv::perception::ObjectDesiredAngularSize angularsize((euclideanDistance + 5), m_mostInterestingObject.getObjectId());
        // odcore::data::Container objectContainer0(angularsize);
        // getConference().send(objectContainer0); 
      }
      
    }
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





void IntersectionLeft::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_currentLane = kv.getValue<std::string>("knowledge-gcdc16-intersectionleft.initial_lane");
  m_isTail = kv.getValue<std::string>("knowledge-gcdc16-intersectionleft.is_tail");
  m_platoonId = kv.getValue<std::string>("knowledge-gcdc16-intersectionleft.platoon_id");
  m_scenarioType = kv.getValue<std::string>("knowledge-gcdc16-intersectionleft.scenario");
  m_hasSetupBeenRun = true;
}

void IntersectionLeft::tearDown()
{
}

} // intersectionleft
} // rule
} // gcdc16
} // knowledge
} // opendlv
