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
#include <fstream>
#include <thread>
#include <chrono>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/wrapper/SerialPort.h"
#include "opendavinci/odcore/wrapper/SerialPortFactory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lidar/lidar.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Lidar::Lidar(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-lidar")
    , m_sick()
    , m_lidarStringDecoder()
{
}

Lidar::~Lidar()
{
}

void Lidar::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  //  std::string const type = kv.getValue<std::string>("proxy-lidar.type");

  const double x = kv.getValue<float>("proxy-lidar.mount.x");
  const double y = kv.getValue<float>("proxy-lidar.mount.y");
  const double z = kv.getValue<float>("proxy-lidar.mount.z");
  m_lidarStringDecoder = std::unique_ptr<LidarStringDecoder>(new LidarStringDecoder(getConference(), x, y, z));


  const string SERIAL_PORT = kv.getValue<std::string>("proxy-lidar.port");
  const uint32_t BAUD_RATE = 9600; // Fixed baud rate.
  try {
    m_sick = shared_ptr<odcore::wrapper::SerialPort>(odcore::wrapper::SerialPortFactory::createSerialPort(SERIAL_PORT, BAUD_RATE));
    m_sick->setStringListener(m_lidarStringDecoder.get());
    m_sick->start();
    cout << "Connected to SICK, waiting for configuration..." << std::endl;
  }
  catch (string &exception) {
    cerr << "[" << getName() << "] Could not connect to SICK: " << exception << endl;
  }
}

void Lidar::tearDown()
{
  StopScan();
  m_sick->stop();
  m_sick->setStringListener(NULL);
}

// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Lidar::body()
{
  // Initialization sequence.
  uint32_t counter = 0;
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    counter++;
    if (counter == 30) {
      cout << "Sending stop scan" << endl;
      StopScan();
    }
    if (counter == 32) {
      cout << "Sending status request" << endl;
      Status();
    }
    if (counter == 34) {
      cout << "Sending settings mode" << endl;
      SettingsMode();
    }
    if (counter == 38) {
      cout << "Sending centimeter mode" << endl;
      SetCentimeterMode();
    }
    if (counter == 40) {
      cout << "Start scanning" << endl;
      StartScan();
      break;
    }
  }

  // "Do nothing" sequence.
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    // Do nothing.
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Lidar::Status()
{
  const unsigned char statusCall[] = {0x02, 0x00, 0x01, 0x00, 0x31, 0x15, 0x12};
  const std::string statusString(reinterpret_cast<char const *>(statusCall), 7);
  m_sick->send(statusString);
}

void Lidar::StartScan()
{
  const unsigned char streamStart[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x24, 0x34, 0x08};
  const std::string startString(reinterpret_cast<char const *>(streamStart), 8);
  m_sick->send(startString);
}

void Lidar::StopScan()
{
  const unsigned char streamStop[] = {0x02, 0x00, 0x02, 0x00, 0x20, 0x25, 0x35, 0x08};
  const std::string stopString(reinterpret_cast<char const *>(streamStop), 8);
  m_sick->send(stopString);
}

void Lidar::SettingsMode()
{
  const unsigned char settingsMode[] = {0x02, 0x00, 0x0A, 0x00, 0x20, 0x00, 0x53, 0x49, 0x43, 0x4B, 0x5F, 0x4C, 0x4D, 0x53, 0xBE, 0xC5};
  const std::string settingString(reinterpret_cast<char const *>(settingsMode), 16);
  m_sick->send(settingString);
}

void Lidar::SetCentimeterMode()
{
  const unsigned char centimeterMode[] = {0x02, 0x00, 0x21, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xCB};
  const std::string centimeterString(reinterpret_cast<char const *>(centimeterMode), 39);
  m_sick->send(centimeterString);
}

} // lidar
} // proxy
} // opendlv
