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

#include <cmath>

#include <iostream>
#include <string>
#include <vector>

#include <opendavinci/odcore/data/Container.h>

#include "lidar/lidar.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

LidarStringDecoder::LidarStringDecoder(odcore::io::conference::ContainerConference &a_conference, double a_x, double a_y, double a_z)
    : m_conference(a_conference)
    , m_header(false)
    , m_startConfirmed(false)
    , m_latestReading()
    , m_buffer()
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

void LidarStringDecoder::ConvertToDistances()
{
  uint32_t byte1;
  uint32_t byte2;
  uint32_t distance;
  double cartesian[2];

  std::vector<opendlv::model::Direction> directions; // m_directions.clear();
  std::vector<double> radii;                         // m_radii.clear();

  for (uint32_t i = 0; i < 361; i++) {
    byte1 = (int)m_measurements[i * 2];
    byte2 = (int)m_measurements[i * 2 + 1];

    if (i < 361) {
      distance = byte1 + (byte2 % 32) * 256; //Integer centimeters in local polar coordinates
    }
    else {
      distance = byte2 + (byte1 % 32) * 256; //Integer centimeters in local polar coordinates
    }

    if (distance < 7500) {                                     //We don't send max range responses
      cartesian[0] = distance * sin(M_PI * i / 360.0) / 100.0; //Local cartesian coordinates in meters (rotated coordinate system)
      cartesian[1] = distance * (-cos(M_PI * i / 360.0)) / 100.0;
      cartesian[0] += m_position[0]; //Truck cartesian coordinates
      cartesian[1] += m_position[1];

      double radius = std::sqrt(pow(cartesian[0], 2) + std::pow(cartesian[1], 2));
      float angle = static_cast<float>(std::atan2(cartesian[1], cartesian[0]));
      opendlv::model::Direction direction(angle, 0);
      directions.push_back(direction);
      radii.push_back(radius);
    }
  }

  m_latestReading.setListOfDirections(directions);
  m_latestReading.setListOfRadii(radii);
  m_latestReading.setNumberOfPoints(radii.size());

  // Distribute data.
  odcore::data::Container c(m_latestReading);
  m_conference.send(c);
}

bool LidarStringDecoder::tryDecode()
{
  const uint32_t HEADER_LENGTH = 7;
  const uint32_t START_LENGTH = 10;
  const uint32_t MAX_NUMBER_OF_BYTES_PER_PAYLOAD = 722;
  static uint32_t byteCounter = 0;
  const string s = m_buffer.str();

  if (m_header && (byteCounter < MAX_NUMBER_OF_BYTES_PER_PAYLOAD)) {
    // Store bytes in receive measurement buffer.
    uint32_t processedBytes = 0;
    while ((byteCounter < MAX_NUMBER_OF_BYTES_PER_PAYLOAD) && (processedBytes < s.size())) {
      m_measurements[byteCounter] = static_cast<char>(s.at(processedBytes));
      processedBytes++; // Bytes processed in this cycle.
      byteCounter++;    // Bytes processed in total.
    }
    m_buffer.str(m_buffer.str().substr(processedBytes));

    return true;
  }

  if (m_header && (byteCounter == MAX_NUMBER_OF_BYTES_PER_PAYLOAD)) {
    cout << "Completed payload." << endl;
    // Do ray transformation.
    ConvertToDistances();
    // Consumed all measurements; find next header; there should be three bytes trailing before the next sequence begins.
    m_header = false;
    return false;
  }

  // Find header.
  if (!m_header && (s.size() >= HEADER_LENGTH)) {
    m_header = true;
    for (uint32_t i = 0; (i < HEADER_LENGTH) && m_header; i++) {
      m_header &= ((int)(uint8_t)s.at(i) == (int)(uint8_t)m_measurementHeader[i]);
    }
    if (m_header) {
      cout << "Received header." << endl;
      // Remove 7 bytes.
      m_buffer.str(m_buffer.str().substr(HEADER_LENGTH));
      m_buffer.seekg(0, ios_base::end);

      // Reset byte counter for processing payload.
      byteCounter = 0;
      return true;
    }
    // Not found, try next state.
  }

//    // Not working reliably.
//    // Find centimeter mode.
//    if ((!m_centimeterMode) && (m_buffer.str().size() >= 44)) {
//        m_centimeterMode = true;
//        for (uint32_t i = 0; i < 44; i++) {
//            cout << "s = " << (int)(uint8_t)s.at(i) << ", resp = " << (int)(uint8_t)m_centimeterResponse[i] << endl;
//            m_centimeterMode &= ((int)(uint8_t)s.at(i) == (int)(uint8_t) m_centimeterResponse[i]);
//        }
//        if (m_centimeterMode) {
//          cout << "Received centimeterMode." << endl;
//            // Remove 10 bytes.
//            m_buffer.str(m_buffer.str().substr(44));
//            m_buffer.seekg(0, ios_base::end);
//            return true;
//        }
//    }

  // Find start confirmation.
  if (s.size() >= START_LENGTH) {
    // Try to find start message.
    m_startConfirmed = true;
    for (uint32_t i = 0; (i < START_LENGTH) && m_startConfirmed; i++) {
      // Byte 7 and 8 might be different.
      if ((i != 7) && (i != 8)) {
        m_startConfirmed &= ((int)(uint8_t)s.at(i) == (int)(uint8_t)m_startResponse[i]);
      }
    }
    if (m_startConfirmed) {
      cout << "Received start confirmation." << endl;
      // Remove 10 bytes.
      m_buffer.str(m_buffer.str().substr(START_LENGTH));
      m_buffer.seekg(0, ios_base::end);
      return true;
    }
    // Not found, shorten buffer.
  }

  // Nothing processed.
  return false;
}

void LidarStringDecoder::nextString(std::string const &a_string)
{
  for (uint32_t i = 0; i < a_string.size(); i++) {
    char c = a_string.at(i);
    m_buffer.write(&c, sizeof(char));
  }

  string s = m_buffer.str();
  // We need at least 10 bytes before we can continue.
  if (s.size() >= 10) {
    while (s.size() > 0) {
      if (tryDecode()) {
        // If decoding succeeds, don't modify buffer but try to consume more.
      }
      else {
        // Remove first byte before continuing processing.
        m_buffer.str(m_buffer.str().substr(1));
      }
      // Check remaining length.
      s = m_buffer.str();
    }
  }

  // Always add at the end for more bytes to buffer.
  m_buffer.seekg(0, ios_base::end);
}

} // gps
} // proxy
} // opendlv
