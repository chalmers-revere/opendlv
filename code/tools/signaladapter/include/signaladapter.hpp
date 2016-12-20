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

#ifndef SIGNALADAPTER_SIGNALADAPTER_HPP_
#define SIGNALADAPTER_SIGNALADAPTER_HPP_

#include <map>
#include <memory>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/udp/UDPReceiver.h>

namespace opendlv {
namespace tools {
namespace signaladapter {

class SignalSender;
class SignalStringListener;

class SignalAdapter
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  SignalAdapter(int32_t const &, char **);
  SignalAdapter(SignalAdapter const &) = delete;
  SignalAdapter &operator=(SignalAdapter const &) = delete;
  virtual ~SignalAdapter();

  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  
  void SetUpReceivers();

  std::unique_ptr<SignalSender> m_signalSender;
  std::unique_ptr<SignalStringListener> m_signalStringListener;
  std::vector<std::shared_ptr<odcore::io::udp::UDPReceiver>> m_udpReceivers;
  bool m_debug;
};

} // signaladapter
} // tools
} // opendlv

#endif
