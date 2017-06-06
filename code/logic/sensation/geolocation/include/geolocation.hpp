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

#ifndef LOGIC_SENSATION_GEOLOCATION_GEOLOCATION_HPP
#define LOGIC_SENSATION_GEOLOCATION_GEOLOCATION_HPP

#include <memory>
    
#include "opendavinci/odcore/base/Mutex.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

#include "opendlv/data/environment/Point3.h"
#include "opendlv/data/environment/WGS84Coordinate.h"

#include "odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdtrimble/GeneratedHeaders_ODVDTrimble.h"
#include "odvdimu/GeneratedHeaders_ODVDIMU.h"

#include "kalman/ExtendedKalmanFilter.hpp"

#include "kinematicmodel.hpp"
#include "kinematicobservationmodel.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

class Geolocation
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Geolocation(int32_t const &, char **);
  Geolocation(Geolocation const &) = delete;
  Geolocation &operator=(Geolocation const &) = delete;
  virtual ~Geolocation();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();
  void nextContainer(odcore::data::Container &);
  double calculatePositionConfidence(bool &);
  double calculateHeadingConfidence(bool &);
  double calculateHeadingRateConfidence(bool &);
  double calculateSpeedConfidence(bool &);
  void filterReset(opendlv::data::environment::Point3 &, opendlv::core::sensors::trimble::GpsReading &);

  odcore::base::Mutex m_gpsReadingMutex;

  Kalman::ExtendedKalmanFilter<State<double>> m_ekf;
  opendlv::data::environment::WGS84Coordinate m_gpsReading;

  opendlv::proxy::MagnetometerReading m_magnetometerReading; 
  opendlv::proxy::AccelerometerReading m_accelerometerReading;
  opendlv::proxy::reverefh16::Steering m_steeringReading;
  opendlv::proxy::reverefh16::Propulsion m_propulsionReading;

  bool m_debug;

  bool m_initialised;
  double m_gpsToCoGDisplacement[3] = {-1.0f,-2.3f,-22.0f};
  double m_steeringToWheelRatio = -22.0f;
};

}
}
}

#endif
