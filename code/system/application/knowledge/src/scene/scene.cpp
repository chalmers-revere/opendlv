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
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "knowledge-scene")
    , m_savedSurfaces()
    , m_savedObjects()
    , m_objectCounter(0)
    , m_surfaceCounter(0)
{
}

Scene::~Scene()
{
}

/**
 * Receives .
 * Sends .
 */

 odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Scene::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    SendStuff();

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

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
    opendlv::perception::Surface unpackedSurface =
    a_container.getData<opendlv::perception::Surface>();
    std::vector<opendlv::model::Cartesian3> corners = unpackedSurface.getListOfEdges();

    TimeCheck();

    bool exists = false;
    opendlv::model::Cartesian3 cross = CrossingPoint(corners);

    for(uint32_t i = 0; i < m_savedSurfaces.size() && !exists; i++) {
      if(IsInRectangle(cross, m_savedSurfaces[i].getListOfEdges())) {
        MergeSurfaces(unpackedSurface,i);

        exists = true;
      }

    }
    if(!exists) {
      //Create new
      unpackedSurface.setSurfaceId(m_surfaceCounter);
      m_surfaceCounter++;
      m_savedSurfaces.push_back(unpackedSurface);
    }

  }
}

void Scene::MergeSurfaces(opendlv::perception::Surface a_newSurface, uint32_t a_index)
{
  m_savedSurfaces[a_index].setIdentified(a_newSurface.getIdentified());

  float scale = a_newSurface.getEdgesConfidence()/(a_newSurface.getEdgesConfidence()+m_savedSurfaces[a_index].getEdgesConfidence());
  std::vector<opendlv::model::Cartesian3> mergedPoints = MergePoints(a_newSurface.getListOfEdges(), m_savedSurfaces[a_index].getListOfEdges(), scale);
  m_savedSurfaces[a_index].setEdgesConfidence((m_savedSurfaces[a_index].getEdgesConfidence() + a_newSurface.getEdgesConfidence())/2.0f);
  
  if(!m_savedSurfaces[a_index].getTraversable() || !a_newSurface.getTraversable()) {
    m_savedSurfaces[a_index].setTraversable(false);
  }

  if(a_newSurface.getConfidence() > m_savedSurfaces[a_index].getConfidence()) {
    m_savedSurfaces[a_index].setConfidence(a_newSurface.getConfidence());
  }
/*
  auto sourceSearch = std::find(std::begin(m_savedSurfaces[a_index].getListOfSources()), std::end(m_savedSurfaces[a_index].getListOfSources()), a_newSurface.getListOfSources()[0]);
  if (sourceSearch == std::end(m_savedSurfaces[a_index].getListOfSources())) {
    m_savedSurfaces[a_index].getListOfSources().push_back(a_newSurface.getListOfSources()[0]);
    m_savedSurfaces[a_index].setConfidence(m_savedSurfaces[a_index].getConfidence() + (a_newSurface.getConfidence() / 2)); 
  }
  */
  for (uint32_t i = 0; i < a_newSurface.getListOfProperties().size(); i++) {
    auto propertySearch = std::find(std::begin(m_savedSurfaces[a_index].getListOfProperties()), std::end(m_savedSurfaces[a_index].getListOfProperties()), a_newSurface.getListOfProperties()[i]);
    if (propertySearch == std::end(m_savedSurfaces[a_index].getListOfProperties())) {
      m_savedSurfaces[a_index].getListOfProperties().push_back(a_newSurface.getListOfProperties()[i]);
    }
  }

  //list<int16> connectedWith [id = 11];
  //list<int16> traversableTo [id = 12];
}

std::vector<opendlv::model::Cartesian3> Scene::MergePoints(std::vector<opendlv::model::Cartesian3> points1, std::vector<opendlv::model::Cartesian3> points2, float scale)
{
  std::vector<opendlv::model::Cartesian3> newPoints;
  for(uint32_t i = 0; i < 4; i++) {
    opendlv::model::Cartesian3 point;
    point.setX(points1[i].getX() * scale + points2[i].getX() * (scale - 1) );
    point.setY(points1[i].getY() * scale + points2[i].getY() * (scale - 1) );
    newPoints.push_back(point);
  }
  return newPoints;
}

