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
#include "manualoverride/manualoverride.hpp"

namespace opendlv {
namespace safety {
namespace manualoverride {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
ManualOverride::ManualOverride(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, 
          "safety-manualoverride"),
    m_accelerationPedalPositionThreshold(0.0),
    m_brakePedalPositionThreshold(0.0),
    m_torsionBarTorqueThreshold(0.0)
{
}

ManualOverride::~ManualOverride()
{
}

void ManualOverride::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_accelerationPedalPositionThreshold = kv.getValue<double>(
      "safety-manualoverride.accelerationPedalPositionThreshold");
  m_brakePedalPositionThreshold = kv.getValue<double>(
      "safety-manualoverride.brakePedalPositionThreshold");
  m_torsionBarTorqueThreshold = kv.getValue<double>(
      "safety-manualoverride.torsionBarTorqueThreshold");
}

void ManualOverride::tearDown()
{
}

void ManualOverride::nextContainer(odcore::data::Container &a_container)
{
  // TODO: currently vehicle specific.
  /*if (a_container.getDataType() == opendlv::proxy::reverefh16::Steering::ID()) {

    auto steering = a_container.getData<opendlv::proxy::reverefh16::Steering>();
    double steeringwheelangle = steering.getSteeringwheelangle();
    //std::cout << "Steeringwheel angle: " << steeringwheelangle << std::endl;

    if (std::abs(steeringwheelangle) > 0.3) {
      // std::cout << "Steering wheel override (" 
      //     << torsionBarTorque << ")" << std::endl;
      std::cout << "Steering Override!" << std::endl;

      opendlv::proxy::ControlState controlState(false);
      odcore::data::Container c(controlState);
      getConference().send(c);
    }
  }*/



  if (a_container.getDataType() == opendlv::proxy::reverefh16::ManualControl::ID()) {

    auto manualControl = 
        a_container.getData<opendlv::proxy::reverefh16::ManualControl>();
    double accelerationPedalPosition = manualControl.getAccelerationPedalPosition();
    double brakePedalPosition = manualControl.getBrakePedalPosition();

    

 
    //std::cout << "Pedals: " << accelerationPedalPosition << " (acc.ped.) " << std::endl;
    //std::cout <<  brakePedalPosition << " (brake ped.) " << std::endl;

    if (accelerationPedalPosition > 10.0 && accelerationPedalPosition < 102.0) {
      //std::cout << "Acceleration pedal override (" 
       //   << accelerationPedalPosition << ")" << std::endl;
      std::cout << "Acceleration Override!" << std::endl;

      opendlv::proxy::ControlOverrideState controlState(true);
      odcore::data::Container c(controlState);
      getConference().send(c);
    } else if (brakePedalPosition > 10.0 && brakePedalPosition < 102.0) {
      //std::cout << "Brake pedal override (" 
      //    << brakePedalPosition << ")" << std::endl;
      std::cout << "Break Override!" << std::endl;


      opendlv::proxy::ControlOverrideState controlState(true);
      odcore::data::Container c(controlState);
      getConference().send(c);
    } else {
      opendlv::proxy::ControlOverrideState controlState(false);
      odcore::data::Container c(controlState);
      getConference().send(c);
    }    
  }
}

} // manualoverride
} // safety
} // opendlv
