
/**
 * Copyright (C) 2017 Chalmers Revere
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


#include "scene.hpp"

namespace opendlv {
namespace logic {
namespace knowledge {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Scene::Scene(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-knowledge-scene")
    , m_initialised(false)
    , m_savedSurfaces()
    , m_savedObjects()
    , m_objectCounter(0)
    , m_surfaceCounter(0)
    , m_mutex()
    , m_mergeDistance()
    , m_validUntilDuration()
    , m_memoryCapacity()
    , m_debug()
{
}

Scene::~Scene()
{
}

void Scene::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_mergeDistance = kv.getValue<float>("logic-knowledge-scene.mergeDistance");
  m_validUntilDuration = kv.getValue<int32_t>("logic-knowledge-scene.validUntilDuration");
  m_memoryCapacity = kv.getValue<float>("logic-knowledge-scene.memoryCapacity");
  m_debug = (kv.getValue<int32_t>("logic-knowledge-scene.debug") == 1);
  m_initialised = true;
}

void Scene::tearDown()
{
}
/**
 * Receives objects from low level sensors.
 * Sends environment to high level applications.
 */

 odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Scene::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    TimeCheck();
    SendStuff();

  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Scene::nextContainer(odcore::data::Container &a_container)
{
  if(!m_initialised){
    return;
  }
  if(a_container.getDataType() == opendlv::perception::Object::ID()) {
    opendlv::perception::Object unpackedObject =
    a_container.getData<opendlv::perception::Object>();

    int16_t identity = unpackedObject.getObjectId();

    if (identity == -1) {
          
      std::string type = unpackedObject.getType();
      opendlv::model::Direction direction = unpackedObject.getDirection();
      float azimuth = direction.getAzimuth();
      float distance = unpackedObject.getDistance();
      std::vector<std::string> properties = unpackedObject.getListOfProperties();
      bool objectExists = false;
      for (uint32_t i = 0; i < m_savedObjects.size(); i++) {
        if (properties.size() > 0 && m_savedObjects[i].getListOfProperties().size() > 0) {
          std::string stationId = m_savedObjects[i].getListOfProperties()[0];
          if (properties[0] == stationId){
            MergeObjects(unpackedObject, i);
            objectExists = true;
            break;
          }
        } 

        double betweenObjects = PointDistance(azimuth, distance, m_savedObjects[i].getDirection().getAzimuth(), m_savedObjects[i].getDistance());
        if (betweenObjects < 2.0) {
          MergeObjects(unpackedObject, i);
          objectExists = true;
          break;
        }
      }

      if (!objectExists) {
        unpackedObject.setObjectId(m_objectCounter);//TODO: Modulus to Object ID
        m_objectCounter++;
        m_savedObjects.push_back(unpackedObject);
      }
   }
  } else if(a_container.getDataType() == opendlv::perception::Surface::ID()) {
    opendlv::perception::Surface unpackedSurface =
        a_container.getData<opendlv::perception::Surface>();
    std::vector<opendlv::model::Cartesian3> corners = unpackedSurface.getListOfEdges();

    bool exists = false;
    opendlv::model::Cartesian3 cross = CrossingPoint(corners);

    for(uint32_t i = 0; i < m_savedSurfaces.size() && !exists; i++) {
      if(IsInRectangle(cross, m_savedSurfaces[i].getListOfEdges())) {
        MergeSurfaces(unpackedSurface,i);
        exists = true;
        // std::cout << i << std::endl;
      }

    }
    if(!exists) {
      //Create new
      // std::cout << "Created new surface." << std::endl;
      unpackedSurface.setSurfaceId(m_surfaceCounter);
      m_surfaceCounter++;
      m_savedSurfaces.push_back(unpackedSurface);
    }

  }
}


std::vector<opendlv::model::Cartesian3> Scene::MergePoints(std::vector<opendlv::model::Cartesian3> points1, std::vector<opendlv::model::Cartesian3> points2, float scale)
{
  std::vector<opendlv::model::Cartesian3> newPoints;
  for(uint32_t i = 0; i < 4; i++) {
    opendlv::model::Cartesian3 point;
    point.setX(points1[i].getX() * scale + points2[i].getX() * (1-scale) );
    point.setY(points1[i].getY() * scale + points2[i].getY() * (1-scale) );
    point.setZ(points1[i].getZ() * scale + points2[i].getZ() * (1-scale) );
    // std::cout << point.toString() << std::endl;
    newPoints.push_back(point);
  }
  return newPoints;
}

