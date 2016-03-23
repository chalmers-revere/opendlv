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
#include "opendavinci/odcore/io/udp/UDPSender.h"
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
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "proxy-v2v")
    , m_device()
    , m_udpsender()
    , m_udpreceiver()
{
}
// V2v::V2v()
// {
// }

V2v::~V2v()
{
}

void V2v::nextPacket(const odcore::io::Packet &p)
{
    std::cout << "Received a packet from " << p.getSender() << ", "
         << "with " << p.getData().length() 
         // << " bytes containing '"
         // << p.getData() << "'" 
         << std::endl;
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

  void V2v::nextContainer(odcore::data::Container &c)
  {
    if(c.getDataType() == opendlv::proxy::V2vOutbound::ID()){
      // std::cout << "Got an outbound message" << std::endl;
      opendlv::proxy::V2vOutbound message = 
      c.getData<opendlv::proxy::V2vOutbound>();
      std::string data = message.getData();

      try {

        m_udpsender->send(data);
      }
      catch(string &exception) {
          cerr << "Data could not be sent: " << exception << endl;
      }
    }
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


  const std::string RECEIVER = "0.0.0.0";
  const uint32_t RECEIVERPORT = kv.getValue<uint32_t>("proxy-v2v.listenPort");
  std::cout<< RECEIVERPORT << std::endl;
  std::cout << "Trying to receive UDP on port " << RECEIVERPORT << "." << std::endl;
  try
  {
    m_udpreceiver = std::shared_ptr<odcore::io::udp::UDPReceiver>(odcore::io::udp::UDPFactory::createUDPReceiver(RECEIVER, RECEIVERPORT));
    m_udpreceiver->setPacketListener(this);
    m_udpreceiver->start();
  }
  catch(std::string &exception)
  {
    std::cerr << "Error while creating UDP receiver:  " << exception 
        << std::endl;
  }



  const string TARGET = kv.getValue<std::string>("proxy-v2v.comboxIp");
  const uint32_t TARGETPORT =  kv.getValue<uint32_t>("proxy-v2v.comboxPort");;
  std::cout << "Trying to send UDP on " << TARGET << ":" << TARGETPORT << "." << std::endl;
  
  try {
    m_udpsender = std::shared_ptr<odcore::io::udp::UDPSender>(odcore::io::udp::UDPFactory::createUDPSender(TARGET, TARGETPORT));
  }
  catch(string &exception) {
    cerr << "Error while creating UDP sender: " << exception << endl;
  }
}

void V2v::tearDown()
{

    m_udpreceiver->stop();
    m_udpreceiver->setPacketListener(NULL);
}

} // v2v
} // proxy
} // opendlv