opendlv::model::Cartesian3 Scene::CrossingPoint(std::vector<opendlv::model::Cartesian3> linePoints)
{
  if(linePoints.size() == 4) {
    float k1 = (linePoints[0].getY() - linePoints[2].getY())/(linePoints[0].getX() - linePoints[2].getX());
    float k2 = (linePoints[1].getY() - linePoints[3].getY())/(linePoints[1].getX() - linePoints[3].getX());
    float m1 = linePoints[0].getY() - k1 * linePoints[0].getX();
    float m2 = linePoints[1].getY() - k2 * linePoints[1].getX();

    if(std::abs(k1-k2) < 0.01f) {
      float x = (m2-m1)/(k1-k2);
      float y = k1 * x + m1;
      return opendlv::model::Cartesian3(x,y,0.0f);
    }
  }
  return opendlv::model::Cartesian3(-1.0f,-1.0f,-1.0f);
}

bool Scene::IsInRectangle(opendlv::model::Cartesian3 point, std::vector<opendlv::model::Cartesian3> corners)
{
  float x = point.getX();
  float y = point.getY();
  float xMin = x;
  float xMax = x;
  float yMin = y;
  float yMax = y;

  for(uint32_t i = 0; i < 4; i++) {
    float xTemp = corners[i].getX();
    float yTemp = corners[i].getY();
    if(xTemp < xMin) {
      xMin = xTemp;
    }
    if(xTemp > xMax) {
      xMax = xTemp;
    }
    if(yTemp < yMin) {
      yMin = yTemp;
    }
    if(yTemp > yMax) {
      yMax = yTemp;
    }
  }
  if(fabs(x - xMax) < 0.001f || fabs(x - xMin) < 0.001f || fabs(y - yMin) < 0.001f || fabs(y - yMax) < 0.001f) {
    return false;
  }
  return true;
}

