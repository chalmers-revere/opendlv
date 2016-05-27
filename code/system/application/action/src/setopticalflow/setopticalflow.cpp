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

#include "setopticalflow/setopticalflow.hpp"

namespace opendlv {
namespace action {
namespace setopticalflow {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
SetOpticalFlow::SetOpticalFlow(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "action-setopticalflow"),
    m_maxSpeed(0.0f),
    m_currentSpeed(0.0f),
    m_speedCorrection(0.0f),
    m_desiredSpeed(0.0f),
    m_currentEstimatedAcceleration(0.0f),
    m_velocityMemory()
{
}

SetOpticalFlow::~SetOpticalFlow()
{
}

/**
 * Receives current and desired flow.
 * Sends speed correction commands (throttle) and in rare cases a halt command
 * (brake) to Act.
 */

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SetOpticalFlow::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    float deltaTime = 1.0f/static_cast<float>(getFrequency());
    uint32_t nrAccAvg = 50; 
    m_velocityMemory.push_back(m_currentSpeed);
    if (m_velocityMemory.size() > nrAccAvg+1) {
      m_velocityMemory.erase(m_velocityMemory.begin());

      float sumAcc = 0;
      for (uint32_t i=0; i<m_velocityMemory.size()-1; i++) {
        sumAcc += (m_velocityMemory.at(i+1) - m_velocityMemory.at(i))/deltaTime;
      }
      m_currentEstimatedAcceleration = sumAcc/nrAccAvg;
      std::cout << "Current estimated acceleration: " 
          << m_currentEstimatedAcceleration << std::endl;
    }

    std::cout << "Current speed: " << m_currentSpeed << std::endl;
    std::cout << "Desired speed: " << m_desiredSpeed << std::endl;
    
    // TODO hardcoded parameter...
    float acceptableSpeedDeviation = 2.0f / 3.6f; // 2 km/h
    float speedCorrectionFactor = 10.0f;
    float speedDiffEstimateLimit = 5.0f;
    float estimateAmplitudeSpeedFactor = 1.8f;

    float speedDiff = 0;
    if (m_currentSpeed < (m_desiredSpeed - acceptableSpeedDeviation)) {
        
      speedDiff = m_desiredSpeed - m_currentSpeed;

      // TODO magical number
      m_speedCorrection = speedDiff / speedCorrectionFactor;

    } else if (m_currentSpeed > (m_desiredSpeed + acceptableSpeedDeviation)) {
      
      speedDiff = m_desiredSpeed - m_currentSpeed;

      // TODO magical number
      m_speedCorrection = speedDiff / speedCorrectionFactor;
    
    } else {
      m_speedCorrection = 0.0f;
    }

    if (speedDiff > speedDiffEstimateLimit) {
      odcore::data::TimeStamp t0;
      float estimateAmplitude = std::max(std::min(
          speedDiff*estimateAmplitudeSpeedFactor, 100.0f), 0.0f);
      opendlv::action::Estimate estimate(t0, "accelerate", estimateAmplitude);
      odcore::data::Container container(estimate);
      getConference().send(container);
      std::cout << "Speed Estimate: " << estimateAmplitude << std::endl;
    }
    else {
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction1(
          t0, "accelerate", false, m_speedCorrection);
      odcore::data::Container container(correction1);
      getConference().send(container);
      std::cout << "Speed Correction: " << m_speedCorrection << std::endl;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}



void SetOpticalFlow::nextContainer(odcore::data::Container &a_container)
{
  
  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    opendlv::proxy::reverefh16::Propulsion propulsion = 
        a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    
    float currentSpeedKmh = 
        static_cast<float>(propulsion.getPropulsionShaftVehicleSpeed());

    m_currentSpeed = currentSpeedKmh / 3.6f;
  }
  else if (a_container.getDataType() == opendlv::sensation::DesiredOpticalFlow::ID()) {

    opendlv::sensation::DesiredOpticalFlow flow = 
        a_container.getData<opendlv::sensation::DesiredOpticalFlow>();

    m_desiredSpeed = flow.getDesiredOpticalFlow();

    if (m_desiredSpeed > m_maxSpeed) {
      m_desiredSpeed = m_maxSpeed;
      std::cout << "Desired speed " << m_desiredSpeed 
          << " was larger than allowed max speed (" << m_maxSpeed 
          << "). Corrected." << std::endl;
    }

    std::cout << "Desired speed is now: " << m_desiredSpeed << std::endl;
  }

}

void SetOpticalFlow::setUp()
{
  odcore::base::KeyValueConfiguration kv1 = getKeyValueConfiguration();
  m_maxSpeed = kv1.getValue<float>("action-setopticalflow.max_speed");
}

void SetOpticalFlow::tearDown()
{
}

} // setopticalflow
} // action
} // opendlv
