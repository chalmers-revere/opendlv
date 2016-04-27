/**
 * Copyright (C) 2016 Chalmers REVERE
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

#include <iostream>

#include <opendavinci/odcore/data/Container.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lanekeeping/lanekeeping.hpp"
#include "lanekeeping/vehicle.hpp"

namespace opendlv {
namespace tools {
namespace simulation {
namespace lanekeeping {

LaneKeeping::LaneKeeping(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-simulation-lanekeeping"),
    m_vehicle(new Vehicle),
    m_steering(0.0f)
{
}

LaneKeeping::~LaneKeeping()
{
}

void LaneKeeping::setUp()
{
  m_vehicle->SetSpeed(9.0f);
  m_vehicle->SetHeading(0.2f);

  std::cout << "Set up" << std::endl;
}

void LaneKeeping::tearDown()
{
}


void LaneKeeping::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() == opendlv::proxy::Actuation::ID()){
    auto actuation = a_c.getData<opendlv::proxy::Actuation>();
    m_steering = actuation.getSteering();
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode LaneKeeping::body()
{
  odcore::data::TimeStamp previousTimestep;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    odcore::data::TimeStamp thisTimestep;
    odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
    previousTimestep = thisTimestep;

    double deltaTime = duration.toMicroseconds() / 1000000.0;

    m_vehicle->Update(m_steering, deltaTime);

    double lateralPosition = m_vehicle->GetLateralPosition();
    double heading = m_vehicle->GetHeading();

    std::cout << "Heading: " << heading << std::endl;

    opendlv::perception::LanePosition lanePosition(lateralPosition, heading);
    odcore::data::Container msg(lanePosition);  
    getConference().send(msg);
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // lanekeeping
} // simulation
} // tools
} // opendlv
