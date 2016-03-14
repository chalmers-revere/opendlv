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

#ifndef SENSOR_SONARARRAY_DEVICE_HPP_
#define SENSOR_SONARARRAY_DEVICE_HPP_

namespace opendlv {
namespace proxy {
namespace sensor {
namespace sonararray {

class Device {
 public:
  Device();
  Device(Device const &) = delete;
  Device &operator=(Device const &) = delete;
  virtual ~Device();
};

} // sonararray
} // sensor
} // proxy
} // opendlv

#endif
