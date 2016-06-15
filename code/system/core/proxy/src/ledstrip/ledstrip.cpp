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

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "ledstrip/ledstrip.hpp"
#include "ledstrip/device.hpp"

namespace opendlv {
namespace proxy {
namespace ledstrip {


using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore;
using namespace odcore::wrapper;

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Ledstrip::Ledstrip(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-ledstrip")
    , m_device(),
    m_angle(0.0f),
    m_tooClose(false),
    m_R(0),
    m_G(0),
    m_B(0),
    m_timeStamp()
{
}

Ledstrip::~Ledstrip()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Ledstrip::body()
{
  const string SERIAL_PORT = "/dev/ttyACM0";
  const uint32_t BAUD_RATE = 9600;

  std::shared_ptr<SerialPort> serial;
   try {
        serial = std::shared_ptr<SerialPort>(SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    }
    catch(string &exception) {
        cerr << "Serial port could not be created: " << exception << endl;
        return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
    }


  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 
    std::cout<<"Start whhile luup\n";

    odcore::data::TimeStamp now;

    if((now-m_timeStamp).toMicroseconds() / 1000000.0f > 1.0f){
      m_tooClose = false; 
    }

    if(m_tooClose){
      m_R = 255;
      m_G = 0;
      m_B = 0;
    } else if(std::fabs(m_angle) > 25.0f * 3.14f / 180.0f){
      m_R = 255;
      m_G = 140;
      m_B = 0;
    } else {
      m_R = 0;
      m_G = 255;
      m_B = 0;
    }


    int nSegments = 0;


    if(std::fabs(m_angle) > 0.402f){
      if(m_angle >= 0.0f){
        m_angle = 0.402f;
      } else {
        m_angle = -0.402f;
      }
    }

    int midPoint = (int)(147.0f/2.0f -(m_angle / 0.402f * 147.0f / 2.0f) ) ;
    std::cout<<"Mid point " << midPoint << std::endl;

    std::vector<int> leftMostSegment; 
    for(int i = midPoint-10; i < midPoint-8; i++){
      if (i >= 0 && i < 146){
        leftMostSegment.push_back(i);
        }
    } 

    std::vector<int> leftSegment;
    for(int i = midPoint-8; i < midPoint-4; i++){
      if (i >= 0 && i < 146){
        leftSegment.push_back(i);
        }
    } 
    std::vector<int> midSegment;
    for(int i = midPoint-4; i < midPoint+4; i++){
      if (i >= 0 && i < 146){
        midSegment.push_back(i);
        }
    } 
    std::vector<int> rightSegment; 
    for(int i = midPoint+4; i < midPoint+8; i++){
      if (i >= 0 && i < 146){
        rightSegment.push_back(i);
        }
    } 
    std::vector<int> rightMostSegment;
    for(int i = midPoint+8; i < midPoint+10; i++){
      if (i >= 0 && i < 146){
        rightMostSegment.push_back(i);
        }
    } 

    std::vector<uint8_t> leftMostData;
    if(!leftMostSegment.empty()){
      nSegments++;
      //Color
      leftMostData.push_back(m_R / 3);
      leftMostData.push_back(m_G / 3);
      leftMostData.push_back(m_B / 3);
      //Number of pixels
      leftMostData.push_back(leftMostSegment.size());
      for(uint k = 0; k < leftMostSegment.size(); k++){
        leftMostData.push_back(leftMostSegment.at(k));
      }
    }

    std::vector<uint8_t> leftData;
    if(!leftSegment.empty()){
      nSegments++;
      //Color
      leftData.push_back(m_R * 2 / 3);
      leftData.push_back(m_G * 2 / 3);
      leftData.push_back(m_B * 2 / 3);
      //Number of pixels
      leftData.push_back(leftSegment.size());
      for(uint k = 0; k < leftSegment.size(); k++){
        leftData.push_back(leftSegment.at(k));
      }
    }

    std::vector<uint8_t> midData;
    if(!midSegment.empty()){
      nSegments++;
      //Color
      midData.push_back(255);
      midData.push_back(0);
      midData.push_back(0);
      //Number of pixels
      midData.push_back(midSegment.size());
      for(uint k = 0; k < midSegment.size(); k++){
        midData.push_back(midSegment.at(k));
      }
    }

    std::vector<uint8_t> rightData;
    if(!rightSegment.empty()){
      nSegments++;
      //Color
      rightData.push_back(m_R * 2 / 3);
      rightData.push_back(m_G * 2 / 3);
      rightData.push_back(m_B * 2 / 3);
      //Number of pixels
      rightData.push_back(rightSegment.size());
      for(uint k = 0; k < rightSegment.size(); k++){
        rightData.push_back(rightSegment.at(k));
      }
    }

    std::vector<uint8_t> rightMostData;
    if(!rightMostSegment.empty()){
      nSegments++;
      //Color
      rightMostData.push_back(m_R / 3);
      rightMostData.push_back(m_G / 3);
      rightMostData.push_back(m_B / 3);
      //Number of pixels
      rightMostData.push_back(rightMostSegment.size());
      for(uint k = 0; k < rightMostSegment.size(); k++){
        rightMostData.push_back(rightMostSegment.at(k));
      }
    }

    // Construct led segments
    std::vector<uint8_t> ledRequest;
    ledRequest.push_back('s');
    ledRequest.push_back('t');
    ledRequest.push_back('a');
    ledRequest.push_back('r');
    ledRequest.push_back('t');
    ledRequest.push_back(nSegments);

    ledRequest.insert(ledRequest.end(),leftMostData.begin(),leftMostData.end());
    ledRequest.insert(ledRequest.end(),leftData.begin(),leftData.end());
    ledRequest.insert(ledRequest.end(),midData.begin(),midData.end());
    ledRequest.insert(ledRequest.end(),rightData.begin(),rightData.end());
    ledRequest.insert(ledRequest.end(),rightMostData.begin(),rightMostData.end());


    std::string stringToSend(ledRequest.begin(),ledRequest.end());
    std::cout<<"String size " << stringToSend.size()<<std::endl;
    std::cout<< stringToSend << std::endl;
    serial->send(stringToSend);//"startderp World\r\n");
    std::cout<<" Sent string\n";  

  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Ledstrip::nextContainer(odcore::data::Container &a_container)
{ 
  odcore::data::TimeStamp now;
  if (a_container.getDataType() == opendlv::knowledge::DesiredDirectionOfMovement::ID()) {
    opendlv::knowledge::DesiredDirectionOfMovement directionObject = 
    a_container.getData<opendlv::knowledge::DesiredDirectionOfMovement>();

    opendlv::model::Direction direction = directionObject.getDirection();

    m_angle = direction.getAzimuth();

  }


  if (a_container.getDataType() == (opendlv::perception::Object::ID() + 300)){
    opendlv::perception::Object inputObject = a_container.getData<opendlv::perception::Object>();
  
    if(inputObject.getDistance() < 10){
      m_tooClose = true;
      m_timeStamp = now;
    }
  }

}

void Ledstrip::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type =
  kv.getValue<std::string>("proxy-ledstrip.type");
  /*  std::string const port = kv.getValue<std::string>(
   *      "proxy-ledstrip.port");
  */
  if (type.compare("victor") == 0) {
    //      m_device = std::unique_ptr<Device>(new VictorDevice());
  }

  // if (m_device.get() == nullptr) {
  //   std::cerr << "[proxy-ledstrip] No valid device driver defined."
  //             << std::endl;
  // }
}

void Ledstrip::tearDown()
{

}

} // ledstrip
} // proxy
} // opendlv
