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

#include <cmath>
#include <iostream>

#include <opendavinci/odcore/data/Container.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "checkactuation/checkactuation.hpp"

namespace opendlv {
namespace safety {
namespace checkactuation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
CheckActuation::CheckActuation(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, 
          "safety-checkactuation"),
    m_maxAllowedDeceleration(0.0)
{
}

CheckActuation::~CheckActuation()
{
}

void CheckActuation::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_maxAllowedDeceleration = kv.getValue<double>(
      "safety-checkactuation.maxAllowedDeceleration");
}

void CheckActuation::tearDown()
{
}

void CheckActuation::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::proxy::Actuation::ID()){
    opendlv::proxy::Actuation actuation 
        = a_container.getData<opendlv::proxy::Actuation>();
    float acceleration = actuation.getAcceleration();

    if (acceleration < -m_maxAllowedDeceleration) {
      acceleration = -m_maxAllowedDeceleration;
      actuation.setAcceleration(acceleration);
    }

    actuation.setIsValid(true);
    
    odcore::data::Container c(actuation);
    getConference().send(c);
  }
}

} // checkactuation
} // safety
} // opendlv
