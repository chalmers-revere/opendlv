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

#include "scene/scene.hpp"

namespace opendlv {
namespace knowledge {
namespace scene {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Scene::Scene(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "knowledge-scene")
{
}

Scene::~Scene()
{
}

/**
 * Receives .
 * Sends .
 */
void Scene::nextContainer(odcore::data::Container &a_container)
{

	  if(c.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    m_ID = unpackedObject.getObjectId();

    if (m_ID == -1) {
    	m_timeStamp = unpackedObject.getIdentified();
    	m_type = unpackedObject.getDataType();
    	m_direction = unpackedObject.getDirection();
    	m_azimuth = m_direction.getAzimuth();
    	m_directionConfidence = unpackedObject.getDirectionConfidence();
    	m_

    	
    }

  }












  message opendlv.perception.Object [id = 179] {
  odcore::data::TimeStamp identified [id = 1];
  string type [id = 2];

  opendlv.coordinate.Direction direction [id = 3];
  float directionConfidence [id = 4];
  opendlv.coordinate.Direction directionRate [id = 5];
  float directionRateConfidence [id = 6];

  float distance [id = 7];
  float distanceConfidence [id = 8];

  float angularSize [id = 9];
  float angularSizeConfidence [id = 10];
  float angularSizeRate [id = 11];
  float angularSizeRateConfidence [id = 12];

  list<string> properties [id = 13];

  uint16 objectId [id = 14];
}

void Scene::setUp()
{
}

void Scene::tearDown()
{
}

} // scene
} // knowledge
} // opendlv
