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

#ifndef RELAY_GPIODEVICE_HPP_
#define RELAY_GPIODEVICE_HPP_

#include <cstdint>
#include <iostream>
#include <string>

#include "relay/device.hpp"

namespace opendlv {
namespace proxy {
namespace relay {

class GpioDevice : public Device {
 public:
  GpioDevice(std::vector<bool>, std::vector<uint16_t>);
  GpioDevice(GpioDevice const &) = delete;
  GpioDevice &operator=(GpioDevice const &) = delete;
  virtual ~GpioDevice();
  bool IsActive(uint16_t const) const;
  void Reset();
  void SetValue(uint16_t const, bool const);

 private:
  std::vector<uint16_t> m_pins;
};

} // relay
} // proxy
} // opendlv

#endif
