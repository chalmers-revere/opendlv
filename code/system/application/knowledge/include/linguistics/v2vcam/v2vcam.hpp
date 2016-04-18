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

#ifndef LINGUISTICS_V2VCAM_V2VCAM_HPP_
#define LINGUISTICS_V2VCAM_V2VCAM_HPP_

#include <fstream>
#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace knowledge {
namespace linguistics {
namespace v2vcam {

/**
 * This class provides...
 */
class V2vCam
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  V2vCam(int32_t const &, char **);
  V2vCam(V2vCam const &) = delete;
  V2vCam &operator=(V2vCam const &) = delete;
  virtual ~V2vCam();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  std::ofstream m_sendLog;
  std::ofstream m_receiveLog;

  unsigned char const m_messageId = 2;
  int32_t m_stationId = 115;
  // Generation time of the (CAM) message in milliseconds
  int32_t m_generationDeltaTime = 0; 
  unsigned char m_containerMask = 128;
  int32_t m_stationType = 8;
  int32_t m_latitude = 900000001;
  int32_t m_longitude = 1800000001;
  int32_t m_semiMajorConfidence = 4095;
  int32_t m_semiMinorConfidence = 4095;
  int32_t m_semiMajorOrientation = 3601;
  int32_t m_altitude = 800001;
  int32_t m_heading = 3601;
  int32_t m_headingConfidence = 127;
  int32_t m_speed = 16383; //
  int32_t m_speedConfidence = 127;
  int32_t m_vehicleLength = 73;
  int32_t m_vehicleWidth = 25;
  int32_t m_longitudinalAcc = 161;
  int32_t m_longitudinalAccConf = 102;
  int32_t m_yawRateValue = 32767;
  int32_t m_yawRateConfidence = 8;
  int32_t m_vehicleRole = 0;
};

} // v2vcam
} // linguistics
} // knowledge
} // opendlv

#endif
