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

#include "keepobjectsize/keepobjectsize.hpp"

namespace opendlv {
namespace action {
namespace keepobjectsize {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
KeepObjectSize::KeepObjectSize(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "action-keepobjectsize")
{
}

KeepObjectSize::~KeepObjectSize()
{
}

/**
 * Receives object to keep stationary, including id, current angle,
 * current size, and desired size.
 * Sends speed correction commands (throttle) to Act.
 */
void KeepObjectSize::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::perception::LeadVehicleSize::ID()){
    opendlv::perception::LeadVehicleSize leadVehicleSize = 
        c.getData<opendlv::perception::LeadVehicleSize>();
    float size = leadVehicleSize.getSize();
  
    std::cout << "REC OBJECT SIZE " << size << std::endl;

    // TODO: Quick way.
    float desiredSize = 100.0f;
    float error = (desiredSize - size) / desiredSize;
 
    std::cout << "ERROR " << error << std::endl;

    if (error < -0.25f) {
      // We are to to close, try to decrease acceleration.
      float amplitude = 0.5f * error;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "accelerate", false, 
          amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
    
      std::cout << "SEND ACCELERATE " << amplitude << std::endl;
    } else if (error < -0.75f) {
      // We are way to close, start braking!
      float amplitude = 1.5f * error;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "brake", false, amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
      
      std::cout << "SEND BRAKE " << amplitude << std::endl;
    }
  }
}

void KeepObjectSize::setUp()
{
}

void KeepObjectSize::tearDown()
{
}

} // keepobjectsize
} // action
} // opendlv
