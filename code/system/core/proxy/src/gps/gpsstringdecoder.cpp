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
#include <iomanip>      // std::setprecision
#include <iostream>
#include <sstream>
#include <vector>

#include <opendavinci/odcore/data/Container.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "gps/gps.hpp"
#include "gps/device.hpp"

namespace opendlv {
namespace proxy {
namespace gps {

GpsStringDecoder::GpsStringDecoder(odcore::io::conference::ContainerConference &conference) :
    m_conference(conference)
{
}

GpsStringDecoder::~GpsStringDecoder()
{
}

void GpsStringDecoder::nextString(std::string const &s) {
  double timestamp;
  double latitude;
  double longitude;
  float altitude;
  float northHeading;
  float speed;
  uint8_t latitudeDirection;
  uint8_t longitudeDirection;
  uint8_t satelliteCount;
  bool hasHeading;
  bool hasRtk;

  bool gotGpgga = false;
  bool gotGpvtg = false;

  std::vector<std::string> messages;

  std::istringstream ss1(s);
  std::string msg;
  while(std::getline(ss1, msg, '$')) {
    messages.push_back(msg);
  }

  if (messages.empty()) {
    return;
  }

  for (auto message : messages) {

    if (message.empty()) {
      continue;
    }

    std::vector<std::string> fields;

    std::istringstream ss2(message);
    std::string field;
    while(std::getline(ss2, field, ',')) {
      fields.push_back(field);
    }

    if (fields.empty()) {
      continue;
    }

    std::string type = fields.at(0);
    // for(auto qq : fields){
    //   std::cout << qq << ", ";
    // }

    if (type == "GPGGA") {
      gotGpgga = true;

      timestamp = std::stod(fields.at(1));
      
      latitude = std::stod(fields.at(2));
      latitudeDirection = fields.at(3)[0];
      
      longitude = std::stod(fields.at(4));
      longitudeDirection = fields.at(5)[0];

      // Convert from format dd mm,mmmm
      latitude=latitude / 100.0;
      longitude=longitude / 100.0;
      latitude = static_cast<int>(latitude) 
          + (latitude - static_cast<int>(latitude)) * 100.0 / 60.0;
      longitude = static_cast<int>(longitude) 
          + (longitude - static_cast<int>(longitude)) * 100.0 / 60.0;

      // std::cout << "[proxy-gpsstringdecoder] GPS received signals : Latitude : " << std::setprecision(19) << latitude
      //              << " Longitude : " << std::setprecision(19) << longitude << std::endl;


      // 0: Non valid, 1: GPS fix, 2: DGPS fix, 4: RTK fix int, 5: RTK float int
      int gpsQuality = std::stoi(fields.at(6));
      if (gpsQuality == 4 || gpsQuality == 5) {
        hasRtk = true;
      } else {
        hasRtk = false;
      }

      satelliteCount = std::stoi(fields.at(7));

   //   float hdop = std::stof(fields.at(8));

      altitude = std::stof(fields.at(9));
      //std::string altitudeUnit = fields.at(10);

  //    float geoidSeparation = std::stof(fields.at(11));
  //    std::string geodSeparationUnit = fields.at(12);

    } else if (type == "GPVTG") {
      gotGpvtg = true;

      std::string headingStr = fields.at(3);

      if (headingStr.empty()) {
        northHeading = 0.0f;
        hasHeading = false;
      } else {
        double pi = static_cast<double>(opendlv::Constants::PI);
        northHeading = std::stod(headingStr) * pi / 180.0;
        hasHeading = true;
      }

      // Convert to m/s.
      speed = std::stof(fields.at(7)) / 3.6f;

    } else if (type == "GPHDT") {

    } else if (type == "GPRMC") {

    } else {
      std::cout << "[proxy-gpsstringdecoder] WARNING: Unknown packet type. " << type << std::endl;
    }
  }

  std::cout << "[proxy-gpsstringdecoder] GPS sending signals : Latitude : " << latitude << std::setprecision(19)
               << " Longitude : " << std::setprecision(19) << longitude << " Heading: " << northHeading << " hasRtk: " << hasRtk << std::endl;


  if (gotGpgga && gotGpvtg) {
    opendlv::proxy::GpsReading nextGps(timestamp, latitude, longitude,
        altitude, northHeading, speed, latitudeDirection, longitudeDirection,
        satelliteCount, hasHeading, hasRtk);
    odcore::data::Container c(nextGps);
    m_conference.send(c);
    (void)nextGps;
  }

  // Send opendlv::proxy::Wgs84Gps
}

} // gps
} // proxy
} // opendlv
