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

  if(a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    m_ID = unpackedObject.getObjectId();

    if (m_ID != -1) {
    	return;    	
    } else {    	
    	m_timeStamp = unpackedObject.getIdentified();
    	m_type = unpackedObject.getDataType();
    	m_direction = unpackedObject.getDirection();
    	m_azimuth = m_direction.getAzimuth();
    	m_directionConfidence = unpackedObject.getDirectionConfidence();
    	m_directionRate = unpackedObject.getDirectionRate();
    	m_directionRateAzimuth = m_directionRate.getAzimuth();
    	m_directionRateConfidence = unpackedObject.getDirectionRateConfidence();
    	m_distance = unpackedObject.getDistance();
    	m_distanceConfidence = unpackedObject.getDistanceConfidence();
    	m_angularSize = unpackedObject.getAngularSize();
    	m_angularSizeConfidence = unpackedObject.getAngularSizeConfidence();
    	m_angularSizeRate = unpackedObject.getAngularSizeRate();
    	m_angularSizeRateConfidence = unpackedObject.getAngularSizeRateConfidence();
    	m_confidence = unpackedObject.getConfidence();
    	m_sources = unpackedObject.getSources();
    	m_properties = unpackedObject.getProperties();
    }
  }




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
