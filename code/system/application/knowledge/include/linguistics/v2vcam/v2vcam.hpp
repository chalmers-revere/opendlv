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
#include <cmath>
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

  void ReadDynamicState(opendlv::model::DynamicState const &);
  void ReadGeolocation(opendlv::sensation::Geolocation const &);
  void ReadVoice(opendlv::sensation::Voice const &);
  void ReadInsight(opendlv::knowledge::Insight const &);
  void SendWGS84Coordinate();

  unsigned char GetMessageId() const;
  int32_t GetStationId() const;
  uint32_t GenerateGenerationTime() const;
  int32_t GenerateGenerationDeltaTime();
  unsigned char GetContainerMask() const;
  int32_t GetStationType() const;
  int32_t GetLatitude() const;
  int32_t GetLongitude() const;
  int32_t GetSemiMajorConfidence() const;
  int32_t GetSemiMinorConfidence() const;
  int32_t GetSemiMajorOrientation() const;
  int32_t GetAltitude() const;
  int32_t GetHeading() const;
  int32_t GetHeadingConfidence() const;
  int32_t GetSpeed() const;
  int32_t GetSpeedConfidence() const;
  int32_t GetVehicleLength() const;
  int32_t GetVehicleWidth() const;
  int32_t GetLongitudinalAcc() const;
  int32_t GetLongitudinalAccConf() const;
  int32_t GetYawRateValue() const;
  int32_t GetYawRateConfidence() const;
  int32_t GetVehicleRole() const;

  std::ofstream m_sendLog;
  std::ofstream m_receiveLog;
  std::time_t m_timeType2004;

  unsigned char const m_messageId = 2;
  int32_t m_stationId = 0;
  int32_t m_generationDeltaTime = 0; 
  unsigned char m_containerMask = 0;
  int32_t m_stationType = 0;
  double m_latitude = 90.0000001;
  double m_longitude = 180.0000001;
  double m_semiMajorConfidence = -1;
  double m_semiMinorConfidence = -1;
  double m_semiMajorOrientation = -1;
  double m_altitude = 8000.01;
  double m_heading = -1;
  double m_headingConfidence = -1;
  double m_speed = -1; //
  double m_speedConfidence = -1;
  double m_vehicleLength = -1;
  double m_vehicleWidth = -1;
  double m_longitudinalAcc = -1;
  double m_longitudinalAccConf = -1;
  double m_yawRateValue = 0;
  double m_yawRateConfidence = -1;
  int32_t m_vehicleRole = 0;
};

} // v2vcam
} // linguistics
} // knowledge
} // opendlv

#endif
