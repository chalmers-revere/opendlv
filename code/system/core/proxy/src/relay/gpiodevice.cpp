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

#include "relay/gpiodevice.hpp"

namespace opendlv {
namespace proxy {
namespace relay {

/**
 * Constructor.
 *
 */
GpioDevice::GpioDevice(std::vector<bool> a_initialValues, 
    std::vector<uint16_t> a_pins, std::string const &a_deviceNode):
  Device(a_initialValues),
  m_device() 
{
  (void)a_pins;
  (void)a_deviceNode;
}

GpioDevice::~GpioDevice()
{
}

bool GpioDevice::IsActive(uint16_t const a_index) const
{
  (void) a_index;
  return false;
}

void GpioDevice::SetValue(uint16_t const a_index, bool const a_value)
{
  (void) a_index;
  (void) a_value;
}

} // relay
} // proxy
} // opendlv

