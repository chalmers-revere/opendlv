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

#ifndef TOOL_SIGNALADAPTER_SIGNALSENDERBUFFERED_HPP_
#define TOOL_SIGNALADAPTER_SIGNALSENDERBUFFERED_HPP_

#include <opendavinci/odcore/base/Mutex.h>

#include "signalsender.hpp"

namespace opendlv {
namespace tool {

class SignalSenderBuffered : public SignalSender {
 public:
  SignalSenderBuffered(std::string const &, std::string const &, 
      std::string const &, std::string const &, std::string const &, bool);
  SignalSenderBuffered(SignalSenderBuffered const &) = delete;
  SignalSenderBuffered &operator=(SignalSenderBuffered const &) = delete;
  virtual ~SignalSenderBuffered();

  void AddMappedMessage(odcore::reflection::Message &, uint32_t);
  void Update();

 private:
  bool IsAllMessagesSeen();

  std::map<uint16_t, std::string> m_buffers;
  std::map<uint16_t, bool> m_isFresh;
  std::map<uint16_t, bool> m_isSeen;
  std::shared_ptr<odcore::io::udp::UDPSender> m_udpSender;
  odcore::base::Mutex m_mutex;
};

}
}

#endif
