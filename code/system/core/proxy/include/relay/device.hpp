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

#ifndef RELAY_DEVICE_HPP_
#define RELAY_DEVICE_HPP_

#include <vector>

namespace opendlv {
namespace proxy {
namespace relay {

class Device {
 public:
  Device(std::vector<bool>);
  Device(Device const &) = delete;
  Device &operator=(Device const &) = delete;
  virtual ~Device();
  virtual bool IsActive(uint16_t const) const = 0;
  virtual void Reset() = 0;
  virtual void SetValue(uint16_t const, bool const) = 0;

 protected:
  std::vector<bool> m_initialValues;
};

} // relay
} // proxy
} // opendlv

#endif
