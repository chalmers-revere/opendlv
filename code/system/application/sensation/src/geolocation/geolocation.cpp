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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include <opendlv/data/environment/Point3.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "geolocation/geolocation.hpp"



namespace opendlv {
namespace sensation {
namespace geolocation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Geolocation::Geolocation(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-geolocation"),
    m_ekf()
{
}

Geolocation::~Geolocation()
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  opendlv::data::environment::WGS84Coordinate gpsReference;
  bool hasGpsReference = false;

  odcore::data::TimeStamp previousTimestep;
  odcore::data::TimeStamp previousDataTimestamp;
  double timestamp = 0.0;

  Control<double> control; 
  State<double> state; 
  SystemModel<double> systemModel;
    
  state.setZero();

  KinematicObservationModel<double> kinematicObservationModel(
      0.0, 0.0,  0.0, 0.0);


  // To dump data structures into a CSV file, you create an output file first.
  // std::ofstream fout("../Exp_data/output.csv");
  std::ofstream fout_ekfState("output_ekf.csv");
  fout_ekfState << "%HEADER: Output of the Extended Kalman Filter, data format : \n"
                << "%timestamp (s), ground truth: x (m),  y (m), theta (rad), theta_dot(rad/s), commands : velocity (m/s) steering angle (rad), noisy data: x (m), y (m), theta (rad), theta_dot (rad/s), ekf estimation vector: x (m), x_dot (m/s), y (m), y_dot (ms), theta (rad), theta_dot(rad/s)  \n"
                << "%t lat long yaw long_vel wheels_angle Z_x Z_y Z_theta Z_theta_dot HAS_DATA X_x X_x_dot X_y X_y_dot X_theta X_theta_dot sent_lat sent_long sent_alt sent_heading " << endl;


  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == 
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    auto gpsReadingContainer = getKeyValueDataStore().get(
        opendlv::proxy::GpsReading::ID());
    auto gpsReading = gpsReadingContainer.getData<opendlv::proxy::GpsReading>();

//    std::cout << getName() << "\tLatidude  =  " << gpsReading.getLatitude()
//              << "  Longitude  =  " << gpsReading.getLongitude() << std::endl;

    if (gpsReadingContainer.getReceivedTimeStamp().toMicroseconds() > 0) {
      if (!hasGpsReference) {
        gpsReference = opendlv::data::environment::WGS84Coordinate(
            gpsReading.getLatitude(),
            opendlv::data::environment::WGS84Coordinate::NORTH,
            gpsReading.getLongitude(),
            opendlv::data::environment::WGS84Coordinate::EAST);
        hasGpsReference = true;

        // TODO: now this variable is set only once using the first data, it is
        // necessary to write a function able to reset this value to a new
        // reference frame.
        previousTimestep = gpsReadingContainer.getReceivedTimeStamp();
      }

      odcore::data::TimeStamp thisTimestep;
      odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
      previousTimestep = thisTimestep;


      auto propulsionContainer = getKeyValueDataStore().get(
          opendlv::proxy::reverefh16::Propulsion::ID());
      auto propulsion = propulsionContainer.getData<
          opendlv::proxy::reverefh16::Propulsion>();

      if (propulsionContainer.getReceivedTimeStamp().getSeconds() > 0) {
        control.v() = propulsion.getPropulsionShaftVehicleSpeed();
      }

      // if we don't get any data from the CAN, we try to fill the speed from GPS data
      auto gpsSpeed = gpsReading.getSpeed();
      if (propulsion.getPropulsionShaftVehicleSpeed() < 0.00001) control.v() = gpsSpeed;


      auto steeringContainer = getKeyValueDataStore().get(
          opendlv::proxy::reverefh16::Steering::ID());
      auto steering = steeringContainer.getData<
          opendlv::proxy::reverefh16::Steering>();

      auto vehicleStateContainer = getKeyValueDataStore().get(
                  opendlv::proxy::reverefh16::VehicleState::ID());
      auto vehicleState = vehicleStateContainer.getData<
              opendlv::proxy::reverefh16::VehicleState>();
      double vehicleYawRate = vehicleState.getYawRate();


      if (steeringContainer.getReceivedTimeStamp().getSeconds() > 0) {
        control.phi() = steering.getRoadwheelangle();
      }

      std::cout   << getName() << "\t" << "timestamp="
        << timestamp << "\t control: steering wheel angle = "  << vehicleState.getYawRate()
        << "  vel " << propulsion.getPropulsionShaftVehicleSpeed()
        << std::endl;
      std::cout   << getName() << "\t" << "timestamp="
        << timestamp << "\t original GPS data  "
        << gpsReading.getLatitude() << "  vel "
        << gpsReading.getLongitude() << " altitude "
        << gpsReading.getAltitude() << std::endl;

      opendlv::data::environment::WGS84Coordinate currentLocation(
          gpsReading.getLatitude(),
          opendlv::data::environment::WGS84Coordinate::NORTH,
          gpsReading.getLongitude(),
          opendlv::data::environment::WGS84Coordinate::EAST);

      opendlv::data::environment::Point3 currentCartesianLocation = 
          gpsReference.transform(currentLocation);

        //kinematic kalman block
        KinematicObservationVector<double> observationVector = kinematicObservationModel.h(state);
        observationVector.Z_x() = currentCartesianLocation.getX();
        observationVector.Z_y() = currentCartesianLocation.getY();
        if (gpsReading.getHasHeading()) {
          observationVector.Z_theta() = gpsReading.getNorthHeading();
        } else {
          observationVector.Z_theta() = state.theta();
        }
        observationVector.Z_theta_dot() = vehicleYawRate; // TODO: Put yaw rate here...

      
        double deltaTime = duration.toMicroseconds() / 1000000.0;
        systemModel.updateDeltaT(deltaTime);

      
        state = m_ekf.predict(systemModel, control);

        bool gpsHasData = false;
        if (gpsReadingContainer.getReceivedTimeStamp().toMicroseconds() >
            previousDataTimestamp.toMicroseconds()) {
          state = m_ekf.update(kinematicObservationModel, observationVector);
          previousDataTimestamp = gpsReadingContainer.getReceivedTimeStamp();
          gpsHasData = true;
        }

        timestamp += systemModel.getDeltaT();

        std::cout   << getName() << "\t" << "timestamp="
          << timestamp << "\t hasData=" << gpsHasData << "\tx = " << state.x() << ", y = " <<
          state.y() << ", theta = " << state.theta() << std::endl;





 /*       auto cov = m_ekf.getCovariance();

        auto cov_eigenvalues = cov.eigenvalues();

        auto cov_diagonal = cov.diagonal();

        auto cov_determinant = cov.determinant();

        std::cout   << getName() << "\t"
                    << "Covariance Matrix = \n" << cov
                    << "Eigenvalues = \n" << cov_eigenvalues
                    << "Diagonal = \n " << cov_diagonal
                    << "Determinand =   " << cov_determinant
                    << std::endl;
*/





      // Build the proper GPS coordinates to send
      opendlv::data::environment::Point3 currentStateEstimation
              (state.x(), state.y(), currentCartesianLocation.getZ());
      opendlv::data::environment::WGS84Coordinate currentWGS84CoordinateEstimation =
              gpsReference.transform(currentStateEstimation);
      double heading = state.theta();

      std::cout   << getName() << "\t" << "timestamp="
        << timestamp << "\t "
        << "\tlat=" << currentWGS84CoordinateEstimation.getLatitude()
        << ", long=" << currentWGS84CoordinateEstimation.getLongitude()
        << ", theta=" << state.theta() << std::endl;

      // Send the message
      opendlv::sensation::Geolocation geoLocationEstimation(currentWGS84CoordinateEstimation.getLatitude(),
                                                            currentWGS84CoordinateEstimation.getLongitude(),
                                                            gpsReading.getAltitude(),
                                                            heading);
      odcore::data::Container msg(geoLocationEstimation);
      getConference().send(msg);

      //save data to file
      bool   saveToFile = true;
      if (  saveToFile){
      fout_ekfState << timestamp << " "
                    << gpsReading.getLatitude() << " " << gpsReading.getLongitude() << " " << gpsReading.getNorthHeading() <<  " "
                    << control.v() << " " << control.phi() << " "
                    << observationVector.Z_x() << " " << observationVector.Z_y() << " " << observationVector.Z_theta() << " " << observationVector.Z_theta_dot() << " " << gpsHasData << " "
                    << state.x() << " " << state.x_dot() << " "  << state.y() << " " << state.y_dot() << " " << state.theta() << " " << state.theta_dot() << " "
                    << currentWGS84CoordinateEstimation.getLatitude() <<  currentWGS84CoordinateEstimation.getLongitude() << gpsReading.getAltitude() << heading
                    << endl;
      }


    }
    else   {
        std::cout << getName() << "\t"<< " NO DATA " << std::endl;
    }

  }
   
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;

}

void Geolocation::setUp()
{
}

void Geolocation::tearDown()
{
}

} // geolocation
} // sensation
} // opendlv
