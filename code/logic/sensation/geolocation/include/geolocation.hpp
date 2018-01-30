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

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendavinci/odcore/wrapper/Eigen.h>
#include <opendavinci/odcore/base/Lock.h>
#include "opendlv/data/environment/Point3.h"
#include "opendlv/data/environment/WGS84Coordinate.h"

//#include "opendlv/data/environment/Point3.h"
//#include "opendlv/data/environment/WGS84Coordinate.h"

#include "odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h"
//#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdtrimble/GeneratedHeaders_ODVDTrimble.h"
#include "odvdimu/GeneratedHeaders_ODVDIMU.h"
#include "odvdopendlvstandardmessageset/GeneratedHeaders_ODVDOpenDLVStandardMessageSet.h"
//Include cfsd??
//#include <odvdcfsd18/GeneratedHeaders_ODVDcfsd18.h>

namespace opendlv {
namespace logic {
namespace sensation {

class Geolocation : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Geolocation(int32_t const &, char **);
  Geolocation(Geolocation const &) = delete;
  Geolocation &operator=(Geolocation const &) = delete;
  virtual ~Geolocation();
  virtual void nextContainer(odcore::data::Container &);
  

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();
  MatrixXd UKFWeights();
  MatrixXd sigmaPoints(MatrixXd &x);
  MatrixXd UKFPrediction(MatrixXd &x);
  MatrixXd UKFUpdate(MatrixXd &x);
  MatrixXd vehicleModel(MatrixXd &x);
  MatrixXd measurementModel(MatrixXd &x);
  double magicFormula(double &alpha, double &Fz, double const &mu);
  double rackTravelToFrontWheelSteering(double &rackTravel);
  odcore::data::TimeStamp m_gpsTimeStamp;
  odcore::data::TimeStamp m_accTimeStamp;
  odcore::data::TimeStamp m_gyrTimeStamp;
  odcore::data::TimeStamp m_groundSpeedTimeStamp;
  odcore::base::Mutex m_sensorMutex;
  MatrixXf m_accXYReading;
  float m_yawReading;
  MatrixXd m_gpsReading;
  opendlv::data::environment::WGS84Coordinate m_gpsReference;
  MatrixXd m_Q;
  MatrixXd m_R;
  MatrixXd m_stateCovP;
  MatrixXd m_vehicleModelParameters;
  double m_delta;
  double m_sampleTime;
  double m_headingReading;
  double m_groundSpeedReading;
  //odcore::base::Mutex m_egoStateMutex;

  //void nextContainer(odcore::data::Container &);
};

}
}
}

#endif
