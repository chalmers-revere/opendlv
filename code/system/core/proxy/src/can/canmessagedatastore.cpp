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
#include <automotivedata/generated/automotive/GenericCANMessage.h>
#include <odcantools/CANDevice.h>
#include <reverefh16mapping/GeneratedHeaders_reverefh16mapping.h>

#include "can/canmessagedatastore.hpp"

namespace opendlv {
namespace proxy {
namespace can {

CanMessageDataStore::CanMessageDataStore(
std::shared_ptr<automotive::odcantools::CANDevice> canDevice)
    : automotive::odcantools::MessageToCANDataStore(canDevice),
    m_enabled(true)
{
}

void CanMessageDataStore::add(odcore::data::Container const &a_container)
{
  // TODO: Kids, do not try this at home.
  odcore::data::Container &container = const_cast<odcore::data::Container &>(
      a_container);
 
  if (container.getDataType() == opendlv::proxy::ControlState::ID()){
    opendlv::proxy::ControlState controlState 
      = container.getData<opendlv::proxy::ControlState>();

    m_enabled = controlState.getAllowAutonomous();

  } else if (container.getDataType() == 
      opendlv::proxy::reverefh16::Pedals::ID()) {

    auto manualControl = 
        container.getData<opendlv::proxy::reverefh16::Pedals>();
    double accelerationPedalPosition = manualControl.getAccelerationPedalPosition();
    double brakePedalPosition = manualControl.getBrakePedalPosition();
   // double torsionBarTorque = manualControl.getTorsionBarTorque();
    
//    std::cout << "Torsion bar: " << torsionBarTorque << std::endl;
 
    // TODO: Hard-hacked constants.
    std::cout << "Pedals: " << accelerationPedalPosition << " (acc.ped.) " <<
        brakePedalPosition << " (brake ped.) " << std::endl;

    if (std::abs(accelerationPedalPosition) > 0.1) {
     // std::cout << "ACCELERATION PEDAL OVERRIDE!" << std::endl;
   //   m_enabled = false;
    }
    if (std::abs(brakePedalPosition) > 0.1) {
     // std::cout << "BRAKE PEDAL OVERRIDE!" << std::endl;
   //   m_enabled = false;
    }
   // if (std::abs(torsionBarTorque) > 2.0) {
   //   m_enabled = false;
   // }

  } else if (container.getDataType() == opendlv::proxy::Actuation::ID()){
    opendlv::proxy::Actuation actuation 
      = container.getData<opendlv::proxy::Actuation>();
    float acceleration = actuation.getAcceleration();
    //float steering = actuation.getSteering();

    if (acceleration < 0.0f) {

      if (acceleration < -3.0f) {
        acceleration = -3.0f;
      }

      opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
      brakeRequest.setEnableRequest(m_enabled);
      brakeRequest.setBrake(acceleration);
      odcore::data::Container brakeRequestContainer(brakeRequest);


      canmapping::opendlv::proxy::reverefh16::BrakeRequest 
          brakeRequestMapping;
      automotive::GenericCANMessage genericCanMessage = brakeRequestMapping.encode(brakeRequestContainer);
      m_canDevice->write(genericCanMessage);

    } else {

      if (acceleration > 4.0f) {
        acceleration = 4.0f;
      }

      opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
      accelerationRequest.setEnableRequest(m_enabled);
      accelerationRequest.setAcceleration(acceleration);
      odcore::data::Container accelerationRequestContainer(accelerationRequest);

      canmapping::opendlv::proxy::reverefh16::AccelerationRequest 
          accelerationRequestMapping;
      automotive::GenericCANMessage genericCanMessage = 
          accelerationRequestMapping.encode(accelerationRequestContainer);
      m_canDevice->write(genericCanMessage);
    }
    




/*    opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
    steeringRequest.setEnableRequest(false);
    steeringRequest.setSteeringRoadWheelAngle(0.0f);
    steeringRequest.setSteeringDeltaTorque(-5.0f); // Must be -32 to disable deltatorque.
    odcore::data::Container steeringRequestContainer(steeringRequest);
*/

   // std::cout << "Deceleration: " << deceleration << "  " << m_enabled << std::endl;







//    canmapping::opendlv::proxy::reverefh16::SteeringRequest 
//        steeringRequestMapping;
//    genericCanMessage = steeringRequestMapping.encode(steeringRequestContainer);
//    m_canDevice->write(genericCanMessage);
   
//    std::cout << "payload " << std::hex << genericCanMessage.getData() << std::endl;
  }
}

} // can
} // proxy
} // opendlv
