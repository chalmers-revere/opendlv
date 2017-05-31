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

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdv2v/GeneratedHeaders_ODVDV2V.h"

#include "communicate.hpp"

namespace opendlv {
namespace logic {
namespace action {

Communicate::Communicate(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "logic-action-communicate")
{
}

Communicate::~Communicate()
{
}

void Communicate::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() != opendlv::knowledge::Message::ID()){
    return;
  }
  // std::cout << "Got message!" << std::endl;
  opendlv::knowledge::Message message = 
      a_c.getData<opendlv::knowledge::Message>();

  opendlv::proxy::V2vRequest nextMessage(message.getSize(),message.getData());

  odcore::data::Container containerToSend(nextMessage);
  getConference().send(containerToSend);
}

void Communicate::setUp()
{
}

void Communicate::tearDown()
{
}

}
}
}
