/**
 * Copyright (C) 2015 Christian Berger
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <cmath>
#include <cstdio>

#include <iostream>

#include <opendavinci/odcore/base/Thread.h>
#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>
#include <opendavinci/odcore/io/URL.h>
#include <opendlv/core/wrapper/graph/DirectedGraph.h>
#include <opendlv/core/wrapper/graph/Edge.h>
#include <opendlv/core/wrapper/graph/Vertex.h>
#include <opendlv/data/environment/Polygon.h>
#include <opendlv/data/environment/Obstacle.h>
#include <opendlv/data/graph/WaypointsEdge.h>
#include <opendlv/data/graph/WaypointVertex.h>
#include <opendlv/data/planning/Route.h>
#include <opendlv/data/scenario/Scenario.h>
#include <opendlv/scenario/SCNXArchive.h>
#include <opendlv/scenario/SCNXArchiveFactory.h>
#include <opendlv/scenario/ScenarioFactory.h>
#include <opendlv/scenario/LaneVisitor.h>

#include "automotivedata/GeneratedHeaders_AutomotiveData.h"
#include "odvdopendlvstandardmessageset/GeneratedHeaders_ODVDOpenDLVStandardMessageSet.h"
#include "odvdvehicle/generated/opendlv/proxy/ActuationRequest.h"

#include "simpledriver.hpp"

namespace opendlv {
namespace logic {
namespace legacy {

using namespace std;
using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::data;
using namespace automotive;
using namespace automotive::miniature;
using namespace opendlv::data::environment;

SimpleDriver::SimpleDriver(const int32_t &argc, char **argv)
    : TimeTriggeredConferenceClientModule(argc, argv, "logic-legacy-simpledriver"),
      m_receveivedFirstWGS84Position(false),
      WGS84Reference(),
      m_oldPosition(),
      m_oldPositionForDirection(),
      m_egoStateMutex(),
      m_egoState(),
      m_currentSpeedMutex(),
      m_currentSpeed(),
      m_speedErrorSum()
{
}

SimpleDriver::~SimpleDriver()
{
}

void SimpleDriver::setUp()
{
  odcore::io::URL const urlOfSCNXFile(getKeyValueConfiguration().getValue<string>("global.scenario"));

  double const latitude = getKeyValueConfiguration().getValue<double>("global.reference.WGS84.latitude");
  double const longitude = getKeyValueConfiguration().getValue<double>("global.reference.WGS84.longitude");

  WGS84Reference = WGS84Coordinate(latitude, longitude);
}

void SimpleDriver::tearDown()
{
}
    
void SimpleDriver::nextContainer(odcore::data::Container &c) {
  // TODO: Create a microservice from the following lines.
  if (c.getDataType() == opendlv::data::environment::WGS84Coordinate::ID()) {
    WGS84Coordinate WGS84current = c.getData<WGS84Coordinate>();
    Point3 currentPosition = WGS84Reference.transform(WGS84current);

    if (m_receveivedFirstWGS84Position) {
      Lock l(m_egoStateMutex);

      double const d = (currentPosition - m_oldPositionForDirection).lengthXY();
      double const POSITION_UPDATE_DELTA_THRESHOLD = 0.2;
      if (d > POSITION_UPDATE_DELTA_THRESHOLD) {
        Point3 direction = (currentPosition - m_oldPositionForDirection);
        m_egoState.setRotation(direction);
        m_oldPositionForDirection = currentPosition;
      }
      m_egoState.setPosition(currentPosition);
      c = Container(m_egoState);
      getConference().send(c);
    }

    m_oldPosition = currentPosition;
    m_receveivedFirstWGS84Position = true;
  } else if (c.getDataType() == opendlv::proxy::GroundSpeedReading::ID()) {
    Lock l(m_currentSpeedMutex);
    auto groundSpeed = c.getData<opendlv::proxy::GroundSpeedReading>();
    m_currentSpeed = groundSpeed.getGroundSpeed();
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SimpleDriver::body()
{
  odcore::io::URL const urlOfSCNXFile(getKeyValueConfiguration().getValue<string>("global.scenario"));

  core::wrapper::graph::DirectedGraph m_graph;
  if (urlOfSCNXFile.isValid()) {
    opendlv::scenario::SCNXArchive &scnxArchive = opendlv::scenario::SCNXArchiveFactory::getInstance().getSCNXArchive(urlOfSCNXFile);

    opendlv::data::scenario::Scenario &scenario = scnxArchive.getScenario();

    // Construct road network.
    opendlv::scenario::LaneVisitor lv(m_graph, scenario);
    scenario.accept(lv);

    // Print graph in dot format.
    cout << m_graph.toGraphizDot() << endl << endl;
  }

  string startWaypoint = "";
  string endWaypoint = "";

  // Try to load coordinates from configuration.
  try {
    startWaypoint = getKeyValueConfiguration().getValue<string>("simpledriver.startwaypoint");
    endWaypoint = getKeyValueConfiguration().getValue<string>("simpledriver.endwaypoint");
  }
  catch (...) {
    startWaypoint = "";
    endWaypoint = "";
  }

  cout << endl;
  cout << "[" << getName() << "]: " << "Welcome to SimpleDriver" << endl << endl;
  if (startWaypoint == "") {
    cout << "[" << getName() << "]: " << "Where do you want to start your route? (The point should be in front of us! Example: 1.1.1.1)." << endl;
    cin >> startWaypoint;
  }
  if (endWaypoint == "") {
    cout << "[" << getName() << "]: " << "Where do you want to end your route? (Example: 1.4.1.2)." << endl;
    cin >> endWaypoint;
  }

  cout << "[" << getName() << "]: " << "Start: '" << startWaypoint << "'" << endl;
  cout << "[" << getName() << "]: " << "End: '" << endWaypoint << "'" << endl;

  opendlv::data::scenario::PointID pidStart(startWaypoint);
  opendlv::data::scenario::PointID pidEnd(endWaypoint);

  opendlv::data::graph::WaypointVertex v1;
  v1.setLayerID(pidStart.getLayerID());
  v1.setRoadID(pidStart.getRoadID());
  v1.setLaneID(pidStart.getLaneID());
  v1.setWaypointID(pidStart.getPointID());

  opendlv::data::graph::WaypointVertex v2;
  v2.setLayerID(pidEnd.getLayerID());
  v2.setRoadID(pidEnd.getRoadID());
  v2.setLaneID(pidEnd.getLaneID());
  v2.setWaypointID(pidEnd.getPointID());

  opendlv::data::planning::Route route;
  vector<const core::wrapper::graph::Vertex*> resultingRoute = m_graph.getShortestPath(v1, v2);
  if (resultingRoute.size() > 0) {
    vector<const core::wrapper::graph::Vertex*>::const_iterator it = resultingRoute.begin();
    while (it != resultingRoute.end()) {
      opendlv::data::graph::WaypointVertex const *v = dynamic_cast<const opendlv::data::graph::WaypointVertex*>(*it++);
      if (v != NULL) {
        route.add(v->getPosition());
      }
    }
  }

  if (route.getSize() > 0) {
    cout << "[" << getName() << "]: " << "Shortest route from " << v1.toString() << " to " << v2.toString() << ": " << endl;
    cout << route.toString() << endl;

    Container c;
    if (route.getSize() < 500) {
      c = Container(route);
      getConference().send(c);
    } else {
      cout << "[" << getName() << "]: " << "Route too long for visualization." << endl;
    }

    // Get initial EgoState.
    EgoState es;
    {
      Lock l(m_egoStateMutex);
      es = m_egoState;
    }


    uint32_t waitingBeforeStart = 2;
    while (waitingBeforeStart > 0) {
      cout << "[" << getName() << "]: " << "Still waiting " << waitingBeforeStart << " seconds..." << endl;
      waitingBeforeStart--;
      Thread::usleepFor(1 * 1000 * 1000);
    }

    Point3 previousPoint = es.getPosition();
    double previousHeading = es.getRotation().getAngleXY();

    bool hasSentActuation = false;

    // Try to cover the waypoints one-by-one from the route.
    vector<Point3> listOfPointsWaypoints = route.getListOfPoints();
    const uint32_t SIZE = listOfPointsWaypoints.size();
    for (uint32_t i = 0; (i < SIZE) && (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING); i++) {

      // Get next point to reach.
      Point3 nextPoint = listOfPointsWaypoints.at(i);
      while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        double currentSpeed = 0;
        {
          Lock l(m_egoStateMutex);
          es = m_egoState;

          Lock l2(m_currentSpeedMutex);
          currentSpeed = m_currentSpeed;
        }

        // Get our current position.
        Point3 currentPosition = es.getPosition();
        const double heading = es.getRotation().getAngleXY();
        if (fabs(heading - previousHeading) > 0.2) {
          cout << "[" << getName() << "]: " << "Large heading difference: " << fabs(heading - previousHeading) << endl;
        }

        previousHeading = heading;

        double const minPreviewDistance = getKeyValueConfiguration().getValue<double>("simpledriver.minimum_preview_distance");
        Point3 minPreviewPoint(minPreviewDistance, 0, 0);
        minPreviewPoint.rotateZ(heading);
        minPreviewPoint += currentPosition;

        // Skip points
        if (!hasSentActuation) {
          double const inclusionAngle = getKeyValueConfiguration().getValue<double>("simpledriver.aim_point_inclusion_angle_deg") * 0.0175;
          double anglePadding = (inclusionAngle - cartesian::Constants::PI) / 2.0;
          bool const isBehindToLeft = !minPreviewPoint.isInFront(nextPoint, heading + anglePadding);
          bool const isBehindToRight = !minPreviewPoint.isInFront(nextPoint, heading - anglePadding);
          if (isBehindToLeft && isBehindToRight) {
            cout << "[" << getName() << "]: " << "Discard behind " << nextPoint.toString() << endl;
            break;
          }

        } else {
          double discardThresholdTimeHeadway = getKeyValueConfiguration().getValue<double>("simpledriver.discard_threshold_time_headway");
          double discardThresholdDistance = currentSpeed * discardThresholdTimeHeadway;

          double nextPointDistance = (nextPoint - currentPosition).lengthXY();
          if (nextPointDistance < discardThresholdDistance) {
            //  cout << "[" << getName() << "]: " << "Discard at distance at " << nextPointDistance << " with threshold " << discardThresholdDistance << " " << nextPoint.toString() << endl;
            break;
          }
        }


        // Get angle to aim-point.
        Point3 aimPoint = nextPoint - currentPosition;

        // Normalize angle to -pi ..+pi.
        double aimPointAngle = aimPoint.getAngleXY() - heading;
        while (aimPointAngle < -cartesian::Constants::PI) {
          aimPointAngle += 2.0 * cartesian::Constants::PI;
        }
        while (aimPointAngle > cartesian::Constants::PI) {
          aimPointAngle -= 2.0 * cartesian::Constants::PI;
        }


        double const slowSpeed = getKeyValueConfiguration().getValue<double>("simpledriver.slow_speed");
        double const fastSpeed = getKeyValueConfiguration().getValue<double>("simpledriver.fast_speed");

        // Look ahead to detect a turn.
        double const lookAheadDistance = getKeyValueConfiguration().getValue<double>("simpledriver.look_ahead_distance");
        double const turnAngleThreshold = getKeyValueConfiguration().getValue<double>("simpledriver.turn_mean_angle_threshold_deg") * 0.0175;

        Point3 futureNextPoint = nextPoint;
        bool isEndOfRoad = true;
        double futurePathAngleAbsSum = 0.0;
        double futurePathDistanceSum = 0.0;
        double firstAngle = 0.0;
        for (uint16_t j = i+1; j < SIZE; j++) {

          if (j != SIZE - 1) {
            isEndOfRoad = false;
          }



          Point3 futureNextNextPoint = listOfPointsWaypoints.at(j);
          Point3 futurePathSegment = futureNextNextPoint - futureNextPoint;
          futurePathDistanceSum += futurePathSegment.lengthXY();

          if (j == (i+1)) {
            firstAngle = futurePathSegment.getAngleXY();
          }
          else {
            double absAngleDiff = fabs(futurePathSegment.getAngleXY() - firstAngle);
            futurePathAngleAbsSum += absAngleDiff;
          }

          if (futurePathDistanceSum > lookAheadDistance) {
            break;
          }

          futureNextPoint = futureNextNextPoint;
        }

        double futurePathMeanAngle = futurePathAngleAbsSum / lookAheadDistance;
        double targetSpeed;
        
        cout << "Future path mean angle (deg): " << (futurePathMeanAngle / 0.0175) << endl;
        
        if (futurePathMeanAngle > turnAngleThreshold) {
          if (isVerbose()) {
          //      cout << "[" << getName() << "]: " << "Turn detected, will slow down! (sum of abs angles = " << futurePathAngleAbsSum << ")" << endl;
          }
          targetSpeed = slowSpeed;
        } else if (isEndOfRoad) {
          if (isVerbose()) {
            cout << "[" << getName() << "]: " << "End of road detected." << endl;
          }
          targetSpeed = slowSpeed;
        } else if (fabs(aimPointAngle) > 0.2) {
          if (isVerbose()) {
            cout << "[" << getName() << "]: " << "Will drive slow since aim point angle is large." << endl;
          }
          targetSpeed = slowSpeed;
        } else {
          targetSpeed = fastSpeed;
        }

        if (targetSpeed < fastSpeed) {
          cout << "Driving slow." << endl;
        } else {
          cout << "Driving fast." << endl;
        }

        Line currentPath(nextPoint, previousPoint);
        Point3 perpendicularPoint = currentPath.getPerpendicularPoint(currentPosition);

        double theta = (nextPoint - currentPosition).getAngleXY();
        double phi = (perpendicularPoint - currentPosition).getAngleXY();
        double lateralOffset = -(nextPoint - currentPosition).lengthXY() * cos(phi - theta);

        double const aimPointGainSlowSpeed = getKeyValueConfiguration().getValue<double>("simpledriver.aim_point_gain_slow_speed");
        double const aimPointGainFastSpeed = getKeyValueConfiguration().getValue<double>("simpledriver.aim_point_gain_fast_speed");
        double lateralOffsetGain = getKeyValueConfiguration().getValue<double>("simpledriver.lateral_offset_gain");

        double aimPointGain = aimPointGainSlowSpeed;
        if (fabs(targetSpeed - fastSpeed) < 1e-3) {
          aimPointGain = aimPointGainFastSpeed;
        }

        lateralOffsetGain = 0.0;
        double steeringWheelAngle = aimPointGain * aimPointAngle + lateralOffsetGain * lateralOffset;

        steeringWheelAngle = (steeringWheelAngle > 3.0 * cartesian::Constants::PI) ? 9.3 : steeringWheelAngle;
        steeringWheelAngle = (steeringWheelAngle < -3.0 * cartesian::Constants::PI) ? -9.3 : steeringWheelAngle;

        if (isVerbose()) {
          cout << "[" << getName() << "]: " << "Lateral control: current position = " << currentPosition.toString() << ", next position = " << nextPoint.toString() << ", aim point angle = " << aimPointAngle << ", lateral offset = " << lateralOffset  << endl;
        }

        {
          // Visualize steering control.
          Line l(currentPosition, nextPoint);
          Point3 perpendicularPoint2 = l.getPerpendicularPoint(nextPoint);

          Polygon p;
          p.add(currentPosition);
          p.add(nextPoint);
          p.add(perpendicularPoint2);

          Obstacle o(1, Obstacle::UPDATE);
          o.setPolygon(p);
          c = Container(o);
          getConference().send(c);
        }


        double speedError = targetSpeed - currentSpeed;

        double const pidK = getKeyValueConfiguration().getValue<double>("simpledriver.speed_control_k");
        double const pidI = getKeyValueConfiguration().getValue<double>("simpledriver.speed_control_i");
        double const accelerationRequestLimit = getKeyValueConfiguration().getValue<double>("simpledriver.speed_control_global_limit");
        double const speedErrorSumLimit = getKeyValueConfiguration().getValue<double>("simpledriver.speed_control_error_sum_limit");;

        double kPart = pidK * speedError;

        m_speedErrorSum += speedError;
        m_speedErrorSum = (m_speedErrorSum > speedErrorSumLimit) ? speedErrorSumLimit : m_speedErrorSum;
        m_speedErrorSum = (m_speedErrorSum < -speedErrorSumLimit) ? -speedErrorSumLimit : m_speedErrorSum;
        double iPart = pidI * m_speedErrorSum;

        double accelerationRequest = kPart + iPart;
        accelerationRequest = (accelerationRequest > accelerationRequestLimit) ? accelerationRequestLimit : accelerationRequest;
        accelerationRequest = (accelerationRequest < -accelerationRequestLimit) ? -accelerationRequestLimit : accelerationRequest;

        if (isVerbose()) {
          cout << "[" << getName() << "]: " << "Longitudinal control: current speed = " << currentSpeed << ", speed error = " << speedError << ", K = " << kPart << ", I = " << iPart << ", sum or errors = " << m_speedErrorSum << ", acceleration request: " << accelerationRequest << endl;
        }

        // Create vehicle control data.
        opendlv::proxy::ActuationRequest ar;
        ar.setAcceleration(accelerationRequest);
        ar.setSteering(steeringWheelAngle);
        ar.setIsValid(true);

        c = Container(ar);
        getConference().send(c);
        
        hasSentActuation = true;
      }

      previousPoint = nextPoint;
    }
  }

  // Stop vehicle.
  opendlv::proxy::ActuationRequest ar;
  ar.setAcceleration(-3.0);
  ar.setSteering(0.0);
  ar.setIsValid(true);

  // Create container for finally sending the data.
  Container c(ar);
  // Send container.
  getConference().send(c);

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

}
}
}
