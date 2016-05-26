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

#include "setopticalrotation/setopticalrotation.hpp"

 namespace opendlv {
  namespace action {
    namespace setopticalrotation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
  SetOpticalRotation::SetOpticalRotation(int32_t const &a_argc, char **a_argv)
  : DataTriggeredConferenceClientModule(
    a_argc, a_argv, "action-setopticalrotation"),
    m_desiredAzimuth(0.0f),
    m_currentAzimuth(0.0f),
    m_logRotation()
  {
    odcore::data::TimeStamp now;
    std::string filename("Rotation" + now.getYYYYMMDD_HHMMSS() + ".log");
    m_logRotation.open(filename, std::ios::out|std::ios::app);
  }

  SetOpticalRotation::~SetOpticalRotation()
  {
    m_logRotation.close();
  }

/**
 * Receives aim-point angle error.
 * Sends a optical rotation (steer) command to Act.
 */
 void SetOpticalRotation::nextContainer(odcore::data::Container &a_container)
 {
  if (a_container.getDataType() == opendlv::sensation::DesiredDirectionOfMovement::ID()) {
    opendlv::sensation::DesiredDirectionOfMovement desiredMovement = a_container.getData<opendlv::sensation::DesiredDirectionOfMovement>();
    opendlv::model::Direction desiredDirection = desiredMovement.getDirection();
    m_desiredAzimuth = desiredDirection.getAzimuth();
  } else if (a_container.getDataType() == opendlv::sensation::DirectionOfMovement::ID()) {
    opendlv::sensation::DirectionOfMovement directionMovement = a_container.getData<opendlv::sensation::DirectionOfMovement>();
    opendlv::model::Direction direction = directionMovement.getDirection();
    m_currentAzimuth = direction.getAzimuth();
  }

  odcore::base::KeyValueConfiguration kv1 = getKeyValueConfiguration();
  float const gainAzimuth = kv1.getValue<float>("action-setopticalrotation.gain_heading");
  odcore::base::KeyValueConfiguration kv2 = getKeyValueConfiguration();
  float const  azimuthTolerance = kv2.getValue<float>("action-setopticalrotation.heading_tolerance");
  float azimuthCorrection = m_desiredAzimuth - m_currentAzimuth; 

  if ( std:abs(azimuthCorrection) > azimuthTolerance ) {
    float steeringAmplitude = gainAzimuth * azimuthCorrection;
    //std::cout << "Stearing Amplitude: " << steeringAmplitude << std::endl;
    m_logRotation << steeringAmplitude << std::endl;
    odcore::data::TimeStamp t0;
    opendlv::action::Correction correction(t0, "steering", false, steeringAmplitude);
    odcore::data::Container container(correction);
    getConference().send(container);
  }
}

void SetOpticalRotation::setUp()
{
}

void SetOpticalRotation::tearDown()
{
}

} // setopticalrotation
} // action
} // opendlv
