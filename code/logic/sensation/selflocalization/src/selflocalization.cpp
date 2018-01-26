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

#include "selflocalization.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Selflocalization::Selflocalization(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-selflocalization")
{
}

Selflocalization::~Selflocalization()
{
}
/*
*Takes data from conference, in our case image?
*/
void Selflocalization::nextContainer(odcore::data::Container &a_c) 
{


    odcore::data::Container container(nextMessage);
    getConference().send(container);
  }
}

void Selflocalization::setUp()
{
}

void Selflocalization::tearDown()
{
}

}
}
}
