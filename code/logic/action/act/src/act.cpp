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

#include <iostream>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>

#include <odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h>

#include "act.hpp"

namespace opendlv {
namespace logic {
namespace action {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
Act::Act(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-action-act"),
    m_startTimesAccelerate(),
    m_startTimesBrake(),
    m_startTimesSteering(),
    m_amplitudesAccelerate(),
    m_amplitudesBrake(),
    m_amplitudesSteering(),
    m_accelerationValue(0.0f),
    m_brakeValue(0.0f),
    m_steeringValue(0.0f),
    m_correctionDuration(0.5f)
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
  if (a_container.getDataType() == opendlv::action::Correction::ID()) {
    opendlv::action::Correction correction = 
        a_container.getData<opendlv::action::Correction>();

  //  auto startTime = correction.getStartTime();
    std::string type = correction.getType();
  //  bool isInhibitory = correction.getIsInhibitory();
    float amplitude = correction.getAmplitude();
    
    if (type == "accelerate") {
  /*    if (isInhibitory) {
        m_startTimesAccelerate.clear();
        m_amplitudesAccelerate.clear();
      }
        
      m_startTimesAccelerate.push_back(startTime);
      m_amplitudesAccelerate.push_back(amplitude);
      */
      m_accelerationValue = amplitude;

    } else if (type == "brake") {
      /*
      if (isInhibitory) {
        m_startTimesBrake.clear();
        m_amplitudesBrake.clear();
      }
        
      m_startTimesBrake.push_back(startTime);
      m_amplitudesBrake.push_back(amplitude);
      */
      m_brakeValue = amplitude;


    } else if (type == "steering") {
      /*
      if (isInhibitory) {
        m_startTimesSteering.clear();
        m_amplitudesSteering.clear();
      }
        
      m_startTimesSteering.push_back(startTime);
      m_amplitudesSteering.push_back(amplitude);
      */

      m_steeringValue = amplitude;
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
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
    odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    /*
    m_accelerationValue = 0.0f;
    m_brakeValue = 0;
    m_steeringValue = 0;

    odcore::data::TimeStamp now;

    std::vector<odcore::data::TimeStamp> startTimesAccelerateToSave;
    std::vector<float> amplitudesAccelerateToSave;
    for (uint8_t i = 0; i < m_startTimesAccelerate.size(); i++) {
      auto t0 = m_startTimesAccelerate[i];
      auto amplitude = m_amplitudesAccelerate[i];

      float t1 = static_cast<float>(now.toMicroseconds() 
          - t0.toMicroseconds()) / 1000000.0f;

      if (t1 < m_correctionDuration) {
      //  float ratio = t1 / m_correctionDuration;
      //  float deltaValue = amplitude * ratio;
        m_accelerationValue = 25.0f; // += deltaValue;

        startTimesAccelerateToSave.push_back(t0);
        amplitudesAccelerateToSave.push_back(amplitude);
      }
    }

    m_startTimesAccelerate = startTimesAccelerateToSave;
    m_amplitudesAccelerate = amplitudesAccelerateToSave;


    std::vector<odcore::data::TimeStamp> startTimesBrakeToSave;
    std::vector<float> amplitudesBrakeToSave;
    for (uint8_t i = 0; i < m_startTimesBrake.size(); i++) {
      auto t0 = m_startTimesBrake[i];
      auto amplitude = m_amplitudesBrake[i];

      float t1 = static_cast<float>(now.toMicroseconds() 
          - t0.toMicroseconds()) / 1000000.0f;

      if (t1 < m_correctionDuration) {
        float ratio = t1 / m_correctionDuration;
        float deltaValue = amplitude * ratio;
        m_brakeValue += deltaValue;

        startTimesBrakeToSave.push_back(t0);
        amplitudesBrakeToSave.push_back(amplitude);
      }
    }

    m_startTimesBrake = startTimesBrakeToSave;
    m_amplitudesBrake = amplitudesBrakeToSave;


    std::vector<odcore::data::TimeStamp> startTimesSteeringToSave;
    std::vector<float> amplitudesSteeringToSave;
    for (uint8_t i = 0; i < m_startTimesSteering.size(); i++) {
      auto t0 = m_startTimesSteering[i];
      auto amplitude = m_amplitudesSteering[i];

      float t1 = static_cast<float>(now.toMicroseconds() 
          - t0.toMicroseconds()) / 1000000.0f;

      if (t1 < m_correctionDuration) {
        float ratio = t1 / m_correctionDuration;
        float deltaValue = amplitude * ratio;
        m_steeringValue += deltaValue;

        startTimesSteeringToSave.push_back(t0);
        amplitudesSteeringToSave.push_back(amplitude);
      }
    }

    m_startTimesSteering = startTimesSteeringToSave;
    m_amplitudesSteering = amplitudesSteeringToSave;
    */


    if (m_brakeValue > 0.0f) {
      // std::cout << "Sent actuation request" << std::endl;
      opendlv::proxy::ActuationRequest actuationRequest(m_brakeValue, 
          m_steeringValue, true);
      odcore::data::Container actuationContainer(actuationRequest,opendlv::proxy::ActuationRequest::ID());
      getConference().send(actuationContainer);

     // std::cout << "Send steering " << m_steeringValue << " brake " << m_brakeValue << std::endl;

    } else {
      // std::cout << "Sent actuation request" << std::endl;
      opendlv::proxy::ActuationRequest actuationRequest(m_accelerationValue, 
          m_steeringValue, true);
      odcore::data::Container actuationContainer(actuationRequest,opendlv::proxy::ActuationRequest::ID());
      getConference().send(actuationContainer);
      
     // std::cout << "Send steering " << m_steeringValue << " acceleration " << m_accelerationValue << std::endl;
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

}
}
}
