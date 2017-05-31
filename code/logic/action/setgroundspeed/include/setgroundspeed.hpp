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

#ifndef LOGIC_ACTION_SETGROUNDSPEED_SETGROUNDSPEED_HPP
#define LOGIC_ACTION_SETGROUNDSPEED_SETGROUNDSPEED_HPP

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

namespace opendlv {
namespace logic {
namespace action {

class SetGroundSpeed
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  SetGroundSpeed(int32_t const &, char **);
  SetGroundSpeed(SetGroundSpeed const &) = delete;
  SetGroundSpeed &operator=(SetGroundSpeed const &) = delete;
  virtual ~SetGroundSpeed();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void AddStimulus(odcore::data::TimeStamp const &, opendlv::perception::StimulusGroundSpeed const &);
  void Correct();
  bool IsPatient() const;
  void setUp();
  void tearDown();

  bool m_initialised;
  std::vector<odcore::data::TimeStamp> m_stimulusTime;
  std::vector<float> m_stimulus;
  std::vector<float> m_stimulusRate;
  odcore::data::TimeStamp m_correctionTime;
  float m_correction;
  float m_correctionGain;
  float m_maxStimulusAge;
  float m_patienceDuration;
  float m_stimulusJerk;
  float m_stimulusJerkThreshold;
  float m_stimulusRateThreshold;
  float m_stimulusThreshold;
  float m_equilibrium;
};

}
}
}

#endif
