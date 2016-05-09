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
m_brakeCorrection(0.0f),
m_steeringCorrection(0.0f),
m_startTimeVectorAccelerate(),
m_startTimeVectorBrake(),
m_startTimeVectorSteering(),
m_amplitudeVectorAccelerate(),
m_amplitudeVectorBrake(),
m_amplitudeVectorSteering(),
m_isInhibitory(false),
m_amplitude(0),
m_t0(""),
m_type(""),
m_logSteering()
{
  setUp();
    odcore::data::TimeStamp now;
    std::string filename("Steering" + now.getYYYYMMDD_HHMMSS() + ".log");
    m_logSteering.open(filename, std::ios::out|std::ios::app);
}

Act::~Act()
{
  m_logSteering.close();
}

/**
* Main function that gets the correction
* m_type and m_amplitude as well as inhibitory
* signal. Values are saved as needed and
* packaged and sent to Actuation.
*/
void Act::nextContainer(odcore::data::Container &c)
{

  if(c.getDataType() == opendlv::action::Correction::ID()) {
    opendlv::action::Correction correction = 
    c.getData<opendlv::action::Correction>();

    m_t0 = correction.getStartTime();
    m_type = correction.getType();
    m_isInhibitory = correction.getIsInhibitory();
    m_amplitude = correction.getAmplitude();

    std::cout << "m_amplitude:" << m_amplitude <<std::endl <<std::endl;
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
  odcore::data::TimeStamp previousTimestep;

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
    odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    float sumOfAccelerate = 0.0f;
    float sumOfBrake = 0.0f;
    float sumOfSteering = 0.0f;
    odcore::data::TimeStamp thisTimestep;
    odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
    previousTimestep = thisTimestep;
    float previousAmplitude = 0;

    if ( m_type == "accelerate" ) {
      if ( !m_amplitudeVectorAccelerate.empty() ) {
        previousAmplitude = m_amplitudeVectorAccelerate.at(m_amplitudeVectorAccelerate.size() - 1 );
      }
      inhibitoryCheck(m_isInhibitory, m_startTimeVectorAccelerate, m_amplitudeVectorAccelerate);
      if ( m_amplitude*previousAmplitude < 0 || fabs(m_amplitude) > fabs(previousAmplitude) ) {
        m_startTimeVectorAccelerate.push_back(m_t0);
        m_amplitudeVectorAccelerate.push_back(m_amplitude);
        timeCheck(m_startTimeVectorAccelerate, m_amplitudeVectorAccelerate);
      }
      sumOfAccelerate = std::accumulate(m_amplitudeVectorAccelerate.begin(), m_amplitudeVectorAccelerate.end(), 0.0 );
    }

    else if ( m_type == "brake" ) {
      if ( !m_amplitudeVectorBrake.empty() ) {
        previousAmplitude = m_amplitudeVectorBrake.at(m_amplitudeVectorBrake.size() -1 );
      }
      inhibitoryCheck(m_isInhibitory, m_startTimeVectorBrake, m_amplitudeVectorBrake);
      if ( m_amplitude*previousAmplitude < 0 || fabs(m_amplitude) > fabs(previousAmplitude) ) {
        m_startTimeVectorBrake.push_back(m_t0);
        m_amplitudeVectorBrake.push_back(m_amplitude);
        timeCheck(m_startTimeVectorBrake, m_amplitudeVectorBrake);
      }
      sumOfBrake = std::accumulate(m_amplitudeVectorBrake.begin(), m_amplitudeVectorBrake.end(), 0.0 );
    }

    else if ( m_type == "steering" ) {
      if ( !m_amplitudeVectorSteering.empty() ) {
        previousAmplitude = m_amplitudeVectorSteering.at(m_amplitudeVectorSteering.size() -1 );
      }
      inhibitoryCheck(m_isInhibitory, m_startTimeVectorSteering, m_amplitudeVectorSteering);
      if ( m_amplitude*previousAmplitude < 0 || fabs(m_amplitude) > fabs(previousAmplitude) ) {
        m_startTimeVectorSteering.push_back(m_t0);
        m_amplitudeVectorSteering.push_back(m_amplitude);
        timeCheck(m_startTimeVectorSteering, m_amplitudeVectorSteering);
      }
    sumOfSteering = std::accumulate(m_amplitudeVectorSteering.begin(), m_amplitudeVectorSteering.end(), 0.0 );
    
    }
    double durationInSeconds = duration.toMicroseconds() / 1000000.0;
    float freq = 1 / durationInSeconds;
    m_accelerationCorrection = sumOfAccelerate / freq;
    m_brakeCorrection  = -sumOfBrake / freq;
    m_steeringCorrection = sumOfSteering / freq;
    //std::cout << "Steering Correction : " << m_steeringCorrection <<std::endl;
    //std::cout << "Acceleration Correction : " << m_accelerationCorrection+m_brakeCorrection <<std::endl;
    m_logSteering << m_steeringCorrection << std::endl;
    
    if ( m_brakeCorrection < 0 ) {
      opendlv::proxy::Actuation actuation(m_brakeCorrection, m_steeringCorrection, true);
      odcore::data::Container actuationContainer(actuation);
      getConference().send(actuationContainer);
      std::cout << "Steering Correction 1: " << m_steeringCorrection << std::endl;
    }

    else {
      opendlv::proxy::Actuation actuation(m_accelerationCorrection, m_steeringCorrection, true);
      odcore::data::Container actuationContainer(actuation);
      getConference().send(actuationContainer);
      std::cout << "Steering Correction 2: " << m_steeringCorrection << std::endl;
    }    
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
* Function to ensure that the first element of
* the time vector is less than deltaTime seconds
* ago. Truncates the vector if it is.
*/
void Act::timeCheck(std::vector<odcore::data::TimeStamp> &timeVector, std::vector<float> &amplitudeVector)
{

  if ( timeVector.size() == 0 ){
    return;
  }
  double deltaTime = 1000000 * 0.5f;
  odcore::data::TimeStamp nowTimeStamp;
  double now = nowTimeStamp.toMicroseconds();
  double firstTime = timeVector.at(0).toMicroseconds();
  if ( now - firstTime >= deltaTime ) {
    for ( uint32_t count = 0; count < timeVector.size() - 1; count++ ) {
      timeVector.at(count) = timeVector.at(count + 1);
      amplitudeVector.at(count) = amplitudeVector.at(count + 1);
    }
    timeVector.erase(timeVector.begin() + timeVector.size() - 1);
    amplitudeVector.erase(amplitudeVector.begin() + amplitudeVector.size() - 1);
  }
}

/**
* Checks if an inhibitory signal is recieved
* and if so clears the relevant time and m_amplitude
* vectors prior to adding new corrections.
*/
void Act::inhibitoryCheck(bool a_isInhibitory, std::vector<odcore::data::TimeStamp> &timeVector, std::vector<float> &amplitudeVector)
{
  if ( a_isInhibitory ) {
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
