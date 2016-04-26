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
  m_breakingcorrection(0.0f),
  m_steeringCorrection(0.0f),
  counterAccelerate(0),
  counterBrake(0),
  counterSteering(0)
  {
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

  //  std::cout << "Send acc. " << m_acceleration << " Steering: " << m_steering << std::endl;

    opendlv::proxy::Actuation actuation(m_acceleration, m_steering, false);
    odcore::data::Container c(actuation);
    getConference().send(c);
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

  if(c.getDataType() == opendlv::action::Correction::ID()) {
    opendlv::action::Correction correction = 
    c.getData<opendlv::action::Correction>();

    odcore::data::TimeStamp t0 = correction.getStartTime();
    std::string type = correction.getType();
    bool isInhibitory = correction.getIsInhibitory();
    float amplitude = correction.getAmplitude();

    float startTimeVectorAccelerate[];
    float startTimeVectorBrake[];
    float startTimeVectorSteering[];
    float amplitudeVectorAccelerate[];
    float amplitudeVectorBrake[];
    float amplitudeVectorSteering[];

    if (type == "accelerate") {
      inhibitoryCheck(isInhibitory, startTimeVectorAccelerate[], amplitudeVectorAccelerate[], counterAccelerate);
      startTimeVectorAccelerate[counterAccelerate] = t0;
      amplitudeVectorAccelerate[counterAccelerate] = amplitude;
      counterAccelerate++;
      timeCheck(startTimeVectorAccelerate[], amplitudeVectorAccelerate[], counterAccelerate);
      sumOfAccelerate = std::accumulate(amplitudeVectorAccelerate.begin(), amplitudeVectorAccelerate.end(), 0.0);
    }

    else if (type == "brake") {
      inhibitoryCheck(isInhibitory, startTimeVectorBrake[], amplitudeVectorBrake[], counterBrake);
      startTimeVectorBrake[counterBrake] = t0;
      amplitudeVectorBrake[counterBrake] = amplitude;
      counterBrake++;
      timeCheck(startTimeVectorBrake[], amplitudeVectorBrake[], counterBrake);
      sumOfBrake = std::accumulate(amplitudeVectorBrake.begin(), amplitudeVectorBrake.end(), 0.0);
    }

    else if (type == "steering") {
      inhibitoryCheck(isInhibitory, startTimeVectorSteering[], amplitudeVectorSteering[], counterSteering);
      startTimeVectorSteering[counterSteering] = t0;
      amplitudeVectorSteering[counterSteering] = amplitude;
      counterSteering++;
      timeCheck(startTimeVectorSteering[], amplitudeVectorSteering[], counterSteering);
      sumOfSteering = std::accumulate(amplitudeVectorSteering.begin(), amplitudeVectorSteering.end(), 0.0);
    }
  }
  float freq = getFrequency();
  m_accelerationCorrection = sumOfAccelerate/freq;
  m_breakingcorrection = -sumOfBrake/freq;
  m_steeringCorrection = sumOfSteering/freq;

  if (m_breakingcorrection < 0) {
  	opendlv::proxy::Actuation actuation(m_breakingcorrection, m_steeringCorrection, false);
  	odcore::data::Container c(actuation);
  	getConference().send(c);
  }

  else {
  	opendlv::proxy::Actuation actuation(m_accelerationCorrection, m_steeringCorrection, false);
  	odcore::data::Container c(actuation);
  	getConference().send(c);
  }
}

/**
 * Function to ensure that the first element of
 * the time vector is less than deltaTime seconds
 * ago. Truncates the vector if it is.
 */
 void Act::timeCheck(float &timeVector, float &amplitudeVector, uint32_t &counter)
 {
   float deltaTime = 0.5f;
   double now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
   if ( now - timeVector[0] >= deltaTime ) {
    for (uint32_t count = 0; count < counter; count++) {
      timeVector[count] = timeVector[count+1]
      amplitudeVector[count] = amplitudeVector[count+1]
    }
    counter = counter -1;
  }
}

/**
 * Checks if an inhibitory signal is recieved
 * and if so clears the relevant time and amplitude
 * vectors prior to adding new corrections.
 */
 int Act::inhibitoryCheck(bool isInhibitory, float &timeVector[], float &amplitudeVector[], uint32_t &counter )
 {
  if (isInhibitory) {
    timeVector.clear;
    amplitudeVector.clear;
    counter = 0;
  }
  return counter;
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
