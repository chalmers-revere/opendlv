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

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "linguistics/v2vcam/buffer.hpp"
#include "linguistics/v2vcam/v2vcam.hpp"


namespace opendlv {
namespace knowledge {
namespace linguistics {
namespace v2vcam {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vCam::V2vCam(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "knowledge-linguistics-v2vcam")
{
}

V2vCam::~V2vCam()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vCam::body()
{
  unsigned long const millisecondsTo2004FromUnixEpoch = 1072915200000;
  unsigned char const m_messageId = 2;
  int32_t m_stationId = 0;
  // Generation time of the (CAM) message in milliseconds
  int32_t m_generationDeltaTime = 0; 
  unsigned char m_containerMask = 128;
  int32_t m_stationType = 0;
  int32_t m_latitude = 900000001;
  int32_t m_longitude = 1800000001;
  int32_t m_semiMajorConfidence = 4095;
  int32_t m_semiMinorConfidence = 4095;
  int32_t m_semiMajorOrientation = 4095;
  int32_t m_altitude = 800001;
  int32_t m_heading = 3601;
  int32_t m_headingConfidence = 127;
  int32_t m_speed = 16383; //
  int32_t m_speedConfidence = 127;
  int32_t m_vehicleLength = 0;
  int32_t m_vehicleWidth = 0;
  int32_t m_longitudinalAcc = 161;
  int32_t m_longitudinalAccConf = 1001;
  int32_t m_yawRateValue = 32767;
  int32_t m_yawRateConfidence = 8;
  int32_t m_vehicleRole = 0;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    // std::cout << "Inside the main processing loop." << std::endl;
    unsigned long millisecondsSince2004Epoch =
        std::chrono::system_clock::now().time_since_epoch() /
        std::chrono::milliseconds(1) - millisecondsTo2004FromUnixEpoch;
    m_generationDeltaTime = millisecondsSince2004Epoch%65536;
    std::shared_ptr<opendlv::Buffer> outBuffer(new opendlv::Buffer());
    // Reverser for big and little endian specification of V2V.
    outBuffer->Reversed();
    outBuffer->AppendByte(m_messageId); //messageId
    outBuffer->AppendInteger(m_stationId); //stationId
    outBuffer->AppendInteger(m_generationDeltaTime); //generationDeltaTime
    outBuffer->AppendByte(m_containerMask); //containerMask
    outBuffer->AppendInteger(m_stationType); //stationType                
    outBuffer->AppendInteger(m_latitude); //latitude
    outBuffer->AppendInteger(m_longitude); //longitude
    outBuffer->AppendInteger(m_semiMajorConfidence); //semiMajorConfidence
    outBuffer->AppendInteger(m_semiMinorConfidence); //semiMinorConfidence
    outBuffer->AppendInteger(m_semiMajorOrientation); //semiMajorOrientation
    outBuffer->AppendInteger(m_altitude); //altitude
    outBuffer->AppendInteger(m_heading); //heading value
    outBuffer->AppendInteger(m_headingConfidence); //headingConfidence
    outBuffer->AppendInteger(m_speed); //speedValue
    outBuffer->AppendInteger(m_speedConfidence); //speedConfidence        
    outBuffer->AppendInteger(m_vehicleLength); //vehicleLength
    outBuffer->AppendInteger(m_vehicleWidth); //vehicleWidth
    outBuffer->AppendInteger(m_longitudinalAcc); //longitudinalAcc
    outBuffer->AppendInteger(m_longitudinalAccConf); //longitudinalAccConf
    outBuffer->AppendInteger(m_yawRateValue); //yawRateValue
    outBuffer->AppendInteger(m_yawRateConfidence); //yawRateConfidence        
    outBuffer->AppendInteger(m_vehicleRole); //vehicleRole

    std::vector<unsigned char> bytes = outBuffer->GetBytes();
    std::string bytesString(bytes.begin(),bytes.end());
    // std::cout<< bytesString << std::endl;
    opendlv::knowledge::Message nextMessage(bytesString.size(),bytesString);
    odcore::data::Container c(nextMessage);
    getConference().send(c);

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Receives .
 * Sends .
 */
void V2vCam::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::sensation::Voice::ID())
  {
    opendlv::sensation::Voice message = c.getData<opendlv::sensation::Voice>();
    // std::cout<<message.getType()<<std::endl;
    if(strcmp(message.getType().c_str(),"cam") == 0)
    {
      std::string dataString = message.getData();
      std::vector<unsigned char> data(dataString.begin(), dataString.end());
    
      // std::vector<unsigned char> v = message.getListOfData();
      // std::cout<< "Received CAM \n";
      std::shared_ptr<Buffer const> buffer(new Buffer(data));
      // std::cout<< buffer->GetSize()<<std::endl;
      std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
      //Long and little endian reverser
      inIterator->ItReversed();
      
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
      int32_t stationType = inIterator->ReadInteger();
      int32_t latitude = inIterator->ReadInteger();
      int32_t longitude = inIterator->ReadInteger();
      int32_t semiMajorConfidence = inIterator->ReadInteger();
      int32_t semiMinorConfidence = inIterator->ReadInteger();
      int32_t semiMajorOrientation = inIterator->ReadInteger();
      int32_t altitude = inIterator->ReadInteger();
      int32_t heading = inIterator->ReadInteger();
      int32_t headingConfidence = inIterator->ReadInteger();
      int32_t speed = inIterator->ReadInteger();
      int32_t speedConfidence = inIterator->ReadInteger();
      int32_t vehicleLength = inIterator->ReadInteger();
      int32_t vehicleWidth = inIterator->ReadInteger();
      int32_t longitudinalAcc = inIterator->ReadInteger();
      int32_t longitudinalAccConf = inIterator->ReadInteger();
      int32_t yawRateValue = inIterator->ReadInteger();
      int32_t yawRateConfidence = inIterator->ReadInteger();
      int32_t vehicleRole = inIterator->ReadInteger();

      std::string output = "*** Cam object ***\n";

      output += "Message Id: " + std::to_string(messageId) + "\n";
      output += "Station Id: " + std::to_string(stationId) + "\n";
      output += "Generation delta time: " 
          + std::to_string(generationDeltaTime) + "\n";
      output += "Container mask: " + std::to_string(containerMask) + "\n";
      output += "Station type: " + std::to_string(stationType) + "\n";
      output += "Latitude: " + std::to_string(latitude) + "\n";
      output += "Longitude: " + std::to_string(longitude) + "\n";
      output += "Semi major confidence: " 
          + std::to_string(semiMajorConfidence) + "\n";
      output += "Semi minor confidence: " 
          + std::to_string(semiMinorConfidence) + "\n";
      output += "Semi major orientation: " 
          + std::to_string(semiMajorOrientation) + "\n";
      output += "Altitude: " + std::to_string(altitude) + "\n";
      output += "Heading: " + std::to_string(heading) + "\n";
      output += "Heading confidence: " 
          + std::to_string(headingConfidence) + "\n";
      output += "Speed: " + std::to_string(speed) + "\n";
      output += "Speed confidence: " + std::to_string(speedConfidence) + "\n";
      output += "Vehicle length: " + std::to_string(vehicleLength) + "\n";
      output += "Vehicle width: " + std::to_string(vehicleWidth) + "\n";
      output += "Longitudinal acc: " + std::to_string(longitudinalAcc) + "\n";
      output += "Longitudinal acc conf: " 
          + std::to_string(longitudinalAccConf) + "\n";
      output += "Yaw rate value: " + std::to_string(yawRateValue) + "\n";
      output += "Yaw rate confidence: " 
          + std::to_string(yawRateConfidence) + "\n";
      output += "Vehicle role: " + std::to_string(vehicleRole) + "\n";
      std::cout << output;

    }
  }
}

void V2vCam::setUp()
{
  
}

void V2vCam::tearDown()
{
}

} // v2vcam
} // linguistics
} // knowledge
} // opendlv
