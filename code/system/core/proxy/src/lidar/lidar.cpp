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

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/wrapper/SerialPort.h"
#include "opendavinci/odcore/wrapper/SerialPortFactory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lidar/lidar.hpp"
#include "lidar/device.hpp"

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
    , m_firstHeader(true)
    , m_indicator(true)
    , m_startConfirmed(false)
    , m_settingsMode(false)
    , m_centimeterMode(false)
    , m_counter(0)
    , m_directions()
    , m_radii()
    , m_latestReading()
    , m_device()
    , m_sick()
{
}

Lidar::~Lidar()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Lidar::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    if(m_startConfirmed) {
      SendData();
    }
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Lidar::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-lidar.type");

  m_position[0] = kv.getValue<float>("proxy-lidar.mount.x");
  m_position[1] = kv.getValue<float>("proxy-lidar.mount.y");
  m_position[2] = kv.getValue<float>("proxy-lidar.mount.z");

  /*
  if (type.compare("sick") == 0) {
    //      m_device = std::unique_ptr<Device>(new SickDevice());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-lidar] No valid device driver defined."
              << std::endl;
  }
  */

  string SERIAL_PORT = kv.getValue<std::string>("proxy-lidar.port");
  uint32_t BAUD_RATE = 9600; //TODO: Put in configuration file


  if(BAUD_RATE != 9600) {
    try {
    shared_ptr<odcore::wrapper::SerialPort> serial(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, 9600));
    serial->setStringListener(this);
    serial->start();
    SetBaud38400();      
    serial->stop();
    serial->setStringListener(NULL);
    }
    catch(string &exception) {
      cerr << "[" << getName() << "] Could not connect to Sickan: " << exception << endl;
    }
  }

  try {
    m_sick = shared_ptr<odcore::wrapper::SerialPort>(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    m_sick->setStringListener(this);
    m_sick->start();
  }
  catch(string &exception) {
    cerr << "[" << getName() << "] Could not connect to Sickan: " << exception << endl;
  }

  Status();

  std::cout << "Connected to Sickan, please wait for configuration" << std::endl;

  m_startResponse[0] = 0x06;
  m_startResponse[1] = 0x02;
  m_startResponse[2] = 0x80;
  m_startResponse[3] = 0x03;
  m_startResponse[4] = 0x00;
  m_startResponse[5] = 0xA0;
  m_startResponse[6] = 0x00;
  m_startResponse[7] = 0x10;
  m_startResponse[8] = 0x16;
  m_startResponse[9] = 0x0A;
  m_measurementHeader[0] = 0x02;
  m_measurementHeader[1] = 0x80;
  m_measurementHeader[2] = 0xD6;
  m_measurementHeader[3] = 0x02;
  m_measurementHeader[4] = 0xB0;
  m_measurementHeader[5] = 0x69;
  m_measurementHeader[6] = 0x01; //01 for centimeters, 41 for millimeters
  m_centimeterResponse[0] = 0x06;
  m_centimeterResponse[1] = 0x02;
  m_centimeterResponse[2] = 0x80;
  m_centimeterResponse[3] = 0x25;
  m_centimeterResponse[4] = 0x00;
  m_centimeterResponse[5] = 0xF7;
  m_centimeterResponse[6] = 0x00;
  m_centimeterResponse[7] = 0x00;
  m_centimeterResponse[8] = 0x00;
  m_centimeterResponse[9] = 0x46;
  m_centimeterResponse[10] = 0x00;
  m_centimeterResponse[11] = 0x00;
  m_centimeterResponse[12] = 0x0D;
  m_centimeterResponse[13] = 0x00;
  m_centimeterResponse[14] = 0x00;
  m_centimeterResponse[15] = 0x00;
  m_centimeterResponse[16] = 0x02;
  m_centimeterResponse[17] = 0x02;
  m_centimeterResponse[18] = 0x00;
  m_centimeterResponse[19] = 0x00;
  m_centimeterResponse[20] = 0x00;
  m_centimeterResponse[21] = 0x00;
  m_centimeterResponse[22] = 0x00;
  m_centimeterResponse[23] = 0x00;
  m_centimeterResponse[24] = 0x00;
  m_centimeterResponse[25] = 0x00;
  m_centimeterResponse[26] = 0x00;
  m_centimeterResponse[27] = 0x00;
  m_centimeterResponse[28] = 0x00;
  m_centimeterResponse[29] = 0x00;
  m_centimeterResponse[30] = 0x00;
  m_centimeterResponse[31] = 0x00;
  m_centimeterResponse[32] = 0x00;
  m_centimeterResponse[33] = 0x00;
  m_centimeterResponse[34] = 0x00;
  m_centimeterResponse[35] = 0x00;
  m_centimeterResponse[36] = 0x00;
  m_centimeterResponse[37] = 0x00;
  m_centimeterResponse[38] = 0x00;
  m_centimeterResponse[39] = 0x02;
  m_centimeterResponse[40] = 0xCB;
  m_centimeterResponse[41] = 0x10;
  m_centimeterResponse[42] = 0xB0;
  m_centimeterResponse[43] = 0x11;

  SettingsMode(); //Enter settings mode

  bool once = false;
  while(!m_startConfirmed) {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    if(m_settingsMode && !once) { //wait for settingsmode
      SetCentimeterMode();        //set mode to centimeter
      m_settingsMode = false;
      once = true;
    }
    if(m_centimeterMode) {        //wait for centimeter mode  
      StartScan();                //Start scanning
      m_centimeterMode = false;
    }
    if(m_settingsMode && once) {  //wait for start confirmation
      m_startConfirmed = true;
      m_settingsMode = false;
    }
  }
  std::cout << "Configuration succesful!" << std::endl;
}

