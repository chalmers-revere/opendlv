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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/io/Packet.h"
#include "opendavinci/odcore/io/udp/UDPReceiver.h"
#include "opendavinci/odcore/io/udp/UDPFactory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"


#include "v2v/v2v.hpp"
#include "v2v/device.hpp"

namespace opendlv {
namespace proxy {
namespace v2v {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2v::V2v(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-v2v")
    , m_device()
{
}
// V2v::V2v()
// {
// }

V2v::~V2v()
{
}

void V2v::nextPacket(const odcore::io::Packet &p) {
    std::cout << "Received a packet from " << p.getSender() << ", "
         << "with " << p.getData().length() 
         // << " bytes containing '"
         // << p.getData() << "'" 
         << std::endl;
    // TODO: Forward the packet to other layer alternatively packing up data.
    opendlv::proxy::V2vInbound nextMessage;
    nextMessage.setSize(p.getData().length());
    const std::string packetString = p.getData();
    // std::vector<unsigned char> data(packetString.begin(), packetString.end());
    
    // std::vector<unsigned char> const bytes = data;
    // std::stringstream ss;
    // for (uint i = 0; i < bytes.size(); i++) {
    //     ss << std::to_string(bytes.at(i));
    //     ss << "|";
    // }
    // std::cout<<ss.str()<<std::endl;

    nextMessage.setData(packetString);
    odcore::data::Container c(nextMessage);
    getConference().send(c);

}


// This method will do the main data processing job.
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode V2v::body()
{
  const std::string RECEIVER = "0.0.0.0";
  const uint32_t PORT = 5000;
  std::cout << "Trying to receive UDP on port " << PORT << "." << std::endl;
  try
  {
    std::shared_ptr<odcore::io::udp::UDPReceiver>
            udpreceiver(odcore::io::udp::UDPFactory::createUDPReceiver(RECEIVER, PORT));

    udpreceiver->setPacketListener(this);
    udpreceiver->start();
    //For continuous listening
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
      // Do nothing
    }
    udpreceiver->stop();
    udpreceiver->setPacketListener(NULL);
  }
  catch(std::string &exception)
  {
    std::cerr << "Error while creating UDP receiver:  " << exception 
        << std::endl;
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void V2v::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type = kv.getValue<std::string>("proxy-v2v.type");
  /*  std::string const port =
    kv.getValue<std::string>("proxy-v2v.port");
    float const mountX = kv.getValue<float>("proxy-v2v.mount.x");
    float const mountY = kv.getValue<float>("proxy-v2v.mount.y");
    float const mountZ = kv.getValue<float>("proxy-v2v.mount.z");
  */
  if (type.compare("geonetworking-dual") == 0) {
    //      m_device = std::unique_ptr<Device>(new GeonetworkingDualDevice());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-v2v] No valid device driver defined."
              << std::endl;
  }
}

void V2v::tearDown()
{
}

} // v2v
} // proxy
} // opendlv
