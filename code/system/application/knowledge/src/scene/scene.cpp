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
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "knowledge-scene")
    , savedSurfaces()
    , m_savedObjects()
    , m_objectCounter(0)
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

    int16_t identity = unpackedObject.getObjectId();

    if (identity == -1) {
    	   	
  		//odcore::data::TimeStamp m_timeStamp = unpackedObject.getIdentified();
  		std::string type = unpackedObject.getType();
  		//float  typeConfidence = unpackedObject.getTypeConfidence();
  		opendlv::model::Direction direction = unpackedObject.getDirection();
  		float azimuth = direction.getAzimuth();
  		//float directionConfidence = unpackedObject.getDirectionConfidence();
  		//opendlv::model::Direction directionRate = unpackedObject.getDirectionRate();
  		//float directionRateAzimuth = directionRate.getAzimuth();
  		//float directionRateConfidence = unpackedObject.getDirectionRateConfidence();
  		float distance = unpackedObject.getDistance();
  		//float distanceConfidence = unpackedObject.getDistanceConfidence();
  		//float m_angularSize = unpackedObject.getAngularSize();
  		//float m_angularSizeConfidence = unpackedObject.getAngularSizeConfidence();
  		//float m_angularSizeRate = unpackedObject.getAngularSizeRate();
  		//float m_angularSizeRateConfidence = unpackedObject.getAngularSizeRateConfidence();
  		//float m_confidence = unpackedObject.getConfidence();
  		//std::vector<std::string> m_sources = unpackedObject.getListOfSources();
  		std::vector<std::string> properties = unpackedObject.getListOfProperties();

      std::cout << "ID: " << identity << std::endl;
      std::cout << "Type: " << type << std::endl;
      std::cout << "Distance: " << distance << std::endl;
      std::cout << "Angle: " << azimuth << std::endl << std::endl;


      TimeCheck();
      bool objectExists =false;
      for (uint32_t i = 0; i < m_savedObjects.size(); i++) {
        //std::cout << "Debug0: " << std::endl;

        if (properties.size() > 0 && m_savedObjects[i].getListOfProperties().size() > 0) {
        
          std::string stationId = m_savedObjects[i].getListOfProperties()[0];
          if (properties[0] == stationId){
            MergeObjects(unpackedObject, i);
           //std::cout << "Debug1: " << std::endl;
            objectExists = true;
            break;
          }
        }    
        double betweenObjects = PointDistance(azimuth, distance, m_savedObjects[i].getDirection().getAzimuth(), m_savedObjects[i].getDistance());
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
        m_savedObjects.push_back(unpackedObject);
      }
      //std::cout << "Debug4: " << std::endl;
      std::cout << "Number of IDs: " << m_savedObjects.size() << std::endl << std::endl;

	 }
  }
  else if(a_container.getDataType() == opendlv::perception::Surface::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Surface>();
  }
}

