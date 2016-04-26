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
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "camera/camera.hpp"
#include "camera/opencvdevice.hpp"

namespace opendlv {
namespace proxy {
namespace camera {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Camera::Camera(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-camera")
    , m_device()
{
}

Camera::~Camera()
{
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Camera::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    auto sharedImage = m_device->Capture();
    odcore::data::Container c(sharedImage);
    getConference().send(c);
    
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Camera::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-camera.type");
  std::string const resolution =
      kv.getValue<std::string>("proxy-camera.resolution");
  int32_t const bpp = kv.getValue<int32_t>("proxy-camera.bpp");

  /*
  float const mountX = kv.getValue<float>("proxy-camera.mount.x");
  float const mountY = kv.getValue<float>("proxy-camera.mount.y");
  float const mountZ = kv.getValue<float>("proxy-camera.mount.z");
  float const mountRoll = kv.getValue<float>("proxy-camera.mount.roll");
  float const mountPitch = kv.getValue<float>(
      "proxy-camera.mount.pitch");
  float const mountYaw = kv.getValue<float>("proxy-camera.mount.yaw");
  */

  std::size_t const pos = resolution.find("x");
  std::size_t const len = resolution.length();
  int const width = stoi(resolution.substr(0, pos));
  int const height = stoi(resolution.substr(pos + 1, len - pos - 1));

  if (type.compare("opencv-ip-axis") == 0) {
    std::string port = kv.getValue<std::string>("proxy-camera.port");
    std::string username = kv.getValue<std::string>(
        "proxy-camera.username");
    std::string password = kv.getValue<std::string>(
        "proxy-camera.password");
    
    std::string const name = kv.getValue<std::string>(
        "proxy-camera.name");
    m_device =
    std::unique_ptr<Device>(new OpenCvDevice(name, port, username, password, 
        width, height, bpp));
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-camera] No valid device driver defined."
              << std::endl;
  }
}

void Camera::tearDown()
{
}

} // camera
} // proxy
} // opendlv
