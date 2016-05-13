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
#include <iomanip>      // std::setprecision

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
    , m_gpsStringDecoder()
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

    // Separated string decoding from GPS messages from this class.
    // Therefore, we need to pass the getConference() reference to the other instance so that it can send containers.
    m_gpsStringDecoder = std::unique_ptr<GpsStringDecoder>(new GpsStringDecoder(getConference()));

    try {
        m_trimble = shared_ptr<TCPConnection>(TCPFactory::createTCPConnectionTo(RECEIVER, PORT));
        m_trimble->setRaw(true);
        // The other instance is handling strings.
        m_trimble->setStringListener(m_gpsStringDecoder.get());
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

void Gps::nextContainer(odcore::data::Container &c) {
  (void)c;
}

} // gps
} // proxy
} // opendlv
