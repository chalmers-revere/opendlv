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

/*
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
*/

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
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "knowledge-scene"),
    savedObjects(),
    m_objectCounter(0)
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

    int16_t m_ID = unpackedObject.getObjectId();

    if (m_ID == -1) {
    	   	
  		//odcore::data::TimeStamp m_timeStamp = unpackedObject.getIdentified();
  		std::string m_type = unpackedObject.getType();
  		//float  m_typeConfidence = unpackedObject.getTypeConfidence();
  		opendlv::model::Direction m_direction = unpackedObject.getDirection();
  		float m_azimuth = m_direction.getAzimuth();
  		//float m_directionConfidence = unpackedObject.getDirectionConfidence();
  		//opendlv::model::Direction m_directionRate = unpackedObject.getDirectionRate();
  		//float m_directionRateAzimuth = m_directionRate.getAzimuth();
  		//float m_directionRateConfidence = unpackedObject.getDirectionRateConfidence();
  		float m_distance = unpackedObject.getDistance();
  		//float m_distanceConfidence = unpackedObject.getDistanceConfidence();
  		//float m_angularSize = unpackedObject.getAngularSize();
  		//float m_angularSizeConfidence = unpackedObject.getAngularSizeConfidence();
  		//float m_angularSizeRate = unpackedObject.getAngularSizeRate();
  		//float m_angularSizeRateConfidence = unpackedObject.getAngularSizeRateConfidence();
  		//float m_confidence = unpackedObject.getConfidence();
  		//std::vector<std::string> m_sources = unpackedObject.getListOfSources();
  		std::vector<std::string> m_properties = unpackedObject.getListOfProperties();

      std::cout << "ID: " << m_ID << std::endl;
      std::cout << "Type: " << m_type << std::endl;
      std::cout << "Distance: " << m_distance << std::endl;
      std::cout << "Angle: " << m_azimuth << std::endl << std::endl;


      TimeCheck();
      bool objectExists =false;
      for (uint32_t i = 0; i < savedObjects.size(); i++) {
        //std::cout << "Debug0: " << std::endl;

        if (m_properties.size() > 0 && savedObjects[i].getListOfProperties().size() > 0) {
        
          std::string stationId = savedObjects[i].getListOfProperties()[0];
          if (m_properties[0] == stationId){
            MergeObjects(unpackedObject, i);
           //std::cout << "Debug1: " << std::endl;
            objectExists = true;
            break;
          }
        }    
        double betweenObjects = PointDistance(m_azimuth, m_distance, savedObjects[i].getDirection().getAzimuth(), savedObjects[i].getDistance());
        if (betweenObjects < 2.0) {
          MergeObjects(unpackedObject, i);
          //std::cout << "Debug2: " << std::endl;
          objectExists = true;
          break;
        }
      }

      if (!objectExists) {
        //std::cout << "Debug3: " << std::endl;
        unpackedObject.setObjectId(m_objectCounter);//TODO: Modulus to Object ID
        m_objectCounter++;
        savedObjects.push_back(unpackedObject);
      }
      //std::cout << "Debug4: " << std::endl;
      std::cout << "Number of IDs: " << savedObjects.size() << std::endl << std::endl;

	 }
  }
}

double Scene::PointDistance(float a_angle1, double a_dist1, float a_angle2, double a_dist2)
{
    double x1 = std::cos(static_cast<double>(a_angle1))*a_dist1;
    double y1 = std::sin(static_cast<double>(a_angle1))*a_dist1;
    double x2 = std::cos(static_cast<double>(a_angle2))*a_dist2;
    double y2 = std::sin(static_cast<double>(a_angle2))*a_dist2;
    double between = sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    return between;
}

