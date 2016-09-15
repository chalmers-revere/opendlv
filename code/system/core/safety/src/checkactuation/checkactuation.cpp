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
    m_initialised(false),
    m_steeringLimit(),
    m_accMaxLimit(),
    m_maxAllowedDeceleration()
{
}

CheckActuation::~CheckActuation()
{
}

void CheckActuation::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_steeringLimit = kv.getValue<float>(
      "safety-checkactuation.steeringLimit");
  m_accMaxLimit = kv.getValue<float>(
      "safety-checkactuation.accMaxLimit");
  m_maxAllowedDeceleration = kv.getValue<float>(
      "safety-checkactuation.maxAllowedDeceleration");
  m_initialised = true;
}

void CheckActuation::tearDown()
{
}

void CheckActuation::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::proxy::ActuationRequest::ID()+300){
    opendlv::proxy::ActuationRequest actuationRequest 
        = a_container.getData<opendlv::proxy::ActuationRequest>();
    float acceleration = actuationRequest.getAcceleration();
    float steering = actuationRequest.getSteering();




    // clamp steering
    if (steering < -m_steeringLimit) {
      steering = -m_steeringLimit;
      std::cout << "steering request was capped to " << steering << std::endl;
    }
    else if (steering > m_steeringLimit) {
      steering = m_steeringLimit;
      std::cout << "steering request was capped to " << steering << std::endl;
    }

    // clamp acceleration
    if (acceleration < -m_maxAllowedDeceleration) {
      acceleration = -m_maxAllowedDeceleration;
      std::cout << "acceleration request was capped to " << acceleration << std::endl;
    }
    else if (acceleration > m_accMaxLimit) {
      acceleration = m_accMaxLimit;
      std::cout << "acceleration request was capped to " << acceleration << std::endl;
    }


    actuationRequest.setAcceleration(acceleration);
    actuationRequest.setSteering(steering);

    actuationRequest.setIsValid(true);
    
    odcore::data::Container c(actuationRequest);
    getConference().send(c);
  }
}

} // checkactuation
} // safety
} // opendlv
