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

  /*
  if(a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t m_ID = unpackedObject.getObjectId();

    if (m_ID != -1) {
    	return;    	
    } else {    	
    	odcore::data::TimeStamp m_timeStamp = unpackedObject.getIdentified();
    	std::string m_type = unpackedObject.getType();
    	float m_typeConfidence = unpackedObject.getTypeConfidence();
    	opendlv::model::Direction m_direction = unpackedObject.getDirection();
    	float m_azimuth = m_direction.getAzimuth();
    	float m_directionConfidence = unpackedObject.getDirectionConfidence();
    	opendlv::model::Direction m_directionRate = unpackedObject.getDirectionRate();
    	float m_directionRateAzimuth = m_directionRate.getAzimuth();
    	float m_directionRateConfidence = unpackedObject.getDirectionRateConfidence();
    	float m_distance = unpackedObject.getDistance();
    	float m_distanceConfidence = unpackedObject.getDistanceConfidence();
    	float m_angularSize = unpackedObject.getAngularSize();
    	float m_angularSizeConfidence = unpackedObject.getAngularSizeConfidence();
    	float m_angularSizeRate = unpackedObject.getAngularSizeRate();
    	float m_angularSizeRateConfidence = unpackedObject.getAngularSizeRateConfidence();
    	float m_confidence = unpackedObject.getConfidence();
    	uint16_t m_sources = unpackedObject.getSources();
    	std::vector<std::string> m_properties = unpackedObject.getListOfProperties();
    }
  }

*/
(void)a_container;

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
