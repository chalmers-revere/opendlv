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

#include "removeloomingfront/removeloomingfront.hpp"

 namespace opendlv {
  namespace action {
    namespace removeloomingfront {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
  RemoveLoomingFront::RemoveLoomingFront(int32_t const &a_argc, char **a_argv)
  : DataTriggeredConferenceClientModule(
    a_argc, a_argv, "action-removeloomingfront")
  {
  }

  RemoveLoomingFront::~RemoveLoomingFront()
  {
  }

/**
 * Receives looming front with related angle and size
 * (from Perception or Sensation).
 * Sends halt command (brake) or in rare cases a world rotation (steer) command
 * to Act.
 */
 void RemoveLoomingFront::nextContainer(odcore::data::Container &c)
 {
   if(c.getDataType() == opendlv::perception::Object::ID()){
    opendlv::perception::Object m_object = c.getData<opendlv::perception::Object>();
    float m_size = m_object.getSize();

    std::cout<< "Object Size: " << m_size << std::endl<<std::endl;

    if (!(m_size > 0.3f)) {
      odcore::data::TimeStamp t0;
      opendlv::action::Correction correction(t0, "brake", false, 0);
      odcore::data::Container container(correction);
      getConference().send(container);
      
    }
    else {
      double m_brakeAmplitude = -1.0;
      odcore::data::TimeStamp t1;
      opendlv::action::Correction correction(t1, "brake", false, m_brakeAmplitude);
      odcore::data::Container container(correction);
      getConference().send(container);
      std::cout<< "Braking!: " << m_size << std::endl<<std::endl;
    }
  }
}

void RemoveLoomingFront::setUp()
{
}

void RemoveLoomingFront::tearDown()
{
}

} // removeloomingfront
} // action
} // opendlv
