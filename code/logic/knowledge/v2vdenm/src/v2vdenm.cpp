/**
 * Copyright (C) 2017 Chalmers Revere
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
 
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "buffer.hpp"
#include "v2vdenm.hpp"

namespace opendlv {
namespace logic {
namespace knowledge {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vDenm::V2vDenm(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "logic-knowledge-v2vdenm"),
    // m_sendLog(),
    m_receiveLog(),
    m_timeType2004()
{
  struct stat st;
  if (::stat("var/application/knowledge/linguistics/v2vdenm", &st) == -1) {
    ::system("mkdir -p ./var/application/knowledge/linguistics/v2vdenm");
    std::cout<<"Created dir"<<std::endl;
  }
  odcore::data::TimeStamp now;

  // std::stringstream filenameSend;
  std::stringstream filenameReceive;

  // filenameSend << "/" << now.getYYYYMMDD_HHMMSS() << " denm send.log";
  filenameReceive << "/" << now.getYYYYMMDD_HHMMSS() << " denm receive.log";
  // m_sendLog.open("var/application/knowledge/linguistics/v2vdenm" 
  //   + filenameSend.str(), std::ios::out | std::ios::app);
  m_receiveLog.open("var/application/knowledge/linguistics/v2vdenm" 
    + filenameReceive.str(), std::ios::out | std::ios::app);

  std::string header =  "#time, \
      messageId, \
      stationId, \
      generationDeltaTime, \
      containerMask, \
      managementMask, \
      detectionTime, \
      referenceTime, \
      termination, \
      latitude, \
      longitude, \
      semiMajorConfidence, \
      semiMinorConfidence, \
      semiMajorOrientation, \
      altitude, \
      relevanceDistance, \
      relevanceTrafficDirection, \
      validityDuration, \
      transmissionInterval, \
      stationType, \
      situationMask, \
      informationQuality, \
      causeCode, \
      subCauseCode, \
      linkedCauseCode, \
      linkedSubCauseCode, \
      alacarteMask, \
      lanePosition, \
      temperature, \
      positioningSolutionType";

  // m_sendLog << header << std::endl;
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

V2vDenm::~V2vDenm()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vDenm::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void V2vDenm::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() == opendlv::sensation::Voice::ID())
  {
    opendlv::sensation::Voice message = a_c.getData<opendlv::sensation::Voice>();
    if(strcmp(message.getType().c_str(),"denm") != 0)
    {
      return;
    }
    std::string dataString = message.getData();
    std::vector<unsigned char> data(dataString.begin(), dataString.end());
  
    std::shared_ptr<Buffer const> buffer(new Buffer(data));
    std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
    //Long and little endian reverser
    inIterator->ItReversed();


    unsigned char messageId = inIterator->ReadByte();
    int32_t stationId = inIterator->ReadInteger();
    int32_t generationDeltaTime = inIterator->ReadInteger();
    unsigned char containerMask =  inIterator->ReadByte();
    unsigned char managementMask = inIterator->ReadByte();
    int32_t detectionTime = inIterator->ReadInteger();
    int32_t referenceTime = inIterator->ReadInteger();
    int32_t termination = inIterator->ReadInteger();
    int32_t latitude = inIterator->ReadInteger();
    int32_t longitude = inIterator->ReadInteger();
    int32_t semiMajorConfidence = inIterator->ReadInteger();
    int32_t semiMinorConfidence = inIterator->ReadInteger();
    int32_t semiMajorOrientation = inIterator->ReadInteger();
    int32_t altitude = inIterator->ReadInteger();
    int32_t relevanceDistance = inIterator->ReadInteger();
    int32_t relevanceTrafficDirection = inIterator->ReadInteger();
    int32_t validityDuration = inIterator->ReadInteger();
    int32_t transmissionInterval = inIterator->ReadInteger();
    int32_t stationType = inIterator->ReadInteger();
    int32_t situationMask = inIterator->ReadByte();
    int32_t informationQuality = inIterator->ReadInteger();
    int32_t causeCode = inIterator->ReadInteger();
    int32_t subCauseCode = inIterator->ReadInteger();
    int32_t linkedCauseCode = inIterator->ReadInteger();
    int32_t linkedSubCauseCode = inIterator->ReadInteger();
    unsigned char alacarteMask = inIterator->ReadByte();
    int32_t lanePosition = inIterator->ReadInteger();
    int32_t temperature = inIterator->ReadInteger();
    int32_t positioningSolutionType = inIterator->ReadInteger();



    std::string output = "*** Denm object ***\n";
    output += "Message Id: " + std::to_string(messageId) + "\n";
    output += "Station Id: " + std::to_string(stationId) + "\n";
    output += "Generation delta time: " + std::to_string(generationDeltaTime) + "\n";
    output += "Container mask: " + std::to_string(containerMask) + "\n";
    output += "Management mask: " + std::to_string(managementMask) + "\n";
    output += "Situation mask: " + std::to_string(situationMask) + "\n";
    output += "Alacarte mask: " + std::to_string(alacarteMask) + "\n";
    output += "Detection time: " + std::to_string(detectionTime) + "\n";
    output += "Reference time: " + std::to_string(referenceTime) + "\n";
    output += "Termination: " + std::to_string(termination) + "\n";
    output += "Latitude: " + std::to_string(latitude) + "\n";
    output += "Longitude: " + std::to_string(longitude) + "\n";
    output += "Semi major confidence: " + std::to_string(semiMajorConfidence) + "\n";
    output += "Semi minor confidence: " + std::to_string(semiMinorConfidence) + "\n";
    output += "Semi major orientation: " + std::to_string(semiMajorOrientation) + "\n";
    output += "Altitude: " + std::to_string(altitude) + "\n";
    output += "Relevance distance: " + std::to_string(relevanceDistance) + "\n";
    output += "Relevance traffic direction: " + std::to_string(relevanceTrafficDirection) + "\n";
    output += "Validity duration: " + std::to_string(validityDuration) + "\n";
    output += "Transmission interval: " + std::to_string(transmissionInterval) + "\n";
    output += "Station type: " + std::to_string(stationType) + "\n";
    output += "Information quality: " + std::to_string(informationQuality) + "\n";
    output += "Cause code: " + std::to_string(causeCode) + "\n";
    output += "Sub cause code: " + std::to_string(subCauseCode) + "\n";
    output += "Linked cause code: " + std::to_string(linkedCauseCode) + "\n";
    output += "Linked sub cause code: " + std::to_string(linkedSubCauseCode) + "\n";
    output += "Lane position: " + std::to_string(lanePosition) + "\n";
    output += "Temperature: " + std::to_string(temperature) + "\n";
    output += "Positioning solution type: " + std::to_string(positioningSolutionType) + "\n";
    // std::cout << output;


    m_receiveLog <<  std::setprecision(15) << std::to_string(GenerateGenerationTime())+
        + "," + std::to_string(messageId)+ //messageId
        + "," + std::to_string(stationId)+ //stationId
        + "," + std::to_string(generationDeltaTime)+ //generationDeltaTime
        + "," + std::to_string(containerMask)+ //containerMask
        + "," + std::to_string(managementMask)+ //managementMask
        + "," + std::to_string(detectionTime)+ //detectionTime
        + "," + std::to_string(referenceTime)+ //referenceTime
        + "," + std::to_string(termination)+ //termination
        + "," + std::to_string(latitude)+ //latitude
        + "," + std::to_string(longitude)+ //longitude
        + "," + std::to_string(semiMajorConfidence)+ //semiMajorConfidence
        + "," + std::to_string(semiMinorConfidence)+ //semiMinorConfidence
        + "," + std::to_string(semiMajorOrientation)+ //semiMajorOrientation
        + "," + std::to_string(altitude)+ //altitude
        + "," + std::to_string(relevanceDistance)+ //relevanceDistance
        + "," + std::to_string(relevanceTrafficDirection)+ //relevanceTrafficDirection
        + "," + std::to_string(validityDuration)+ //validityDuration
        + "," + std::to_string(transmissionInterval)+ //transmissionInterval
        + "," + std::to_string(stationType)+ //stationType
        + "," + std::to_string(situationMask)+ //situationMask
        + "," + std::to_string(informationQuality)+ //informationQuality
        + "," + std::to_string(causeCode)+ //causeCode
        + "," + std::to_string(subCauseCode)+ //subCauseCode
        + "," + std::to_string(linkedCauseCode)+ //linkedCauseCode
        + "," + std::to_string(linkedSubCauseCode)+ //linkedSubCauseCode
        + "," + std::to_string(alacarteMask)+ //alacarteMask
        + "," + std::to_string(lanePosition)+ //lanePosition
        + "," + std::to_string(temperature)+ //temperature
        + "," + std::to_string(positioningSolutionType); //positioningSolutionType
    m_receiveLog << std::endl;

    odcore::data::TimeStamp now;
    opendlv::knowledge::Insight rsuEvent(now,"rsuEvent");
    odcore::data::Container c(rsuEvent, opendlv::knowledge::Insight::ID() + 300);
    getConference().send(c);

  }
}

void V2vDenm::setUp()
{
}

void V2vDenm::tearDown()
{
}


uint64_t V2vDenm::GenerateGenerationTime() const
{
  std::chrono::system_clock::time_point start2004TimePoint = 
      std::chrono::system_clock::from_time_t(m_timeType2004);
  uint64_t millisecondsSince2004Epoch =
      std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1) 
      - start2004TimePoint.time_since_epoch() / std::chrono::milliseconds(1);
  return millisecondsSince2004Epoch;
}

}
}
}
