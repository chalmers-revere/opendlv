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
#include <vector>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/base/KeyValueConfiguration.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/io/udp/UDPFactory.h>
#include "opendavinci/odcore/reflection/Message.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData_Helper.h"

#include "samplebuffer.hpp"
#include "samplevisitor.hpp"
#include "signaladapter.hpp"
#include "signalstringlistener.hpp"

namespace opendlv {
namespace tools {
namespace signaladapter {


SignalAdapter::SignalAdapter(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::DataTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-signaladapter"),
      m_udpReceivers(),
      m_udpSenders(),
      m_signalStringListener(new SignalStringListener(getConference()))
{
}

SignalAdapter::~SignalAdapter()
{
}

void SignalAdapter::setUp()
{
  SetUpReceivers();
  SetUpSenders();
}

void SignalAdapter::SetUpReceivers()
{
  std::string address = "0.0.0.0";

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const messageIds = 
      kv.getValue<std::string>("tools-signaladapter.receiver.messages");
  std::string const ports =
      kv.getValue<std::string>("tools-signaladapter.receiver.ports");
  std::vector<std::string> messageIdStrings = 
      odcore::strings::StringToolbox::split(messageIds, ',');
  std::vector<std::string> portStrings = 
      odcore::strings::StringToolbox::split(ports, ',');
  if (messageIdStrings.size() != portStrings.size()) {
    // TODO: How to properly handle errors in OpenDLV?
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    uint16_t port = std::stoi(portStrings[i]);
    auto udpReceiver = odcore::io::udp::UDPFactory::createUDPReceiver(address, port);
    std::cout << "Receiving message '" << messageId << "' on port " << port << std::endl;

    udpReceiver->setStringListener(m_signalStringListener.get());
    udpReceiver->start();

    m_udpReceivers[messageId] = udpReceiver;
  }
}

void SignalAdapter::SetUpSenders()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const address =
      kv.getValue<std::string>("tools-signaladapter.sender.address");

  std::string const messageIds = 
      kv.getValue<std::string>("tools-signaladapter.sender.messages");
  std::string const ports =
      kv.getValue<std::string>("tools-signaladapter.sender.ports");
  std::vector<std::string> messageIdStrings = 
      odcore::strings::StringToolbox::split(messageIds, ',');
  std::vector<std::string> portStrings = 
      odcore::strings::StringToolbox::split(ports, ',');
  if (messageIdStrings.size() != portStrings.size()) {
    // TODO: How to properly handle errors in OpenDLV?
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    uint16_t port = std::stoi(portStrings[i]);
    auto udpSender = odcore::io::udp::UDPFactory::createUDPSender(address, port);
    std::cout << "Sending message '" << messageId << "' to " << address << ":" << port << std::endl;

    m_udpSenders[messageId] = udpSender;
  }
}

void SignalAdapter::tearDown()
{
  for (auto receiver : m_udpReceivers) {
    receiver.second->stop();
  }
}


void SignalAdapter::nextContainer(odcore::data::Container &a_container)
{
  int32_t messageId = a_container.getDataType();
  bool is_served = m_udpSenders.count(messageId);

  if (is_served) {
    bool successfullyMapped = false;
    odcore::reflection::Message msg;
    if (!successfullyMapped) {
      msg = GeneratedHeaders_ODVDOpenDLVData_Helper::__map(a_container,
          successfullyMapped);
    }

    std::cout << "Success: " << successfullyMapped << std::endl;
    if (successfullyMapped) {
      std::cout << msg.getLongName() << std::endl;
      
      std::shared_ptr<SampleBuffer> sampleBuffer(new SampleBuffer);
      SampleVisitor sampleVisitor(sampleBuffer);
      msg.accept(sampleVisitor);

      std::string data = sampleBuffer->GetDataString();

      std::cout << "Sending data: " << data << " (len: " << data.length() 
        << ")" << std::endl;

      m_udpSenders[messageId]->send(data);
    }
  }
}

} // signaladapter
} // tools
} // opendlv
