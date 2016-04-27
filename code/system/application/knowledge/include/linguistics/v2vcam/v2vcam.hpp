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

#include <ctime>
#include <fstream>
#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

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

  unsigned char GetMessageId();
  int32_t GetStationId();
  int32_t GetGenerationDeltaTime();
  unsigned char GetContainerMask();
  int32_t GetStationType();
  int32_t GetLatitude();
  int32_t GetLongitude();
  int32_t GetSemiMajorConfidence();
  int32_t GetSemiMinorConfidence();
  int32_t GetSemiMajorOrientation();
  int32_t GetAltitude();
  int32_t GetHeading();
  int32_t GetHeadingConfidence();
  int32_t GetSpeed();
  int32_t GetSpeedConfidence();
  int32_t GetVehicleLength();
  int32_t GetVehicleWidth();
  int32_t GetLongitudinalAcc();
  int32_t GetLongitudinalAccConf();
  int32_t GetYawRateValue();
  int32_t GetYawRateConfidence();
  int32_t GetVehicleRole();

  std::ofstream m_sendLog;
  std::ofstream m_receiveLog;
  std::time_t m_tt;

  unsigned char const m_messageId = 2;
  int32_t m_stationId = 115;
  // Generation time of the (CAM) message in milliseconds
  int32_t m_generationDeltaTime = 0; 
  unsigned char m_containerMask = 128;
  int32_t m_stationType = 8;
  double m_latitude = 90.0000001;
  double m_longitude = 180.0000001;
  int32_t m_semiMajorConfidence = 4095;
  int32_t m_semiMinorConfidence = 4095;
  int32_t m_semiMajorOrientation = 3601;
  float m_altitude = 8000.01;
  float m_heading = 360.1*opendlv::Constants::DEG2RAD;
  int32_t m_headingConfidence = 127;
  float m_speed = 163.83; //
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