void Scene::SendStuff()
{
  odcore::data::TimeStamp validUntil;
  opendlv::perception::Environment environment(validUntil, m_savedObjects);

  odcore::data::Container objectContainerEnvironment(environment);
  getConference().send(objectContainerEnvironment);

  for(uint32_t i = 0; i < m_savedObjects.size(); i++) {
    odcore::data::Container objectContainer(m_savedObjects[i]);
    getConference().send(objectContainer);
  }
  for(uint32_t i = 0; i < m_savedSurfaces.size(); i++) {
    odcore::data::Container surfaceContainer(m_savedSurfaces[i]);
    getConference().send(surfaceContainer);
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
  m_savedObjects[a_index].setIdentified(a_object.getIdentified()); //Sets the saved objects timestamp to the new timestamp


  m_savedObjects[a_index].setDirection(a_object.getDirection());
  m_savedObjects[a_index].setDirectionConfidence(a_object.getDirectionConfidence());
  m_savedObjects[a_index].setAngularSize(a_object.getAngularSize());
  m_savedObjects[a_index].setAngularSizeConfidence(a_object.getAngularSizeConfidence());
  m_savedObjects[a_index].setDistance(a_object.getDistance());
  m_savedObjects[a_index].setDistanceConfidence(a_object.getDistanceConfidence());
  m_savedObjects[a_index].setType(a_object.getType());
  m_savedObjects[a_index].setTypeConfidence(a_object.getTypeConfidence());
  m_savedObjects[a_index].setDirectionRate(a_object.getDirectionRate());
  m_savedObjects[a_index].setDirectionRateConfidence(a_object.getDirectionRateConfidence());
  m_savedObjects[a_index].setAngularSizeRate(a_object.getAngularSizeRate());
  m_savedObjects[a_index].setAngularSizeRateConfidence(a_object.getAngularSizeRateConfidence());

  if(m_savedObjects[a_index].getListOfSources().size() > 0) {
    auto sourceSearch = std::find(std::begin(m_savedObjects[a_index].getListOfSources()), std::end(m_savedObjects[a_index].getListOfSources()), a_object.getListOfSources()[0]);
    if (sourceSearch == std::end(m_savedObjects[a_index].getListOfSources())) {
      m_savedObjects[a_index].getListOfSources().push_back(a_object.getListOfSources()[0]);
      m_savedObjects[a_index].setConfidence(m_savedObjects[a_index].getConfidence() + (a_object.getConfidence() / 2));
    }
  }
  
  for (uint32_t i = 0; i < a_object.getListOfProperties().size(); i++) {
    auto propertySearch = std::find(std::begin(m_savedObjects[a_index].getListOfProperties()), std::end(m_savedObjects[a_index].getListOfProperties()), a_object.getListOfProperties()[i]);
    if (propertySearch == std::end(m_savedObjects[a_index].getListOfProperties())) {
      m_savedObjects[a_index].getListOfProperties().push_back(a_object.getListOfProperties()[i]);
    }
  }
}











/*

  
  if (m_savedObjects[a_index].getTypeConfidence() < a_object.getTypeConfidence()) {
    m_savedObjects[a_index].setType(a_object.getType());
    m_savedObjects[a_index].setTypeConfidence(a_object.getTypeConfidence());
  }

  float confidenceModifierDirection = 1.0f;

  if (std::fabs(m_savedObjects[a_index].getDirection().getAzimuth() - a_object.getDirection().getAzimuth()) < 0.07f) {
    confidenceModifierDirection = 1.25f;
  }
  m_savedObjects[a_index].setDirection(a_object.getDirection());


  confidenceDirection = confidenceDirection > 1 ? 1 : confidenceDirection;
  m_savedObjects[a_index].setDirectionConfidence(confidenceDirection); 





  if (m_savedObjects[a_index].getDirectionConfidence() <= a_object.getDirectionConfidence()) { //Borde inte confidence höjas oavsett? /MS
    float confidenceDirection = confidenceModifierDirection * a_object.getDirectionConfidence();
    
    
  }

  if (m_savedObjects[a_index].getDirectionRateConfidence() < a_object.getDirectionRateConfidence()) { //Skulle vi inte alltid ta nya? /MS
    m_savedObjects[a_index].setDirectionRate(a_object.getDirectionRate());
    m_savedObjects[a_index].setDirectionRateConfidence(a_object.getDirectionRateConfidence());
  }


  float confidenceModifierDistance = 1.0f;
  if (std::fabs(m_savedObjects[a_index].getDistance() - a_object.getDistance()) < 0.5f) {
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
*/

void Scene::TimeCheck()
{
  odcore::data::TimeStamp nowTimeStamp;
  for (uint32_t i = 0; i < m_savedObjects.size(); i++) {
    double objectTimeStamp = (nowTimeStamp - m_savedObjects[i].getIdentified()).toMicroseconds() / 1000000.0;
    //std::cout << "Timestamp" << m_savedObjects[i].getIdentified().toMicroseconds() / 1000000.0 << std::endl;
    //std::cout << "Debug time: " << objectTimeStamp << std::endl;
    if (objectTimeStamp > 1.0) { //TODO: Change to config parameter
     m_savedObjects.erase(m_savedObjects.begin() + i);
     std::cout << "Removed object" << std::endl;
     i--;
    }
  }

  for (uint32_t i = 0; i < m_savedSurfaces.size(); i++) {
    double surfaceTimeStamp = (nowTimeStamp - m_savedSurfaces[i].getIdentified()).toMicroseconds() / 1000000.0;
    if (surfaceTimeStamp > 1) { //TODO: Change to config parameter
     m_savedSurfaces.erase(m_savedSurfaces.begin() + i);
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

