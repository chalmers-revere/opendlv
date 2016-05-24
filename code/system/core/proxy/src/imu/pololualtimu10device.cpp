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

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "imu/pololualtimu10device.hpp"

namespace opendlv {
namespace proxy {
namespace imu {

/**
 * Constructor.
 *
 */
PololuAltImu10Device::PololuAltImu10Device(std::string const &a_deviceName) :
    Device(),
    m_deviceFile()
{
  m_deviceFile = open(a_deviceName.c_str(), O_RDWR);
  if (m_deviceFile < 0) {
    std::cerr << "Failed to open the i2c bus." << std::endl;
    return;
  }
 
  std::cout << "I2C bus " << a_deviceName << " opened successfully." 
      << std::endl;

  uint8_t address = 0x6b;
  if (ioctl(m_deviceFile, I2C_SLAVE, address) < 0) {
    std::cerr << "Failed to acquire bus access or talk to slave device." 
        << std::endl;
    return;
  }

  I2cWriteRegister(0x10, 0x80);
  I2cWriteRegister(0x11, 0x80);
  I2cWriteRegister(0x12, 0x04);

  m_initialized = true;
}

PololuAltImu10Device::~PololuAltImu10Device()
{
}

void PololuAltImu10Device::I2cWriteRegister(uint8_t a_register, uint8_t a_value)
{
  char buffer[2];
  buffer[0] = a_register;
  buffer[1] = a_value;

  uint8_t status = write(m_deviceFile, buffer, 2);

  if (status != 2) {
    std::cerr <<  "Failed to write to the i2c bus." << std::endl;
    return;
  }
}

opendlv::proxy::AccelerometerReading PololuAltImu10Device::ReadAccelerometer()
{
  char buffer[1];
  buffer[0] = 0x28;

  uint8_t status = write(m_deviceFile, buffer, 1);
  if (status != 1) {
    std::cerr <<  "Failed to write to the i2c bus." << std::endl;
    return nullptr;
  }

  char outBuffer[6];
  status = read(m_deviceFile, outBuffer, 6);
  if (status != 6) {
    std::cerr <<  "Failed to read to the i2c bus." << std::endl;
  }

  uint8_t xla = outBuffer[0];
  uint8_t xha = outBuffer[1];
  uint8_t yla = outBuffer[2];
  uint8_t yha = outBuffer[3];
  uint8_t zla = outBuffer[4];
  uint8_t zha = outBuffer[5];

  uint16_t x = xha;
  x = (x << 8) | xla;

  uint8_t y = yha;
  y = (y << 8) | yla;

  uint8_t z = zha;
  z = (z << 8) | zla;
  
  //a.x = ~a.x + 1;
  //a.y = ~a.y + 1;
  //a.z = ~a.z + 1;

  float scaledX = static_cast<float>(x);
  float scaledY = static_cast<float>(y);
  float scaledZ = static_cast<float>(z);

  float reading[] = {scaledX, scaledY, scaledZ};
  opendlv::proxy::AccelerometerReading accelerometerReading(reading);
  return accelerometerReading;
}

opendlv::proxy::GyroscopeReading PololuAltImu10Device::ReadGyroscope()
{
  char buffer[1];
  buffer[0] = 0x22;

  uint8_t status = write(m_deviceFile, buffer, 1);
  if (status != 1) {
    std::cerr <<  "Failed to write to the i2c bus." << std::endl;
    return nullptr;
  }

  char outBuffer[6];
  status = read(m_deviceFile, outBuffer, 6);
  if (status != 6) {
    std::cerr <<  "Failed to read to the i2c bus." << std::endl;
  }

  uint8_t xla = outBuffer[0];
  uint8_t xha = outBuffer[1];
  uint8_t yla = outBuffer[2];
  uint8_t yha = outBuffer[3];
  uint8_t zla = outBuffer[4];
  uint8_t zha = outBuffer[5];

  uint16_t x = xha;
  x = (x << 8) | xla;

  uint8_t y = yha;
  y = (y << 8) | yla;

  uint8_t z = zha;
  z = (z << 8) | zla;
  
  //a.x = ~a.x + 1;
  //a.y = ~a.y + 1;
  //a.z = ~a.z + 1;

  float scaledX = static_cast<float>(x);
  float scaledY = static_cast<float>(y);
  float scaledZ = static_cast<float>(z);

  float reading[] = {scaledX, scaledY, scaledZ};
  opendlv::proxy::GyroscopeReading gyroscopeReading(reading);
  return gyroscopeReading;
}

} // imu
} // proxy
} // opendlv
