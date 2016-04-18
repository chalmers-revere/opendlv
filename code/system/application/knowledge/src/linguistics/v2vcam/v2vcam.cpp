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
#include <ctime>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
 
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

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
      a_argc, a_argv, "knowledge-linguistics-v2vcam"),
    m_sendLog(),
    m_receiveLog()
{

  struct stat st;

  if (stat("var/application/knowledge/linguistics/v2vcam", &st) == -1) {
    ::system("mkdir -p ./var/application/knowledge/linguistics/v2vcam");
    // std::cout<<"Created dir"<<std::endl;
  }
  odcore::data::TimeStamp nu;

  std::stringstream filenameSend;
  std::stringstream filenameReceive;

  filenameSend << "/" << nu.getYYYYMMDD_HHMMSS() << " cam send.log";
  filenameReceive << "/" << nu.getYYYYMMDD_HHMMSS() << " cam receive.log";
  m_sendLog.open("var/application/knowledge/linguistics/v2vcam" 
    + filenameSend.str(), std::ios::out | std::ios::app);
  m_receiveLog.open("var/application/knowledge/linguistics/v2vcam" 
    + filenameReceive.str(), std::ios::out | std::ios::app);

  std::string header = "#message id, \
      station id, \
      generation delta time, \
      container mask,\
      station type, \
      latitude, \
      longitude, \
      semi major confidence, \
      semi minor confidence, \
      semi major orientation, \
      altitude, \
      heading, \
      heading confidence, \
      speed, speed confidence, \
      vehicle length, \
      vehicle width, \
      longitudinal acc, \
      longitudinal acc conf, \
      yaw rate value,\
      yaw rate conf, \
      vehicle role";

  m_sendLog << header << std::endl;
  m_receiveLog << header << std::endl;
}