void Scene::SendObjects()
{
  for(uint32_t i = 0; i < m_savedObjects.size(); i++) {
    odcore::data::Container objectContainer(m_savedObjects[i]);
    getConference().send(objectContainer);
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
  m_savedObjects[a_index].setIdentified(a_object.getIdentified());
  
  /*if (m_savedObjects[a_index].getType().empty()) {
    m_savedObjects[a_index].setType(a_object.getType());
  }*/
  if (m_savedObjects[a_index].getTypeConfidence() < a_object.getTypeConfidence()) {
    m_savedObjects[a_index].setType(a_object.getType());
    m_savedObjects[a_index].setTypeConfidence(a_object.getTypeConfidence());
  }
  float confidenceModifierDirection = 1.0f;
  if (std::abs(m_savedObjects[a_index].getDirection().getAzimuth() - a_object.getDirection().getAzimuth()) < 0.07f) {
    confidenceModifierDirection = 1.25f;
  }
  m_savedObjects[a_index].setDirection(a_object.getDirection());  
  if (m_savedObjects[a_index].getDirectionConfidence() <= a_object.getDirectionConfidence()) { //Borde inte confidence höjas oavsett? /MS
    float confidenceDirection = confidenceModifierDirection * a_object.getDirectionConfidence();
    confidenceDirection = confidenceDirection > 1 ? 1 : confidenceDirection;
    m_savedObjects[a_index].setDirectionConfidence(confidenceDirection);
  }
  if (m_savedObjects[a_index].getDirectionRateConfidence() < a_object.getDirectionRateConfidence()) { //Skulle vi inte alltid ta nya? /MS
    m_savedObjects[a_index].setDirectionRate(a_object.getDirectionRate());
    m_savedObjects[a_index].setDirectionRateConfidence(a_object.getDirectionRateConfidence());
  }
  float confidenceModifierDistance = 1.0f;
  if (std::abs(m_savedObjects[a_index].getDistance() - a_object.getDistance()) < 0.5f) {
    confidenceModifierDistance = 1.25f;
  }
  m_savedObjects[a_index].setDistance(a_object.getDistance()); //Samma här, borde den inte öka oavsett? /MS
  if (m_savedObjects[a_index].getDistanceConfidence() < a_object.getDistanceConfidence()) {
    float confidenceDistance = confidenceModifierDistance * a_object.getDistanceConfidence();
    confidenceDistance = confidenceDistance > 1 ? 1 : confidenceDistance;
    m_savedObjects[a_index].setDistanceConfidence(confidenceDistance);
  }
  if (m_savedObjects[a_index].getAngularSizeConfidence() < a_object.getAngularSizeConfidence()) { //Inte nya alltså? /MS
    m_savedObjects[a_index].setAngularSize(a_object.getAngularSize());
    m_savedObjects[a_index].setAngularSizeConfidence(a_object.getAngularSizeConfidence());
  }
  if (m_savedObjects[a_index].getAngularSizeRateConfidence() < a_object.getAngularSizeRateConfidence()) { //Inte nya? /MS
    m_savedObjects[a_index].setAngularSizeRate(a_object.getAngularSizeRate());
    m_savedObjects[a_index].setAngularSizeRateConfidence(a_object.getAngularSizeRateConfidence());
  }
  auto sourceSearch = std::find(std::begin(m_savedObjects[a_index].getListOfSources()), std::end(m_savedObjects[a_index].getListOfSources()), a_object.getListOfSources()[0]);
  if (sourceSearch == std::end(m_savedObjects[a_index].getListOfSources())) { //Är det inte tvärtom? /MS
    m_savedObjects[a_index].getListOfSources().push_back(a_object.getListOfSources()[0]);
    m_savedObjects[a_index].setConfidence(m_savedObjects[a_index].getConfidence() + (a_object.getConfidence() / 2)); //Kan bli för högt? /MS
  }
  for (uint32_t i = 0; i < a_object.getListOfProperties().size(); i++) {
    auto propertySearch = std::find(std::begin(m_savedObjects[a_index].getListOfProperties()), std::end(m_savedObjects[a_index].getListOfProperties()), a_object.getListOfProperties()[i]);
    if (propertySearch == std::end(m_savedObjects[a_index].getListOfProperties())) {
      m_savedObjects[a_index].getListOfProperties().push_back(a_object.getListOfProperties()[i]);
    }
  }
}

void Scene::TimeCheck()
{
  odcore::data::TimeStamp nowTimeStamp;
  for (uint32_t i = 0; i < m_savedObjects.size(); i++) {
    double objectTimeStamp = (nowTimeStamp - m_savedObjects[i].getIdentified()).toMicroseconds() / 1000000.0;
    //std::cout << "Timestamp" << m_savedObjects[i].getIdentified().toMicroseconds() / 1000000.0 << std::endl;
    //std::cout << "Debug time: " << objectTimeStamp << std::endl;
    if (objectTimeStamp > 1) { //TODO: Change to config parameter
     m_savedObjects.erase(m_savedObjects.begin() + i);
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

