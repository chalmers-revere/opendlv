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
  void ActOnEnvironment(opendlv::perception::Environment &);
  void ActOnMio(std::vector<opendlv::perception::Object> &);
  void ActOnLane(std::vector<opendlv::perception::Surface> &);
  void ControlDirectionOfMovement();
  void ControlGroundSpeed(float);
  void setUp();
  void tearDown();

  bool m_enableLaneFollowing;
  bool m_runScenario;
  float m_desiredGroundSpeed;
};

} // intersectionleft
} // rule
} // gcdc16
} // knowledge
} // opendlv

#endif
