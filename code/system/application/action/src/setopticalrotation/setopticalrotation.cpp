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
 void SetOpticalRotation::nextContainer(odcore::data::Container &c)
 {
  if(c.getDataType() == opendlv::perception::LanePosition::ID()){
    opendlv::perception::LanePosition lanePosition = c.getData<opendlv::perception::LanePosition>();
    //float offset = lanePosition.getOffset();
    float heading = lanePosition.getHeading();

    odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
    float const gainHeading = kv.getValue<float>("action-setopticalrotation.gain_heading");  
    
    if ( fabs(heading) > 0.0f ) {
      float steeringAmplitude = gainHeading * heading;
      odcore::data::TimeStamp t0;
      std::cout << "Stearing Amplitude : " << steeringAmplitude << std::endl;
      m_logRotation << steeringAmplitude << std::endl;
      
      if ( heading < 0 ) {
        opendlv::action::Correction correction(t0, "steering", false, steeringAmplitude);
        odcore::data::Container container(correction);
        getConference().send(container);
      }
      else {
      	opendlv::action::Correction correction(t0, "steering", false, steeringAmplitude);
      	odcore::data::Container container(correction);
      	getConference().send(container);
      }
    } 
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
