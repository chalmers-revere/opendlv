/**
 * Copyright (C) 2015 Chalmers REVERE
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

#ifndef V2V_V2V_HPP_
#define V2V_V2V_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/io/PacketListener.h"
#include "opendavinci/odcore/io/udp/UDPReceiver.h"
#include "opendavinci/odcore/io/udp/UDPSender.h"

namespace opendlv {
namespace proxy {
namespace v2v {

class Device;

/**
 * This class provides...
 */
class V2v : public odcore::base::module::DataTriggeredConferenceClientModule,
    public odcore::io::PacketListener {
 public:
  V2v(int32_t const &, char **);
  V2v(V2v const &) = delete;
  V2v &operator=(V2v const &) = delete;
  virtual ~V2v();
  virtual void nextPacket(const odcore::io::Packet &p);
  virtual void nextContainer(odcore::data::Container &c);

 private:
  void setUp();
  void tearDown();
  // odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  std::unique_ptr<Device> m_device;
  std::shared_ptr<odcore::io::udp::UDPSender> m_udpsender;
  std::shared_ptr<odcore::io::udp::UDPReceiver> m_udpreceiver;
};

} // v2v
} // proxy
} // opendlv

#endif
