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

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

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
  void timeCheck(std::vector<odcore::data::TimeStamp> &, std::vector<float> &);
  void inhibitoryCheck(bool, std::vector<odcore::data::TimeStamp> &, std::vector<float> &);

  float m_accelerationCorrection;
  float m_breakingCorrection;
  float m_steeringCorrection;
  std::vector<odcore::data::TimeStamp> m_startTimeVectorAccelerate;
  std::vector<odcore::data::TimeStamp> m_startTimeVectorBrake;
  std::vector<odcore::data::TimeStamp> m_startTimeVectorSteering;
  std::vector<float> m_amplitudeVectorAccelerate;
  std::vector<float> m_amplitudeVectorBrake;
  std::vector<float> m_amplitudeVectorSteering;
  bool m_isInhibitory;
  float m_amplitude;
  odcore::data::TimeStamp m_t0;
  std::string m_type;

};

} // act
} // action
} // opendlv

#endif
