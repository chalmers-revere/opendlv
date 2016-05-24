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

#include "echolocation/echolocation.hpp"

namespace opendlv {
namespace sensation {
namespace echolocation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Echolocation::Echolocation(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-echolocation")
    , m_angles()
    , m_distances()
    , m_times()
{
}

Echolocation::~Echolocation()
{
}

/**
 * Receives .
 * Sends .
 */
void Echolocation::nextContainer(odcore::data::Container &a_c)
{
	if(a_c.getDataType() != opendlv::proxy::EchoReading::ID()){
  	return;
  }
  std::cout << "Echolocation container recieved" << std::endl;

  odcore::data::TimeStamp now;

  odcore::data::TimeStamp diffStamp = now - m_times.back(); //Remove old data
  double diffSeconds = diffStamp.toMicroseconds() / 1000000.0;

  while(diffSeconds < 1.05) {
  	m_times.pop_back();
  	m_angles.pop_back();
  	m_distances.pop_back();
  	diffStamp = now - m_times.back();
  	diffSeconds = diffStamp.toMicroseconds() / 1000000.0;
  }


  opendlv::proxy::EchoReading reading = a_c.getData<opendlv::proxy::EchoReading>(); //Read new data
  std::vector<opendlv::model::Direction> directions = reading.getListOfDirections();
  std::vector<double> distances = reading.getListOfRadii();
  uint32_t nNewPoints = reading.getNumberOfPoints();

  std::vector<float> angles;

  for(uint32_t i = 0; i < nNewPoints; i++) {
  	angles.push_back(directions[i].getAzimuth());
  }

  m_angles.insert(m_angles.begin(), angles.begin(), angles.end());
  m_distances.insert(m_distances.begin(), distances.begin(), distances.end());
  m_times.insert(m_times.begin(),nNewPoints,now);


  
  std::vector<opendlv::perception::Object> identifiedObjects;

  std::vector<uint32_t> usedPoints;
  std::vector<uint32_t> pointCloud;
  
  uint32_t nPoints = m_angles.size();
  uint32_t objectCounter = 0;

	for(uint32_t k = 0; k < nPoints; k++) {
		pointCloud.clear();
		pointCloud.push_back(k);
		if(!Contains(k, usedPoints)) {
		  for(uint32_t i = 0; i < pointCloud.size(); i++) {
				for(uint32_t j = 0; j < nPoints; j++) {
					uint32_t x = pointCloud[i];
					double dist = PointDistance(m_angles[x], m_distances[x], m_angles[j], m_distances[j]);
					if(dist < 1 && !Contains(j,pointCloud)) {
						pointCloud.push_back(j);
						usedPoints.push_back(j);
					}
				}
		  }
		  if(pointCloud.size() > 4) {
		  	
				double minDist = m_distances[pointCloud[0]];
				//uint32_t minIndex = pointCloud[0];
				float minAngle = m_angles[pointCloud[0]];
				float maxAngle = m_angles[pointCloud[0]];

				for(uint32_t i = 1; i < pointCloud.size(); i++) {
					if(m_distances[pointCloud[i]] < minDist) {
						minDist = m_distances[pointCloud[i]];
						//minIndex = pointCloud[i];
					}
					if(m_angles[pointCloud[i]] < minAngle) {
						minAngle = m_angles[pointCloud[i]];
					}
					if(m_angles[pointCloud[i]] > maxAngle) {
						maxAngle = m_angles[pointCloud[i]];
					}
				}
				//Set Object
				//TODO: Dynamic confidence
				float dynConfidence = sqrt(static_cast<float>(pointCloud.size()))/3;
				if(dynConfidence > 1) {
					dynConfidence = 1;
				}
				identifiedObjects[objectCounter] = opendlv::perception::Object();
				identifiedObjects[objectCounter].setIdentified(now);
				identifiedObjects[objectCounter].setTypeConfidence(-1.0f);
				identifiedObjects[objectCounter].setDistance(minDist);
				identifiedObjects[objectCounter].setDistanceConfidence(0.5f);
				opendlv::model::Direction objectDirection = opendlv::model::Direction((maxAngle+minAngle)/2, 0.0f);
				identifiedObjects[objectCounter].setDirection(objectDirection);
				identifiedObjects[objectCounter].setDirectionConfidence(0.5f);
				identifiedObjects[objectCounter].setDirectionRateConfidence(-1.0f);
				identifiedObjects[objectCounter].setAngularSize(maxAngle-minAngle);
				identifiedObjects[objectCounter].setAngularSizeConfidence(0.5);
				identifiedObjects[objectCounter].setAngularSizeRateConfidence(-1.0f);
				identifiedObjects[objectCounter].setConfidence(dynConfidence);
				identifiedObjects[objectCounter].setSources(1);

				objectCounter++;
		  }
	  }
  }


	for(uint32_t i = 0; i < objectCounter; i++) {
  	odcore::data::Container c(identifiedObjects[i]);
  	getConference().send(c);
  	std::cout << "Object sent" << std::endl;
	}

}

bool Echolocation::Contains(uint32_t a_point, std::vector<uint32_t> a_cloud)
{
	uint32_t length = a_cloud.size();
	for(uint32_t i = 0; i < length; i++) {
		if(a_point == a_cloud[i]) {
			return true;
		}
	}
	return false;
}

double Echolocation::PointDistance(float a_angle1, double a_dist1, float a_angle2, double a_dist2)
{
	double x1 = std::cos(static_cast<double>(a_angle1))*a_dist1;
	double y1 = std::sin(static_cast<double>(a_angle1))*a_dist1;
	double x2 = std::cos(static_cast<double>(a_angle2))*a_dist2;
	double y2 = std::sin(static_cast<double>(a_angle2))*a_dist2;
	double between = sqrt(pow(x1-x2,2) + pow(y1-y2,2));
	return between;
}

void Echolocation::setUp()
{
}

void Echolocation::tearDown()
{
}

} // echolocation
} // sensation
} // opendlv
