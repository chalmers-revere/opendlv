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

#include <bitset>
#include <iostream>
#include <vector>
#include <experimental/filesystem>
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
#include "signalstringlistener.hpp"

namespace opendlv {
namespace tools {
namespace signaladapter {

SignalAdapter::SignalAdapter(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::DataTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-signaladapter"),
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
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = kv.getValue<bool>("tools-signaladapter.debug");
  std::string const searchPath = 
    kv.getValue<std::string>("tools-signaladapter.directoriesForSharedLibaries");
  
  std::string const address = 
    kv.getValue<std::string>("tools-signaladapter.sender.address");

  std::string const messageIds = 
    kv.getValue<std::string>("tools-signaladapter.sender.messageIds");
  std::string const ports =
    kv.getValue<std::string>("tools-signaladapter.sender.ports");

  m_signalSender.reset(new SignalSenderMultiPort(messageIds, address, ports,
        searchPath, m_debug));

  SetUpReceivers();
}

void SignalAdapter::SetUpReceivers()
{
  std::string const address = "0.0.0.0";

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const messageIds = 
      kv.getValue<std::string>("tools-signaladapter.receiver.messageIds");
  std::string const ports =
      kv.getValue<std::string>("tools-signaladapter.receiver.ports");
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


void SignalAdapter::nextContainer(odcore::data::Container &a_container)
{
  m_signalSender->AddContainer(a_container);
}

} // signaladapter
} // tools
} // opendlv
