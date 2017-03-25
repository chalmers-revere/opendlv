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

#include <chrono>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
 
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/strings/StringToolbox.h"


#include "linguistics/v2vcam/buffer.hpp"
#include "gcdc16/v2viclcm/v2viclcm.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace v2viclcm {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vIclcm::V2vIclcm(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "knowledge-gcdc16-v2viclcm")
    , m_initialised(false)
    , m_sendLog()
    , m_receiveLog()
    , m_timeType2004()
    , m_scenario()
    , m_mioBeenLeader(false)
    , m_hasMerged(false)
    , m_counterMerge()
    , m_printOutbound()
    , m_printInbound()
    , m_record()
{
  
}

V2vIclcm::~V2vIclcm()
{
}
void V2vIclcm::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();


  m_printOutbound =(kv.getValue<int32_t>("knowledge-gcdc16-v2viclcm.printOutbound") == 1);
  m_printInbound =(kv.getValue<int32_t>("knowledge-gcdc16-v2viclcm.printInbound") == 1);
  m_record = (kv.getValue<int32_t>("knowledge-gcdc16-v2viclcm.record") == 1);
  m_participantsReady = (kv.getValue<int32_t>("knowledge-gcdc16-v2viclcm.participantsReady") == 1);
  

  if (m_record) {

    struct stat st;
    if (::stat("var/application/knowledge/gcdc16/v2viclcm", &st) == -1) {
      ::system("mkdir -p ./var/application/knowledge/gcdc16/v2viclcm");
      // std::cout << "Created dir" << std::endl;
    }
    odcore::data::TimeStamp nu;

    std::stringstream filenameSend;
    std::stringstream filenameReceive;

    filenameSend << "/" << nu.getYYYYMMDD_HHMMSS() << " iclcm send.log";
    filenameReceive << "/" << nu.getYYYYMMDD_HHMMSS() << " iclcm receive.log";
    m_sendLog.open("var/application/knowledge/gcdc16/v2viclcm" 
      + filenameSend.str(), std::ios::out | std::ios::app);
    m_receiveLog.open("var/application/knowledge/gcdc16/v2viclcm" 
      + filenameReceive.str(), std::ios::out | std::ios::app);

    std::string header = "#time, \
        messageId, \
        stationId, \
        containerMask, \
        rearAxleLocation, \
        controllerType, \
        responseTimeConstant, \
        responseTimeDelay, \
        targetLongAcc, \
        timeHeadway, \
        cruiseSpeed, \
        lowFrequencyMask, \
        participantsReady, \
        startPlatoon, \
        endOfScenario, \
        mioId, \
        mioRange, \
        mioBearing, \
        mioRangeRate, \
        lane, \
        forwardId, \
        backwardId, \
        mergeRequest, \
        safeToMerge, \
        flag, \
        flagTail, \
        flagHead, \
        platoonId, \
        distanceTravelledCz, \
        intention, \
        counter";

    m_sendLog << header << std::endl;
    m_receiveLog << header << std::endl;

    std::tm tm = {
        0, // tm_sec
        0, // tm_min
        0, // tm_hour
        1, // tm_mday
        0, // tm_mon
        2004 -1900, // tm_year
        4, // tm_wday
        0, // tm_yday
        -1, // tm_isdst
        0, // tm_gmtoff (NOTE: only in glibc)
        nullptr // time zone (NOTE: only in glibc)
        };

    m_timeType2004 = timegm(&tm);
  }
  m_initialised = true;
}

