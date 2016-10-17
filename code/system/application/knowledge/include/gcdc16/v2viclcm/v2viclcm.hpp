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

#ifndef GCDC16_V2VICLCM_V2VICLCM_HPP_
#define GCDC16_V2VICLCM_V2VICLCM_HPP_


#include <ctime>
#include <fstream>
#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"


namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace v2viclcm {

/**
 * This class provides...
 */
class V2vIclcm
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  V2vIclcm(int32_t const &, char **);
  V2vIclcm(V2vIclcm const &) = delete;
  V2vIclcm &operator=(V2vIclcm const &) = delete;
  virtual ~V2vIclcm();
  virtual void nextContainer(odcore::data::Container &c);

 private:
  void setUp();
  void tearDown();
  void ReadInsight(opendlv::knowledge::Insight &);
  void SendInsight(std::string const &);
  void ReadVoice(opendlv::sensation::Voice &);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();


  uint64_t GenerateGenerationTime() const;
  bool m_initialised;
  std::ofstream m_sendLog;
  std::ofstream m_receiveLog;
  std::time_t m_timeType2004;
  std::string m_scenario;

  bool m_mioBeenLeader;
  bool m_hasMerged;
  uint32_t m_counterMerge = 0;
  bool m_printOutbound;
  bool m_printInbound;
  bool m_record;

  unsigned char m_messageId = 10;
  unsigned char m_containerMask = 0;
  unsigned char m_lowFrequencyMask = 0;
  int32_t m_stationId = 0;
  int32_t m_rearAxleLocation = 4095;
  int32_t m_controllerType = 0;
  int32_t m_responseTimeConstant = 50;
  int32_t m_responseTimeDelay = 50;
  int32_t m_targetLongAcc = 1001;
  int32_t m_timeHeadway = 361;
  int32_t m_cruiseSpeed = 5001;
  //
  int32_t m_participantsReady = 0;
  int32_t m_startPlatoon = 0;
  int32_t m_endOfScenario = 0;
  //
  int32_t m_mioId = 0;
  int32_t m_mioRange = 65535;
  int32_t m_mioBearing = 1572;
  int32_t m_mioRangeRate = 32767;
  int32_t m_lane = 4;
  int32_t m_forwardId = 0;
  int32_t m_backwardId = 0;
  int32_t m_mergeRequest = 0;
  int32_t m_safeToMerge = 0;
  int32_t m_flag = 0;
  int32_t m_flagTail = 0;
  int32_t m_flagHead = 0;
  int32_t m_platoonId = 3;
  int32_t m_distanceTravelledCz = 0;
  int32_t m_intention = 1;
  int32_t m_counter = 0;
};

} // v2viclcm
} // gcdc16
} // knowledge
} // opendlv

#endif
