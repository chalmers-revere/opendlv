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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "sonararray/sonararray.hpp"
#include "sonararray/maxdevice.hpp"

namespace opendlv {
namespace proxy {
namespace sonararray {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
SonarArray::SonarArray(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-sonararray")
    , m_device()
{
}

SonarArray::~SonarArray()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SonarArray::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    auto sonarReadings = m_device->GetEchoReadings();
    odcore::data::Container sonarReadingContainer(sonarReadings);
    getConference().send(sonarReadingContainer);
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void SonarArray::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-sonararray.type");

  std::string const xsString = 
      kv.getValue<std::string>("proxy-sonararray.mount.x");
  std::vector<std::string> xsVector = 
      odcore::strings::StringToolbox::split(xsString, ',');
  std::string const ysString = 
      kv.getValue<std::string>("proxy-sonararray.mount.y");
  std::vector<std::string> ysVector = 
      odcore::strings::StringToolbox::split(ysString, ',');
  std::string const zsString = 
      kv.getValue<std::string>("proxy-sonararray.mount.z");
  std::vector<std::string> zsVector = 
      odcore::strings::StringToolbox::split(zsString, ',');
  std::string const azimuthsString = 
      kv.getValue<std::string>("proxy-sonararray.mount.azimuth");
  std::vector<std::string> azimuthsVector = 
      odcore::strings::StringToolbox::split(azimuthsString, ',');
  std::string const zenithsString = 
      kv.getValue<std::string>("proxy-sonararray.mount.zenith");
  std::vector<std::string> zenithsVector = 
      odcore::strings::StringToolbox::split(zenithsString, ',');

  uint16_t length = xsVector.size();
  if (length != ysVector.size() || length != zsVector.size()
      || length != azimuthsVector.size() || length != zenithsVector.size()) {
    std::cerr << "[proxy-sonararray] Vector size mismatch." << std::endl;
  }

  std::vector<opendlv::model::Cartesian3> positions;
  std::vector<opendlv::model::Direction> directions;
  for (uint16_t i = 0; i < length; i++) {
    float x = std::stof(xsVector[i]);
    float y = std::stof(ysVector[i]);
    float z = std::stof(zsVector[i]);
    opendlv::model::Cartesian3 position(x, y, z);
    positions.push_back(position);

    float azimuth = std::stof(azimuthsVector[i]);
    float zenith = std::stof(zenithsVector[i]);
    opendlv::model::Direction direction(azimuth, zenith);
    directions.push_back(direction);
  }
  
  if (type.compare("max") == 0) {
    std::vector<uint16_t> pins;
    std::string const pinsString = 
        kv.getValue<std::string>("proxy-sonararray.max.pin");
    std::vector<std::string> pinsVector = 
        odcore::strings::StringToolbox::split(pinsString, ',');
    for (auto pinString : pinsVector) {
      uint16_t pin = std::stoi(pinString);
      pins.push_back(pin);
    }
    
    float scaleValue = kv.getValue<float>("proxy-sonararray.max.scale_value");

    m_device = std::unique_ptr<Device>(new MaxDevice(positions, directions,
        pins, scaleValue));
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-sonararray] No valid device driver defined."
              << std::endl;
  }
}

void SonarArray::tearDown()
{
}

} // sonararray
} // proxy
} // opendlv
