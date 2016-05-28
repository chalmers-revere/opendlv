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

#include "keepobjectalignment/keepobjectalignment.hpp"

namespace opendlv {
namespace action {
namespace keepobjectalignment {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
KeepObjectAlignment::KeepObjectAlignment(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "action-keepobjectalignment")
    , m_targetAzimuth(0.0f)
    , m_targetId(-2)
{
}

KeepObjectAlignment::~KeepObjectAlignment()
{
}

/**
 * Receives object to keep stationary, including id, current angle,
 * current size, and desired angle.
 * Sends speed correction commands (throttle) to Act.
 */
void KeepObjectAlignment::nextContainer(odcore::data::Container &a_container)
{
  if(a_container.getDataType() == opendlv::perception::ObjectDesiredDirection::ID()) {
    opendlv::perception::ObjectDesiredDirection desiredDirection =
    a_container.getData<opendlv::perception::ObjectDesiredDirection>();
    opendlv::model::Direction targetDirection = desiredDirection.getDesiredDirection();
    m_targetAzimuth = targetDirection.getAzimuth();
    m_targetId = desiredDirection.getObjectId();
  } 

  if(a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t identity = unpackedObject.getObjectId();

    if (identity == m_targetId) {

      opendlv::model::Direction direction = unpackedObject.getDirection();
      float azimuth = direction.getAzimuth();
      float angleRateCorrection = 0.0f;
      float gainCorrection = 0.3f;

       

        
        float angularDiff = azimuth-m_targetAzimuth;
        if ((angularDiff) < -0.05f) {
          std::cout << "To the right: " << angularDiff << std::endl;
          angleRateCorrection = angularDiff;
        } else if ((angularDiff) > 0.05f) {
          std::cout << "Object to the left:" << angularDiff << std::endl;
          angleRateCorrection = angularDiff; 
        } else {
          std::cout << "zero correction" << std::endl;
        }

        angleRateCorrection *= gainCorrection;
        
        std::cout << "angleRateCorrection: " << angleRateCorrection << std::endl << std::endl;
        odcore::data::TimeStamp t0;
        opendlv::action::Correction correction1(t0, "steering", false, angleRateCorrection);
        odcore::data::Container container(correction1);
        getConference().send(container);
      }
    
  }
  /*
  if(c.getDataType() == opendlv::perception::LanePosition::ID()){
    opendlv::perception::LanePosition lanePosition = 
        c.getData<opendlv::perception::LanePosition>();
    //float offset = lanePosition.getOffset();
    float heading = lanePosition.getHeading();
  
    // TODO: Quick way.
    float error = heading;
 
    if (std::abs(error) > 0.05f) {
      float amplitude = 0.2f * error;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "steering", false, 
          amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
    } 
  }
  */
}

void KeepObjectAlignment::setUp()
{
}

void KeepObjectAlignment::tearDown()
{
}

} // keepobjectalignment
} // action
} // opendlv
