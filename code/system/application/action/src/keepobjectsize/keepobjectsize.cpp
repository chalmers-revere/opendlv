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



#include "keepobjectsize/keepobjectsize.hpp"

namespace opendlv {
namespace action {
namespace keepobjectsize {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
KeepObjectSize::KeepObjectSize(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "action-keepobjectsize"),
      m_object(),
      m_desiredAzimuth(0.0f),
      m_angularSize(0.0f),
      m_targetSize(0.1f),
      m_targetObject(-2),
      m_crossingScenario(false)
{
}

KeepObjectSize::~KeepObjectSize()
{
}

/**
 * Receives object to keep stationary, including id, current angle,
 * current size, and desired size.
 * Sends speed correction commands (throttle) to Act.
 */
void KeepObjectSize::nextContainer(odcore::data::Container &a_container)
{
  if(a_container.getDataType() == opendlv::perception::ObjectDesiredAngularSize::ID()) {
    opendlv::perception::ObjectDesiredAngularSize desiredAngularSize =
    a_container.getData<opendlv::perception::ObjectDesiredAngularSize>();
    m_targetSize = desiredAngularSize.getDesiredAngularSize();
    if(desiredAngularSize.getObjectId() >= 0) {
      m_targetObject = desiredAngularSize.getObjectId();
    }
  } 

  if(a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t identity = unpackedObject.getObjectId();

    if (identity != -1) {

      opendlv::model::Direction direction = unpackedObject.getDirection();
      float azimuth = direction.getAzimuth();
      float speedCorrection = 0.0f;
      float gainCorrection = 5.0f;
      float tolerance = 0.1f;

      if (m_targetObject >= 0) { //Target objectID is known
        if(unpackedObject.getObjectId() == m_targetObject)
        {
          m_object.reset(new opendlv::perception::Object(unpackedObject));

          float distance = m_object->getDistance(); //Using distance instead of angularsize for GCDC

          if (distance < m_targetSize*(1.0f-tolerance)) {
            std::cout << "Too close: " << m_angularSize << std::endl;
            float distanceRatio = distance / m_targetSize ;
            speedCorrection = 1.0f - distanceRatio;
          } else if (distance > m_targetSize*(1.0f+tolerance) && m_crossingScenario) {
            std::cout << "Too far" << std::endl;
            float distanceRatio = m_targetSize / distance;
            speedCorrection = distanceRatio - 1.0f; 
          } else {
            std::cout << "zero correction" << std::endl;
            speedCorrection = 0.0f;
          }

          speedCorrection *= gainCorrection;
        
          std::cout << "speedCorrection: " << speedCorrection << std::endl << std::endl;
          odcore::data::TimeStamp t0;
          opendlv::action::Correction correction1(t0, "accelerate", false, speedCorrection);
          odcore::data::Container container(correction1);
          getConference().send(container);
        }

      }
      else if (std::fabs(azimuth) < 0.22f) { //If target objectID isn't known
        std::cout << "Correct Angle" << std::endl;
        if (m_object == nullptr) {
          m_object.reset(new opendlv::perception::Object(unpackedObject));
        } else {
          if (unpackedObject.getDistance() < m_object->getDistance())
            m_object.reset(new opendlv::perception::Object(unpackedObject));
            std::cout << "Closest object" << std::endl;
        }    

        float distance = m_object->getDistance();       //Using distance instead of angularsize for GCDC

        if (distance < m_targetSize*(1.0f-tolerance)) {
          std::cout << "Too close: " << m_angularSize << std::endl;
          float distanceRatio = distance / m_targetSize ;
          speedCorrection = 1.0f - distanceRatio;
        } else if (distance > m_targetSize*(1.0f+tolerance) && m_crossingScenario) {
          std::cout << "Too far" << std::endl;
          float distanceRatio = m_targetSize / distance;
          speedCorrection = distanceRatio - 1.0f; 
        } else {
          std::cout << "zero correction" << std::endl;
          speedCorrection = 0.0f;
        }

        /*
        m_angularSize = m_object->getAngularSize();


        if (std::fabs(m_angularSize) < m_targetSize*0.9f) {
          std::cout << "Small enough: " << m_angularSize << std::endl;
          float angularSizeRatio = m_angularSize / m_targetSize ;
          speedCorrection = 1.0f - angularSizeRatio;
        } else if (std::fabs(m_angularSize) > m_targetSize*1.1f) {
          std::cout << "Object too big" << std::endl;
          float angularSizeRatio = m_targetSize / m_angularSize;
          speedCorrection = angularSizeRatio - 1.0f; 
        } else {
          std::cout << "zero correction" << std::endl;
          speedCorrection = 0.0f;
        }
        */

        speedCorrection *= gainCorrection;
        
        std::cout << "speedCorrection: " << speedCorrection << std::endl << std::endl;
        odcore::data::TimeStamp t0;
        opendlv::action::Correction correction1(t0, "accelerate", false, speedCorrection);
        odcore::data::Container container(correction1);
        getConference().send(container);
      }
    }
  }
}















  /*
  if(c.getDataType() == opendlv::perception::LeadVehicleSize::ID()){
    opendlv::perception::LeadVehicleSize leadVehicleSize = 
        c.getData<opendlv::perception::LeadVehicleSize>();
    float size = leadVehicleSize.getSize();
  
    // TODO: Quick way.
    float desiredSize = 100.0f;
    float error = (desiredSize - size) / desiredSize;
 
    if (error < -0.25f) {
      // We are to to close, try to decrease acceleration.
      float amplitude = 0.5f * error;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "accelerate", false, 
          amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
    } else if (error < -0.75f) {
      // We are way to close, start braking!
      float amplitude = 1.5f * error;
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "brake", false, amplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
    }
  }
  */


void KeepObjectSize::setUp()
{
}

void KeepObjectSize::tearDown()
{
}

} // keepobjectsize
} // action
} // opendlv
