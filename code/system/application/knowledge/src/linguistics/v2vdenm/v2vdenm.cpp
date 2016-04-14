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

#include <cstdlib>
#include <chrono>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
 
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "linguistics/v2vcam/buffer.hpp"
#include "linguistics/v2vdenm/v2vdenm.hpp"

namespace opendlv {
namespace knowledge {
namespace linguistics {
namespace v2vdenm {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vDenm::V2vDenm(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "knowledge-linguistics-v2vdenm"),
    m_sendLog(),
    m_receiveLog()
{

  struct stat st;

  if (stat("var/application/knowledge/linguistics/v2vdenm", &st) == -1) {
    ::system("mkdir -p ./var/application/knowledge/linguistics/v2vdenm");
    // std::cout<<"Created dir"<<std::endl;
  }
  odcore::data::TimeStamp nu;

  std::stringstream filenameSend;
  std::stringstream filenameReceive;

  filenameSend << "/" << nu.getYYYYMMDD_HHMMSS() << " denm send.log";
  filenameReceive << "/" << nu.getYYYYMMDD_HHMMSS() << " denm receive.log";
  m_sendLog.open("var/application/knowledge/linguistics/v2vdenm" 
    + filenameSend.str(), std::ios::out | std::ios::app);
  m_receiveLog.open("var/application/knowledge/linguistics/v2vdenm" 
    + filenameReceive.str(), std::ios::out | std::ios::app);

  std::string header =  "#messageId, \
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

  m_sendLog << header << std::endl;
  m_receiveLog << header << std::endl;
}

V2vDenm::~V2vDenm()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vDenm::body()
{
/*

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    // unsigned long millisecondsSince2004Epoch =
    //     std::chrono::system_clock::now().time_since_epoch() /
    //     std::chrono::milliseconds(1) - millisecondsTo2004FromUnixEpoch;
    std::shared_ptr<opendlv::Buffer> outBuffer(new opendlv::Buffer());
    // m_generationDeltaTime = millisecondsSince2004Epoch%65536;
    // m_referenceTime = millisecondsSince2004Epoch;
    
    // std::cout<< std::chrono::system_clock::now().time_since_epoch() /
    //     std::chrono::milliseconds(1) << "\n" << millisecondsTo2004FromUnixEpoch<< std::endl;
    // std::cout<< "reference: " << m_referenceTime << std::endl;

    outBuffer->Reversed();

    outBuffer->AppendByte(m_messageId); //messageId
    outBuffer->AppendInteger(m_stationId); //stationId
    outBuffer->AppendInteger(m_generationDeltaTime); //generationDeltaTime
    
    outBuffer->AppendByte(m_containerMask); //containerMask
      
    outBuffer->AppendByte(m_managementMask); //managementMask
    outBuffer->AppendInteger(m_detectionTime); //detectionTime
    //Todo fix this - Error in VA
    outBuffer->AppendInteger(m_referenceTime); //referenceTime
    outBuffer->AppendInteger(m_termination); //termination
    outBuffer->AppendInteger(m_latitude); //latitude
    outBuffer->AppendInteger(m_longitude); //longitude
    outBuffer->AppendInteger(m_semiMajorConfidence); //semiMajorConfidence
    outBuffer->AppendInteger(m_semiMinorConfidence); //semiMinorConfidence
    outBuffer->AppendInteger(m_semiMajorOrientation); //semiMajorOrientation
    outBuffer->AppendInteger(m_altitude); //altitude
    outBuffer->AppendInteger(m_relevanceDistance); //relevanceDistance
    outBuffer->AppendInteger(m_relevanceTrafficDirection); //relevanceTrafficDirection
    outBuffer->AppendInteger(m_validityDuration); //validityDuration
    outBuffer->AppendInteger(m_transmissionInterval); //transmissionInterval
    outBuffer->AppendInteger(m_stationType); //stationType
    outBuffer->AppendByte(m_situationMask); //situationMask
    outBuffer->AppendInteger(m_informationQuality); //informationQuality
    outBuffer->AppendInteger(m_causeCode); //causeCode
    outBuffer->AppendInteger(m_subCauseCode); //subCauseCode
    outBuffer->AppendInteger(m_linkedCauseCode); //linkedCauseCode
    outBuffer->AppendInteger(m_linkedSubCauseCode); //linkedSubCauseCode
    outBuffer->AppendByte(m_alacarteMask); //alacarteMask
    outBuffer->AppendInteger(m_lanePosition); //lanePosition
    outBuffer->AppendInteger(m_temperature); //temperature
    outBuffer->AppendInteger(m_positioningSolutionType); //positioningSolutionType

    std::vector<unsigned char> bytes = outBuffer->GetBytes();
    // std::cout<< outBuffer->GetSize() << std::endl;
    std::string bytesString(bytes.begin(),bytes.end());
    // std::cout<< bytesString << std::endl;
    opendlv::knowledge::Message nextMessage(bytesString.size(),bytesString);
    odcore::data::Container c(nextMessage);
    getConference().send(c);
    // std::cout << "Sent." << std::endl;

    m_sendLog<<std::to_string(m_messageId)+ //messageId
        + "," + std::to_string(m_stationId)+ //stationId
        + "," + std::to_string(m_generationDeltaTime)+ //generationDeltaTime
        + "," + std::to_string(m_containerMask)+ //containerMask
        + "," + std::to_string(m_managementMask)+ //managementMask
        + "," + std::to_string(m_detectionTime)+ //detectionTime
        + "," + std::to_string(m_referenceTime)+ //referenceTime
        + "," + std::to_string(m_termination)+ //termination
        + "," + std::to_string(m_latitude)+ //latitude
        + "," + std::to_string(m_longitude)+ //longitude
        + "," + std::to_string(m_semiMajorConfidence)+ //semiMajorConfidence
        + "," + std::to_string(m_semiMinorConfidence)+ //semiMinorConfidence
        + "," + std::to_string(m_semiMajorOrientation)+ //semiMajorOrientation
        + "," + std::to_string(m_altitude)+ //altitude
        + "," + std::to_string(m_relevanceDistance)+ //relevanceDistance
        + "," + std::to_string(m_relevanceTrafficDirection)+ //relevanceTrafficDirection
        + "," + std::to_string(m_validityDuration)+ //validityDuration
        + "," + std::to_string(m_transmissionInterval)+ //transmissionInterval
        + "," + std::to_string(m_stationType)+ //stationType
        + "," + std::to_string(m_situationMask)+ //situationMask
        + "," + std::to_string(m_informationQuality)+ //informationQuality
        + "," + std::to_string(m_causeCode)+ //causeCode
        + "," + std::to_string(m_subCauseCode)+ //subCauseCode
        + "," + std::to_string(m_linkedCauseCode)+ //linkedCauseCode
        + "," + std::to_string(m_linkedSubCauseCode)+ //linkedSubCauseCode
        + "," + std::to_string(m_alacarteMask)+ //alacarteMask
        + "," + std::to_string(m_lanePosition)+ //lanePosition
        + "," + std::to_string(m_temperature)+ //temperature
        + "," + std::to_string(m_positioningSolutionType);
    m_sendLog<< std::endl; //positioningSolutionType



  }
*/

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Receives .
 * Sends .
 */
void V2vDenm::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::sensation::Voice::ID())
  {
    opendlv::sensation::Voice message = c.getData<opendlv::sensation::Voice>();
    // std::cout<<message.getType()<<std::endl;
    if(strcmp(message.getType().c_str(),"denm") == 0)
    {
      std::string dataString = message.getData();
      std::vector<unsigned char> data(dataString.begin(), dataString.end());
    
      // std::cout<< "Received CAM \n";
      std::shared_ptr<Buffer const> buffer(new Buffer(data));
      // std::cout<< buffer->GetSize()<<std::endl;
      std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
      //Long and little endian reverser
      inIterator->ItReversed();
      
      //Print the information 
      // std::vector<unsigned char> const bytes = v;
      // std::stringstream ss;
      // for (uint i = 0; i < bytes.size(); i++) {
      //     ss << std::to_string(bytes.at(i));
      //     ss << "|";
      // }
      // std::cout<<ss.str()<<std::endl;


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

      // std::string output = "*** Denm object ***\n";
      // output += "Message Id: " + std::to_string(messageId) + "\n";
      // output += "Station Id: " + std::to_string(stationId) + "\n";
      // output += "Generation delta time: " + std::to_string(generationDeltaTime) + "\n";
      // output += "Container mask: " + std::to_string(containerMask) + "\n";
      // output += "Management mask: " + std::to_string(managementMask) + "\n";
      // output += "Situation mask: " + std::to_string(situationMask) + "\n";
      // output += "Alacarte mask: " + std::to_string(alacarteMask) + "\n";
      // output += "Detection time: " + std::to_string(detectionTime) + "\n";
      // output += "Reference time: " + std::to_string(referenceTime) + "\n";
      // output += "Termination: " + std::to_string(termination) + "\n";
      // output += "Latitude: " + std::to_string(latitude) + "\n";
      // output += "Longitude: " + std::to_string(longitude) + "\n";
      // output += "Semi major confidence: " + std::to_string(semiMajorConfidence) + "\n";
      // output += "Semi minor confidence: " + std::to_string(semiMinorConfidence) + "\n";
      // output += "Semi major orientation: " + std::to_string(semiMajorOrientation) + "\n";
      // output += "Altitude: " + std::to_string(altitude) + "\n";
      // output += "Relevance distance: " + std::to_string(relevanceDistance) + "\n";
      // output += "Relevance traffic direction: " + std::to_string(relevanceTrafficDirection) + "\n";
      // output += "Validity duration: " + std::to_string(validityDuration) + "\n";
      // output += "Transmission interval: " + std::to_string(transmissionInterval) + "\n";
      // output += "Station type: " + std::to_string(stationType) + "\n";
      // output += "Information quality: " + std::to_string(informationQuality) + "\n";
      // output += "Cause code: " + std::to_string(causeCode) + "\n";
      // output += "Sub cause code: " + std::to_string(subCauseCode) + "\n";
      // output += "Linked cause code: " + std::to_string(linkedCauseCode) + "\n";
      // output += "Linked sub cause code: " + std::to_string(linkedSubCauseCode) + "\n";
      // output += "Lane position: " + std::to_string(lanePosition) + "\n";
      // output += "Temperature: " + std::to_string(temperature) + "\n";
      // output += "Positioning solution type: " + std::to_string(positioningSolutionType) + "\n";
      // std::cout << output;


      m_receiveLog<<std::to_string(messageId)+ //messageId
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

    }
  }
}

void V2vDenm::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_stationId = kv.getValue<int32_t>("knowledge-linguistics-v2vdenm.stationId");
  m_stationType = kv.getValue<int32_t>("knowledge-linguistics-v2vdenm.stationType");

}

void V2vDenm::tearDown()
{
}

} // v2vdenm
} // linguistics
} // knowledge
} // opendlv
