/**
 * Copyright (C) 2016 Chalmers Revere
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

#ifndef SIGNALADAPTER_SIGNALSENDERBUFFERED_HPP_
#define SIGNALADAPTER_SIGNALSENDERBUFFERED_HPP_

#include <opendavinci/odcore/base/Mutex.h>

#include "signalsender.hpp"

namespace opendlv {
namespace tools {
namespace signaladapter {

class SignalSenderBuffered : public SignalSender {
 public:
  SignalSenderBuffered(std::string const &, std::string const &, 
      std::string const &, std::string const &, bool);
  SignalSenderBuffered(SignalSenderBuffered const &) = delete;
  SignalSenderBuffered &operator=(SignalSenderBuffered const &) = delete;
  virtual ~SignalSenderBuffered();

  void AddMappedMessage(odcore::reflection::Message &);
  void Update();

 private:
  std::map<uint32_t, std::string> m_buffers;
  std::map<uint32_t, bool> m_isFresh;
  std::shared_ptr<odcore::io::udp::UDPSender> m_udpSender;
  odcore::base::Mutex m_mutex;
};

} // signaladapter
} // tools
} // opendlv

#endif
