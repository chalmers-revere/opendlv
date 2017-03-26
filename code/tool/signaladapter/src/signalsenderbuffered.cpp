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
#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/io/udp/UDPFactory.h>
#include "opendavinci/odcore/reflection/Message.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData_Helper.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI_Helper.h"

#include "samplebuffer.hpp"
#include "samplevisitor.hpp"
#include "signalsenderbuffered.hpp"

namespace opendlv {
namespace tool {

SignalSenderBuffered::SignalSenderBuffered(std::string const &a_messageIds,
    std::string const &a_senderStamps, std::string const &a_address, 
    std::string const &a_ports, std::string const &a_libraryPath, bool a_debug)
    : SignalSender(a_messageIds, a_senderStamps, a_libraryPath, a_debug),
      m_buffers(),
      m_isFresh(),
      m_isSeen(),
      m_udpSender(), 
      m_mutex()

{
  std::vector<std::string> portStrings = 
    odcore::strings::StringToolbox::split(a_ports, ',');
  
  if (portStrings.size() < 1) {
    std::cerr << "No port specified." << std::endl; 
    return;
  }

  int32_t port = std::stoi(portStrings[0]);

  std::cout << "Will send buffered messages ";
  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    uint32_t messageId = m_messageIds[i];
    uint32_t senderStamp = m_senderStamps[i];
    std::cout << messageId << " from " << senderStamp << ", ";

    std::string data;
    m_buffers[i] = data;
    m_isFresh[i] = false;
    m_isSeen[i] = false;
  }
  std::cout << " to " << a_address << ":" << port << std::endl;

  m_udpSender = odcore::io::udp::UDPFactory::createUDPSender(a_address, port);
}

SignalSenderBuffered::~SignalSenderBuffered()
{
}

void SignalSenderBuffered::AddMappedMessage(odcore::reflection::Message &a_message,
    uint32_t a_senderStamp)
{
  odcore::base::Lock l(m_mutex);
  
  uint32_t messageId = a_message.getID();

  int16_t index = -1;
  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    if (m_messageIds[i] == messageId && m_senderStamps[i] == a_senderStamp) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    std::shared_ptr<SampleBuffer> sampleBuffer(new SampleBuffer);
    
    SampleVisitor sampleVisitor(sampleBuffer);
    a_message.accept(sampleVisitor);

    std::string data = sampleBuffer->GetDataString();

    m_buffers[index] = data;
    m_isFresh[index] = true;
    m_isSeen[index] = true;
  }
}

bool SignalSenderBuffered::IsAllMessagesSeen()
{
  bool allSeen = true;
  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    bool isSeen = m_isSeen[i];
    if (!isSeen) {
      if (m_debug) {
        std::cout << "Waiting for message " << m_messageIds[i] << " from sender " 
          << m_senderStamps[i] << std::endl;
      }
      allSeen = false;
    }
  } 
  return allSeen;
}

void SignalSenderBuffered::Update()
{
  odcore::base::Lock l(m_mutex);

  if (m_debug) {
    std::cout << "Try to send buffered message..." << std::endl;
  }

  bool allSeen = IsAllMessagesSeen();
  if (!allSeen) {
    return;
  }

  SampleBuffer buffer;
  buffer.AppendByte(static_cast<uint8_t>(m_messageIds.size()));

  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    uint32_t messageId = m_messageIds[i];
    uint32_t senderStamp = m_senderStamps[i];
    bool status = m_isFresh[i];     

    buffer.AppendInteger32(messageId);
    buffer.AppendInteger32(senderStamp);
    buffer.AppendBoolean(status);

    std::string msg = m_buffers[i];
    buffer.AppendByte(static_cast<uint8_t>(msg.length()));
    buffer.AppendStringRaw(msg);

    m_isFresh[i] = false;
  }

  std::string data = buffer.GetDataString();

  if (m_debug) {
    std::cout << "Sending buffered message: " << std::endl;
    for(std::size_t i = 0; i < data.size(); i++) {
      std::cout << std::bitset<CHAR_BIT>(data[i]) << " ";
    }
    std::cout << std::endl;
  }

  m_udpSender->send(data);
}

}
}
