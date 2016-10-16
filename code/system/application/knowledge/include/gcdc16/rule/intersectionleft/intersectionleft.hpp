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

#ifndef GCDC16_RULE_INTERSECTIONLEFT_INTERSECTIONLEFT_HPP_
#define GCDC16_RULE_INTERSECTIONLEFT_INTERSECTIONLEFT_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace intersectionleft {

/**
 * This class provides...
 */
class IntersectionLeft : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  IntersectionLeft(int32_t const &, char **);
  IntersectionLeft(IntersectionLeft const &) = delete;
  IntersectionLeft &operator=(IntersectionLeft const &) = delete;
  virtual ~IntersectionLeft();
  virtual void nextContainer(odcore::data::Container &);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();
  //double getDistances(double);
  //bool euclideanDistance(double);

  void receivedContainerMergeScenario(odcore::data::Container &a_container);
  void receivedContainerIntersectionScenario(odcore::data::Container &a_container);
  void bodyMergeScenario();
  void bodyIntersectionScenario();

 private:

  std::unique_ptr<opendlv::perception::Object> m_object;
  opendlv::perception::Object m_closestObject;
  opendlv::perception::Object m_secondClosestObject;
  opendlv::perception::Object m_mostInterestingObject;
  float m_desiredAzimuth;
  float m_cruiseSpeed;
  float m_desiredAngularSize;
  float m_speed;
  bool m_isAutonomous;
  std::string m_platoonId;
  std::string m_currentLane;
  std::string m_isTail;
  bool m_hasMerged;
  bool m_isInitialized;
  bool m_scenarioIsReady;
  bool m_isLeader;
  std::string m_scenarioType;
  bool m_hasSetupBeenRun;
  bool m_isCreatingGap;
  //double standstillDistance;
  //double headway;
  //double minimumEuclideanDistance;
};

} // intersectionleft
} // rule
} // gcdc16
} // knowledge
} // opendlv

#endif