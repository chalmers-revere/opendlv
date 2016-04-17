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

#ifndef LINGUISTICS_V2VDENM_V2VDENM_HPP_
#define LINGUISTICS_V2VDENM_V2VDENM_HPP_

#include <fstream>
#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace knowledge {
namespace linguistics {
namespace v2vdenm {

/**
 * This class provides...
 */
class V2vDenm
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  V2vDenm(int32_t const &, char **);
  V2vDenm(V2vDenm const &) = delete;
  V2vDenm &operator=(V2vDenm const &) = delete;
  virtual ~V2vDenm();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  std::ofstream m_sendLog;
  std::ofstream m_receiveLog;

  unsigned char const m_messageId = 1;
  int32_t m_stationId = 0;
  int32_t m_generationDeltaTime = 0; // Generation time of the (Denm) message in milliseconds
  unsigned char m_containerMask = 160;
  unsigned char m_managementMask = 248;
  unsigned char m_situationMask = 8;
  unsigned char m_alacarteMask = 168;  
  int32_t m_detectionTime = 0; 
  int32_t m_referenceTime = 1; 
  int32_t m_termination = 0;
  int32_t m_latitude = 900000001;
  int32_t m_longitude = 1800000001;
  int32_t m_semiMajorConfidence = 4095;
  int32_t m_semiMinorConfidence = 4095;
  int32_t m_semiMajorOrientation = 3601;
  int32_t m_altitude = 800001;
  int32_t m_relevanceDistance = 0;
  int32_t m_relevanceTrafficDirection = 0;
  int32_t m_validityDuration = 0;
  int32_t m_transmissionInterval = 1;
  int32_t m_stationType = 0;
  int32_t m_informationQuality = 4;
  int32_t m_causeCode = 0;
  int32_t m_subCauseCode = 0;
  int32_t m_linkedCauseCode = 0;
  int32_t m_linkedSubCauseCode = 0;
  int32_t m_lanePosition = -1;
  int32_t m_temperature = 0;
  int32_t m_positioningSolutionType = 5;


};

} // v2vdenm
} // linguistics
} // knowledge
} // opendlv

#endif
