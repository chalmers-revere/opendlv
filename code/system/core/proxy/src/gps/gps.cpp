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

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <opendavinci/odcore/base/KeyValueConfiguration.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/io/tcp/TCPFactory.h>
#include <opendlv/data/sensor/nmea/GPRMC.h>
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "gps/gps.hpp"
#include "gps/device.hpp"

namespace opendlv {
namespace proxy {
namespace gps {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Gps::Gps(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "proxy-gps")
    , m_device()
    , m_trimble()
{
}

Gps::~Gps()
{
}

void Gps::setUp()
{
  using namespace std;
  using namespace odcore::io::tcp;

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-gps.type");
  /*  std::string const port =
    kv.getValue<std::string>("proxy-gps.port");
    float const mountX = kv.getValue<float>("proxy-gps.mount.x");
    float const mountY = kv.getValue<float>("proxy-gps.mount.y");
    float const mountZ = kv.getValue<float>("proxy-gps.mount.z");
  */
  if (type.compare("trimble") == 0) {
    //      m_device = std::unique_ptr<Device>(new TrimbleDevice());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[" << getName() << "] No valid device driver defined."
              << std::endl;
  }

  {
    string RECEIVER = kv.getValue<std::string>("proxy-gps.trimble.ip");
    uint32_t PORT = kv.getValue<uint32_t>("proxy-gps.trimble.port");

    try {
        m_trimble = shared_ptr<TCPConnection>(TCPFactory::createTCPConnectionTo(RECEIVER, PORT));
        m_trimble->setRaw(true);
        m_trimble->setStringListener(this);
        m_trimble->start();
    }
    catch(string &exception) {
        cerr << "[" << getName() << "] Could not connect to Trimble: " << exception << endl;
    }
  }
}

void Gps::tearDown()
{
}

void Gps::nextString(std::string const &s) {


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
  bool gotGphdt = false;

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

    if (type == "GPGGA") {
      gotGpgga = true;

      timestamp = std::stod(fields.at(1));
      
      latitude = std::stod(fields.at(2));
      latitudeDirection = fields.at(3)[0];
      
      longitude = std::stod(fields.at(4));
      longitudeDirection = fields.at(5)[0];

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

      // Convert to m/s.
      speed = std::stof(fields.at(7)) / 3.6f;

    } else if (type == "GPHDT") {
      gotGphdt = true;

      std::string headingStr = fields.at(1);
      if (headingStr.empty()) {
        northHeading = 0.0f;
        hasHeading = false;
      } else {
        northHeading = std::stof(headingStr);
        hasHeading = true;
      }

    } else if (type == "GPRMC") {

    } else {
      std::cout << "[proxy-gps] WARNING: Unknown packet type." << std::endl;
    }
  }

  // according to the page 11 of this guide http://www2.etown.edu/wunderbot/DOWNLOAD/AgGPS114/NMEA_Messages_RevA_Guide_ENG.pdf
  // the GPS out data with the following format dd mm,mmmm
  // convert to deg befor sending
  latitude=latitude/100;
  longitude=longitude/100;

  // just a check before sending the signal!
  // std::cout << " GPS reading signals : LAT   " << latitude << "  LONG    " << longitude << std::endl;


  if (gotGpgga && gotGpvtg && gotGphdt) {
    opendlv::proxy::GpsReading nextGps(timestamp, latitude, longitude,
        altitude, northHeading, speed, latitudeDirection, longitudeDirection,
        satelliteCount, hasHeading, hasRtk);
    odcore::data::Container c(nextGps);
    getConference().send(c);
  }


  // Send opendlv::proxy::Wgs84Gps
}

void Gps::nextContainer(odcore::data::Container &c) {
  (void)c;
}

} // gps
} // proxy
} // opendlv