void Lidar::nextString(const std::string &s) 
{
  std::string alpha = "0123456789ABCDEF";
  const uint32_t stringLength = s.length();

  uint32_t bufferSize = 44; //TODO: Set a constant somewhere 

  for(uint32_t i = 0; i < stringLength; i++) {

    //Updating buffer
    for(uint32_t j = 0; j < bufferSize - 1; j++) { 
      m_buffer[j] = m_buffer[j+1]; 
    }

    unsigned char byte = static_cast<unsigned char>(s[i]);

    std::cout << alpha[(int)byte/16] << alpha[(int)byte%16] << ' ';

    m_buffer[bufferSize-1] = byte;
  
    m_measurements[m_counter] = byte;
    m_counter++;

    if(!m_startConfirmed) {
      m_indicator = true; //Checking for start/settings confirmation
      for(uint32_t j = 0; j < 10; j++) {
        if(m_buffer[bufferSize - 10 + j] != m_startResponse[j]) { m_indicator = false; }        
      }
      if(m_indicator) { 
        m_counter = 0;
        m_settingsMode = true; 
      }

      m_indicator = true; //Checking for centimeter mode confirmation
      for(uint32_t j = 0; j < 44; j++) {
        if(m_buffer[bufferSize - 44 + j] != m_centimeterResponse[j]) { m_indicator = false; }        
      }
      if(m_indicator) { 
        m_counter = 0;
        m_centimeterMode = true; 
      }
    }
    else {
      m_indicator = true; //Checking for output header
      for(uint32_t j = 0; j < 7; j++) {
        if(m_buffer[bufferSize - 7 + i] != m_measurementHeader[i]) { m_indicator = false; }
      }
      if(m_indicator) {
        if(!m_firstHeader) {
          ConvertToDistances();
        }
        else {
          m_firstHeader = false;
        } 
        m_counter = 0;  
      }
    }

    if(m_counter > 999) { //Safety clause
    m_counter--;
    }
  }
}

void Lidar::ConvertToDistances()
{
  uint32_t byte1;
  uint32_t byte2;
  uint32_t distance;
  double cartesian[2];
  double PI = 3.14159265;

  m_directions.clear();
  m_radii.clear();

  for(uint32_t i = 0; i < 361; i++) {
    byte1 = (int)m_measurements[i*2];
    byte2 = (int)m_measurements[i*2+1];
    distance = byte1 + (byte2%32)*256; //Integer centimeters in local polar coordinates
    cartesian[0] = distance * sin(PI * i /360) / 100; //Local cartesian coordinates in meters (rotated coordinate system)
    cartesian[1] = distance * (-cos(PI * i /360)) / 100;
    cartesian[0] += m_position[0]; //Truck cartesian coordinates
    cartesian[1] += m_position[1];

    
    double radius = sqrt(pow(cartesian[0],2) + pow(cartesian[1],2));
    float angle = atan(cartesian[1]/cartesian[0]) - PI/2;
    opendlv::model::Direction direction(angle,0);
    m_directions.push_back(direction);
    m_radii.push_back(radius);
  }

  m_latestReading.setListOfDirections(m_directions);
  m_latestReading.setListOfRadii(m_radii);
  m_latestReading.setNumberOfPoints(m_radii.size());
  WriteToFile();
}

void Lidar::WriteToFile()
{
  /*
  std::ofstream write;
  write.open("Test.txt",fstream::out | std::ofstream::app);



  for(uint32_t i = 0; i < 361; i++)
  {
    float* temp = m_freshCoordinates[i].getP();
    write << temp[0] << ' ';
  }
  write << std::endl;


  write.close();
  */
}

void Lidar::SendData()
{
  odcore::data::Container c(m_latestReading);
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