V2vCam::~V2vCam()
{
  m_sendLog.close();
  m_receiveLog.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vCam::body()
{
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

  std::time_t tt = timegm(&tm);

  std::chrono::system_clock::time_point start2004TimePoint = 
      std::chrono::system_clock::from_time_t(tt);

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    // std::cout << "Inside the main processing loop." << std::endl;
    unsigned long millisecondsSince2004Epoch =
        std::chrono::system_clock::now().time_since_epoch() /
        std::chrono::milliseconds(1) - start2004TimePoint.time_since_epoch() / std::chrono::milliseconds(1);
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

    m_sendLog << std::to_string(m_messageId) +
        + "," + std::to_string(m_stationId) +
        + "," + std::to_string(m_generationDeltaTime) +
        + "," + std::to_string(m_containerMask) +
        + "," + std::to_string(m_stationType) +
        + "," + std::to_string(m_latitude) +
        + "," + std::to_string(m_longitude) +
        + "," + std::to_string(m_semiMajorConfidence) +
        + "," + std::to_string(m_semiMinorConfidence) +
        + "," + std::to_string(m_semiMajorOrientation) +
        + "," + std::to_string(m_altitude) +
        + "," + std::to_string(m_heading) +
        + "," + std::to_string(m_headingConfidence) +
        + "," + std::to_string(m_speed)
        + "," + std::to_string(m_speedConfidence) +
        + "," + std::to_string(m_vehicleLength) +
        + "," + std::to_string(m_vehicleWidth) +
        + "," + std::to_string(m_longitudinalAcc) +
        + "," + std::to_string(m_longitudinalAccConf) +
        + "," + std::to_string(m_yawRateValue) +
        + "," + std::to_string(m_yawRateConfidence) +
        + "," + std::to_string(m_vehicleRole);
    m_sendLog << std::endl;

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Receives .
 * Sends .
 */
void V2vCam::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::proxy::GpsReading::ID()) {
    opendlv::proxy::GpsReading gpsReading = c.getData<opendlv::proxy::GpsReading>();

    m_latitude = gpsReading.getLatitude();
    m_longitude = gpsReading.getLongitude();
    m_altitude = gpsReading.getAltitude();
    m_heading = gpsReading.getNorthHeading();
    m_speed = gpsReading.getSpeed();

    // TODO!! Real values from GPS, but not scaled properly.

    std::cout << "Latitude: " << m_latitude << " Longitude: " << m_longitude << " Speed: " << m_speed << std::endl;

  } else if(c.getDataType() == opendlv::sensation::Voice::ID()) {
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

      // std::string output = "*** Cam object ***\n";

      // output += "Message Id: " + std::to_string(messageId) + "\n";
      // output += "Station Id: " + std::to_string(stationId) + "\n";
      // output += "Generation delta time: " 
      //     + std::to_string(generationDeltaTime) + "\n";
      // output += "Container mask: " + std::to_string(containerMask) + "\n";
      // output += "Station type: " + std::to_string(stationType) + "\n";
      // output += "Latitude: " + std::to_string(latitude) + "\n";
      // output += "Longitude: " + std::to_string(longitude) + "\n";
      // output += "Semi major confidence: " 
      //     + std::to_string(semiMajorConfidence) + "\n";
      // output += "Semi minor confidence: " 
      //     + std::to_string(semiMinorConfidence) + "\n";
      // output += "Semi major orientation: " 
      //     + std::to_string(semiMajorOrientation) + "\n";
      // output += "Altitude: " + std::to_string(altitude) + "\n";
      // output += "Heading: " + std::to_string(heading) + "\n";
      // output += "Heading confidence: " 
      //     + std::to_string(headingConfidence) + "\n";
      // output += "Speed: " + std::to_string(speed) + "\n";
      // output += "Speed confidence: " + std::to_string(speedConfidence) + "\n";
      // output += "Vehicle length: " + std::to_string(vehicleLength) + "\n";
      // output += "Vehicle width: " + std::to_string(vehicleWidth) + "\n";
      // output += "Longitudinal acc: " + std::to_string(longitudinalAcc) + "\n";
      // output += "Longitudinal acc conf: " 
      //     + std::to_string(longitudinalAccConf) + "\n";
      // output += "Yaw rate value: " + std::to_string(yawRateValue) + "\n";
      // output += "Yaw rate confidence: " 
      //     + std::to_string(yawRateConfidence) + "\n";
      // output += "Vehicle role: " + std::to_string(vehicleRole) + "\n";
      // std::cout << output;

      m_receiveLog << std::to_string(messageId) +
        + "," + std::to_string(stationId) +
        + "," + std::to_string(generationDeltaTime) +
        + "," + std::to_string(containerMask) +
        + "," + std::to_string(stationType) +
        + "," + std::to_string(latitude) +
        + "," + std::to_string(longitude) +
        + "," + std::to_string(semiMajorConfidence) +
        + "," + std::to_string(semiMinorConfidence) +
        + "," + std::to_string(semiMajorOrientation) +
        + "," + std::to_string(altitude) +
        + "," + std::to_string(heading) +
        + "," + std::to_string(headingConfidence) +
        + "," + std::to_string(speed)
        + "," + std::to_string(speedConfidence) +
        + "," + std::to_string(vehicleLength) +
        + "," + std::to_string(vehicleWidth) +
        + "," + std::to_string(longitudinalAcc) +
        + "," + std::to_string(longitudinalAccConf) +
        + "," + std::to_string(yawRateValue) +
        + "," + std::to_string(yawRateConfidence) +
        + "," + std::to_string(vehicleRole);
        m_receiveLog << std::endl;

    }
  }
}

void V2vCam::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  // std::string const type = kv.getValue<std::string>("proxy-v2v.type");
  
  m_stationId = kv.getValue<int32_t>("knowledge-linguistics-v2vcam.stationId");
  m_stationType = kv.getValue<int32_t>("knowledge-linguistics-v2vcam.stationType");
  m_vehicleLength = kv.getValue<int32_t>("knowledge-linguistics-v2vcam.vehicleLength");
  m_vehicleWidth = kv.getValue<int32_t>("knowledge-linguistics-v2vcam.vehicleWidth");
  m_vehicleRole = kv.getValue<int32_t>("knowledge-linguistics-v2vcam.vehicleRole");
  
}

void V2vCam::tearDown()
{
}

} // v2vcam
} // linguistics
} // knowledge
} // opendlv
