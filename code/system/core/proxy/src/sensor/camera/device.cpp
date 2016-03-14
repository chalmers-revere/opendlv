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

#include <iostream>

#include "opendavinci/odcore/base/Lock.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"

#include "sensor/camera/device.hpp"

namespace opendlv {
namespace proxy {
namespace sensor {
namespace camera {

/**
 * Constructor.
 *
 * @param name Name of the shared memory segment.
 * @param id Device identifier.
 * @param width
 * @param height
 * @param bpp
 */
Device::Device(std::string const &a_name, uint32_t const &a_id,
uint32_t const &a_width, uint32_t const &a_height, uint32_t const &a_bpp)
    : m_name(a_name)
    , m_id(a_id)
    , m_width(a_width)
    , m_height(a_height)
    , m_bpp(a_bpp)
    , m_size(a_width * a_height * a_bpp)
    , m_sharedImage()
    , m_sharedMemory()
{
  m_sharedMemory =
  odcore::wrapper::SharedMemoryFactory::createSharedMemory(a_name, m_size);

  m_sharedImage.setName(a_name);
  m_sharedImage.setWidth(a_width);
  m_sharedImage.setHeight(a_height);
  m_sharedImage.setBytesPerPixel(a_bpp);
  m_sharedImage.setSize(m_size);
}

Device::~Device()
{
}

std::string const Device::GetName() const
{
  return m_name;
}

uint32_t Device::GetId() const
{
  return m_id;
}

uint32_t Device::GetWidth() const
{
  return m_width;
}

uint32_t Device::GetHeight() const
{
  return m_height;
}

uint32_t Device::GetBpp() const
{
  return m_bpp;
}

uint32_t Device::GetSize() const
{
  return m_size;
}

/**
 * @return Meta information about the image.
 */
odcore::data::image::SharedImage Device::Capture()
{
  if (IsValid()) {
    if (CaptureFrame()) {
      if (m_sharedMemory.get() && m_sharedMemory->isValid()) {
        odcore::base::Lock l(m_sharedMemory);
        CopyImageTo(
        static_cast<char *>(m_sharedMemory->getSharedMemory()), m_size);
      }
    }
  }
  return m_sharedImage;
}

} // camera
} // sensor
} // proxy
} // opendlv
