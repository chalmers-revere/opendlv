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
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "action-setopticalflow"),
    m_stimulii(),
    m_correctionTimes(),
    m_opticFlowIdentified(0, 0),
    m_opticFlow(0.0f)
{
}

SetOpticalFlow::~SetOpticalFlow()
{
}

void SetOpticalFlow::nextContainer(odcore::data::Container &a_container)
{
  // TODO: Action should never see proxy, should go through sensation
  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    auto propulsion = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    
    float currentSpeedKmh = 
        static_cast<float>(propulsion.getPropulsionShaftVehicleSpeed());

    m_opticFlow = currentSpeedKmh / 3.6f;
    m_opticFlowIdentified = odcore::data::TimeStamp();
  } else if (a_container.getDataType() == opendlv::perception::StimulusOpticalFlow::ID()) {

//    auto desiredOpticalFlow = 
//        a_container.getData<opendlv::perception::StimulusOpticalFlow>();

   // float flow = desiredOpticalFlow.getDesiredOpticalFlow();

  }
}

/*

    // TODO hardcoded parameter...
    float acceptableSpeedDeviation = 2.0f / 3.6f; // 2 km/h
    float speedCorrectionFactor = 5.0f;
 //   float speedDiffEstimateLimit = 0.5f;
   // float estimateAmplitudeSpeedFactor = 25.0f;
    float accelerationPredictionTime = 0.5f;



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
    float predictedVelocity = m_currentSpeed + 
        m_currentEstimatedAcceleration*accelerationPredictionTime;

    std::cout << "Current speed: " << m_currentSpeed << std::endl;
    std::cout << "Desired speed: " << m_desiredSpeed << std::endl;
    std::cout << "Predicted speed " << accelerationPredictionTime 
        << " seconds ahead: " << predictedVelocity << std::endl;
    

    float speedDiff = 0;
    if (m_currentSpeed < (m_desiredSpeed - acceptableSpeedDeviation)) {
        
      speedDiff = m_desiredSpeed - predictedVelocity;

      // TODO magical number
      m_speedCorrection = speedDiff / speedCorrectionFactor;

    } else if (m_currentSpeed > (m_desiredSpeed + acceptableSpeedDeviation)) {
      
      speedDiff = m_desiredSpeed - predictedVelocity;

      // TODO magical number
      m_speedCorrection = speedDiff / speedCorrectionFactor;
    
    } else {
      m_speedCorrection = 0.0f;
    }


    std::cout << speedDiff << std::endl;



    if (predictedVelocity < 0.4f * m_desiredSpeed) {
      std::cout << "Mode 1" << std::endl;
      odcore::data::TimeStamp t0;
      float estimateAmplitude = 60.0f;
      opendlv::action::Estimate estimate(t0, "accelerate", estimateAmplitude);
      odcore::data::Container container(estimate);
      getConference().send(container);
      std::cout << "Speed Estimate: " << estimateAmplitude << std::endl;
    }
    else if (predictedVelocity > 0.4f * m_desiredSpeed && predictedVelocity < 0.8f * m_desiredSpeed) {
      std::cout << "Mode 2" << std::endl;
      odcore::data::TimeStamp t0;
      float estimateAmplitude = 30.0f;
      opendlv::action::Estimate estimate(t0, "accelerate", estimateAmplitude);
      odcore::data::Container container(estimate);
      getConference().send(container);
      std::cout << "Speed Estimate: " << estimateAmplitude << std::endl;
    }
    else if (predictedVelocity > 0.8f * m_desiredSpeed && predictedVelocity < 0.9f * m_desiredSpeed) {
      std::cout << "Mode 3" << std::endl;
      odcore::data::TimeStamp t0;
      float estimateAmplitude = 20.0f;
      opendlv::action::Estimate estimate(t0, "accelerate", estimateAmplitude);
      odcore::data::Container container(estimate);
      getConference().send(container);
      std::cout << "Speed Estimate: " << estimateAmplitude << std::endl;
    }
    else {
      
      std::cout << "CORRECTION MODE CORRECTION MODE CORRECTION MODE!!!!!" << std::endl;


      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction1(
          t0, "accelerate", false, m_speedCorrection);
      odcore::data::Container container(correction1);
      getConference().send(container);
      std::cout << "Speed Correction: " << m_speedCorrection << std::endl;
    }
*/


void SetOpticalFlow::setUp()
{
//  odcore::base::KeyValueConfiguration kv1 = getKeyValueConfiguration();
//  m_maxSpeed = kv1.getValue<float>("action-setopticalflow.max_speed");
}

void SetOpticalFlow::tearDown()
{
}

} // setopticalflow
} // action
} // opendlv
