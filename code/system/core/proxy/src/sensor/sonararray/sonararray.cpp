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

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "sensor/sonararray/sonararray.hpp"
#include "sensor/sonararray/device.hpp"

namespace opendlv {
namespace proxy {
namespace sensor {
namespace sonararray {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
SonarArray::SonarArray(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-sensor-sonararray")
    , m_device()
{
}

SonarArray::~SonarArray()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SonarArray::body()
{
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void SonarArray::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type =
  kv.getValue<std::string>("proxy-sensor-sonararray.type");

  if (type.compare("maxsonar") == 0) {
    //      m_device = std::unique_ptr<Device>(new MaxSonarDevice());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-sensor-sonararray] No valid device driver defined."
              << std::endl;
  }
}

void SonarArray::tearDown()
{
}

} // sonararray
} // sensor
} // proxy
} // opendlv
