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
#include <chrono>
#include <numeric>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "act/act.hpp"

namespace opendlv {
namespace action {
namespace act {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
Act::Act(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(a_argc, a_argv, "action-act"),
    m_maxDuration(0.5f),
    m_startTimesAccelerate(),
    m_startTimesBrake(),
    m_startTimesSteering(),
    m_amplitudesAccelerate(),
    m_amplitudesBrake(),
    m_amplitudesSteering(),
    m_accelerationValue(0.0f),
    m_brakeValue(0.0f),
    m_steeringValue(0.0f),
    m_hasEstimateAcc(false),
    m_timeOfEstimateAcc(),
    m_hasEstimateBrake(false),
    m_timeOfEstimateBrake(),
    m_hasEstimateSteer(false),
    m_timeOfEstimateSteer(),
    m_targetDistance(0),
    m_targetObjectId(-1),
    m_targetObject(),
    m_desiredSpeed(0),
    m_currentSpeed(0)
{
}

Act::~Act()
{
}

/**
* Main function that gets the correction
* m_type and m_amplitude as well as inhibitory
* signal. Values are saved as needed and
* packaged and sent to ActuationRequest.
*/
void Act::nextContainer(odcore::data::Container &a_container)
{
  odcore::data::TimeStamp now;


  if(a_container.getDataType() == opendlv::perception::ObjectDesiredAngularSize::ID()) {
    // This is really a desired distance
    opendlv::perception::ObjectDesiredAngularSize desiredAngularSize =
        a_container.getData<opendlv::perception::ObjectDesiredAngularSize>();
    m_targetDistance = desiredAngularSize.getDesiredAngularSize();
    if(desiredAngularSize.getObjectId() >= 0) {
      m_targetObjectId = desiredAngularSize.getObjectId();
    }
  } 

  if(a_container.getDataType() == (opendlv::perception::Object::ID() + 300)) {
    opendlv::perception::Object object =
        a_container.getData<opendlv::perception::Object>();

    int16_t identity = object.getObjectId();

    if (identity != m_targetObjectId) {
      m_targetObject = object;
    }
  }

  if (a_container.getDataType() == opendlv::sensation::DesiredOpticalFlow::ID()) {

    // This is really a desired velocity

    opendlv::sensation::DesiredOpticalFlow flow = 
        a_container.getData<opendlv::sensation::DesiredOpticalFlow>();

    m_desiredSpeed = flow.getDesiredOpticalFlow();

    

    //std::cout << "Desired speed is now: " << m_desiredSpeed << std::endl;
  }

  if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    opendlv::proxy::reverefh16::Propulsion propulsion = 
        a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
    
    float currentSpeedKmh = 
        static_cast<float>(propulsion.getPropulsionShaftVehicleSpeed());

    m_currentSpeed = currentSpeedKmh / 3.6f;
  }

  
  /*
  // Check if any estimates have expired
  if (m_hasEstimateAcc) {
    if ((now - m_timeOfEstimateAcc).toMicroseconds()/1000000.0f 
        > m_maxDuration) {
      m_hasEstimateAcc = false;
      m_startTimesAccelerate.clear();
      m_amplitudesAccelerate.clear();
    }
  }
  */
  if (m_hasEstimateBrake) {
    if ((now - m_timeOfEstimateBrake).toMicroseconds()/1000000.0f 
        > m_maxDuration) {
      m_hasEstimateBrake = false;
      m_startTimesBrake.clear();
      m_amplitudesBrake.clear();
    }
  }
  if (m_hasEstimateSteer) {
    if ((now - m_timeOfEstimateSteer).toMicroseconds()/1000000.0f 
        > m_maxDuration) {
      m_hasEstimateSteer = false;
      m_startTimesSteering.clear();
      m_amplitudesSteering.clear();
    }
  }



  // If we receive an estimate...
  if (a_container.getDataType() == opendlv::action::Estimate::ID()) {
    opendlv::action::Estimate estimate = 
        a_container.getData<opendlv::action::Estimate>();

    std::cout << "Received estimate..." << std::endl;


    auto startTime = estimate.getStartTime();
    std::string type = estimate.getType();
    float amplitude = estimate.getAmplitude();

    // If not already has estimate, add one and clear anything existing
    /*
    if (type == "accelerate") {

      m_startTimesAccelerate.clear();
      m_amplitudesAccelerate.clear();
      
      m_startTimesAccelerate.push_back(startTime);
      m_amplitudesAccelerate.push_back(amplitude);

      m_hasEstimateAcc = true;
      m_timeOfEstimateAcc = now;

      m_accelerationValue = amplitude;

    } else  */if (type == "brake") {
      m_startTimesBrake.clear();
      m_amplitudesBrake.clear();
    
      m_startTimesBrake.push_back(startTime);
      m_amplitudesBrake.push_back(amplitude);

      m_hasEstimateBrake = true;
      m_timeOfEstimateBrake = now;

      m_brakeValue = amplitude;

    } else if (type == "steering") {
      m_startTimesSteering.clear();
      m_amplitudesSteering.clear();

      m_startTimesSteering.push_back(startTime);
      m_amplitudesSteering.push_back(amplitude);

      m_hasEstimateSteer = true;
      m_timeOfEstimateSteer = now;
      
      m_steeringValue = amplitude;
      
    }
  }


  // If we reeive a correction...
  if (a_container.getDataType() == opendlv::action::Correction::ID()) {
    opendlv::action::Correction correction = 
        a_container.getData<opendlv::action::Correction>();

    std::cout << "Received correction..." << std::endl;

    auto startTime = correction.getStartTime();
    std::string type = correction.getType();
    bool isInhibitory = correction.getIsInhibitory();
    float amplitude = correction.getAmplitude();
    
    // If not already following an estimate, apply correction

    /*
    if (type == "accelerate") {
      if (!m_hasEstimateAcc) {
        if (isInhibitory) {
          m_startTimesAccelerate.clear();
          m_amplitudesAccelerate.clear();
        }
        
        m_startTimesAccelerate.push_back(startTime);
        m_amplitudesAccelerate.push_back(amplitude);
      }
    } else */if (type == "brake") {
      if (!m_hasEstimateBrake) {
        if (isInhibitory) {
          m_startTimesBrake.clear();
          m_amplitudesBrake.clear();
        }
        
        m_startTimesBrake.push_back(startTime);
        m_amplitudesBrake.push_back(amplitude);
      }
    } else if (type == "steering") {
      if (!m_hasEstimateSteer) {
        if (isInhibitory) {
          m_startTimesSteering.clear();
          m_amplitudesSteering.clear();
        }
        
        m_startTimesSteering.push_back(startTime);
        m_amplitudesSteering.push_back(amplitude);
      }
    }
  }
}

/**
* Receives control correction requests, including a modality, if inhibitory,
* m_amplitude, and a start time.
* Sends modulated contol signal as individual samples, per modality to Proxy
* actuators.
*/
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Act::body()
{
  float freq = static_cast<float>(getFrequency());

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
    odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    odcore::data::TimeStamp now;


    float distanceToObject = m_targetObject.getDistance();

    float minThreshold = 20;
    float maxThreshold = 50;
    float velocityCorrectionFactor = 0.5f;
    float distanceCorrectionFactor = 3.0f;


    float speedCorrection = 0;


    if (distanceToObject < minThreshold) {
      // Want to brake
      speedCorrection = 0;
    }
    else if (distanceToObject > maxThreshold) {
      // Too far away

      // Correct according to desired velocity
      float speedDiff = m_desiredSpeed - m_currentSpeed;
      speedCorrection = speedDiff*velocityCorrectionFactor;
    }
    else {
      // In the interval
      float distanceRatio = m_targetDistance / distanceToObject ;
      speedCorrection = 1.0f - distanceRatio;

      speedCorrection *= distanceCorrectionFactor;

      // Correct according to vehicle in front
    }
    m_accelerationValue += speedCorrection;
    if (m_accelerationValue < 0.001f)
    {
        m_accelerationValue = 0.0;
    }

    std::cout << "\n\n distance to objects " << distanceToObject
              << "\n desired speed" << m_desiredSpeed
              << "\n speedCorrection " << speedCorrection << std::endl;


    
    /*
    if (!m_hasEstimateAcc) {
      std::vector<odcore::data::TimeStamp> startTimesAccelerateToSave;
      std::vector<float> amplitudesAccelerateToSave;
      for (uint8_t i = 0; i < m_startTimesAccelerate.size(); i++) {
        auto t0 = m_startTimesAccelerate[i];
        auto amplitude = m_amplitudesAccelerate[i];

        float t1 = static_cast<float>(now.toMicroseconds() 
            - t0.toMicroseconds()) / 1000000.0f;

        if (t1 < m_maxDuration) {
          float deltaValue = amplitude / freq;
          m_accelerationValue += deltaValue;

          startTimesAccelerateToSave.push_back(t0);
          amplitudesAccelerateToSave.push_back(amplitude);
        }
      }

      // stores a new list instead of the old one???
      m_startTimesAccelerate = startTimesAccelerateToSave;
      m_amplitudesAccelerate = amplitudesAccelerateToSave;
    }
    */
    if (!m_hasEstimateBrake) {
      std::vector<odcore::data::TimeStamp> startTimesBrakeToSave;
      std::vector<float> amplitudesBrakeToSave;
      for (uint8_t i = 0; i < m_startTimesBrake.size(); i++) {
        auto t0 = m_startTimesBrake[i];
        auto amplitude = m_amplitudesBrake[i];

        float t1 = static_cast<float>(now.toMicroseconds() 
            - t0.toMicroseconds()) / 1000000.0f;

        if (t1 < m_maxDuration) {
          float deltaValue = amplitude / freq;
          m_brakeValue += deltaValue;

          startTimesBrakeToSave.push_back(t0);
          amplitudesBrakeToSave.push_back(amplitude);
        }
      }

      // stores a new list instead of the old one???
      m_startTimesBrake = startTimesBrakeToSave;
      m_amplitudesBrake = amplitudesBrakeToSave;
    }

    if (!m_hasEstimateSteer) {
      std::vector<odcore::data::TimeStamp> startTimesSteeringToSave;
      std::vector<float> amplitudesSteeringToSave;
      for (uint8_t i = 0; i < m_startTimesSteering.size(); i++) {
        auto t0 = m_startTimesSteering[i];
        auto amplitude = m_amplitudesSteering[i];

        float t1 = static_cast<float>(now.toMicroseconds() 
            - t0.toMicroseconds()) / 1000000.0f;

        if (t1 < m_maxDuration) {
          float deltaValue = amplitude / freq;
          m_steeringValue += deltaValue;

          startTimesSteeringToSave.push_back(t0);
          amplitudesSteeringToSave.push_back(amplitude);
        }
      }

      // stores a new list instead of the old one???
      m_startTimesSteering = startTimesSteeringToSave;
      m_amplitudesSteering = amplitudesSteeringToSave;
    }

/*
  // Old code.
    if (m_brakeValue > 0.0f) {
      opendlv::proxy::ActuationRequest actuationRequest(m_brakeValue, 
          m_steeringValue, false);
      odcore::data::Container actuationContainer(actuationRequest);
      getConference().send(actuationContainer);
    } else {
      opendlv::proxy::ActuationRequest actuationRequest(m_accelerationValue, 
          m_steeringValue, false);
      odcore::data::Container actuationContainer(actuationRequest);
      getConference().send(actuationContainer);
    }
*/
    {
        opendlv::proxy::ActuationRequest actuationRequest;
        if (m_brakeValue > 0.0f) {
          actuationRequest = opendlv::proxy::ActuationRequest(m_brakeValue, m_steeringValue, false);
        } else {
          actuationRequest = opendlv::proxy::ActuationRequest(m_accelerationValue, m_steeringValue, false);
        }

        // Former code from checkActuation.
        float acceleration = actuationRequest.getAcceleration();
        float steering = actuationRequest.getSteering();

        float m_maxAllowedDeceleration = 3;
        float steeringLimit = 0.2;
        float accMaxLimit = 80;


        // TODO if acceleration is negative it is m/s^2, if positive percent of acceleration pedal

        // clamp steering
        if (steering < -steeringLimit) {
          steering = -steeringLimit;
          std::cout << "steering request was capped to " << steering << std::endl;
        }
        else if (steering > steeringLimit) {
          steering = steeringLimit;
          std::cout << "steering request was capped to " << steering << std::endl;
        }

        // clamp acceleration
        if (acceleration < -m_maxAllowedDeceleration) {
          acceleration = -m_maxAllowedDeceleration;
          std::cout << "acceleration request was capped to " << acceleration << std::endl;
        }
        else if (acceleration > accMaxLimit) {
          acceleration = accMaxLimit;
          std::cout << "acceleration request was capped to " << acceleration << std::endl;
        }


        actuationRequest.setAcceleration(acceleration);
        actuationRequest.setSteering(steering);

        actuationRequest.setIsValid(true);

        odcore::data::Container actuationContainer(actuationRequest);
        getConference().send(actuationContainer);
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Act::setUp()
{
}

void Act::tearDown()
{
}

} // act
} // action
} // opendlv
