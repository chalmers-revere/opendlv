/**
 * Copyright (C) 2017 Chalmers Revere
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

#include <bitset>
#include <iostream>
#include <vector>
#include <limits.h>

#include <dlfcn.h>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/base/KeyValueConfiguration.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/io/udp/UDPFactory.h>
#include "opendavinci/odcore/reflection/Message.h"

#include "signaladapter.hpp"
#include "signalsendermultiport.hpp"
#include "signalsenderbuffered.hpp"
#include "signalstringlistener.hpp"

namespace opendlv {
namespace tool {

SignalAdapter::SignalAdapter(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tool-signaladapter"),
      m_signalSender(),
      m_signalStringListener(),
      m_udpReceivers(),
      m_debug()
{
}

SignalAdapter::~SignalAdapter()
{
}

void SignalAdapter::setUp()
{
  SetUpSender();
  SetUpReceivers();
}

void SignalAdapter::SetUpSender()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = kv.getValue<bool>("tool-signaladapter.debug");
  std::string const searchPath = 
    kv.getValue<std::string>("tool-signaladapter.directoriesForSharedLibaries");
  
  std::string const address = 
    kv.getValue<std::string>("tool-signaladapter.sender.address");

  std::string const messageIds = 
    kv.getValue<std::string>("tool-signaladapter.sender.messageIds");
  std::string const senderStamps = 
    kv.getValue<std::string>("tool-signaladapter.sender.senderStamps");
  std::string const ports =
    kv.getValue<std::string>("tool-signaladapter.sender.ports");

  std::string const mode =
    kv.getValue<std::string>("tool-signaladapter.sender.mode");

  if (mode == "multiport") {
    m_signalSender.reset(new SignalSenderMultiPort(messageIds, senderStamps, 
        address, ports, searchPath, m_debug));
  } else if (mode == "buffered") {
    m_signalSender.reset(new SignalSenderBuffered(messageIds, senderStamps, 
        address, ports, searchPath, m_debug));
  } else {
    std::cerr << "ERROR: Unrecognized mode '" << mode << "'." << std::endl;
  }
}

void SignalAdapter::SetUpReceivers()
{
  std::string const address = "0.0.0.0";

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const messageIds = 
      kv.getValue<std::string>("tool-signaladapter.receiver.messageIds");
  std::string const ports =
      kv.getValue<std::string>("tool-signaladapter.receiver.ports");
  std::vector<std::string> messageIdStrings = 
      odcore::strings::StringToolbox::split(messageIds, ',');
  std::vector<std::string> portStrings = 
      odcore::strings::StringToolbox::split(ports, ',');
  if (messageIdStrings.size() != portStrings.size()) {
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  m_signalStringListener = std::unique_ptr<SignalStringListener>(
      new SignalStringListener(getConference(), m_debug));

  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    uint16_t port = std::stoi(portStrings[i]);
    auto udpReceiver = odcore::io::udp::UDPFactory::createUDPReceiver(address, port);
    std::cout << "Will receive message '" << messageId << "' on port " 
      << port << std::endl;

    udpReceiver->setStringListener(m_signalStringListener.get());
    udpReceiver->start();

    m_udpReceivers.push_back(udpReceiver);
  }
}

void SignalAdapter::tearDown()
{
  for (auto receiver : m_udpReceivers) {
    receiver->stop();
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SignalAdapter::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    m_signalSender->Update();
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void SignalAdapter::nextContainer(odcore::data::Container &a_container)
{
  m_signalSender->AddContainer(a_container);
}

}
}