void Scene::MergeObjects(opendlv::perception::Object a_object, uint32_t a_index) //TODO: Check angular size to get rate, direction for direction rate
{
  savedObjects[a_index].setIdentified(a_object.getIdentified());
  
  /*if (savedObjects[a_index].getType().empty()) {
    savedObjects[a_index].setType(a_object.getType());
  }*/
  if (savedObjects[a_index].getTypeConfidence() < a_object.getTypeConfidence()) {
    savedObjects[a_index].setType(a_object.getType());
    savedObjects[a_index].setTypeConfidence(a_object.getTypeConfidence());
  }
  float confidenceModifierDirection = 1.0f;
  if (std::abs(savedObjects[a_index].getDirection().getAzimuth() - a_object.getDirection().getAzimuth()) < 0.07f) {
    confidenceModifierDirection = 1.25f;
  }
  savedObjects[a_index].setDirection(a_object.getDirection());  
  if (savedObjects[a_index].getDirectionConfidence() <= a_object.getDirectionConfidence()) {
    float confidenceDirection = confidenceModifierDirection * a_object.getDirectionConfidence();
    confidenceDirection = confidenceDirection > 1 ? 1 : confidenceDirection;
    savedObjects[a_index].setDirectionConfidence(confidenceDirection);
  }
  if (savedObjects[a_index].getDirectionRateConfidence() < a_object.getDirectionConfidence()) {
    savedObjects[a_index].setDirectionRate(a_object.getDirectionRate());
    savedObjects[a_index].setDirectionRateConfidence(a_object.getDirectionRateConfidence());
  }
  float confidenceModifierDistance = 1.0f;
  if (std::abs(savedObjects[a_index].getDistance() - a_object.getDistance()) < 0.5f) {
    confidenceModifierDistance = 1.25f;
  }
  savedObjects[a_index].setDistance(a_object.getDistance());
  if (savedObjects[a_index].getDistanceConfidence() < a_object.getDistanceConfidence()) {
    float confidenceDistance = confidenceModifierDistance * a_object.getDistanceConfidence();
    confidenceDistance = confidenceDistance > 1 ? 1 : confidenceDistance;
    savedObjects[a_index].setDistanceConfidence(confidenceDistance);
  }
  if (savedObjects[a_index].getAngularSizeConfidence() < a_object.getAngularSizeConfidence()) {
    savedObjects[a_index].setAngularSize(a_object.getAngularSize());
    savedObjects[a_index].setAngularSizeConfidence(a_object.getAngularSizeConfidence());
  }
  if (savedObjects[a_index].getAngularSizeRateConfidence() < a_object.getAngularSizeRateConfidence()) {
    savedObjects[a_index].setAngularSizeRate(a_object.getAngularSizeRate());
    savedObjects[a_index].setAngularSizeRateConfidence(a_object.getAngularSizeRateConfidence());
  }
  auto sourceSearch = std::find(std::begin(savedObjects[a_index].getListOfSources()), std::end(savedObjects[a_index].getListOfSources()), a_object.getListOfSources()[0]);
  if (sourceSearch == std::end(savedObjects[a_index].getListOfSources())) {
    savedObjects[a_index].getListOfSources().push_back(a_object.getListOfSources()[0]);
    savedObjects[a_index].setConfidence(savedObjects[a_index].getConfidence() + (a_object.getConfidence() / 2));
  }
  for (uint32_t i = 0; i < a_object.getListOfProperties().size(); i++) {
    auto propertySearch = std::find(std::begin(savedObjects[a_index].getListOfProperties()), std::end(savedObjects[a_index].getListOfProperties()), a_object.getListOfProperties()[i]);
    if (propertySearch == std::end(savedObjects[a_index].getListOfProperties())) {
      savedObjects[a_index].getListOfProperties().push_back(a_object.getListOfProperties()[i]);
    }
  }
}

void Scene::TimeCheck()
{
  odcore::data::TimeStamp nowTimeStamp;
  for (uint32_t i = 0; i < savedObjects.size(); i++) {
    double objectTimeStamp = (nowTimeStamp - savedObjects[i].getIdentified()).toMicroseconds() / 1000000.0;
    //std::cout << "Timestamp" << savedObjects[i].getIdentified().toMicroseconds() / 1000000.0 << std::endl;
    //std::cout << "Debug time: " << objectTimeStamp << std::endl;
    if (objectTimeStamp > 1) { //TODO: Change to config parameter
     savedObjects.erase(savedObjects.begin() + i);
     std::cout << "Removed object" << std::endl;
     i--;
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

