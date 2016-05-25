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
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "sonararray/sonararray.hpp"
#include "sonararray/maxdevice.hpp"

namespace opendlv {
namespace proxy {
namespace sonararray {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
SonarArray::SonarArray(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-sonararray")
    , m_device()
{
}

SonarArray::~SonarArray()
{
}

void SonarArray::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::proxy::SonarArrayRequest::ID()) {
    opendlv::proxy::SonarArrayRequest request = 
    a_container.getData<opendlv::proxy::SonarArrayRequest>();

    std::string deviceId = request.getDeviceId();

    if (deviceId != getIdentifier()) {
      return;
    }

    bool sonararrayValue = request.getSonarArrayValue();
    uint8_t sonararrayIndex = request.getSonarArrayIndex();

    m_device->SetValue(sonararrayIndex, sonararrayValue);
  }
}

void SonarArray::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-sonararray.type");

  std::string const valuesString = 
      kv.getValue<std::string>("proxy-sonararray.values");

  std::vector<bool> values;

  std::vector<std::string> valuesVector = 
      odcore::strings::StringToolbox::split(valuesString, ',');
  for (auto valueString : valuesVector) {
    bool value = static_cast<bool>(std::stoi(valueString));
    values.push_back(value);
  }

  if (type.compare("max") == 0) {
    std::string const pinsString = 
        kv.getValue<std::string>("proxy-sonararray.max.pins");

    std::vector<uint16_t> pins;

    std::vector<std::string> pinsVector = 
        odcore::strings::StringToolbox::split(pinsString, ',');
    for (auto pinString : pinsVector) {
      uint16_t pin = std::stoi(pinString);
      pins.push_back(pin);
    }

     m_device = std::unique_ptr<Device>(new MaxDevice(values, pins));
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-sonararray] No valid device driver defined."
              << std::endl;
  }
}

void SonarArray::tearDown()
{
}

} // sonararray
} // proxy
} // opendlv
