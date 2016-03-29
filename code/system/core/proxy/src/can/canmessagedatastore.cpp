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
    m_enabled(false),
    m_acceleration(0.0f),
    m_deceleration(0.0f),
    m_steering(0.0f)
{
}

void CanMessageDataStore::Add(odcore::data::Container &container)
{
  if (container.getDataType() == opendlv::proxy::ControlState::ID()){
    opendlv::proxy::ControlState controlState 
      = container.getData<opendlv::proxy::ControlState>();

    m_enabled = controlState.getAllowAutonomous();

  } else if (container.getDataType() == opendlv::proxy::Actuation::ID()){
    opendlv::proxy::Actuation actuation 
      = container.getData<opendlv::proxy::Actuation>();
    float acceleration = actuation.getAcceleration();
    float steering = actuation.getSteering();
    
    float deceleration;
    if (acceleration < 0.0f) {
      deceleration = acceleration;
      acceleration = 0.0f;
    } else {
      deceleration = 0.0f;
    }

    opendlv::proxy::reverefh16::AccelerationRequest accelerationRequest;
    accelerationRequest.setEnableRequest(m_enabled);
    accelerationRequest.setAcceleration(acceleration);
    odcore::data::Container accelerationRequestContainer(accelerationRequest);
    
    opendlv::proxy::reverefh16::BrakeRequest brakeRequest;
    brakeRequest.setEnableRequest(m_enabled);
    brakeRequest.setBrake(deceleration);
    odcore::data::Container brakeRequestContainer(brakeRequest);

    opendlv::proxy::reverefh16::SteeringRequest steeringRequest;
    steeringRequest.setEnableRequest(m_enabled);
    steeringRequest.setSteeringRoadWheelAngle(steering);
    steeringRequest.setSteeringDeltaTorque(-32); // Must be -32 to disable deltatorque.
    odcore::data::Container steeringRequestContainer(steeringRequest);

    automotive::GenericCANMessage genericCanMessage;

    canmapping::opendlv::proxy::reverefh16::AccelerationRequest 
        accelerationRequestMapping;
    genericCanMessage = 
        accelerationRequestMapping.encode(accelerationRequestContainer);
    m_canDevice->write(genericCanMessage);

    canmapping::opendlv::proxy::reverefh16::BrakeRequest 
        brakeRequestMapping;
    genericCanMessage = brakeRequestMapping.encode(brakeRequestContainer);
    m_canDevice->write(genericCanMessage);

    canmapping::opendlv::proxy::reverefh16::SteeringRequest 
        steeringRequestMapping;
    genericCanMessage = steeringRequestMapping.encode(steeringRequestContainer);
    m_canDevice->write(genericCanMessage);
  }
}

} // can
} // proxy
} // opendlv
