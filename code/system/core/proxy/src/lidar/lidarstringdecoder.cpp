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

#include <stdint.h>

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include <opendavinci/odcore/data/Container.h>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lidar/lidar.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

LidarStringDecoder::LidarStringDecoder(odcore::io::conference::ContainerConference &a_conference, double a_x, double a_y, double a_z) 
    : m_conference(a_conference)
    , m_firstHeader(true)
    , m_startConfirmed(false)
    , m_settingsMode(false)
    , m_centimeterMode(false)
    , m_settingsDone(false)
    , m_counter(0)
    , m_bufferSize(44)
    //, m_directions()
    //, m_radii()
    , m_latestReading()
{
  m_position[0] = a_x;
  m_position[1] = a_y;
  m_position[2] = a_z;

  m_startResponse[0] = 0x06;
  m_startResponse[1] = 0x02;
  m_startResponse[2] = 0x80;
  m_startResponse[3] = 0x03;
  m_startResponse[4] = 0x00;
  m_startResponse[5] = 0xA0;
  m_startResponse[6] = 0x00;
  m_startResponse[7] = 0x10; //11?
  m_startResponse[8] = 0x16; //17?
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
  m_centimeterResponse[41] = 0x10; //11?
  m_centimeterResponse[42] = 0xB0; //B1?
  m_centimeterResponse[43] = 0x11;
}

LidarStringDecoder::~LidarStringDecoder()
{
}

bool LidarStringDecoder::CheckForCentimeterResponse()
{
  for(uint32_t j = 0; j < 44; j++) {
    if(m_buffer[m_bufferSize - 44 + j] != m_centimeterResponse[j]) { 
      return false; 
    }        
  }   
  m_counter = 0;
  return true; 
  
}

bool LidarStringDecoder::CheckForMeasurementHeader()
{
  for(uint32_t j = 0; j < 7; j++) {
    if(m_buffer[m_bufferSize - 7 + j] != m_measurementHeader[j]) { 
      return false; 
    }
  }
  
  return true; 
  m_counter = 0;
}

bool LidarStringDecoder::CheckForSettingsResponse()
{
  for(uint32_t j = 0; j < 10; j++) {
    if(m_buffer[m_bufferSize - 10 + j] != m_startResponse[j]) { 
      return false;
    }        
  }

  m_counter = 0;
  m_settingsDone = true;

  return true;
}

bool LidarStringDecoder::CheckForStartResponse()
{
  for(uint32_t j = 0; j < 10; j++) {
    if(m_buffer[m_bufferSize - 10 + j] != m_startResponse[j]) { 
      return false;
    }        
  }

  m_counter = 0;

  return true;
}

opendlv::proxy::EchoReading LidarStringDecoder::GetLatestReading() const
{
//  std::lock_guard<std::mutex> guard(g_readingMutex);
  return m_latestReading;
}

void LidarStringDecoder::ConvertToDistances()
{
  uint32_t byte1;
  uint32_t byte2;
  uint32_t distance;
  double cartesian[2];
  double PI = 3.14159265;

  std::vector<opendlv::model::Direction> directions; // m_directions.clear();
  std::vector<double> radii; // m_radii.clear();

  for(uint32_t i = 0; i < 361; i++) {
    byte1 = (int)m_measurements[i*2];
    byte2 = (int)m_measurements[i*2+1];
    distance = byte1 + (byte2%32)*256; //Integer centimeters in local polar coordinates

    if(distance < 7500) { //We don't send max range responses
      cartesian[0] = distance * sin(PI * i /360.0) / 100.0; //Local cartesian coordinates in meters (rotated coordinate system)
      cartesian[1] = distance * (-cos(PI * i /360.0)) / 100.0;
      cartesian[0] += m_position[0]; //Truck cartesian coordinates
      cartesian[1] += m_position[1];

      
      double radius = std::sqrt(pow(cartesian[0],2) + std::pow(cartesian[1],2));
      float angle = static_cast<float>(std::atan2(cartesian[1],cartesian[0]));
      opendlv::model::Direction direction(angle,0);
      directions.push_back(direction);
      radii.push_back(radius);
    }
  }

 // std::lock_guard<std::mutex> guard(g_readingMutex);

  //std::cout << "radius: " << radii[0];

  m_latestReading.setListOfDirections(directions);
  m_latestReading.setListOfRadii(radii);
  m_latestReading.setNumberOfPoints(radii.size());
  //std::cout << " in latestreading: " << m_latestReading.getListOfRadii()[0] << std::endl;
}

void LidarStringDecoder::nextString(std::string const &a_string) 
{
  //For debugging
  //std::string alpha = "0123456789ABCDEF";

  const uint32_t stringLength = a_string.length();

  for(uint32_t i = 0; i < stringLength; i++) {

    unsigned char byte = static_cast<unsigned char>(a_string[i]);

    //Updating buffer
    for(uint32_t j = 0; j < m_bufferSize - 1; j++) { 
      m_buffer[j] = m_buffer[j+1]; 
    }

    m_buffer[m_bufferSize-1] = byte;

    
    //For debugging
    //std::cout << alpha[(int)byte/16] << alpha[(int)byte%16] << ' ';

    
  
    m_measurements[m_counter] = byte;
    m_counter++;

    if(!m_startConfirmed) {

      if(m_settingsDone) {
        m_startConfirmed = CheckForStartResponse();
      }
      else {
        m_settingsMode = CheckForSettingsResponse(); //Happens to be identical to startresponse
      }
      m_centimeterMode = CheckForCentimeterResponse();

    }
    else if(CheckForMeasurementHeader()) {
      if(m_firstHeader) {
        m_firstHeader = false;
        std::cout << "Configuration done, output started" << std::endl;
      }
      else {
        std::cout << "ConvertToDistances" << std::endl;
        ConvertToDistances();
        m_counter = 0;
      }
    }

    if(m_counter > 999) { //Safety clause
      m_counter--;
    }
  }
}

bool LidarStringDecoder::IsCentimeterMode() const
{
  return m_centimeterMode;
}

bool LidarStringDecoder::IsRunning() const
{
  return m_startConfirmed;
}

bool LidarStringDecoder::IsSettingsMode() const
{
  return m_settingsMode;
}

void LidarStringDecoder::NotCentimeterMode()
{
  m_centimeterMode = false;
}

void LidarStringDecoder::NotSettingsMode()
{
  m_settingsMode = false;
}

} // gps
} // proxy
} // opendlv
