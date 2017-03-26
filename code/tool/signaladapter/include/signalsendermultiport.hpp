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

#ifndef TOOL_SIGNALADAPTER_SIGNALSENDERMULTIPORT_HPP_
#define TOOL_SIGNALADAPTER_SIGNALSENDERMULTIPORT_HPP_

#include "signalsender.hpp"

namespace opendlv {
namespace tool {

class SignalSenderMultiPort : public SignalSender {
 public:
  SignalSenderMultiPort(std::string const &, std::string const &, 
      std::string const &, std::string const &, std::string const &, bool);
  SignalSenderMultiPort(SignalSenderMultiPort const &) = delete;
  SignalSenderMultiPort &operator=(SignalSenderMultiPort const &) = delete;
  virtual ~SignalSenderMultiPort();

  void AddMappedMessage(odcore::reflection::Message &, uint32_t);
  void Update();

 private:
  std::map<uint32_t, std::shared_ptr<odcore::io::udp::UDPSender>> m_udpSenders;
};

}
}

#endif
