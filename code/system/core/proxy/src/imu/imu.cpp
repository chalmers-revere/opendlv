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

#include "imu/imu.hpp"
#include "imu/device.hpp"

namespace opendlv {
namespace proxy {
namespace imu {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Imu::Imu(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-imu")
    , m_device()
{
}

Imu::~Imu()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Imu::body()
{

  // Send opendlv::proxy::Compass
  // Send opendlv::proxy::Gyroscope
  // Send opendlv::proxy::Accelerometer
  // Send opendlv::proxy::Altimeter

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Imu::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-imu.type");
  /*  std::string const port =
    kv.getValue<std::string>("proxy-imu.port");
    float const mountX = kv.getValue<float>("proxy-imu.mount.x");
    float const mountY = kv.getValue<float>("proxy-imu.mount.y");
    float const mountZ = kv.getValue<float>("proxy-imu.mount.z");
  */
  if (type.compare("pololu-10") == 0) {
    //      m_device = std::unique_ptr<Device>(new Pololu10Device());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-imu] No valid device driver defined."
              << std::endl;
  }
}

void Imu::tearDown()
{
}

} // imu
} // proxy
} // opendlv
