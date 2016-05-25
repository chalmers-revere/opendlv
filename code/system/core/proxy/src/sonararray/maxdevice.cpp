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

#include <fstream>

#include "sonararray/maxdevice.hpp"

namespace opendlv {
namespace proxy {
namespace sonararray {

/**
 * Constructor.
 *
 */
MaxDevice::MaxDevice(std::vector<bool> a_initialValues, 
    std::vector<uint16_t> a_pins):
  Device(a_initialValues),
  m_pins(a_pins)
{
  std::string path = std::string("/sys/class/max/");
  std::string filename = path + "export";
  std::ofstream exportFile(filename, std::ofstream::out);
  
  for (uint16_t i = 0; i < a_pins.size(); i++) {

    uint16_t pin = a_pins[i];
    bool initialValue = a_initialValues[i];

    exportFile << pin;

    std::string maxDir = path + "max" + std::to_string(pin) + "/";
    std::string maxDirectionFilename = maxDir + "direction";
    std::string maxValueFilename = maxDir + "value";

    std::ofstream maxDirectionFile(maxDirectionFilename, std::ofstream::out);
    maxDirectionFile << "out";

    std::ofstream maxValueFile(maxValueFilename, std::ofstream::out);
    maxDirectionFile << static_cast<uint16_t>(initialValue);

    maxDirectionFile.close();
    maxValueFile.close();
  }

  exportFile.close();
}

MaxDevice::~MaxDevice()
{
  std::string path = std::string("/sys/class/max/");
  std::string filename = path + "unexport";
  std::ofstream unexportFile(filename, std::ofstream::out);
  
  for (uint16_t pin : m_pins) {
    unexportFile << pin;
  }

  unexportFile.close();
}

bool MaxDevice::IsActive(uint16_t const a_index) const
{
  uint16_t pin = m_pins[a_index];

  std::string filename = std::string("/sys/class/max/max" 
      + std::to_string(pin) + "/value");
  std::ifstream file(filename, std::ifstream::in);
  
  std::string line;
  std::getline(file, line);
  bool value = std::stoi(line);

  file.close();

  return value;
}

void MaxDevice::SetValue(uint16_t const a_index, bool const a_value)
{
  uint16_t pin = m_pins[a_index];

  std::string filename = std::string("/sys/class/max/max" 
      + std::to_string(pin) + "/value");
  std::ofstream file(filename, std::ofstream::out);
  
  file << a_value;

  file.close();
}

} // sonararray
} // proxy
} // opendlv

