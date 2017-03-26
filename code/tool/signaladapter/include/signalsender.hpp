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

#ifndef TOOL_SIGNALADAPTER_SIGNALSENDER_HPP_
#define TOOL_SIGNALADAPTER_SIGNALSENDER_HPP_

#include <map>
#include <memory>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/udp/UDPSender.h>
#include <opendavinci/odcore/reflection/MessageResolver.h>

namespace opendlv {
namespace tool {

class SignalSender {
 public:
  SignalSender(std::string const &, std::string const &, std::string const &,
      bool);
  SignalSender(SignalSender const &) = delete;
  SignalSender &operator=(SignalSender const &) = delete;
  virtual ~SignalSender();

  void AddContainer(odcore::data::Container &);
  virtual void AddMappedMessage(odcore::reflection::Message &, uint32_t) = 0;
  virtual void Update() = 0;

 protected:
  std::unique_ptr<odcore::reflection::MessageResolver> m_messageResolver;
  std::vector<uint32_t> m_messageIds;
  std::vector<uint32_t> m_senderStamps;
  bool m_debug;
};

}
}

#endif