opendlv::model::Cartesian3 Scene::CrossingPoint(std::vector<opendlv::model::Cartesian3> linePoints)
{
  if(linePoints.size() == 4) {
    float sum[] = {0,0,0};
    for(uint8_t i = 0; i < 4; i++) {
      sum[0] +=linePoints[i].getX();
      sum[1] +=linePoints[i].getY();
      sum[2] +=linePoints[i].getZ();
    }

    // float k1 = (linePoints[0].getY() - linePoints[2].getY())/(linePoints[0].getX() - linePoints[2].getX());
    // float k2 = (linePoints[1].getY() - linePoints[3].getY())/(linePoints[1].getX() - linePoints[3].getX());
    // float m1 = linePoints[0].getY() - k1 * linePoints[0].getX();
    // float m2 = linePoints[1].getY() - k2 * linePoints[1].getX();

    // if(std::abs(k1-k2) < 0.01f) {
      // float x = (m2-m1)/(k1-k2);
      // float y = k1 * x + m1;
      // return opendlv::model::Cartesian3(x,y,0.0f);
    // }
    return opendlv::model::Cartesian3(sum[0]/4,sum[1]/4,sum[2]/4);
  }
  return opendlv::model::Cartesian3(-1.0f,-1.0f,-1.0f);
}

bool Scene::IsInRectangle(opendlv::model::Cartesian3 point, std::vector<opendlv::model::Cartesian3> corners)
{
  float x = point.getX();
  float y = point.getY();

  float xMin = corners[1].getX();
  float xMax = corners[0].getX();
  float yMin = corners[2].getY();
  float yMax = corners[1].getY();
  // std::cout << "x: " << x << " y: " << y << std::endl;
  // std::cout << "xMin: " << xMin << " xMax: " << xMax << " yMin: " << yMin << " yMax: " << yMax << std::endl;  
  if(x > xMin && x < xMax && y > yMin && y < yMax) {
    return true;
  } else {
    return false;
  }
  // for(uint32_t i = 0; i < 4; i++) {
  //   float xTemp = corners[i].getX();
  //   float yTemp = corners[i].getY();
  //   if(xTemp < xMin) {
  //     xMin = xTemp;
  //   }
  //   if(xTemp > xMax) {
  //     xMax = xTemp;
  //   }
  //   if(yTemp < yMin) {
  //     yMin = yTemp;
  //   }
  //   if(yTemp > yMax) {
  //     yMax = yTemp;
  //   }
  // }
  // if(fabs(x - xMax) < 0.001f || fabs(x - xMin) < 0.001f || fabs(y - yMin) < 0.001f || fabs(y - yMax) < 0.001f) {
  //   return false;
  // }
  // return true;
}

