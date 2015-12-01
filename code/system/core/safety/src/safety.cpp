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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "GeneratedHeaders_OpenDLVData.h"

#include "safety.hpp"

namespace opendlv {
namespace system {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Safety::Safety(int32_t const &a_argc, char **a_argv) :
    DataTriggeredConferenceClientModule(a_argc, a_argv, "safety")
{
}

Safety::~Safety() 
{
}

void Safety::setUp() 
{
  // This method will be call automatically _before_ running body().
}

void Safety::tearDown() 
{
  // This method will be call automatically _after_ return from body().
}

void Safety::nextContainer(core::data::Container &) 
{
}

} // system
} // opendlv

/*
        cout << "[safety] received container of type " << c.getDataType() <<
                                           " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() <<
                                       " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << endl;

        if (c.getDataType() == Container::USER_DATA_1) {
            revere::generic::ExampleMessageForSafety payload = c.getData<revere::generic::ExampleMessageForSafety>();
            cout << "[safety]: " << payload.toString() << endl;

            // Example: Create message revere.generic.ExampleMessageForPerception.
            revere::generic::ExampleMessageForPerception nextMessage;
            nextMessage.setData1(-42);

            Container c2(Container::USER_DATA_2, nextMessage);
            getConference().send(c2);
        }
    }
*/
