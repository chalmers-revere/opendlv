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

#include <cmath>

#include <iostream>
#include <string>
#include <vector>

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "canmessagedatastore.hpp"
#include "keyboard/keyboard.hpp"

namespace opendlv {
namespace tools {
namespace can {
namespace keyboard {

Keyboard::Keyboard(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-can-keyboard")
    , automotive::odcantools::GenericCANMessageListener()
    , m_fifo()
    , m_device()
    , m_canMessageDataStore()
    , m_revereFh16CanMessageMapping()
    , m_keyIncreaseAcceleration('w')
    , m_keyDecreaseAcceleration('s')
    , m_keyIncreaseBrake('i')
    , m_keyDecreaseBrake('k')
    , m_keyLeft('a')
    , m_keyRight('d')
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::setUp()
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odtools::recorder;
  using namespace automotive::odcantools;

  string const DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("tools-can-keyboard.devicenode");

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

  // Setup keyboard control.
  {
    struct termios ttystate;

    // get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    // turn off canonical mode
    ttystate.c_lflag &= ~ICANON;
    // minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    cout << "KeyboardController: " << endl;
    cout << "'" << m_keyIncreaseAcceleration << "': Increase acceleration" << endl;
    cout << "'" << m_keyDecreaseAcceleration << "': Decrease acceleration" << endl;
    cout << "'" << m_keyIncreaseBrake << "': Increase brake" << endl;
    cout << "'" << m_keyDecreaseBrake << "': Decrease brake" << endl;
    cout << "'" << m_keyLeft << "': Turn left" << endl;
    cout << "'" << m_keyRight << "': Turn right" << endl;
  }
}

void Keyboard::tearDown()
{
  if (m_device.get()) {
    // Stop the wrapper CAN device.
    m_device->stop();
  }

  // Deactivate keyboard control.
  {
    struct termios ttystate;
    // get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    // turn on canonical mode
    ttystate.c_lflag |= ICANON;
    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }
}

void Keyboard::nextGenericCANMessage(
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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Keyboard::body()
{
//  float braking = -4; // value for braking
//  float steering = 15.0 * M_PI/180.0; // value for steering

  const float REQUIRED_FREQ = 100.0;
//  const float REQUIRED_FREQ = 1.0;
  if (fabs(getFrequency() - REQUIRED_FREQ) > 1e-5f) {
    cerr << getName() << " was not started with --freq=" << REQUIRED_FREQ << "! Aborting..." << endl;
  }
  else {
      while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
//        {
//          struct timeval tv;
//          fd_set fds;
//          tv.tv_sec = 0;
//          tv.tv_usec = 0;
//          FD_ZERO(&fds);
//          FD_SET(STDIN_FILENO, &fds); // STDOUT_FILENO is 0
//          select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

//          if (FD_ISSET(STDIN_FILENO, &fds) != 0) {
//            char c = fgetc(stdin);

//            // Check pressed key.
//            if (c == m_keyIncreaseAcceleration) {
//              cout << "Increase accelerate" << endl;
//              v += INCREMENT;
//            }
//            else if (c == m_keyDecreaseAcceleration) {
//              cout << "Decrease accelerate" << endl;
//              v -= INCREMENT;
//            }
//            else if (c == m_keyIncreaseBrake) {
//              cout << "Increase brake" << endl;
//            }
//            else if (c == m_keyDecreaseBrake) {
//              cout << "Decrease brake" << endl;
//            }
//            else if (c == m_keyLeft) {
//              cout << "Left" << endl;
//            }
//            else if (c == m_keyRight) {
//              cout << "Right" << endl;
//            }
//          }
//        }

//        {
//            opendlv::proxy::reverefh16::SteeringRequest steerRequest;
//            steerRequest.setEnableRequest(true);
//            steerRequest.setSteeringRoadWheelAngle(steering);
//            steerRequest.setSteeringDeltaTorque(0);

//            // Create the message mapping.
//            canmapping::opendlv::proxy::reverefh16::SteeringRequest steeringRequestMapping;
//            // The high-level message needs to be put into a Container.
//            odcore::data::Container c(steerRequest);
//            automotive::GenericCANMessage gcm = steeringRequestMapping.encode(c);
////cout << "SR1 = " << steerRequest.toString() << ", GCM = " << gcm.toString() << endl;
////            gcm.setLength(6);
////            gcm.setData(0x10000003C43); // working first time 1=0x116027235 0.5=116023111 0.1=0x106585636
//            m_device->write(gcm);
//cout << "SR2 = " << steerRequest.toString() << ", GCM = " << gcm.toString() << endl;
//cout << endl;
//        }

//        {
//            opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
//            brakeRequest.setEnableRequest(true);
//            brakeRequest.setBrake(braking);

//            // Create the message mapping.
//            canmapping::opendlv::proxy::reverefh16::BrakeRequest brakeRequestMapping;
//            // The high-level message needs to be put into a Container.
//            odcore::data::Container c(brakeRequest);
//            automotive::GenericCANMessage gcm = brakeRequestMapping.encode(c);
//cout << "BR = " << brakeRequest.toString() << ", GCM = " << gcm.toString() << endl;
//            m_device->write(gcm);
//cout << endl;
//        }

//        {
//            opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
//            accelerationRequest.setEnableRequest(true);
//            accelerationRequest.setAcceleration(v);

//            // Create the message mapping.
//            canmapping::opendlv::proxy::reverefh16::AccelerationRequest accelerationRequestMapping;
//            // The high-level message needs to be put into a Container.
//            odcore::data::Container c(accelerationRequest);
//            automotive::GenericCANMessage gcm = accelerationRequestMapping.encode(c);
//            m_device->write(gcm);
//cout << "AR = " << accelerationRequest.toString() << ", GCM = " << gcm.toString() << endl;
//cout << endl;
//        }
      }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // keyboard
} // can
} // tools
} // opendlv
