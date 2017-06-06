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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdv2v/GeneratedHeaders_ODVDV2V.h"

#include "audition.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Audition::Audition(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-audition")
{
}

Audition::~Audition()
{
}

void Audition::nextContainer(odcore::data::Container &a_c)
{
  // cout << "Received container of type " << c.getDataType() 
  //     << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() 
  //     <<" received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms()
  //     << endl;
  if(a_c.getDataType() != opendlv::proxy::V2vReading::ID()){
    return;
  }

  // std::cout << "Received a message of type ."
  opendlv::proxy::V2vReading v2vReading = 
      a_c.getData<opendlv::proxy::V2vReading>();
  std::string dataString = v2vReading.getData();
  std::vector<unsigned char> data(dataString.begin(), dataString.end());
  
  // string value = message.getValue();
  // vector<string> tokens = odcore::strings::StringToolbox::split(value, '|');


  // std::cout << std::to_string(static_cast<unsigned char>(*data.begin())) 
  //     << std::endl;
  // std::vector<unsigned char> const bytes = data;
  // std::stringstream ss;
  // for (uint i = 0; i < bytes.size(); i++) {
  //     ss << std::to_string(bytes.at(i));
  //     ss << "|";
  // }
  // std::cout<<ss.str()<<std::endl;
  // std::cout<<value<<std::endl;

  unsigned char v2vMsgId = data.at(0);
  // std::cout <<  std::to_string(v2vMsgId)<<std::endl;
  std::string v2vMsgType;
  switch(v2vMsgId)
  {
    case 1:
      v2vMsgType = "denm";
      // std::cout << "Sent denm." << std::endl;
    break;
    case 2:
      v2vMsgType = "cam";
      // std::cout << "Sent cam." << std::endl;
    break;
    case 10:
      v2vMsgType = "iclcm";
      // std::cout << "Sent iclcm." << std::endl;
    break;
    default:
      std::cout << "Received invalid message ID.";
  }
  if(!v2vMsgType.empty())
  {
    // std::cout<<"Sorted and sending to next layer.\n";
    opendlv::sensation::Voice nextMessage(v2vMsgType, v2vReading.getSize(), 
        dataString);
    odcore::data::Container container(nextMessage);
    getConference().send(container);
  }
}

void Audition::setUp()
{
}

void Audition::tearDown()
{
}

}
}
}
