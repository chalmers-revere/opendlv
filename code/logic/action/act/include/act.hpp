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

#ifndef LOGIC_ACTION_ACT_ACT_HPP
#define LOGIC_ACTION_ACT_ACT_HPP

#include <memory>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

namespace opendlv {
namespace logic {
namespace action {

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

  std::vector<odcore::data::TimeStamp> m_startTimesAccelerate;
  std::vector<odcore::data::TimeStamp> m_startTimesBrake;
  std::vector<odcore::data::TimeStamp> m_startTimesSteering;
  std::vector<float> m_amplitudesAccelerate;
  std::vector<float> m_amplitudesBrake;
  std::vector<float> m_amplitudesSteering;
  float m_accelerationValue;
  float m_brakeValue;
  float m_steeringValue;
  float m_correctionDuration;
};

}
}
}

#endif
