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

#include "diagnostics.hpp"

namespace opendlv {
namespace system {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Diagnostics::Diagnostics(int32_t const &a_argc, char **a_argv) :
    DataTriggeredConferenceClientModule(a_argc, a_argv, "diagnostics")
{
}

Diagnostics::~Diagnostics() 
{
}

void Diagnostics::setUp() 
{
  // This method will be call automatically _before_ running body().
}

void Diagnostics::tearDown() 
{
  // This method will be call automatically _after_ return from body().
}

void Diagnostics::nextContainer(core::data::Container &) 
{
}

} // system
} // opendlv


/*
  std::cout << "[diagnostics] received container of type " << a_c.getDataType()
      << " sent at " << a_c.getSentTimeStamp().getYYYYMMDD_HHMMSSms()
      << " received at " << a_c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() 
      << std::endl;

  if (a_c.getDataType() == Container::USER_DATA_0) {
    opendlv::generic::ExampleMessageForDiagnostics payload = 
        a_c.getData<opendlv::generic::ExampleMessageForDiagnostics>();
    
    std::cout << "[diagnostics]: " << payload.toString() << std::endl;

    // Example: Create message opendlv.generic.ExampleMessageForSafety.
    opendlv::generic::ExampleMessageForSafety nextMessage;
    nextMessage.setData1(1.2345);

    Container c2(Container::USER_DATA_1, nextMessage);
    getConference().send(c2);
  }
 */
