/**
 * Copyright (C) 2016 Chalmers REVERE
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
#include <string>
#include <vector>

#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
#include <cstdlib>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#endif

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "canmessagedatastore.hpp"
#include "joystick/joystick.hpp"

namespace opendlv {
namespace tools {
namespace can {
namespace joystick {

Joystick::Joystick(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-can-joystick")
    , automotive::odcantools::GenericCANMessageListener()
    , m_fifo()
    , m_device()
    , m_canMessageDataStore()
    , m_revereFh16CanMessageMapping()
    , m_joystickFD(0)
    , m_axes(0)
    , m_lastAxis0(0)
    , m_lastAxis1(0)
{
}

Joystick::~Joystick()
{
}

void Joystick::setUp()
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odtools::recorder;
  using namespace automotive::odcantools;

  string const JOYSTICK_DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>(
  "tools-can-joystick.joystickdevicenode");

  string const DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("tools-can-joystick.devicenode");

  // Try to open CAN device and register this instance as receiver for
  // GenericCANMessages.
  m_device = shared_ptr<CANDevice>(new CANDevice(DEVICE_NODE, *this));

  // If the device could be successfully opened, create a recording file with a
  // dump of the data.
  if (m_device.get() && m_device->isOpen()) {
    cout << "Successfully opened CAN device '" << DEVICE_NODE << "'." << endl;

    // Start the wrapped CAN device to receive CAN messages concurrently.
    m_device->start();
  }

  // Setup joystick control.
  {
    cout << "Trying to open joystick " << JOYSTICK_DEVICE_NODE << endl;
#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
    int num_of_axes = 0;
    int num_of_buttons = 0;

    int name_of_joystick[80];

    if ((m_joystickFD = open(JOYSTICK_DEVICE_NODE.c_str(), O_RDONLY)) == -1) {
      cerr << "Could not open joystick " << JOYSTICK_DEVICE_NODE << endl;
      return;
    }

    ioctl(m_joystickFD, JSIOCGAXES, &num_of_axes);
    ioctl(m_joystickFD, JSIOCGBUTTONS, &num_of_buttons);
    ioctl(m_joystickFD, JSIOCGNAME(80), &name_of_joystick);

    m_axes = (int *)calloc(num_of_axes, sizeof(int));
    cerr << "Joystick found " << name_of_joystick
         << ", number of axes: " << num_of_axes
         << ", number of buttons: " << num_of_buttons << endl;

    // Use non blocking reading.
    fcntl(m_joystickFD, F_SETFL, O_NONBLOCK);
#endif
  }
}

void Joystick::tearDown()
{
  if (m_device.get()) {
    // Stop the wrapper CAN device.
    m_device->stop();
  }

  // Deactivate joystick control.
  {
    close(m_joystickFD);
  }
}

void Joystick::nextGenericCANMessage(
const automotive::GenericCANMessage &gcm)
{
  // Map CAN message to high-level data structure.
  vector<odcore::data::Container> result = m_revereFh16CanMessageMapping.mapNext(gcm);

  if (result.size() > 0) {
    auto it = result.begin();
    while (it != result.end()) {
      odcore::data::Container c = (*it);
      // Send container to conference.
      getConference().send(c);
      it++;
    }
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Joystick::body()
{
  const double MAX_RANGE = 32768.0;
  const double FACTOR_ACCELERATION = 80;
  const double FACTOR_ROTATION = 30;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    {
#if !defined(WIN32) && !defined(__gnu_hurd__) && !defined(__APPLE__)
      struct js_event js;
      read(m_joystickFD, &js, sizeof(struct js_event));

      // Check event.
      switch (js.type & ~JS_EVENT_INIT) {
        case JS_EVENT_AXIS:
          m_axes[js.number] = js.value;
          break;
        default:
          break;
      }
#endif

      double acceleration = 0;
      double deceleration = 0;
      double rotation = 0;
      if (m_axes[1] > 0) {
        // Braking.
        deceleration = (m_axes[1] / MAX_RANGE) * FACTOR_ACCELERATION;
      }
      else {
        // Accelerating.
        acceleration = ((-m_axes[1]) / MAX_RANGE) * FACTOR_ACCELERATION;
      }

      // Steering.
      rotation = (m_axes[0] / MAX_RANGE) * FACTOR_ROTATION;

      cout << "Values: A: " << acceleration << ", B: " << deceleration
           << ", R: " << rotation << endl;
    }


    opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
    brakeRequest.setEnableRequest(false);
    brakeRequest.setBrake(0);

    // Create the message mapping.
    canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container c(brakeRequest);
    automotive::GenericCANMessage gcm = brakeRequestMapping.encode(c);
    //    m_device->write(gcm);
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // joystick
} // can
} // tools
} // opendlv
