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

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "communicate/communicate.hpp"

namespace opendlv {
namespace action {
namespace communicate {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Communicate::Communicate(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "action-communicate")
{
}

Communicate::~Communicate()
{
}

/**
 * Receives messages to communicate.
 * Sends message via V2V or HMI.
 */
void Communicate::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::knowledge::Message::ID()){
    // std::cout << "Got message!" << std::endl;
    opendlv::knowledge::Message message = 
        c.getData<opendlv::knowledge::Message>();

    opendlv::proxy::V2vOutbound nextMessage(message.getSize(),message.getData());

    odcore::data::Container containerToSend(nextMessage);
    getConference().send(containerToSend);

  }
}

void Communicate::setUp()
{
}

void Communicate::tearDown()
{
}

} // communicate
} // action
} // opendlv
