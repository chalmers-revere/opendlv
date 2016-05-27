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

#include "gcdc2016/gcdc2016.hpp"
#include "gcdc2016/vehicle.hpp"

namespace opendlv {
namespace tools {
namespace simulation {
namespace gcdc2016 {

Gcdc2016::Gcdc2016(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-simulation-lanekeeping"),
    m_vehicle(new Vehicle),
    m_steering(0.0f)
{
}

Gcdc2016::~Gcdc2016()
{
}

void Gcdc2016::setUp()
{
  m_vehicle->SetSpeed(9.0f);
  m_vehicle->SetHeading(0.2f);

  std::cout << "Set up" << std::endl;
}

void Gcdc2016::tearDown()
{
}


void Gcdc2016::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() == opendlv::proxy::ActuationRequest::ID()){
    auto actuationRequest = a_c.getData<opendlv::proxy::ActuationRequest>();
    m_steering = actuationRequest.getSteering();
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Gcdc2016::body()
{
  odcore::data::TimeStamp previousTimestep;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    odcore::data::TimeStamp thisTimestep;
    odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
    previousTimestep = thisTimestep;


    std::cout << "m_steering: " << m_steering << std::endl;
    double deltaTime = duration.toMicroseconds() / 1000000.0;
    //std::cout << "deltaTime: " << deltaTime << std::endl; 
    m_vehicle->Update(m_steering, deltaTime);

    double lateralPosition = m_vehicle->GetLateralPosition();
    double heading = m_vehicle->GetHeading();

    std::cout << "Heading: " << heading << std::endl;
    std::cout << "Heading (degrees): " << (heading * 57.295779513) << std::endl;
    std::cout << "Lateral position: " << lateralPosition << std::endl;

// TODO: Use Surface instead.
//    opendlv::perception::LanePosition lanePosition(lateralPosition, heading);
//    odcore::data::Container msg(lanePosition);  
//    getConference().send(msg);
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // lanekeeping
} // simulation
} // tools
} // opendlv
