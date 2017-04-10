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

#include <ctype.h>
#include <cstring>
#include <cmath>

#include <algorithm>
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opendavinci/odcore/data/TimeStamp.h>

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "echolocation.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Echolocation::Echolocation(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "logic-sensation-echolocation")
    , m_listAzimuth()
    , m_listZenith()
    , m_listDistance()
    , m_listTime()
    , m_memoryThreshold()
    , m_pointCloudRadius()
    , m_pointCloudSizeMinimum()
    , m_initialised(false)
{
}

Echolocation::~Echolocation()
{
}

void Echolocation::nextContainer(odcore::data::Container &a_c)
{
  if (!m_initialised || a_c.getDataType() != opendlv::proxy::EchoReading::ID()){
    return;
  }
  odcore::data::TimeStamp now;

  odcore::data::TimeStamp diffStamp = now - m_listTime.back(); //Remove old data
  float diffSeconds = diffStamp.toMicroseconds() / 1000000.0;

  while(m_listTime.size() > 0 && diffSeconds > m_memoryThreshold) {
    m_listAzimuth.pop_back();
    m_listZenith.pop_back();
    m_listDistance.pop_back();
    m_listTime.pop_back();
    if(m_listTime.size() == 0){
      break;;
    }
    diffStamp = now - m_listTime.back();
    diffSeconds = diffStamp.toMicroseconds() / 1000000.0;
  }

  opendlv::proxy::EchoReading reading = a_c.getData<opendlv::proxy::EchoReading>(); //Read new data
  uint32_t nNewPoints = reading.getNumberOfPoints();
  std::vector<opendlv::model::Direction> directions = reading.getListOfDirections();
  std::vector<double> distances = reading.getListOfRadii();
  
  for(uint32_t i = 0; i < nNewPoints; i++) {
    m_listAzimuth.push_front(directions[i].getAzimuth());
    m_listZenith.push_front(directions[i].getZenith());
    m_listDistance.push_front(distances[i]);
    m_listTime.push_front(now);
  }
  
  std::vector<opendlv::perception::Object> identifiedObjects;

  std::vector<uint32_t> usedPoints;
  std::vector<uint32_t> pointCloud;
  
  uint32_t nPoints = m_listAzimuth.size();
  uint32_t objectCounter = 0;

  for(uint32_t k = 0; k < nPoints; k++) {
    pointCloud.clear();
    pointCloud.push_back(k);

    std::vector<uint32_t>::iterator it = std::find(usedPoints.begin(),usedPoints.end(),k);
    // if(!Contains(k, usedPoints)) {
    // Cloud grouping
    // if k is not in used points
    if(it == usedPoints.end()) {
      for(uint32_t i = 0; i < pointCloud.size(); i++) {
        for(uint32_t j = 0; j < nPoints; j++) {
          uint32_t x = pointCloud[i];
          double dist = PointDistance(m_listAzimuth[x], m_listDistance[x], m_listAzimuth[j], m_listDistance[j]);
          it = std::find(pointCloud.begin(),pointCloud.end(),j);
          //if j is within radius and not in pointcloud
          if(dist < m_pointCloudRadius && it == pointCloud.end()) {
            pointCloud.push_back(j);
            usedPoints.push_back(j);
          }
        }
      }

      if(pointCloud.size() > m_pointCloudSizeMinimum) { 
        double minDist = m_listDistance[pointCloud[0]];
        float minAngle = m_listAzimuth[pointCloud[0]];
        float maxAngle = m_listAzimuth[pointCloud[0]];

        for(uint32_t i = 1; i < pointCloud.size(); i++) {
          if(m_listDistance[pointCloud[i]] < minDist) {
            minDist = m_listDistance[pointCloud[i]];
          }
          if(m_listAzimuth[pointCloud[i]] < minAngle) {
            minAngle = m_listAzimuth[pointCloud[i]];
          }
          if(m_listAzimuth[pointCloud[i]] > maxAngle) {
            maxAngle = m_listAzimuth[pointCloud[i]];
          }
        }

        //std::cout << "Setting object!" << std::endl;
        //Set Object
        //TODO: Dynamic confidence
        float dynConfidence = std::sqrt(static_cast<float>(pointCloud.size()))/3.0f;
        if(dynConfidence > 1) {
          dynConfidence = 1;
        }
        identifiedObjects.push_back(opendlv::perception::Object());
        identifiedObjects[objectCounter].setIdentified(now);
        identifiedObjects[objectCounter].setTypeConfidence(-1.0f);
        identifiedObjects[objectCounter].setDistance(minDist);
        identifiedObjects[objectCounter].setDistanceConfidence(0.6f);
        opendlv::model::Direction objectDirection = opendlv::model::Direction((maxAngle+minAngle)/2.0f, 0.0f);
        identifiedObjects[objectCounter].setDirection(objectDirection);
        identifiedObjects[objectCounter].setDirectionConfidence(0.5f);
        identifiedObjects[objectCounter].setDirectionRateConfidence(-1.0f);
        identifiedObjects[objectCounter].setAngularSize(maxAngle-minAngle);
        identifiedObjects[objectCounter].setAngularSizeConfidence(0.5f);
        identifiedObjects[objectCounter].setAngularSizeRateConfidence(-1.0f);
        identifiedObjects[objectCounter].setConfidence(dynConfidence);
        identifiedObjects[objectCounter].setObjectId(-1);
        std::string source = "echolocation";
        std::vector<std::string> sources;
        sources.push_back(source);
        identifiedObjects[objectCounter].setListOfSources(sources);

        objectCounter++;
      }
    }
  }

  // for(uint32_t i = 0; i < nNewPoints; i++) {
  //   std::cout << "Dist: " << distances[i] << " Angle: " << angles[i] << std::endl;
  // }

  // std::cout << "===================================" << std::endl;
  for(uint32_t i = 0; i < objectCounter; i++) {
    odcore::data::Container c(identifiedObjects[i]);
    getConference().send(c);
    // std::cout << "Object sent with distance: " << identifiedObjects[i].getDistance() << ", Angle: "  << 
    //     identifiedObjects[i].getDirection().getAzimuth() << " , Angular size: " << 
    //     identifiedObjects[i].getAngularSize() << std::endl;
  }
  // std::cout << "===================================" << std::endl;

}


double Echolocation::PointDistance(float a_angle1, double a_dist1, float a_angle2, double a_dist2)
{
  double x1 = std::cos(static_cast<double>(a_angle1))*a_dist1;
  double y1 = std::sin(static_cast<double>(a_angle1))*a_dist1;
  double x2 = std::cos(static_cast<double>(a_angle2))*a_dist2;
  double y2 = std::sin(static_cast<double>(a_angle2))*a_dist2;
  double between = std::sqrt(std::pow(x1-x2,2) + std::pow(y1-y2,2));
  return between;
}

void Echolocation::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_memoryThreshold = kv.getValue<float>("logic-sensation-echolocation.memoryThreshold");
  m_pointCloudRadius = kv.getValue<double>("logic-sensation-echolocation.pointCloudRadius");
  m_pointCloudSizeMinimum = kv.getValue<int32_t>("logic-sensation-echolocation.pointCloudSizeMinimum");
  m_initialised = true;
}

void Echolocation::tearDown()
{
}

}
}
}
