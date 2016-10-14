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

#include <cmath>
#include <iostream>

#include "sonararray/device.hpp"

namespace opendlv {
namespace proxy {
namespace sonararray {

/**
 * Constructor.
 *
 */
Device::Device(std::vector<opendlv::model::Cartesian3> a_positions, 
    std::vector<opendlv::model::Direction> a_directions):
    m_directions(a_directions),
    m_positions(a_positions)
{
}

Device::~Device()
{
}

opendlv::proxy::EchoReading Device::GetEchoReadings()
{
  auto distances = GetSonarReadings();
  uint16_t numberOfPoints = distances.size();

  std::vector<opendlv::model::Direction> directions;
  std::vector<double> radii;
  for (uint16_t i = 0; i < numberOfPoints; i++) {
    float distance = distances[i];

    float azimuth = m_directions[i].getAzimuth();
    float zenith = m_directions[i].getZenith();

    float x = m_positions[i].getX() + distance * std::sin(azimuth);
    float y = m_positions[i].getY() + distance * std::cos(azimuth);
    float z = m_positions[i].getZ() + distance * std::sin(zenith);
    
    double radius = std::sqrt(x * x + y * y + z * z);
    radii.push_back(radius);

    float azimuth2 = std::atan2(y, z);
    float zenith2 = std::atan2(z, x);
    opendlv::model::Direction direction(azimuth2, zenith2);
    directions.push_back(direction);
  }

  opendlv::proxy::EchoReading echoReading(directions, radii, numberOfPoints);
  return echoReading;
}

} // sonararray
} // proxy
} // opendlv
