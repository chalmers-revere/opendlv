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

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "can/gw/canmessagedatastore.hpp"
#include "can/keyboard/cankeyboard.hpp"

namespace opendlv {
namespace proxy {
namespace can {
namespace keyboard {

CANKeyboard::CANKeyboard(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-can-keyboard")
    , automotive::odcantools::GenericCANMessageListener()
    , m_fifo()
    , m_device()
    , m_canMessageDataStore()
    , m_fh16CANMessageMapping()
, m_keyAcc('w')
, m_keyBrake('s')
, m_keyLeft('a')
, m_keyRight('d')
{
}

CANKeyboard::~CANKeyboard()
{
}

void CANKeyboard::setUp()
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odtools::recorder;
  using namespace automotive::odcantools;

  string const DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("proxy-can-keyboard.devicenode");

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

    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    //turn off canonical mode
    ttystate.c_lflag &= ~ICANON;
    //minimum of number input read.
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    cout << "KeyboardController: " << endl;
    cout << "'" << m_keyAcc <<  "': Accelerate" << endl;
    cout << "'" << m_keyBrake <<  "': Brake" << endl;
    cout << "'" << m_keyLeft <<  "': Turn left" << endl;
    cout << "'" << m_keyRight <<  "': Turn right" << endl;
  }
}

void CANKeyboard::tearDown()
{
  if (m_device.get()) {
    // Stop the wrapper CAN device.
    m_device->stop();
  }

  // Deactivate keyboard control.
  {
    struct termios ttystate;
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    //turn on canonical mode
    ttystate.c_lflag |= ICANON;
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }
}

void CANKeyboard::nextGenericCANMessage(const automotive::GenericCANMessage &gcm)
{
  std::cout << "Received CAN message " << gcm.toString() << std::endl;

//  // Map CAN message to high-level data structure.
//  vector<odcore::data::Container> result = m_fh16CANMessageMapping.mapNext(gcm);

//  std::cout << gcm.toString() << ", decoded: " << result.size() << std::endl;
//  if (result.size() > 0) {
//    auto it = result.begin();
//    while (it != result.end()) {
//      odcore::data::Container c = (*it);
//      if (c.getDataType() == opendlv::gcdc::fh16::Steering::ID()) {
//        opendlv::gcdc::fh16::Steering s =
//        c.getData<opendlv::gcdc::fh16::Steering>();
//        std::cout << s.toString() << std::endl;
//      }
//      if (c.getDataType() == opendlv::gcdc::fh16::VehicleDynamics::ID()) {
//        opendlv::gcdc::fh16::VehicleDynamics v =
//        c.getData<opendlv::gcdc::fh16::VehicleDynamics>();
//        std::cout << v.toString() << std::endl;
//      }
//      it++;
//    }
//  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode CANKeyboard::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING) {
  {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDOUT_FILENO is 0
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    if (FD_ISSET(STDIN_FILENO, &fds) != 0) {
        char c = fgetc(stdin);

        // Check pressed key.
        if (c == m_keyAcc) {
            cout << "Accelerate" << endl;
        }
        else if (c == m_keyBrake) {
            cout << "Decelerate" << endl;
        }
        else if (c == m_keyLeft) {
            cout << "Left" << endl;
        }
        else if (c == m_keyRight) {
            cout << "Right" << endl;
        }
    }
  }


    opendlv::gcdc::fh16::BrakeRequest brakeRequest;
    brakeRequest.setEnable_brakerequest(false);
    brakeRequest.setBrakerequest(0);

    // Create the message mapping.
    canmapping::opendlv::gcdc::fh16::BrakeRequest brakeRequestMapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container c(brakeRequest);
    automotive::GenericCANMessage gcm = brakeRequestMapping.encode(c);
//    m_device->write(gcm);

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // keyboard
} // can
} // proxy
} // opendlv
