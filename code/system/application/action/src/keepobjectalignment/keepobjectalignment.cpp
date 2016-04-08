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

#include "keepobjectalignment/keepobjectalignment.hpp"

namespace opendlv {
namespace action {
namespace keepobjectalignment {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
KeepObjectAlignment::KeepObjectAlignment(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "action-keepobjectalignment")
{
}

KeepObjectAlignment::~KeepObjectAlignment()
{
}

/**
 * Receives object to keep stationary, including id, current angle,
 * current size, and desired angle.
 * Sends speed correction commands (throttle) to Act.
 */
void KeepObjectAlignment::nextContainer(odcore::data::Container &)
{
}

void KeepObjectAlignment::setUp()
{
}

void KeepObjectAlignment::tearDown()
{
}

} // keepobjectalignment
} // action
} // opendlv
