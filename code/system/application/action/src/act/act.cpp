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
  m_accelerationCorrection(0.0f),
  m_breakingCorrection(0.0f),
  m_steeringCorrection(0.0f),
  counterAccelerate(0),
  counterBrake(0),
  counterSteering(0),
  startTimeVectorAccelerate(),
  startTimeVectorBrake(),
  startTimeVectorSteering(),
  amplitudeVectorAccelerate(),
  amplitudeVectorBrake(),
  amplitudeVectorSteering(),
  previousTimestep()
  {
    setUp();
  }

  Act::~Act()
  {
  }

/**
 * Receives control correction requests, including a modality, if inhibitory,
 * amplitude, and a start time.
 * Sends modulated contol signal as individual samples, per modality to Proxy
 * actuators.
 */
 odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Act::body()
 {
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
    odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    
  }
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Main function that gets the correction
 * type and amplitude as well as inhibitory
 * signal. Values are saved as needed and
 * packaged and sent to Actuation.
 */
 void Act::nextContainer(odcore::data::Container &c)
 {
  float sumOfAccelerate = 0.0f;
  float sumOfBrake = 0.0f;
  float sumOfSteering = 0.0f;
  odcore::data::TimeStamp thisTimestep;
  odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
  previousTimestep = thisTimestep;

  if(c.getDataType() == opendlv::action::Correction::ID()) {
    opendlv::action::Correction correction = 
    c.getData<opendlv::action::Correction>();

    odcore::data::TimeStamp t0 = correction.getStartTime();
    std::string type = correction.getType();
    bool isInhibitory = correction.getIsInhibitory();
    float amplitude = correction.getAmplitude();

    std::cout << "Type:" << type <<std::endl;
    std::cout << "Amplitude:" << amplitude <<std::endl <<std::endl;

    float previousAmplitude = 0;


    if (type == "accelerate") {
      if (!amplitudeVectorAccelerate.empty()) {
        previousAmplitude = amplitudeVectorAccelerate.at(amplitudeVectorAccelerate.size()-1);
      }
      inhibitoryCheck(isInhibitory, startTimeVectorAccelerate, amplitudeVectorAccelerate);
      if (amplitude*previousAmplitude < 0 || fabs(amplitude) > fabs(previousAmplitude)) {
        startTimeVectorAccelerate.push_back(  t0);
        amplitudeVectorAccelerate.push_back(amplitude);
        timeCheck(startTimeVectorAccelerate, amplitudeVectorAccelerate);
      }
      sumOfAccelerate = std::accumulate(amplitudeVectorAccelerate.begin(), amplitudeVectorAccelerate.end(), 0.0);
    }

    else if (type == "brake") {
      if (!amplitudeVectorBrake.empty()) {
        previousAmplitude = amplitudeVectorBrake.at(amplitudeVectorBrake.size()-1);
      }
      inhibitoryCheck(isInhibitory, startTimeVectorBrake, amplitudeVectorBrake);
      if (amplitude*previousAmplitude < 0 || fabs(amplitude) > fabs(previousAmplitude)) {
        startTimeVectorBrake.push_back(t0);
        amplitudeVectorBrake.push_back(amplitude);
        timeCheck(startTimeVectorBrake, amplitudeVectorBrake);
      }
      sumOfBrake = std::accumulate(amplitudeVectorBrake.begin(), amplitudeVectorBrake.end(), 0.0);
    }

    else if (type == "steering") {
      if (!amplitudeVectorSteering.empty()) {
        previousAmplitude = amplitudeVectorSteering.at(amplitudeVectorSteering.size()-1);
      }
      inhibitoryCheck(isInhibitory, startTimeVectorSteering, amplitudeVectorSteering);
      if (amplitude*previousAmplitude < 0 || fabs(amplitude) > fabs(previousAmplitude)) {


        startTimeVectorSteering.push_back(t0);
        amplitudeVectorSteering.push_back(amplitude);
        timeCheck(startTimeVectorSteering, amplitudeVectorSteering);
      }
      sumOfSteering = std::accumulate(amplitudeVectorSteering.begin(), amplitudeVectorSteering.end(), 0.0);
      std::cout << "Sum Of Steering : " << sumOfSteering <<std::endl;
    }

    double durationInSeconds = duration.toMicroseconds() / 1000000.0;
    float freq = 60/durationInSeconds;
    m_accelerationCorrection = sumOfAccelerate/freq;
    m_breakingCorrection = -sumOfBrake/freq;
    m_steeringCorrection = sumOfSteering/freq;
    std::cout << "Stearing Correction : " << m_steeringCorrection <<std::endl;
    std::cout << "accelerati Correction : " << m_accelerationCorrection <<std::endl;
    std::cout << "Freq : " << freq <<std::endl;

    if (m_breakingCorrection < 0) {
      opendlv::proxy::Actuation actuation(m_breakingCorrection, m_steeringCorrection, false);
      odcore::data::Container actuationContainer(actuation);
      getConference().send(actuationContainer);
    }

    else {
      opendlv::proxy::Actuation actuation(m_accelerationCorrection, m_steeringCorrection, false);
      odcore::data::Container actuationContainer(actuation);
      getConference().send(actuationContainer);
    }
  }
}

/**
 * Function to ensure that the first element of
 * the time vector is less than deltaTime seconds
 * ago. Truncates the vector if it is.
 */
 void Act::timeCheck(std::vector<odcore::data::TimeStamp> &timeVector, std::vector<float> &amplitudeVector)
 {

  if (timeVector.size() == 0){
    return;
  }
  double deltaTime = 1000000*0.5f;
  odcore::data::TimeStamp nowTimeStamp;
  double now = nowTimeStamp.toMicroseconds();
  double firstTime = timeVector.at(0).toMicroseconds();
  if ( now - firstTime >= deltaTime ) {
    for (uint32_t count = 0; count < timeVector.size()-1; count++) {
      timeVector.at(count) = timeVector.at(count+1);
      amplitudeVector.at(count) = amplitudeVector.at(count+1);
    }
    timeVector.erase(timeVector.begin()+timeVector.size()-1);
    amplitudeVector.erase(amplitudeVector.begin()+amplitudeVector.size()-1);
  }
}

/**
 * Checks if an inhibitory signal is recieved
 * and if so clears the relevant time and amplitude
 * vectors prior to adding new corrections.
 */
 void Act::inhibitoryCheck(bool isInhibitory, std::vector<odcore::data::TimeStamp> &timeVector, std::vector<float> &amplitudeVector)
 {
  if (isInhibitory) {
    timeVector.clear();
    amplitudeVector.clear();    
  }
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
