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

#ifndef GCDC16_RULE_PLATOON_PLATOON_HPP_
#define GCDC16_RULE_PLATOON_PLATOON_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace platoon {

/**
 * This class provides...
 */
class Platoon : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Platoon(int32_t const &, char **);
  Platoon(Platoon const &) = delete;
  Platoon &operator=(Platoon const &) = delete;
  virtual ~Platoon();
  virtual void nextContainer(odcore::data::Container &);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void ActOnEnvironment(opendlv::perception::Environment &);
  void ActOnMio(std::vector<opendlv::perception::Object> &);
  void ActOnLane(std::vector<opendlv::perception::Surface> &);
  void ControlDirectionOfMovement(float);
  void ControlGroundSpeed(float);
  void setUp();
  void tearDown();

 private:
  bool m_steerTowardsMio;
  float m_desiredGroundSpeed;
  float m_desiredGroundSpeedMax;
  float m_previousAzimuthFollowed;
  float m_lateralDiscardDistance;
  float m_longitudinalDiscardDistance;
  float m_steerTowardsMioThreshold;
  float m_timeToCollision;
};

} // platoon
} // rule
} // gcdc16
} // knowledge
} // opendlv

#endif
