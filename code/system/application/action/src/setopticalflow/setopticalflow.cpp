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
    m_desiredOpticalFlow(0.0f)
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
void SetOpticalFlow::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == opendlv::model::DynamicState::ID()) {
      opendlv::model::DynamicState dynamicState = a_container.getData<opendlv::model::DynamicState>();
      opendlv::model::Cartesian3 velocity = dynamicState.getVelocity();
      float speed = std::sqrt((velocity.getX() * velocity.getX()) +(velocity.getY()*velocity.getY()));
      float speedCorrection;
      float speedTolerance = 5.0f / 3.6f;

      if (speed > m_maxSpeed) {
        if (speed < m_maxSpeed + speedTolerance) {
          float speedDiff = speed -m_maxSpeed;
          speedCorrection = -10 * speedDiff/speedTolerance;

        }
        else {
        speedCorrection = -10; //TODO: set parameter somewhere
        }
      
      }
      else {
        speedCorrection = 0;
      }
        
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction1(t0, "acceleration", false, speedCorrection);
      odcore::data::Container container(correction1);
      getConference().send(container);
    



  }





/*  if(c.getDataType() == opendlv::perception::DesiredOpticalFlow::ID()) {
    auto desiredOpticalFlow = 
        c.getData<opendlv::perception::DesiredOpticalFlow>();
    m_desiredOpticalFlow = desiredOpticalFlow.getFlow();
  }
  
  if(c.getDataType() == opendlv::sensation::OpticalFlow::ID()) {
    auto opticalFlow = c.getData<opendlv::sensation::OpticalFlow>();
    float flow = opticalFlow.getFlow();
  
    // TODO: Quick way.
    float error = (m_desiredOpticalFlow - flow) / m_desiredOpticalFlow;
 
 //   if (std::abs(error) > 0.01f) {
      float amplitude = 3.0f;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "accelerate", false, 
          amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
 //   } 
//  }
      */
}

void SetOpticalFlow::setUp()
{
  odcore::base::KeyValueConfiguration kv1 = getKeyValueConfiguration();
  std::string m_maxSpeed = kv1.getValue<std::string>("action-setopticalflow.max_speed");
}

void SetOpticalFlow::tearDown()
{
}

} // setopticalflow
} // action
} // opendlv
