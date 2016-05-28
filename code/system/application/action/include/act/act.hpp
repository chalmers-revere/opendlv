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

#ifndef ACT_ACT_HPP_
#define ACT_ACT_HPP_

#include <memory>
 #include <fstream>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

namespace opendlv {
namespace action {
namespace act {

/**
 * This class provides...
 */
class Act : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Act(int32_t const &, char **);
  Act(Act const &) = delete;
  Act &operator=(Act const &) = delete;
  virtual ~Act();
  virtual void nextContainer(odcore::data::Container &);

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();

  float m_maxDuration;

  std::vector<odcore::data::TimeStamp> m_startTimesAccelerate;
  std::vector<odcore::data::TimeStamp> m_startTimesBrake;
  std::vector<odcore::data::TimeStamp> m_startTimesSteering;
  std::vector<float> m_amplitudesAccelerate;
  std::vector<float> m_amplitudesBrake;
  std::vector<float> m_amplitudesSteering;
  float m_accelerationValue;
  float m_brakeValue;
  float m_steeringValue;

  bool m_hasEstimateAcc;
  odcore::data::TimeStamp m_timeOfEstimateAcc;
  bool m_hasEstimateBrake;
  odcore::data::TimeStamp m_timeOfEstimateBrake;
  bool m_hasEstimateSteer;
  odcore::data::TimeStamp m_timeOfEstimateSteer;


  float m_targetDistance;
  int32_t m_targetObjectId;
  opendlv::perception::Object m_targetObject;
  float m_desiredSpeed;
  float m_currentSpeed;
};

} // act
} // action
} // opendlv

#endif
