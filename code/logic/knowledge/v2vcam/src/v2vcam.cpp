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
#include <math.h>
 
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendlv/data/environment/WGS84Coordinate.h"
#include "opendlv/data/environment/Point3.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "buffer.hpp"
#include "v2vcam.hpp"



namespace opendlv {
namespace logic {
namespace knowledge {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vCam::V2vCam(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "logic-knowledge-v2vcam")
    , m_sendLog()
    , m_receiveLog()
    , m_timeType2004()
    , m_printInbound()
    , m_printOutbound()
    , m_record()
    , m_debug(false)
    , m_receivedGeolocation(false)
    , m_messageId(2)
    , m_stationId(0)
    , m_generationDeltaTime(0)
    , m_containerMask(0)
    , m_stationType(0)
    , m_latitude(90.0000001)
    , m_longitude(180.0000001)
    , m_semiMajorConfidence(-1)
    , m_semiMinorConfidence(-1)
    , m_semiMajorOrientation(-1)
    , m_altitude(8000.01)
    , m_heading(-1)
    , m_headingConfidence(-1)
    , m_speed(-1)
    , m_speedConfidence(-1)
    , m_vehicleLength(-1)
    , m_vehicleWidth(-1)
    , m_longitudinalAcc(-1)
    , m_longitudinalAccConf(-1)
    , m_yawRateValue(0)
    , m_yawRateConfidence(-1)
    , m_vehicleRole(0)

{
}

V2vCam::~V2vCam()
{
}

void V2vCam::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_printOutbound = kv.getValue<bool>(
      "logic-knowledge-v2vcam.printOutbound");
  m_printInbound = kv.getValue<bool>(
      "logic-knowledge-v2vcam.printInbound");
  m_record = kv.getValue<bool>(
      "logic-knowledge-v2vcam.record");
  m_debug = kv.getValue<bool>(
      "logic-knowledge-v2vcam.debug");
  
  std::cout << "Print outbound: " << m_printOutbound << " Print inbound: " 
      << m_printInbound << " Record: " << m_record << " Debug: " << m_debug 
      << std::endl;
  
