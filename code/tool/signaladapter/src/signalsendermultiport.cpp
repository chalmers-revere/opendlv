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

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData_Helper.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI_Helper.h"

#include "samplebuffer.hpp"
#include "samplevisitor.hpp"
#include "signalsendermultiport.hpp"

namespace opendlv {
namespace tool {

SignalSenderMultiPort::SignalSenderMultiPort(std::string const &a_messageIds,
    std::string const &a_senderStamps, std::string const &a_address, 
    std::string const &a_ports,
    std::string const &a_libraryPath, bool a_debug)
    : SignalSender(a_messageIds, a_senderStamps, a_libraryPath, a_debug),
      m_udpSenders()
{
  std::vector<std::string> portStrings = 
    odcore::strings::StringToolbox::split(a_ports, ',');
  
  if (m_messageIds.size() != portStrings.size()) {
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  
  for (uint16_t i = 0; i < portStrings.size(); i++) {
    int32_t messageId = m_messageIds[i];
    int32_t senderStamp = m_senderStamps[i];
    int32_t port = std::stoi(portStrings[i]);
    auto udpSender = odcore::io::udp::UDPFactory::createUDPSender(a_address, port);
    std::cout << "Will send message '" << messageId << "' from sender '" 
      << senderStamp << "' to " << a_address << ":" << port << std::endl;
    m_udpSenders[messageId] = udpSender;
  }
}

SignalSenderMultiPort::~SignalSenderMultiPort()
{
}

void SignalSenderMultiPort::AddMappedMessage(odcore::reflection::Message &a_message, uint32_t a_senderStamp)
{
  int32_t messageId = a_message.getID();

  std::shared_ptr<SampleBuffer> sampleBuffer(new SampleBuffer);

  sampleBuffer->AppendInteger32(a_senderStamp);
  sampleBuffer->AppendInteger32(messageId);

  SampleVisitor sampleVisitor(sampleBuffer);
  a_message.accept(sampleVisitor);

  std::string data = sampleBuffer->GetDataString();
  if(m_debug) {
    std::cout << "Sending message '" << a_message.toString() 
      << "' (" << messageId << "): " << std::endl;
    for(std::size_t i = 0; i < data.size(); i++) {
      std::cout << std::bitset<CHAR_BIT>(data[i]) << " ";
    }
    std::cout << std::endl;
  }
  m_udpSenders[messageId]->send(data);
}

void SignalSenderMultiPort::Update()
{
}

}
}
