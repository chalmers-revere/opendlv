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

#include <iostream>
#include <vector>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/io/udp/UDPFactory.h>
#include "opendavinci/odcore/reflection/Message.h"

#include "signalsender.hpp"

namespace opendlv {
namespace tool {

SignalSender::SignalSender(std::string const &a_messageIds,
    std::string const &a_senderStamps,
    std::string const &a_libraryPath, bool a_debug)
  : m_messageResolver()
  ,  m_messageIds()
  ,  m_senderStamps()
  ,  m_debug(a_debug)
{
  std::cout << "Trying to find libodvd*.so files in: " << a_libraryPath << std::endl;

  std::vector<std::string> const paths = odcore::strings::StringToolbox::split(a_libraryPath, ',');

  m_messageResolver.reset(new odcore::reflection::MessageResolver(paths, 
      "libodvd", ".so"));

  std::vector<std::string> messageIdStrings = 
    odcore::strings::StringToolbox::split(a_messageIds, ',');

  std::vector<std::string> senderStampStrings = 
    odcore::strings::StringToolbox::split(a_senderStamps, ',');
  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    int32_t senderStamp = std::stoi(senderStampStrings[i]);
    m_messageIds.push_back(messageId);
    m_senderStamps.push_back(senderStamp);
  }
}

SignalSender::~SignalSender()
{
}

void SignalSender::AddContainer(odcore::data::Container &a_container)
{
  uint32_t messageId = a_container.getDataType();
  uint32_t senderStamp = a_container.getSenderStamp();
 
  bool isServed = false;
  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    if (m_messageIds[i] == messageId && m_senderStamps[i] == senderStamp) {
      isServed = true;
      break;
    }
  }

  if (isServed) {
    bool successfullyMapped = false;
    odcore::reflection::Message msg = m_messageResolver->resolve(a_container, 
        successfullyMapped);
      
    if (successfullyMapped) {
      AddMappedMessage(msg, senderStamp);
    }
  }
}

}
}