  if (m_record) {
    struct stat st;
    if (::stat("var/application/knowledge/linguistics/v2vcam", &st) == -1) {
      ::system("mkdir -p ./var/application/knowledge/linguistics/v2vcam");
      std::cout<<"Created dir"<<std::endl;
    }
    odcore::data::TimeStamp now;

    std::stringstream filenameSend;
    std::stringstream filenameReceive;

    filenameSend << "/" << now.getYYYYMMDD_HHMMSS() << " cam send.log";
    filenameReceive << "/" << now.getYYYYMMDD_HHMMSS() << " cam receive.log";
    m_sendLog.open("var/application/knowledge/linguistics/v2vcam" 
      + filenameSend.str(), std::ios::out | std::ios::app);
    m_receiveLog.open("var/application/knowledge/linguistics/v2vcam" 
      + filenameReceive.str(), std::ios::out | std::ios::app);

    std::string header = "#time, \
        message id, \
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
}

void V2vCam::tearDown()
{
  m_sendLog.close();
  m_receiveLog.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2vCam::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    std::shared_ptr<Buffer> outBuffer(new Buffer());
    // Reverser for big and little endian specification of V2V.
    outBuffer->Reversed();
    outBuffer->AppendByte(GetMessageId()); //messageId
    outBuffer->AppendInteger(GetStationId()); //stationId
    outBuffer->AppendInteger(GenerateGenerationDeltaTime()); //generationDeltaTime
    outBuffer->AppendByte(GetContainerMask()); //containerMask
    outBuffer->AppendInteger(GetStationType()); //stationType                
    outBuffer->AppendInteger(GetLatitude()); //latitude
    outBuffer->AppendInteger(GetLongitude()); //longitude
    outBuffer->AppendInteger(GetSemiMajorConfidence()); //semiMajorConfidence
    outBuffer->AppendInteger(GetSemiMinorConfidence()); //semiMinorConfidence
    outBuffer->AppendInteger(GetSemiMajorOrientation()); //semiMajorOrientation
    outBuffer->AppendInteger(GetAltitude()); //altitude
    outBuffer->AppendInteger(GetHeading()); //heading value
    outBuffer->AppendInteger(GetHeadingConfidence()); //headingConfidence
    outBuffer->AppendInteger(GetSpeed()); //speedValue
    outBuffer->AppendInteger(GetSpeedConfidence()); //speedConfidence        
    outBuffer->AppendInteger(GetVehicleLength()); //vehicleLength
    outBuffer->AppendInteger(GetVehicleWidth()); //vehicleWidth
    outBuffer->AppendInteger(GetLongitudinalAcc()); //longitudinalAcc
    outBuffer->AppendInteger(GetLongitudinalAccConf()); //longitudinalAccConf
    outBuffer->AppendInteger(GetYawRateValue()); //yawRateValue
    outBuffer->AppendInteger(GetYawRateConfidence()); //yawRateConfidence        
    outBuffer->AppendInteger(GetVehicleRole()); //vehicleRole

  

    std::vector<unsigned char> bytes = outBuffer->GetBytes();
    std::string bytesString(bytes.begin(),bytes.end());
    // std::cout<< bytesString << std::endl;
    opendlv::knowledge::Message nextMessage(bytesString.size(),bytesString);
    odcore::data::Container c(nextMessage);
    getConference().send(c);

    if(m_record) {
      m_sendLog << std::setprecision(15) 
          << std::to_string(GenerateGenerationTime()) +
          + "," + std::to_string(GetMessageId()) +
          + "," + std::to_string(GetStationId()) +
          + "," + std::to_string(GenerateGenerationDeltaTime()) +
          + "," + std::to_string(GetContainerMask()) +
          + "," + std::to_string(GetStationType()) +
          + "," + std::to_string(GetLatitude()) +
          + "," + std::to_string(GetLongitude()) +
          + "," + std::to_string(GetSemiMajorConfidence()) +
          + "," + std::to_string(GetSemiMinorConfidence()) +
          + "," + std::to_string(GetSemiMajorOrientation()) +
          + "," + std::to_string(GetAltitude()) +
          + "," + std::to_string(GetHeading()) +
          + "," + std::to_string(GetHeadingConfidence()) +
          + "," + std::to_string(GetSpeed())
          + "," + std::to_string(GetSpeedConfidence()) +
          + "," + std::to_string(GetVehicleLength()) +
          + "," + std::to_string(GetVehicleWidth()) +
          + "," + std::to_string(GetLongitudinalAcc()) +
          + "," + std::to_string(GetLongitudinalAccConf()) +
          + "," + std::to_string(GetYawRateValue()) +
          + "," + std::to_string(GetYawRateConfidence()) +
          + "," + std::to_string(GetVehicleRole());
      m_sendLog << std::endl;
      // std::cout<< "Latitude: " << m_latitude << " Longitude: " << m_longitude << std::endl;
    }

    if(m_printOutbound) {
      std::string output = "*** Cam object ***\n";
      output += "Message Id: " + std::to_string(GetMessageId()) + "\n";
      output += "Station Id: " + std::to_string(GetStationId()) + "\n";
      output += "Generation delta time: " 
          + std::to_string(GenerateGenerationDeltaTime()) + "\n";
      output += "Container mask: " + std::to_string(GetContainerMask()) + "\n";
      output += "Station type: " + std::to_string(GetStationType()) + "\n";
      output += "Latitude: " + std::to_string(GetLatitude()) + "\n";
      output += "Longitude: " + std::to_string(GetLongitude()) + "\n";
      output += "Semi major confidence: " 
          + std::to_string(GetSemiMajorConfidence()) + "\n";
      output += "Semi minor confidence: " 
          + std::to_string(GetSemiMinorConfidence()) + "\n";
      output += "Semi major orientation: " 
          + std::to_string(GetSemiMajorOrientation()) + "\n";
      output += "Altitude: " + std::to_string(GetAltitude()) + "\n";
      output += "Heading: " + std::to_string(GetHeading()) + "\n";
      output += "Heading confidence: " 
          + std::to_string(GetHeadingConfidence()) + "\n";
      output += "Speed: " + std::to_string(GetSpeed()) + "\n";
      output += "Speed confidence: " + std::to_string(GetSpeedConfidence()) + "\n";
      output += "Vehicle length: " + std::to_string(GetVehicleLength()) + "\n";
      output += "Vehicle width: " + std::to_string(GetVehicleWidth()) + "\n";
      output += "Longitudinal acc: " + std::to_string(GetLongitudinalAcc()) + "\n";
      output += "Longitudinal acc conf: " 
          + std::to_string(GetLongitudinalAccConf()) + "\n";
      output += "Yaw rate value: " + std::to_string(GetYawRateValue()) + "\n";
      output += "Yaw rate confidence: " 
          + std::to_string(GetYawRateConfidence()) + "\n";
      output += "Vehicle role: " + std::to_string(GetVehicleRole()) + "\n";
      std::cout << output;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Receives geolocation-, voice containers.
 * Sends .
 */
void V2vCam::nextContainer(odcore::data::Container &a_c)
{
  // TODO: Change how Identity works (remove the +400 hack).
  if (a_c.getDataType() == (opendlv::knowledge::Insight::ID() + 400)) {
    opendlv::knowledge::Insight insight = 
        a_c.getData<opendlv::knowledge::Insight>();
    ReadInsight(insight);
  } else if (a_c.getDataType() == opendlv::sensation::Geolocation::ID()) {
    opendlv::sensation::Geolocation geolocation = 
        a_c.getData<opendlv::sensation::Geolocation>();
    ReadGeolocation(geolocation);
  } else if (a_c.getDataType() == opendlv::model::DynamicState::ID()) {
    opendlv::model::DynamicState dynamicState = 
        a_c.getData<opendlv::model::DynamicState>();
    ReadDynamicState(dynamicState);
  } else if (a_c.getDataType() == opendlv::sensation::Voice::ID()) {
    opendlv::sensation::Voice voice = a_c.getData<opendlv::sensation::Voice>();
    if(strcmp(voice.getType().c_str(), "cam") != 0) {
      // std::cout << "Message type not CAM." << std::endl;
      return;
    }
    ReadVoice(voice);
  }
}

void V2vCam::ReadInsight(opendlv::knowledge::Insight const &a_insight)
{
  std::string str = a_insight.getInsight();
  std::vector<std::string> information = 
      odcore::strings::StringToolbox::split(str,'=');
  if(information.size() > 0){
    if (information[0] == "stationId") {
      m_stationId = std::stoi(information[1]);
    } else if (information[0] == "stationType") {
      m_stationType = std::stoi(information[1]);
    } else if (information[0] == "vehicleLength") {
      m_vehicleLength = std::stod(information[1]);
    } else if (information[0] == "vehicleWidth") {
      m_vehicleWidth = std::stod(information[1]);
    } else if (information [0] == "vehicleRole") {
      m_vehicleRole = std::stoi(information[1]);
    }
  }
    
  if (m_debug) {
    std::cout << "Insight received - '" << str << "'" << std::endl; 
  }
}

void V2vCam::ReadDynamicState(
    opendlv::model::DynamicState const &a_dynamicState)
{
  int16_t frameId = a_dynamicState.getFrameId();
  if (frameId == 0) {
    m_speed = a_dynamicState.getVelocity().getX();
    m_speedConfidence = a_dynamicState.getVelocityConfidence().getX();
    m_yawRateValue = a_dynamicState.getAngularVelocity().getZ();
    m_yawRateConfidence = a_dynamicState.getAngularVelocityConfidence().getZ();
    m_longitudinalAcc = a_dynamicState.getAcceleration().getX();
    m_longitudinalAccConf = a_dynamicState.getAccelerationConfidence().getX();
  
    if (m_debug) {
      std::cout << "DynamicState received (frame 0) - speed=" << m_speed << " yawRatevalue=" 
        << m_yawRateValue << " longitudinalAcceleration=" << m_longitudinalAcc << std::endl; 
    }
  }
}

void V2vCam::ReadGeolocation(
    opendlv::sensation::Geolocation const &a_geolocation)
{
  m_latitude = a_geolocation.getLatitude();
  m_longitude = a_geolocation.getLongitude();
  m_altitude = a_geolocation.getAltitude();
  m_heading = a_geolocation.getHeading();
  while (m_heading < 0) {
    m_heading += 2.0 * M_PI;
  }
  while (m_heading > 2*M_PI) {
    m_heading -= 2.0 * M_PI;
  }
  m_headingConfidence = a_geolocation.getHeadingConfidence();
  m_semiMajorConfidence = a_geolocation.getLatitudeConfidence();
  m_semiMinorConfidence = a_geolocation.getLongitudeConfidence();

  m_receivedGeolocation = true;

  if (m_debug) {
    std::cout << "Geolocation received - longitude=" << m_longitude << " latitude=" 
        << m_latitude << " altitude=" << m_altitude << " heading=" 
        << m_heading << std::endl; 
  }
}

void V2vCam::ReadVoice(opendlv::sensation::Voice const &a_voice)
{
  std::string dataString = a_voice.getData();
  std::vector<unsigned char> data(dataString.begin(), dataString.end());
  std::shared_ptr<Buffer const> buffer(new Buffer(data));
  std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
  //Long and little endian reverser
  inIterator->ItReversed();

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

  if(m_printInbound){
    std::string output = "*** Cam object ***\n";
    output += "Message Id: " + std::to_string(messageId) + "\n";
    output += "Station Id: " + std::to_string(stationId) + "\n";
    output += "Generation delta time: " + std::to_string(generationDeltaTime) + "\n";
    output += "Container mask: " + std::to_string(containerMask) + "\n";
    output += "Station type: " + std::to_string(stationType) + "\n";
    output += "Latitude: " + std::to_string(latitude) + "\n";
    output += "Longitude: " + std::to_string(longitude) + "\n";
    output += "Semi major confidence: " + std::to_string(semiMajorConfidence) + "\n";
    output += "Semi minor confidence: " + std::to_string(semiMinorConfidence) + "\n";
    output += "Semi major orientation: " + std::to_string(semiMajorOrientation) + "\n";
    output += "Altitude: " + std::to_string(altitude) + "\n";
    output += "Heading: " + std::to_string(heading) + "\n";
    output += "Heading confidence: " + std::to_string(headingConfidence) + "\n";
    output += "Speed: " + std::to_string(speed) + "\n";
    output += "Speed confidence: " + std::to_string(speedConfidence) + "\n";
    output += "Vehicle length: " + std::to_string(vehicleLength) + "\n";
    output += "Vehicle width: " + std::to_string(vehicleWidth) + "\n";
    output += "Longitudinal acc: " + std::to_string(longitudinalAcc) + "\n";
    output += "Longitudinal acc conf: " + std::to_string(longitudinalAccConf) + "\n";
    output += "Yaw rate value: " + std::to_string(yawRateValue) + "\n";
    output += "Yaw rate confidence: " + std::to_string(yawRateConfidence) + "\n";
    output += "Vehicle role: " + std::to_string(vehicleRole) + "\n";
    std::cout << output;
  }
  if(m_record) {
    m_receiveLog <<  std::setprecision(15) 
        << std::to_string(GenerateGenerationTime()) +  
        + "," + std::to_string(messageId) +
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

  if (m_receivedGeolocation) { 
    odcore::data::TimeStamp now;
    std::string type = "vehicle";
    float typeConfidence = 1.0f;

    opendlv::data::environment::WGS84Coordinate ourGeolocation(
        m_latitude, m_longitude);
    opendlv::data::environment::WGS84Coordinate theirGeolocation(
        latitude / std::pow(10,7), longitude / std::pow(10,7));
    opendlv::data::environment::Point3 currentObjectCartesianLocation =
        ourGeolocation.transform(theirGeolocation);
    double xOffset = currentObjectCartesianLocation.getX();
    double yOffset = currentObjectCartesianLocation.getY();
    double heading1 = m_heading;
    while (heading1 < -M_PI) {
      heading1 += 2.0 * M_PI;
    }
    while (heading1 > M_PI) {
      heading1 -= 2.0 * M_PI;
    }

    double azimuth = std::atan2(yOffset, xOffset) - heading1;
    while (azimuth < -M_PI) {
      azimuth += 2.0 * M_PI;
    }
    while (azimuth > M_PI) {
      azimuth -= 2.0 * M_PI;
    }
    
    opendlv::model::Direction objectDirection(azimuth, 0.0f);
    float objectDirectionConfidence = 0.5f;

    opendlv::model::Direction objectDirectionRate(0.0f, 0.0f);
    float objectDirectionRateConfidence = -1.0f;

    float distance = std::sqrt((xOffset * xOffset) + (yOffset * yOffset));
    float distanceConfidence = 0.5f;

    if(m_debug) {
      std::cout << "Snowfox position: (" << xOffset << "," << yOffset << "), distance: " << distance << ", azmiuth: " << azimuth <<  ", our heading: " << heading1  << std::endl;
    }
    
    double localXOffset = std::sin(azimuth) * static_cast<double>(distance);
    double localYOffset = std::cos(azimuth) * static_cast<double>(distance);

    // TODO: Simplification where we assume that the other vehicle has the same heading as ourselves.
    double azimuthCorner = std::atan2(localYOffset - vehicleWidth / 2.0, localXOffset);

    float angularSize = static_cast<float>(2.0 * (azimuth - azimuthCorner));
    float angularSizeConfidence = 0.5f;
    float angularSizeRate = 0.0f;
    float angularSizeRateConfidence = -1.0f;

    float confidence = 1.0f;

    std::vector<std::string> sources;
    sources.push_back("v2vcam");

    std::vector<std::string> properties;
    properties.push_back("Station Id: " + std::to_string(stationId));
    properties.push_back("Vehicle length: " + std::to_string(vehicleLength));
    properties.push_back("Vehicle width: " + std::to_string(vehicleWidth));
    
    uint16_t objectId = -1;
    
    opendlv::perception::Object detectedObject(now,
        type, 
        typeConfidence, 
        objectDirection, 
        objectDirectionConfidence, 
        objectDirectionRate, 
        objectDirectionRateConfidence,
        distance, 
        distanceConfidence, 
        angularSize,
        angularSizeConfidence, 
        angularSizeRate, 
        angularSizeRateConfidence, 
        confidence, 
        sources, 
        properties, 
        objectId);
    odcore::data::Container objectContainer(detectedObject);
    getConference().send(objectContainer);

    if (m_debug) {
      std::cout << "Object detected - stationId=" << stationId << " distance=" 
        << distance << " direction=" << azimuth << " angularSize=" 
        << angularSize << std::endl;
    }
  }
}

void V2vCam::SendWGS84Coordinate()
{
  opendlv::data::environment::WGS84Coordinate coordPacket(m_latitude, m_longitude);
  // std::cout << std::setprecision(11) << coordPacket.getLatitude() 
  //     << " " << coordPacket.getLongitude() << std::endl;
  odcore::data::Container nextC(coordPacket);
  getConference().send(nextC);
}



unsigned char V2vCam::GetMessageId() const
{
  return m_messageId;
}

int32_t V2vCam::GetStationId() const
{
  return m_stationId;
}

uint64_t V2vCam::GenerateGenerationTime() const
{
  std::chrono::system_clock::time_point start2004TimePoint = 
      std::chrono::system_clock::from_time_t(m_timeType2004);
  uint64_t millisecondsSince2004Epoch =
      std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1) 
      - start2004TimePoint.time_since_epoch() / std::chrono::milliseconds(1);
  return millisecondsSince2004Epoch;
}

int32_t V2vCam::GenerateGenerationDeltaTime()
{
  m_generationDeltaTime = GenerateGenerationTime()%65536;
  return m_generationDeltaTime;
}

unsigned char V2vCam::V2vCam::GetContainerMask() const
{
  return m_containerMask;
}

int32_t V2vCam::GetStationType() const
{
  return m_stationType;
}

int32_t V2vCam::GetLatitude() const
{
  int32_t scale = std::pow(10,7);
  double val = m_latitude*scale;
  if(val < -900000000 || val > 900000000){
    return 900000001;
  }
  else {    
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetLongitude() const
{
  int32_t scale = std::pow(10,7);
  double val = m_longitude*scale;
  if(val< -1800000000 || val > 1800000000){
    return 1800000001;
  }
  else{
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetSemiMajorConfidence() const
{
  int32_t scale = std::pow(10,2);
  double val = m_semiMajorConfidence*scale;
  if(val < 0){
    return 4095;
  }
  else if(val < 1){
    return 1;
  }
  else if(val > 4093){
    return 4094;
  }
  else{
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetSemiMinorConfidence() const
{
  int32_t scale = std::pow(10,2);
  double val = m_semiMinorConfidence*scale;
  if(val < 0){
    return 4095;
  }
  else if(val < 1){
    return 1;
  }
  else if(val > 4093){
    return 4094;
  }
  else{
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetSemiMajorOrientation() const
{
  double conversion = opendlv::Constants::RAD2DEG;
  int32_t scale = std::pow(10,2);
  double val = m_semiMajorOrientation*scale*conversion;
  if(val < 0 || val > 3600){
    return 3601;
  }
  else {
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetAltitude() const
{
  int32_t scale = std::pow(10,2);
  double val = m_altitude*scale;
  if(val < -100000 || val > 800000){
    return 800001;
  }
  else{
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetHeading() const
{
  double conversion = opendlv::Constants::RAD2DEG;
  int32_t scale = std::pow(10,1);
  double val = m_heading*scale*conversion;
  if(val < 0 || val > 3600 || std::isnan(val)){
    return 3601;
  }
  else {
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetHeadingConfidence() const
{
  double conversion = opendlv::Constants::RAD2DEG;
  int32_t scale = std::pow(10,1);
  double val = m_headingConfidence*scale*conversion;
  // std::cout << val << std::endl;
  if(val < 0){
    return 127;
  }
  else if(val < 1){
    return 1;
  }
  else if(val > 125){
    return 126;
  }
  else{
    return static_cast<int32_t>(std::round(val));

  }
}

int32_t V2vCam::GetSpeed() const
{
  int32_t scale = std::pow(10,2);
  double val = m_speed*scale;
  if(val < 0){
    return 16383;
  }
  else if(val > 16382){
    return 16382;
  }
  else {
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetSpeedConfidence() const
{
  // std::cout << m_speedConfidence << std::endl;
  int32_t scale = std::pow(10,2);
  double val = m_speedConfidence*scale;
  if(val < 0){
    return 127;
  }
  else if(val < 1){
    return 1;
  }
  else if(val > 125){
    return 126;
  }
  else{
    return static_cast<int32_t>(std::round(val));
  }
}

int32_t V2vCam::GetVehicleLength() const
{
  int32_t scale = std::pow(10,1);
  double val = m_vehicleLength*scale;
  if(val < 0){
    return 1023;
  }
  else if( val > 1022){
    return 1022;
  }
  else{
    return static_cast<int32_t> (std::round(val));
  }
}

int32_t V2vCam::GetVehicleWidth() const
{
  int32_t scale = std::pow(10,1);
  double val = m_vehicleWidth*scale;
  if(val < 0){
    return 62;
  }
  else if( val > 61){
    return 61;
  }
  else{
    return static_cast<int32_t> (std::round(val));
  }

}

int32_t V2vCam::GetLongitudinalAcc() const
{
  int32_t scale = std::pow(10,1);
  double val = m_longitudinalAcc*scale;
  // std::cout << val << std::endl;
  if(m_longitudinalAccConf < 0){
    return 161;
  }
  else if(val < -160){
    return -160;
  }
  else if(val > 160){
    return 160;
  }
  else{
    return static_cast<int32_t> (std::round(val));
  }
}


int32_t V2vCam::GetLongitudinalAccConf() const
{
  int32_t scale = std::pow(10,1);
  double val = m_longitudinalAccConf*scale;

  if(val < 0){
    return 102;
  } else if (val < 1){
    return 1;
  } else if (val > 100){
    return 101;
  } else {
    return static_cast<int32_t> (std::round(val));
  }
}

int32_t V2vCam::GetYawRateValue() const
{
  if(m_yawRateConfidence < 0){
    return 32767;
  }
  int32_t scale = std::pow(10,2);
  double conversion = opendlv::Constants::RAD2DEG;
  double val = m_yawRateValue*scale*conversion;
  if(val < -32766){
    return -32766;
  }
  else if(val > 32766){
    return 32766;
  }
  else{
    return static_cast<int32_t> (std::round(val));
  }
}



int32_t V2vCam::GetYawRateConfidence() const
{
  double conversion = opendlv::Constants::RAD2DEG;
  double val = m_yawRateConfidence * conversion;
  if(m_yawRateConfidence < 0){
    return 8;
  }else if (val <= 0.01){
    return 0;
  }else if (val <= 0.05){
    return 1;
  }else if (val <= 0.1){
    return 2;
  }else if (val <= 1){
    return 3;
  }else if (val <= 5){
    return 4;
  }else if (val <= 10){
    return 5;
  }else if (val <= 100){
    return 6;
  }else {
    return 7;
  }
}

int32_t V2vCam::GetVehicleRole() const
{
  return m_vehicleRole;
}

}
}
}
