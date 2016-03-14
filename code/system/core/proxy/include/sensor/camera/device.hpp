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

#ifndef SENSOR_CAMERA_DEVICE_HPP_
#define SENSOR_CAMERA_DEVICE_HPP_

#include <stdint.h>
#include <memory>
#include <string>

#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"

namespace opendlv {
namespace proxy {
namespace sensor {
namespace camera {

/**
 * This class wraps a camera and captures its data into a shared memory segment.
 */
class Device {
 public:
  Device(std::string const &, uint32_t const &, uint32_t const &,
  uint32_t const &, uint32_t const &);
  Device(Device const &) = delete;
  Device &operator=(Device const &) = delete;
  virtual ~Device();
  odcore::data::image::SharedImage Capture();

 protected:
  /**
   * This method is responsible to copy the image from the
   * specific camera driver to the shared memory.
   *
   * @param dest Pointer where to copy the data.
   * @param size Number of bytes to copy.
   * @return true if the data was successfully copied.
   */
  virtual bool CopyImageTo(char *, uint32_t const &) = 0;
  virtual bool CaptureFrame() = 0;
  virtual bool IsValid() const = 0;
  std::string const GetName() const;
  uint32_t GetId() const;
  uint32_t GetWidth() const;
  uint32_t GetHeight() const;
  uint32_t GetBpp() const;
  uint32_t GetSize() const;

  std::string m_name;
  uint32_t m_id;
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_bpp;
  uint32_t m_size;

 private:
  odcore::data::image::SharedImage m_sharedImage;
  std::shared_ptr<odcore::wrapper::SharedMemory> m_sharedMemory;
};

} // camera
} // sensor
} // proxy
} // opendlv

#endif