void Scene::SendStuff()
{
  odcore::data::TimeStamp now;
  odcore::data::TimeStamp validUntil = now + odcore::data::TimeStamp(m_validUntilDuration, 0);
  opendlv::perception::Environment environment(validUntil, m_savedObjects, m_savedSurfaces);

  odcore::data::Container objectContainerEnvironment(environment);
  getConference().send(objectContainerEnvironment);


  if(m_debug){
    std::cout << "=====================================" << std::endl;
    std::cout << "Objects sent: " << std::endl;

    for(uint32_t i = 0; i < m_savedObjects.size(); i++) {
      std::cout << "ID: "<< m_savedObjects[i].getObjectId() << std::endl;
      std::cout << "Angle: "<< m_savedObjects[i].getDirection().getAzimuth() << std::endl;
      std::cout << "Distance: "<< m_savedObjects[i].getDistance() << std::endl;
    }
    std::cout << "Surfaces sent: " << std::endl;

    for(uint32_t i = 0; i < m_savedSurfaces.size(); i++) {
      std::cout << "ID: "<< m_savedSurfaces[i].getSurfaceId() << std::endl;
      opendlv::model::Cartesian3 cross = CrossingPoint(m_savedSurfaces[i].getListOfEdges());
      std::cout << "CrossingPoint: "<< cross.toString() << std::endl;
      std::cout << "Sources: ";
      std::vector<std::string> listSources =  m_savedSurfaces[i].getListOfSources();
      for(uint8_t j = 0; j < listSources.size(); j++) {
        std::cout << listSources[j] << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << "=====================================" << std::endl;
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
  if (a_index >= m_savedObjects.size()) {
    // the index is not valid...
    std::cout << "ERROR: index sent into MergeObjects() was not valid: " << a_index << std::endl;
    return;
  }

  // if both objects have station IDs, and the station IDs are different, should NOT MERGE!!!
  std::vector<std::string> props1 = a_object.getListOfProperties();
  std::vector<std::string> props2 = m_savedObjects[a_index].getListOfProperties();
  if (props1.size() > 0 && props2.size() > 0) {
    // both have properties

    // TODO we assume that station ID is always the first property....
    if (props1.at(0) != props2.at(0)) {
      // So, both vehicles had a station ID, and they were not the same.
      // We should therefore NOT merge the objects...
      return;
    }
  }

  m_savedObjects[a_index].setIdentified(a_object.getIdentified()); //Sets the saved objects timestamp to the new timestamp

  // opendlv::model::Direction newDirection((m_savedObjects[a_index].getDirection().getAzimuth() + a_object.getDirection().getAzimuth())/2,(m_savedObjects[a_index].getDirection().getZenith() + a_object.getDirection().getZenith())/2);
  m_savedObjects[a_index].setDirection(a_object.getDirection());
  m_savedObjects[a_index].setDirectionConfidence(a_object.getDirectionConfidence());
  m_savedObjects[a_index].setAngularSize(a_object.getAngularSize());
  m_savedObjects[a_index].setAngularSizeConfidence(a_object.getAngularSizeConfidence());
  m_savedObjects[a_index].setDistance( a_object.getDistance());
  m_savedObjects[a_index].setDistanceConfidence((m_savedObjects[a_index].getDistanceConfidence() + a_object.getDistanceConfidence())/2);
  m_savedObjects[a_index].setType(a_object.getType());
  m_savedObjects[a_index].setTypeConfidence(a_object.getTypeConfidence());
  m_savedObjects[a_index].setDirectionRate(a_object.getDirectionRate());
  m_savedObjects[a_index].setDirectionRateConfidence(a_object.getDirectionRateConfidence());
  m_savedObjects[a_index].setAngularSizeRate(a_object.getAngularSizeRate());
  m_savedObjects[a_index].setAngularSizeRateConfidence(a_object.getAngularSizeRateConfidence());

  if(m_savedObjects[a_index].getListOfSources().size() > 0) {
    if (!m_savedObjects[a_index].contains_ListOfSources(a_object.getListOfSources()[0])) {
      m_savedObjects[a_index].addTo_ListOfSources(a_object.getListOfSources()[0]);
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
void Scene::MergeSurfaces(opendlv::perception::Surface a_newSurface, uint32_t a_index)
{
  // std::cout << "merging!!!" << a_index << std::endl;
  m_savedSurfaces[a_index].setIdentified(a_newSurface.getIdentified());

  // float scale = a_newSurface.getEdgesConfidence()/(a_newSurface.getEdgesConfidence()+m_savedSurfaces[a_index].getEdgesConfidence());
  const float scale = 0.1;
  std::vector<opendlv::model::Cartesian3> mergedPoints = MergePoints(a_newSurface.getListOfEdges(), m_savedSurfaces[a_index].getListOfEdges(), scale);
  m_savedSurfaces[a_index].setListOfEdges(mergedPoints);
  m_savedSurfaces[a_index].setEdgesConfidence((m_savedSurfaces[a_index].getEdgesConfidence() + a_newSurface.getEdgesConfidence())/2.0f);
  
  if(!m_savedSurfaces[a_index].getTraversable() || !a_newSurface.getTraversable()) {
    m_savedSurfaces[a_index].setTraversable(false);
  }

  if(a_newSurface.getConfidence() > m_savedSurfaces[a_index].getConfidence()) {
    m_savedSurfaces[a_index].setConfidence(a_newSurface.getConfidence());
  }
  for (uint32_t i = 0; i < a_newSurface.getListOfProperties().size(); i++) {
    auto propertySearch = std::find(std::begin(m_savedSurfaces[a_index].getListOfProperties()), std::end(m_savedSurfaces[a_index].getListOfProperties()), a_newSurface.getListOfProperties()[i]);
    if (propertySearch == std::end(m_savedSurfaces[a_index].getListOfProperties())) {
      m_savedSurfaces[a_index].getListOfProperties().push_back(a_newSurface.getListOfProperties()[i]);
    }
  }
}

void Scene::TimeCheck()
{
  odcore::data::TimeStamp nowTimeStamp;
  for (uint32_t i = 0; i < m_savedObjects.size(); i++) {
    float objectTimeStamp = (nowTimeStamp - m_savedObjects[i].getIdentified()).toMicroseconds() / 1000000.0;
    //std::cout << "Timestamp" << m_savedObjects[i].getIdentified().toMicroseconds() / 1000000.0 << std::endl;
    //std::cout << "Debug time: " << objectTimeStamp << std::endl;
    if (objectTimeStamp > m_memoryCapacity) { 
     m_savedObjects.erase(m_savedObjects.begin() + i);
     std::cout << "Removed object" << std::endl;
     i--;
    }
  }

  for (uint32_t i = 0; i < m_savedSurfaces.size(); i++) {
    float surfaceTimeStamp = (nowTimeStamp - m_savedSurfaces[i].getIdentified()).toMicroseconds() / 1000000.0;
    if (surfaceTimeStamp > m_memoryCapacity) { 
     m_savedSurfaces.erase(m_savedSurfaces.begin() + i);
     std::cout << "Removed surface" << std::endl;
     i--;
    }
  }
}

}
}
}
