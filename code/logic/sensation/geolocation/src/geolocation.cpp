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

#include <cmath>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>


//#include <odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h>

#include "geolocation.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

Geolocation::Geolocation(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocation")

{
}

Geolocation::~Geolocation()
{
}

void Geolocation::nextContainer(odcore::data::Container &/*a_container*/) //DONT FORGET TO REMOVE COMMENT
{

 
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
   

  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocation::setUp()
{
  std::cout << "Welcome" << std::endl;
}

void Geolocation::tearDown()
{
}

}
}
}
