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
MaxDevice::MaxDevice(std::vector<opendlv::model::Cartesian3> a_positions, 
    std::vector<opendlv::model::Direction> a_directions,
    std::vector<uint16_t> a_pins, float a_scaleValue):
  Device(a_positions, a_directions),
  m_pins(a_pins),
  m_scaleValue(a_scaleValue)
{
  std::string path = std::string("/sys/devices/platform/bone_capemgr/slots");
  std::ofstream exportFile(path, std::ofstream::out);
  exportFile << "BB-ADC";
  exportFile.close();
}

MaxDevice::~MaxDevice()
{
}

std::vector<float> MaxDevice::GetSonarReadings()
{
  std::vector<float> distances;

  for (uint16_t pin : m_pins) {

    // d = A * 0.0012

    std::string path = std::string("/sys/bus/iio/devices/iio:device0/"); 
    std::string filename = path + "in_voltage" + std::to_string(pin) + "_raw";

    std::ifstream file(filename, std::ifstream::in);
    
    std::string line;
    std::getline(file, line);
    int value = std::stoi(line);

    float distance = static_cast<float>(value) * m_scaleValue;

    distances.push_back(distance);

    file.close();
  }

  return distances;
}

} // sonararray
} // proxy
} // opendlv

