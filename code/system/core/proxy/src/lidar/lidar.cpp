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
#include <fstream>
#include <thread>
#include <chrono>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/wrapper/SerialPort.h"
#include "opendavinci/odcore/wrapper/SerialPortFactory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lidar/lidar.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Lidar::Lidar(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-lidar")
    , m_sick()
    , m_lidarStringDecoder()
{
}

Lidar::~Lidar()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Lidar::body()
{
int counter = 0;
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    /*
    opendlv::model::Direction direction(0.0f,0.0f);
    std::vector<opendlv::model::Direction> directions;
    std::vector<double> distances;
    float PI = 3.1415f;
    for(int i = 0; i < 361; i++)
    {
      direction.setAzimuth(i * PI/ 360.0f);
      directions.push_back(direction);
      distances.push_back(1.0);
    }
    opendlv::proxy::EchoReading reading; 

    reading.setListOfDirections(directions);
    reading.setListOfRadii(distances);
    reading.setNumberOfPoints(distances.size());
    odcore::data::Container c(reading);
    getConference().send(c);
    */

//    if(m_lidarStringDecoder->IsRunning()) {
//      SendData();
//      //std::cout << "Echo sent" << std::endl;
//    }
//    else if(m_lidarStringDecoder->IsCentimeterMode()){
//      StartScan();
//      m_lidarStringDecoder->NotCentimeterMode();
//      std::cout << "Centimeter mode!" << std::endl;
//    }
//    else if(m_lidarStringDecoder->IsSettingsMode())
//    {
//      SetCentimeterMode();
//      m_lidarStringDecoder->NotSettingsMode();
//      std::cout << "Settings mode" << std::endl;
//    }


    counter++;
//    if (counter == 10) {
//        cout << "Sending status request" << endl;
//        Status();
//    }
    if (counter == 10) {
        cout << "Sending settings mode" << endl;
        SettingsMode();
    }
    if (counter == 20) {
        cout << "Sending centimeter mode" << endl;
        SetCentimeterMode();
    }
    if (counter == 30) {
        cout << "Start scanning" << endl;
        StartScan();
    }
    
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Lidar::setUp()
{
  
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-lidar.type");

  double x = kv.getValue<float>("proxy-lidar.mount.x");
  double y = kv.getValue<float>("proxy-lidar.mount.y");
  double z = kv.getValue<float>("proxy-lidar.mount.z");
  

  string SERIAL_PORT = kv.getValue<std::string>("proxy-lidar.port");
  uint32_t BAUD_RATE = kv.getValue<std::uint32_t>("proxy-lidar.baudrate");//9600; //TODO: Put in configuration file


BAUD_RATE = 9600;

  m_lidarStringDecoder = std::unique_ptr<LidarStringDecoder>(new LidarStringDecoder(getConference(), x, y, z));

  if(BAUD_RATE != 9600) {
    try {
    shared_ptr<odcore::wrapper::SerialPort> serial(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, 9600));
    serial->setStringListener(m_lidarStringDecoder.get());
    serial->start();
    m_sick = serial;
    SetBaud38400();
    serial->stop();
    serial->setStringListener(NULL);
    m_sick.reset();
    }
    catch(string &exception) {
      cerr << "[" << getName() << "] Could not connect to Sickan: " << exception << endl;
    }
  }

  try {
    m_sick = shared_ptr<odcore::wrapper::SerialPort>(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    m_sick->setStringListener(m_lidarStringDecoder.get());
    m_sick->start();
  }
  catch(string &exception) {
    cerr << "[" << getName() << "] Could not connect to Sickan: " << exception << endl;
  }

  //Status();

  std::cout << "Connected to Sickan, please wait for configuration" << std::endl;

//  SettingsMode(); //Enter settings mode
  
}

void Lidar::SendData()
{
  odcore::data::Container c(m_lidarStringDecoder->GetLatestReading());
  getConference().send(c);
}

void Lidar::Status()
{
  unsigned char statusCall[] = {0x02, 0x00, 0x01, 0x00, 0x31, 0x15, 0x12 };
  std::string statusString( reinterpret_cast< char const* >(statusCall), 7) ;
  m_sick->send(statusString);
}

void Lidar::StartScan()
{
  unsigned char streamStart[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x24, 0x34, 0x08};
  std::string startString( reinterpret_cast< char const* >(streamStart), 8) ;
  m_sick->send(startString);
}

void Lidar::StopScan()
{
  unsigned char streamStop[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x25, 0x35, 0x08};
  std::string stopString( reinterpret_cast< char const* >(streamStop), 8) ;
  m_sick->send(stopString);
}

void Lidar::SetBaud9600()
{
  unsigned char setBaud9600[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x42, 0x52, 0x08};
  std::string baudString( reinterpret_cast< char const* >(setBaud9600), 8) ;
  m_sick->send(baudString);
}

void Lidar::SetBaud38400()
{
  unsigned char setBaud38400[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x40, 0x50, 0x08};
  std::string baudString( reinterpret_cast< char const* >(setBaud38400), 8) ;
  m_sick->send(baudString);
}


void Lidar::SettingsMode()
{
  unsigned char settingsMode[] = {0x02, 0x00, 0x0A, 0x00, 0x20, 0x00, 0x53, 0x49, 0x43, 0x4B, 0x5F, 0x4C, 0x4D, 0x53, 0xBE, 0xC5};
  std::string settingString( reinterpret_cast< char const* >(settingsMode), 16) ;
  m_sick->send(settingString);
}

void Lidar::SetCentimeterMode()
{
  unsigned char centimeterMode[] = {0x02, 0x00, 0x21, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xCB };
  std::string centimeterString( reinterpret_cast< char const* >(centimeterMode), 39) ;
  m_sick->send(centimeterString);
}


void Lidar::tearDown()
{
  StopScan();
  m_sick->stop();
  m_sick->setStringListener(NULL);
}

void Lidar::nextContainer(odcore::data::Container &c) {
  (void)c;
}

} // lidar
} // proxy
} // opendlv