void V2vIclcm::tearDown()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vIclcm::body()
{  
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING){
    std::shared_ptr<opendlv::Buffer> outBuffer(new opendlv::Buffer());
  
    outBuffer->Reversed();
    
    outBuffer->AppendByte(m_messageId); //messageId
    outBuffer->AppendInteger(m_stationId); //stationId
    outBuffer->AppendByte(m_containerMask); //containerMask
    outBuffer->AppendInteger(m_rearAxleLocation); //rearAxleLocation
    outBuffer->AppendInteger(m_controllerType); //controllerType
    outBuffer->AppendInteger(m_responseTimeConstant); //responseTimeConstant
    outBuffer->AppendInteger(m_responseTimeDelay); //responseTimeDelay
    outBuffer->AppendInteger(m_targetLongAcc); //targetLongAcc
    outBuffer->AppendInteger(m_timeHeadway); //timeHeadway
    outBuffer->AppendInteger(m_cruiseSpeed); //cruiseSpeed
    outBuffer->AppendByte(m_lowFrequencyMask); //lowFrequencyMask
    outBuffer->AppendInteger(m_participantsReady); //participantsReady
    outBuffer->AppendInteger(m_startPlatoon); //startPlatoon
    outBuffer->AppendInteger(m_endOfScenario); //endOfScenario
    outBuffer->AppendInteger(m_mioId); //mioId
    outBuffer->AppendInteger(m_mioRange); //mioRange
    outBuffer->AppendInteger(m_mioBearing); //mioBearing
    outBuffer->AppendInteger(m_mioRangeRate); //mioRangeRate
    outBuffer->AppendInteger(m_lane); //lane
    outBuffer->AppendInteger(m_forwardId); //forwardId
    outBuffer->AppendInteger(m_backwardId); //backwardId
    outBuffer->AppendInteger(m_mergeRequest); //mergeRequest
    outBuffer->AppendInteger(m_safeToMerge); //safeToMerge
    outBuffer->AppendInteger(m_flag); //flag
    outBuffer->AppendInteger(m_flagTail); //flagTail
    outBuffer->AppendInteger(m_flagHead); //flagHead
    outBuffer->AppendInteger(m_platoonId); //platoonId
    outBuffer->AppendInteger(m_distanceTravelledCz); //distanceTravelledCz
    outBuffer->AppendInteger(m_intention); //intention
    outBuffer->AppendInteger(m_counter); //counter

    std::vector<unsigned char> bytes = outBuffer->GetBytes();
    std::string bytesString(bytes.begin(),bytes.end());
    // std::cout<< bytesString << std::endl;
    opendlv::knowledge::Message nextMessage(bytesString.size(),bytesString);
    odcore::data::Container c(nextMessage);
    getConference().send(c);

    // std::cout << m_stationId << std::endl;
    // std::cout << m_rearAxleLocation << std::endl;
    // std::cout << m_mioId << std::endl;
    // std::cout << m_backwardId << std::endl;
    // std::cout << m_lane << std::endl;
    // std::cout << m_flagTail << std::endl;
    // std::cout << m_platoonId << std::endl;

    if(m_printOutbound) {
      std::string output = "*** Iclcm object ***\n";
      output += "Message Id: " + std::to_string(m_messageId) + "\n";
      output += "Container mask: " + std::to_string(m_containerMask) + "\n";
      output += "low frequency mask: " + std::to_string(m_lowFrequencyMask) + "\n";
      output += "StationId: " + std::to_string(m_stationId) + "\n";
      output += "Rear axle location: " + std::to_string(m_rearAxleLocation) + "\n";
      output += "Controller type: " + std::to_string(m_controllerType) + "\n";
      output += "Response time constant: " + std::to_string(m_responseTimeConstant) + "\n";
      output += "Response time delay: " + std::to_string(m_responseTimeDelay) + "\n";
      output += "Target long acc: " + std::to_string(m_targetLongAcc) + "\n";
      output += "Time headway: " + std::to_string(m_timeHeadway) + "\n";
      output += "Cruise speed: " + std::to_string(m_cruiseSpeed) + "\n";
      output += "Participants ready: " + std::to_string(m_participantsReady) + "\n";
      output += "Start platoon: " + std::to_string(m_startPlatoon) + "\n";
      output += "End of scenario: " + std::to_string(m_endOfScenario) + "\n";
      output += "Mio Id: " + std::to_string(m_mioId) + "\n";
      output += "Mio range: " + std::to_string(m_mioRange) + "\n";
      output += "Mio bearing: " + std::to_string(m_mioBearing) + "\n";
      output += "Mio range rate: " + std::to_string(m_mioRangeRate) + "\n";
      output += "Lane: " + std::to_string(m_lane) + "\n";
      output += "Forward Id: " + std::to_string(m_forwardId) + "\n";
      output += "Backward Id: " + std::to_string(m_backwardId) + "\n";
      output += "Merge request: " + std::to_string(m_mergeRequest) + "\n";
      output += "Safe to merge: " + std::to_string(m_safeToMerge) + "\n";
      output += "Flag: " + std::to_string(m_flag) + "\n";
      output += "Flag tail: " + std::to_string(m_flagTail) + "\n";
      output += "Flag head: " + std::to_string(m_flagHead) + "\n";
      output += "Platoon Id: " + std::to_string(m_platoonId) + "\n";
      output += "Distance travelled cz: " + std::to_string(m_distanceTravelledCz) + "\n";
      output += "Intention: " + std::to_string(m_intention) + "\n";
      output += "Counter: " + std::to_string(m_counter) + "\n";
      std::cout << output << std::endl;
    }
    // odcore::data::TimeStamp now;
    // if(m_flagHead == 1){
    //   SendInsight("isLeader");
    // }
    // if(m_flag == 1){
    //   m_counterMerge++;
    // }
    // if(m_hasMerged == false && m_counterMerge > (30*25)){
    //   m_hasMerged = true;
    //   m_flag = 0;
    // }
    if(m_record) {
      m_sendLog <<  std::setprecision(15) << std::to_string(GenerateGenerationTime())+
          + "," + std::to_string(m_messageId)+ //messageId
          + "," + std::to_string(m_stationId)+ //stationId
          + "," + std::to_string(m_containerMask)+ //containerMask
          + "," + std::to_string(m_rearAxleLocation)+ //rearAxleLocation
          + "," + std::to_string(m_controllerType)+ //controllerType
          + "," + std::to_string(m_responseTimeConstant)+ //responseTimeConstant
          + "," + std::to_string(m_responseTimeDelay)+ //responseTimeDelay
          + "," + std::to_string(m_targetLongAcc)+ //targetLongAcc
          + "," + std::to_string(m_timeHeadway)+ //timeHeadway
          + "," + std::to_string(m_cruiseSpeed)+ //cruiseSpeed
          + "," + std::to_string(m_lowFrequencyMask)+ //lowFrequencyMask
          + "," + std::to_string(m_participantsReady)+ //participantsReady
          + "," + std::to_string(m_startPlatoon)+ //startPlatoon
          + "," + std::to_string(m_endOfScenario)+ //endOfScenario
          + "," + std::to_string(m_mioId)+ //mioId
          + "," + std::to_string(m_mioRange)+ //mioRange
          + "," + std::to_string(m_mioBearing)+ //mioBearing
          + "," + std::to_string(m_mioRangeRate)+ //mioRangeRate
          + "," + std::to_string(m_lane)+ //lane
          + "," + std::to_string(m_forwardId)+ //forwardId
          + "," + std::to_string(m_backwardId)+ //backwardId
          + "," + std::to_string(m_mergeRequest)+ //mergeRequest
          + "," + std::to_string(m_safeToMerge)+ //safeToMerge
          + "," + std::to_string(m_flag)+ //flag
          + "," + std::to_string(m_flagTail)+ //flagTail
          + "," + std::to_string(m_flagHead)+ //flagHead
          + "," + std::to_string(m_platoonId)+ //platoonId
          + "," + std::to_string(m_distanceTravelledCz)+ //distanceTravelledCz
          + "," + std::to_string(m_intention)+ //intention
          + "," + std::to_string(m_counter); //counter

      m_sendLog << std::endl;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void V2vIclcm::ReadInsight(opendlv::knowledge::Insight &a_insight)
{
  // std::cout<< "Read "<< std::endl;
  std::string str = a_insight.getInsight();

  // std::cout<< str << std::endl;
  std::vector<std::string> information = odcore::strings::StringToolbox::split(str,'=');
  // std::cout<< information.size() << std::endl;
  
  if (information.size() == 0){
    if (str == "mergeScenario") {
      m_scenario = str;
    }
    else if (str == "intersectionScenario") {
      m_scenario = str;
    }
  }
  else {
    if(information[0] == "stationId"){
      m_stationId = std::stoi(information[1]);
    } else if(information[0] == "rearAxleLocation"){
      m_rearAxleLocation = static_cast<int32_t>(std::stod(information[1])*100);
    } else if (information[0] == "mioId") {
      m_mioId = std::stoi(information[1]);
    } else if (information[0] == "mioRange"){
      m_mioRange =  static_cast<int32_t>(std::stod(information[1])*100);
      if(m_mioRange < 0 || m_mioRange > 65534){
        m_mioRange = 65535;
      }
    } else if (information[0] == "mioBearing"){
      m_mioBearing = static_cast<int32_t>(std::stod(information[1])/0.002);
      if(m_mioBearing < -1571 || m_mioBearing > 1571){
        m_mioBearing = 1572;
      }
    } else if (information[0] == "mioRangeRate") {
      m_mioRangeRate = static_cast<int32_t>(std::stod(information[1])/0.01);
      if (m_mioRangeRate < -32766 || m_mioRangeRate > 32766){
        m_mioRangeRate = 32767;
      }
    } else if (information[0] == "forwardId"){
      m_forwardId = std::stoi(information[1]);
    } else if (information[0] == "currentLane") {
      m_lane = std::stoi(information[1]);
    } else if (information[0] == "isHead") {
      m_flagHead = std::stoi(information[1]);
    } else if (information[0] == "isTail")  {
      m_flagTail = std::stoi(information[1]);
    } else if (information[0] == "platoonId") {
      m_platoonId = std::stoi(information[1]);
    } else if (information[0] == "intention") {
      m_intention = std::stoi(information[1]);
    } else if (information[0] == "safeToMerge") {
      m_safeToMerge = std::stoi(information[1]);
    } else if (information[0] == "mergeFlag") {
      m_flag = std::stoi(information[1]);
      if(m_flagHead == 1 && m_flag == 1){
        m_flagHead = 0;
        m_intention = 3;
      }
    } else if (information[0] == "intersectionVehicleCounter") {
      m_counter = std::stoi(information[1]);
    } else if (information[0] == "distanceTravelled"){
      m_distanceTravelledCz = static_cast<int32_t>(std::stod(information[1])/0.1)%10000;
    } else if (information[0] == "timeHeadway"){
      m_timeHeadway = static_cast<int32_t>(std::stod(information[1])/0.1);
      if(m_timeHeadway < 0 || m_timeHeadway > 360){
        m_timeHeadway = 361;
      }
    } 
  }
}

void V2vIclcm::ReadVoice(opendlv::sensation::Voice &a_message){
  std::string dataString = a_message.getData();
  std::vector<unsigned char> data(dataString.begin(), dataString.end());

  std::shared_ptr<Buffer const> buffer(new Buffer(data));
  // std::cout<< buffer->GetSize()<<std::endl;
  std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
  //Long and little endian reverser
  inIterator->ItReversed();


  unsigned char messageId = inIterator->ReadByte();
  int32_t stationId = inIterator->ReadInteger();
  unsigned char containerMask = inIterator->ReadByte();
  int32_t rearAxleLocation = inIterator->ReadInteger();
  int32_t controllerType = inIterator->ReadInteger();
  int32_t responseTimeConstant = inIterator->ReadInteger();
  int32_t responseTimeDelay = inIterator->ReadInteger();
  int32_t targetLongAcc = inIterator->ReadInteger();
  int32_t timeHeadway = inIterator->ReadInteger();
  int32_t cruiseSpeed = inIterator->ReadInteger();

  unsigned char lowFrequencyMask = inIterator->ReadByte();
  int32_t participantsReady = inIterator->ReadInteger();
  int32_t startPlatoon = inIterator->ReadInteger();
  int32_t endOfScenario = inIterator->ReadInteger();
  

  int32_t mioId = inIterator->ReadInteger();
  int32_t mioRange = inIterator->ReadInteger();
  int32_t mioBearing = inIterator->ReadInteger();
  int32_t mioRangeRate = inIterator->ReadInteger();
  int32_t lane = inIterator->ReadInteger();
  int32_t forwardId = inIterator->ReadInteger();
  int32_t backwardId = inIterator->ReadInteger();
  int32_t mergeRequest = inIterator->ReadInteger();
  int32_t safeToMerge = inIterator->ReadInteger();
  int32_t flag = inIterator->ReadInteger();
  int32_t flagTail = inIterator->ReadInteger();
  int32_t flagHead = inIterator->ReadInteger();
  int32_t platoonId = inIterator->ReadInteger();

  int32_t distanceTravelledCz = inIterator->ReadInteger();
  
  int32_t intention = inIterator->ReadInteger();
  int32_t counter = inIterator->ReadInteger();

  if(m_printInbound) {
    std::string output = "*** Iclcm object ***\n";

    output += "Message Id: " + std::to_string(messageId) + "\n";
    output += "Container mask: " + std::to_string(containerMask) + "\n";
    output += "low frequency mask: " + std::to_string(lowFrequencyMask) + "\n";
    output += "StationId: " + std::to_string(stationId) + "\n";
    output += "Rear axle location: " + std::to_string(rearAxleLocation) + "\n";
    output += "Controller type: " + std::to_string(controllerType) + "\n";
    output += "Response time constant: " + std::to_string(responseTimeConstant) + "\n";
    output += "Response time delay: " + std::to_string(responseTimeDelay) + "\n";
    output += "Target long acc: " + std::to_string(targetLongAcc) + "\n";
    output += "Time headway: " + std::to_string(timeHeadway) + "\n";
    output += "Cruise speed: " + std::to_string(cruiseSpeed) + "\n";
    output += "Participants ready: " + std::to_string(participantsReady) + "\n";
    output += "Start platoon: " + std::to_string(startPlatoon) + "\n";
    output += "End of scenario: " + std::to_string(endOfScenario) + "\n";
    output += "Mio Id: " + std::to_string(mioId) + "\n";
    output += "Mio range: " + std::to_string(mioRange) + "\n";
    output += "Mio bearing: " + std::to_string(mioBearing) + "\n";
    output += "Mio range rate: " + std::to_string(mioRangeRate) + "\n";
    output += "Lane: " + std::to_string(lane) + "\n";
    output += "Forward Id: " + std::to_string(forwardId) + "\n";
    output += "Backward Id: " + std::to_string(backwardId) + "\n";
    output += "Merge request: " + std::to_string(mergeRequest) + "\n";
    output += "Safe to merge: " + std::to_string(safeToMerge) + "\n";
    output += "Flag: " + std::to_string(flag) + "\n";
    output += "Flag tail: " + std::to_string(flagTail) + "\n";
    output += "Flag head: " + std::to_string(flagHead) + "\n";
    output += "Platoon Id: " + std::to_string(platoonId) + "\n";
    output += "Distance travelled cz: " + std::to_string(distanceTravelledCz) + "\n";
    output += "Intention: " + std::to_string(intention) + "\n";
    output += "Counter: " + std::to_string(counter) + "\n";
  }

  // if((stationId < 100)){
  //   std::cout << output << std::endl;
  // }
  // if(stationId < 100){
  //   SendInsight("cruiseSpeed="+std::to_string(cruiseSpeed/100.0));
  //   if (participantsReady == 1){
  //     std::cout<< "Got participantsReady flag from "<< stationId << std::endl;
  //   }
  

  //   if (mergeRequest == 1) {
  //     SendInsight("mergeRequest");
  //     std::cout<< "Got mergeRequest flag from "<< stationId << std::endl;
  //   }

  //   if (endOfScenario == 1) {
  //     SendInsight("scenarioEnd");
  //     std::cout << "Got end of scenario message from " << stationId << std::endl;
  //   }
  // }
  // if(m_scenario == "mergeScenario" && m_hasMerged == false){
  //   if ((stationId < 100) &&( (startPlatoon == 0 && m_platoonId == 1) || (startPlatoon == 1 && m_platoonId == 2))) {
  //     SendInsight("scenarioReady");
  //     std::cout << "Got startPlatoon flag from " << stationId << std::endl;
  //   }
  //   if(lane == 1){
  //     if(forwardId == 110){
  //       std::cout << "Backward partner found: " << stationId << "." << std::endl;
  //       m_backwardId = stationId;
  //     }
  //     if(m_forwardId == stationId && flagHead == 1){
  //       std::cout << "Creating distance for station: " << stationId << std::endl;
  //       SendInsight("createDistance");
  //     }
  //     if(stationId == m_forwardId && flag == 1){
  //       std::cout << "Forward parther should be merging: " << stationId << "." << std::endl;
  //     }
  //   }
  //   if(m_lane == 2){

  //     if(stationId == m_mioId){
  //       if(flagHead == 1){
  //         std::cout << "Mio became leader." << std::endl;
  //         m_mioBeenLeader = true;

  //       }
  //       if(m_mioBeenLeader && (flagHead == 0 || lane == 1 || platoonId == 2)){
  //         std::cout << "Mio has merged." << std::endl;
  //         m_flagHead = 1;
  //       }
  //     }
  //     if (forwardId == 110 && m_flagHead == 1){
  //       std::cout << "Backward partner found: " << stationId << "." << std::endl;
  //       m_backwardId = stationId;
  //       if(safeToMerge == 1){
  //         std::cout << "Backward partner says safe to merge." << std::endl;
  //         SendInsight("safeToMerge");
  //       }
  //     }
  //   }
  // }
  // if(m_scenario == "intersectionScenario"){
  if(stationId < 100) {
    if(participantsReady == 1){
      std::cout << "Got participantsReady flag from " << stationId << std::endl;
      SendInsight("scenarioReady");
    }
    if(platoonId == 1){
      std::cout << "Got startPlatoon flag from " << stationId << std::endl;
      SendInsight("scenarioReady");
    }
    if(endOfScenario == 1){
      std::cout << "Got end of scenario message from " << stationId << std::endl;
      SendInsight("scenarioEnd");

    }
  } 
  // } 

  if(m_record){
    m_receiveLog <<  std::setprecision(15) << std::to_string(GenerateGenerationTime())+
        + "," + std::to_string(messageId)+ //messageId
        + "," + std::to_string(stationId)+ //stationId
        + "," + std::to_string(containerMask)+ //containerMask
        + "," + std::to_string(rearAxleLocation)+ //rearAxleLocation
        + "," + std::to_string(controllerType)+ //controllerType
        + "," + std::to_string(responseTimeConstant)+ //responseTimeConstant
        + "," + std::to_string(responseTimeDelay)+ //responseTimeDelay
        + "," + std::to_string(targetLongAcc)+ //targetLongAcc
        + "," + std::to_string(timeHeadway)+ //timeHeadway
        + "," + std::to_string(cruiseSpeed)+ //cruiseSpeed
        + "," + std::to_string(lowFrequencyMask)+ //lowFrequencyMask
        + "," + std::to_string(participantsReady)+ //participantsReady
        + "," + std::to_string(startPlatoon)+ //startPlatoon
        + "," + std::to_string(endOfScenario)+ //endOfScenario
        + "," + std::to_string(mioId)+ //mioId
        + "," + std::to_string(mioRange)+ //mioRange
        + "," + std::to_string(mioBearing)+ //mioBearing
        + "," + std::to_string(mioRangeRate)+ //mioRangeRate
        + "," + std::to_string(lane)+ //lane
        + "," + std::to_string(forwardId)+ //forwardId
        + "," + std::to_string(backwardId)+ //backwardId
        + "," + std::to_string(mergeRequest)+ //mergeRequest
        + "," + std::to_string(safeToMerge)+ //safeToMerge
        + "," + std::to_string(flag)+ //flag
        + "," + std::to_string(flagTail)+ //flagTail
        + "," + std::to_string(flagHead)+ //flagHead
        + "," + std::to_string(platoonId)+ //platoonId
        + "," + std::to_string(distanceTravelledCz)+ //distanceTravelledCz
        + "," + std::to_string(intention)+ //intention
        + "," + std::to_string(counter); //counter
        
    m_receiveLog << std::endl;
  }
}

/**
 * Receives .
 * Sends .
 */
void V2vIclcm::nextContainer(odcore::data::Container &a_c)
{
  if (a_c.getDataType() == opendlv::knowledge::Insight::ID()){
    opendlv::knowledge::Insight insight = 
        a_c.getData<opendlv::knowledge::Insight>();

    ReadInsight(insight);
  } else if (a_c.getDataType() == opendlv::proxy::ControlState::ID()) {
    opendlv::proxy::ControlState controlState =
        a_c.getData<opendlv::proxy::ControlState>(); 
    if (controlState.getIsAutonomous()){
      m_controllerType = 3;
    } else {
      m_controllerType = 1;
    }
  } else if (a_c.getDataType() == opendlv::sensation::Voice::ID()) {
    opendlv::sensation::Voice message = a_c.getData<opendlv::sensation::Voice>();
    // std::cout<<message.getType()<<std::endl;
    if(strcmp(message.getType().c_str(),"iclcm") != 0){
      return;
    }
    ReadVoice(message);
  }  
}

void V2vIclcm::SendInsight(std::string const &a_msg){
  odcore::data::TimeStamp now;
  opendlv::knowledge::Insight insight(now,a_msg);
  odcore::data::Container c(insight, opendlv::knowledge::Insight::ID()+300 );
  getConference().send(c);
}



uint64_t V2vIclcm::GenerateGenerationTime() const
{
  std::chrono::system_clock::time_point start2004TimePoint = 
      std::chrono::system_clock::from_time_t(m_timeType2004);
  uint64_t millisecondsSince2004Epoch =
      std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1) 
      - start2004TimePoint.time_since_epoch() / std::chrono::milliseconds(1);
  return millisecondsSince2004Epoch;
}

} // v2viclcm
} // gcdc16
} // knowledge
} // opendlv
