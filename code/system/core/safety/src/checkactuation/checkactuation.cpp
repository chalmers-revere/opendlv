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
  if (a_container.getDataType() == opendlv::proxy::ActuationRequest::ID()+300){
    opendlv::proxy::ActuationRequest actuationRequest 
        = a_container.getData<opendlv::proxy::ActuationRequest>();
    float acceleration = actuationRequest.getAcceleration();
    float steering = actuationRequest.getSteering();


    float steeringLimit = 0.2;
    float accMaxLimit = 80;


    // TODO if acceleration is negative it is m/s^2, if positive percent of acceleration pedal

    // clamp steering
    if (steering < -steeringLimit) {
      steering = -steeringLimit;
      std::cout << "steering request was capped to " << steering << std::endl;
    }
    else if (steering > steeringLimit) {
      steering = steeringLimit;
      std::cout << "steering request was capped to " << steering << std::endl;
    }

    // clamp acceleration
    if (acceleration < -m_maxAllowedDeceleration) {
      acceleration = -m_maxAllowedDeceleration;
      std::cout << "acceleration request was capped to " << acceleration << std::endl;
    }
    else if (acceleration > accMaxLimit) {
      acceleration = accMaxLimit;
      std::cout << "acceleration request was capped to " << acceleration << std::endl;
    }


    actuationRequest.setAcceleration(acceleration);
    actuationRequest.setSteering(steering);
    // std::cout << "Actuation granted." << std::endl;

    actuationRequest.setIsValid(true);
    
    odcore::data::Container c(actuationRequest);
    getConference().send(c);
  }
}

} // checkactuation
} // safety
} // opendlv
