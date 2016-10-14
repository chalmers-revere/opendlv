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

#include "relay/relay.hpp"
#include "relay/gpiodevice.hpp"

namespace opendlv {
namespace proxy {
namespace relay {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Relay::Relay(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-relay")
    , m_device()
{
}

Relay::~Relay()
{
}

void Relay::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::proxy::RelayRequest::ID()) {
    opendlv::proxy::RelayRequest request = 
        a_container.getData<opendlv::proxy::RelayRequest>();

    std::string deviceId = request.getDeviceId();

    if (deviceId != getIdentifier()) {
      return;
    }

    bool relayValue = request.getRelayValue();
    uint8_t relayIndex = request.getRelayIndex();

    m_device->SetValue(relayIndex, relayValue);
  }
}

void Relay::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-relay.type");

  std::string const valuesString = 
      kv.getValue<std::string>("proxy-relay.values");

  std::vector<bool> values;

  std::vector<std::string> valuesVector = 
      odcore::strings::StringToolbox::split(valuesString, ',');
  for (auto valueString : valuesVector) {
    bool value = static_cast<bool>(std::stoi(valueString));
    values.push_back(value);
  }

  if (type.compare("gpio") == 0) {
    std::string const pinsString = 
        kv.getValue<std::string>("proxy-relay.gpio.pins");

    std::vector<uint16_t> pins;

    std::vector<std::string> pinsVector = 
        odcore::strings::StringToolbox::split(pinsString, ',');
    for (auto pinString : pinsVector) {
      uint16_t pin = std::stoi(pinString);
      pins.push_back(pin);
    }

     m_device = std::unique_ptr<Device>(new GpioDevice(values, pins));
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-relay] No valid device driver defined."
              << std::endl;
  }
}

void Relay::tearDown()
{
}

} // relay
} // proxy
} // opendlv
