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

#include <cmath>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>


#include <odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h>

#include "geolocation.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Geolocation::Geolocation(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocation")
    , m_gpsReadingMutex()
    , m_ekf()
    , m_gpsReading()
    , m_magnetometerReading()
    , m_accelerometerReading()
    , m_steeringReading()
    , m_propulsionReading()
    , m_debug()
    , m_initialised(false)
{
  m_gpsReading = opendlv::data::environment::WGS84Coordinate();
  m_magnetometerReading = opendlv::proxy::MagnetometerReading();
  m_accelerometerReading = opendlv::proxy::AccelerometerReading(0,0,-9.82);
}

Geolocation::~Geolocation()
{
}

void Geolocation::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = (kv.getValue< int32_t >("logic-sensation-geolocation.debug") == 1);
  std::cout << "["<< getName() << "]: Geolocation module has started." << std::endl;
  m_initialised = true;
}

void Geolocation::tearDown()
{
}

void Geolocation::nextContainer(odcore::data::Container &a_c)
{
  if(!m_initialised) {
    return;
  }
  if(a_c.getDataType() == opendlv::data::environment::WGS84Coordinate::ID()) {
    if (m_debug) {
      std::cout << "Got WGS84Coordinate." << std::endl;
    }
    odcore::base::Lock l(m_gpsReadingMutex);
    m_gpsReading = a_c.getData<opendlv::data::environment::WGS84Coordinate>();
  } else if(a_c.getDataType() == opendlv::proxy::MagnetometerReading::ID()) {
    m_magnetometerReading = a_c.getData<opendlv::proxy::MagnetometerReading>();
  } else if(a_c.getDataType() == opendlv::proxy::AccelerometerReading::ID()) {
    m_accelerometerReading = a_c.getData<opendlv::proxy::AccelerometerReading>();
  } else if(a_c.getDataType() == opendlv::proxy::reverefh16::Steering::ID()) {
    m_steeringReading = a_c.getData<opendlv::proxy::reverefh16::Steering>();
  } else if(a_c.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
    m_propulsionReading = a_c.getData<opendlv::proxy::reverefh16::Propulsion>();
  }
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  
  // opendlv::data::environment::WGS84Coordinate gpsReference;
  // bool hasGpsReference = false;

  // odcore::data::TimeStamp previousTimestep;
  // odcore::data::TimeStamp previousDataTimestamp;
  // double velocityBefore = 0.0;

  // opendlv::data::environment::Point3 locationBefore(0.0, 0.0, 0.0);

  // double timestamp = 0.0;
  // double travelDistance = 0.0;

  // Control<double> control;

  // SystemModel<double> systemModel;

  // State<double> state;
  // state.setZero();
  // m_ekf.init(state);

  // KinematicObservationModel<double> kinematicObservationModel(0.0, 0.0, 0.0);

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
    odcore::base::Lock l(m_gpsReadingMutex);
    double latitude = m_gpsReading.getLatitude();
    double longitude = m_gpsReading.getLongitude();
    if (m_debug) {
      std::cout << " Latitude: " << latitude << std::endl;
      std::cout << " Longitude: " << longitude << std::endl; 
    }

//    float altitude = static_cast<float>(m_gpsReading.getAltitude());
    float altitude = 0.0f; // TODO: Use a GPS format with altitude, for example GpsReading, which is also available from all GPSs.
    float positionConfidence = 0.0f;

    // float *tempAcc = m_accelerometerReading.getAcceleration();
    float acc[3] = {m_accelerometerReading.getAccelerationX(),m_accelerometerReading.getAccelerationY(),m_accelerometerReading.getAccelerationZ()};
    // float *tempMag = m_magnetometerReading.getMagneticField();
    float magneticField[3] = {m_magnetometerReading.getMagneticFieldX(),m_magnetometerReading.getMagneticFieldY(),m_magnetometerReading.getMagneticFieldZ()};

    // Tilt compensation
    float roll = atan2(acc[1],acc[2])+static_cast<float>(M_PI);
    while (roll < -static_cast<float>(M_PI)) {
      roll += 2*static_cast<float>(M_PI);
    }
    while (roll > static_cast<float>(M_PI)) {
      roll -= 2*static_cast<float>(M_PI);
    }

    float pitch = atan2(-acc[0], sqrt(acc[1]*acc[1]+acc[2]*acc[2]));
    
    // float roll = atan2(acc[1],acc[2]);
    // float pitch = atan2(-acc[0], acc[1]*sinf(roll)+acc[2]*cosf(roll));
    // float roll = asinf(-acc[0]);
    // float pitch = asinf(acc[1]/cosf(roll));

    float heading = static_cast<float>(atan2(magneticField[1],magneticField[0]));
    if(m_debug){
      std::cout << "Heading: " << 180*heading / static_cast<float>(M_PI) << std::endl;
    }

    magneticField[0] = magneticField[0]*cosf(pitch)+magneticField[2]*sinf(pitch);
    magneticField[1] = magneticField[0]*sinf(pitch)*sinf(roll) + magneticField[1]*cosf(roll) - magneticField[2]*sinf(roll)*cosf(pitch);
    magneticField[2] = -magneticField[0]*cosf(roll)*sinf(pitch) + magneticField[1]*sinf(roll) + magneticField[2]*cosf(roll)*cosf(pitch);
    
    heading = static_cast<float>(atan2(magneticField[1],magneticField[0]));
    if(m_debug){
      std::cout << "Tilt compensation: "<< 180*heading / static_cast<float>(M_PI) << " (Pitch, Roll): " << pitch << "," << roll <<std::endl;
    }

    float headingConfidence = 0.0f;
    


    opendlv::sensation::Geolocation geolocation(latitude,
        positionConfidence,
        longitude,
        positionConfidence,
        altitude,
        heading,  
        headingConfidence);
      
    odcore::data::Container msg(geolocation);
    getConference().send(msg);

    if(m_debug){
      std::cout << geolocation.toString() << std::endl;
    }

    float const v = static_cast<float>(m_propulsionReading.getPropulsionShaftVehicleSpeed()/3.6);

    opendlv::model::Cartesian3 velocity(v, 0.0f, 0.0f);
    // opendlv::model::Cartesian3 velocity(gpsReading.getSpeed(), 0.0f, 0.0f);
    opendlv::model::Cartesian3 velocityConfidence(0.0f, 0.0f, 0.0f);

    opendlv::model::Cartesian3 acceleration(0.0f , 0.0f,0.0f);
    opendlv::model::Cartesian3 accelerationConfidence(0.0f, 0.0f, 0.0f);
    
    opendlv::model::Cartesian3 angularVelocity(0.0f, 0.0f, 0.0f);
    opendlv::model::Cartesian3 angularVelocityConfidence(0.0f, 0.0f, 0.0f);

    opendlv::model::Cartesian3 angularAcceleration(0.0f, 0.0f, 0.0f);
    opendlv::model::Cartesian3 angularAccelerationConfidence(0.0f, 0.0f, 0.0f);


    int16_t frameId = 0;

    opendlv::model::DynamicState dynamicState(velocity, velocityConfidence,
        acceleration, accelerationConfidence, angularVelocity,
        angularVelocityConfidence, angularAcceleration,
        angularAccelerationConfidence, frameId);

    odcore::data::Container dynamicStateContainer(dynamicState);
    getConference().send(dynamicStateContainer);

  /*
    odcore::data::TimeStamp now;


    auto gpsReadingContainer =
        getKeyValueDataStore().get(opendlv::core::sensors::trimble::GpsReading::ID());
    auto gpsReading = gpsReadingContainer.getData<opendlv::core::sensors::trimble::GpsReading>();

    if (gpsReadingContainer.getReceivedTimeStamp().toMicroseconds() > 0) {
      if (!hasGpsReference) {
        gpsReference = opendlv::data::environment::WGS84Coordinate(
            gpsReading.getLatitude(),
            gpsReading.getLongitude());
        hasGpsReference = true;

        // TODO: now this variable is set only once using the first data, it is
        // necessary to write a function able to reset this value to a new
        // reference frame.
        previousTimestep = gpsReadingContainer.getReceivedTimeStamp();
      }


      odcore::data::TimeStamp thisTimestep;
      odcore::data::TimeStamp duration = thisTimestep - previousTimestep;
      previousTimestep = thisTimestep;

      odcore::data::TimeStamp timeAfterGpsReading;
      odcore::data::TimeStamp durationAfterGpsReading = timeAfterGpsReading - now;

      auto propulsionContainer = getKeyValueDataStore().get(
          opendlv::proxy::reverefh16::Propulsion::ID());
      auto propulsion = propulsionContainer.getData<
          opendlv::proxy::reverefh16::Propulsion>();

      if (propulsionContainer.getReceivedTimeStamp().getSeconds() > 0) {
        control.v() = propulsion.getVehicleSpeedShaftVehicleSpeed()/3.6;
        // TODO: to m/s --- get the message in si unit
      }


      if (propulsion.getVehicleSpeedShaftVehicleSpeed() < 0.1) {
          control.v() = 0.0;
      // if we don't get any data from the CAN,
      // we try to fill the speed from GPS data
        // auto gpsSpeed = gpsReading.getSpeed();
        // if (gpsSpeed > 1.0 ){
        // control.v() = gpsSpeed;}
      }

      auto steeringContainer = getKeyValueDataStore().get(
          opendlv::proxy::reverefh16::Steering::ID());
      auto steering = steeringContainer.getData<
          opendlv::proxy::reverefh16::Steering>();

      //auto vehicleStateContainer = getKeyValueDataStore().get(
      //            opendlv::proxy::reverefh16::VehicleState::ID());
      //auto vehicleState = vehicleStateContainer.getData<
      //        opendlv::proxy::reverefh16::VehicleState>();
      // double longitudinalAcceleration = vehicleState.getAccelerationX();
      double longitudinalAcceleration = 0.0;
      //double lateralAcceleration = vehicleState.getAccelerationY();  //TODO: do we need lateral acceleration?
      double longitudinalAccelerationConfidence = -1;

      if (steeringContainer.getReceivedTimeStamp().getSeconds() > 0) {
        control.phi() = steering.getRoadwheelangle()/m_steeringToWheelRatio;
      }
      odcore::data::TimeStamp timeAfterCanReading;
      odcore::data::TimeStamp durationAfterCanReading = timeAfterCanReading - now;

      opendlv::data::environment::WGS84Coordinate currentLocation(
          gpsReading.getLatitude(),
          gpsReading.getLongitude());

      odcore::data::TimeStamp timeBeforeConversionGps2Point;
      opendlv::data::environment::Point3 currentCartesianLocation =
        gpsReference.transform(currentLocation);
      odcore::data::TimeStamp timeAfterConversionGps2Point;
       odcore::data::TimeStamp durationAfterConversionGps2Point = timeAfterConversionGps2Point - timeBeforeConversionGps2Point;


      //kinematic kalman block
      KinematicObservationVector<double> observationVector =
          kinematicObservationModel.h(state);
      observationVector.Z_x() = currentCartesianLocation.getX();
      observationVector.Z_y() = currentCartesianLocation.getY();
      observationVector.Z_theta() = gpsReading.getNorthHeading();
      // if (gpsReading.getHasHeading()) {
      //  observationVector.Z_theta() = gpsReading.getNorthHeading();
      // }
      // else {
      //  observationVector.Z_theta() = state.theta();
      // }


      double deltaTime = duration.toMicroseconds() / 1000000.0; //in sec
      systemModel.updateDeltaT(deltaTime);

      odcore::data::TimeStamp timeBeforeEkf;
      odcore::data::TimeStamp durationBeforeEkf = timeBeforeEkf - now;

      state = m_ekf.predict(systemModel, control);

      // bool gpsHasData = false;
      if (gpsReadingContainer.getReceivedTimeStamp().toMicroseconds() >
          previousDataTimestamp.toMicroseconds()) {

        state = m_ekf.update(kinematicObservationModel, observationVector);
        previousDataTimestamp = gpsReadingContainer.getReceivedTimeStamp();
        // gpsHasData = true;
      }
      opendlv::data::environment::Point3 locationNow (state.x(), state.y(), 0.0);

      double distanceP2P = locationNow.getDistanceTo(locationBefore);
      travelDistance = travelDistance + distanceP2P;
      locationBefore = locationNow;

      timestamp += systemModel.getDeltaT();

      //TODO: convert here x and y to get the position of the rear axle
      opendlv::data::environment::WGS84Coordinate currentWGS84CoordinateEstimation;
      bool ekfSuccess=true;
      // if one of the coordinates is nan, it gives the last GPS available measure
      if(std::isfinite(state.x()) && std::isfinite(state.y())) {
        // Build the proper GPS coordinates to send
        opendlv::data::environment::Point3 currentStateEstimation
                (state.x() + m_gpsToCoGDisplacement[0],
                 state.y() + m_gpsToCoGDisplacement[1],
                 currentCartesianLocation.getZ() + m_gpsToCoGDisplacement[2]);

        currentWGS84CoordinateEstimation = gpsReference.transform(currentStateEstimation);
      } else {
        currentWGS84CoordinateEstimation = currentLocation;
        std::cout << "NAN - sending gps data instead"<<std::endl;
        filterReset( currentCartesianLocation, gpsReading);
        state = m_ekf.getState();
        ekfSuccess = false;
      }



      double heading = 0.0;
      if (std::isfinite(state.theta()) ) {
        // heading = state.theta() - 
        //           std::ceil((state.theta()-M_PI)/(M_PI)) * (M_PI);  // make sure that the heading is in [0,2pi]
        heading = std::fmod(std::abs(state.theta()),2*M_PI);
      } else {
        filterReset( currentCartesianLocation, gpsReading);
        state = m_ekf.getState();
        if (std::isfinite(gpsReading.getNorthHeading())) {
          // heading = gpsReading.getNorthHeading() - 
          //        std::ceil((gpsReading.getNorthHeading()-M_PI)/(M_PI)) * (M_PI);  // make sure that the heading is in [0,+2pi]
          heading = std::fmod(std::abs(gpsReading.getNorthHeading()),2*M_PI);
        } else {
          heading = 0.0;
        } 
        ekfSuccess = false;
      }
      heading = std::fmod(std::abs(gpsReading.getNorthHeading()),2*M_PI);

      double positionConfidence = calculatePositionConfidence(ekfSuccess);
      double headingConfidence = calculateHeadingConfidence(ekfSuccess);
      double speedConfidence = calculateSpeedConfidence(ekfSuccess);
      double yawRate = state.theta_dot();
      double yawRateConfidence = calculateHeadingConfidence(ekfSuccess);


      longitudinalAcceleration = (control.v() - velocityBefore)/systemModel.getDeltaT();
      velocityBefore = control.v();


      // Send the message
      opendlv::sensation::Geolocation geoLocationEstimation(gpsReading.getLatitude(),
                                                            //currentWGS84CoordinateEstimation.getLatitude(),
                                                            positionConfidence,
                                                            gpsReading.getLongitude(),
                                                            //currentWGS84CoordinateEstimation.getLongitude(),
                                                            positionConfidence,
                                                            gpsReading.getAltitude(),
                                                            heading,  
                                                            headingConfidence
                                                            );
      
      odcore::data::Container msg(geoLocationEstimation);
      getConference().send(msg);

      opendlv::knowledge::Insight msgTravelDistance(now, "distanceTravelled="+std::to_string(travelDistance));
      odcore::data::Container containerTravelDistance(msgTravelDistance);
      getConference().send(containerTravelDistance);


      // TODO: This should be sent from another module = sensation/geolocation 
      // should be split in two as discussed previously
      opendlv::model::Cartesian3 velocity(control.v(), 0.0f, 0.0f);
      // opendlv::model::Cartesian3 velocity(gpsReading.getSpeed(), 0.0f, 0.0f);
      opendlv::model::Cartesian3 velocityConfidence(speedConfidence, 0.0f, 
          0.0f);

      opendlv::model::Cartesian3 acceleration(longitudinalAcceleration/3.6, 0.0f, 
          0.0f);
      opendlv::model::Cartesian3 accelerationConfidence(
          longitudinalAccelerationConfidence, 0.0f, 0.0f);
      
      opendlv::model::Cartesian3 angularVelocity(0.0f, 0.0f, yawRate);
      opendlv::model::Cartesian3 angularVelocityConfidence(0.0f, 0.0f,
          yawRateConfidence);

      opendlv::model::Cartesian3 angularAcceleration(0.0f, 0.0f, 0.0f);
      opendlv::model::Cartesian3 angularAccelerationConfidence(0.0f, 0.0f,
          0.0f);

      int16_t frameId = 0;

      opendlv::model::DynamicState dynamicState(velocity, velocityConfidence,
          acceleration, accelerationConfidence, angularVelocity,
          angularVelocityConfidence, angularAcceleration,
          angularAccelerationConfidence, frameId);

      odcore::data::Container dynamicStateContainer(dynamicState);
      getConference().send(dynamicStateContainer);



      // std::cout << getName()
      //           << "\n   Latitude  =  " << gpsReading.getLatitude()
      //           << "\n   Longitude  =  " << gpsReading.getLongitude()
      //           << "\n   North Heading = " << gpsReading.getNorthHeading()
      //           << "\nControl vector : \n   vel = " << control.v()
      //           << "\n   steering = " << control.phi()
      //           << "\nObservation vector : \n   x = " << observationVector.Z_x()
      //           << "\n   y = " << observationVector.Z_y()
      //           << "\n   northHeading = " << observationVector.Z_theta()
      //           << "\n Timing: "
      //           << "\nEkf time elapsed (microseconds) = " << ekfTimeElapsed.toMicroseconds()
      //           << "\n   durationBeforeEkf = " << durationBeforeEkf.toMicroseconds()
      //           << "\n   durationAfterEkf = " << durationAfterEkf.toMicroseconds()
      //           << "\n   durationAfterGpsReading  = " << durationAfterGpsReading.toMicroseconds()
      //           << "\n   durationAfterCanReading  = " << durationAfterCanReading.toMicroseconds()
      //           << "\n   durationAfterConversionGps2Point  =  " << durationAfterConversionGps2Point.toMicroseconds()
      //           << "\n   durationAfterConversionPoint2Gps  =  " << durationAfterConversionPoint2Gps.toMicroseconds()
      //           << "\n   durationBeforeMessgeSending = " << durationBeforeMessgeSending.toMicroseconds()
      //           << "\n   timeAfterMessgeSending  = " << durationAfterMessageSending.toMicroseconds()
      //           << "\n   durationOfMessageSending  = " << durationOfMessageSending.toMicroseconds()
      //           << "\nTravel distance = " << travelDistance
      //           << "\nEstimation "
      //           << "\n   timestamp = " << timestamp
      //           << "\n   hasData = " << gpsHasData
      //           << "\n   x = " << state.x()
      //           << "\n   y = " << state.y()
      //           << "\n   theta = " << state.theta()
      //           << "\nSent message "
      //           << "\n   timestamp = " << timestamp << "\t " << std::setprecision(19)
      //           << "\n   latitude = " << currentWGS84CoordinateEstimation.getLatitude()
      //           << "\n   longitude = " << currentWGS84CoordinateEstimation.getLongitude()
      //           << "\n   northHeading =" << heading
      //           << "\n   longitudinalAcceleration " << longitudinalAcceleration
      //           << "\n\n" <<  std::endl;
      
    }
    */
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}




double Geolocation::calculatePositionConfidence(bool& a_filterSuccess)
{
  if (!a_filterSuccess){
      return -1;
  }
  auto covSR = m_ekf.getCovariance();

  double positionConfidence_x = std::sqrt(std::abs(covSR(0,0)));
  double positionConfidence_y = std::sqrt(std::abs(covSR(2,2)));
//  return std::max(std::sqrt(positionConfidence_x),
//                  std::sqrt(positionConfidence_y));
  double confidence = std::sqrt(positionConfidence_x * positionConfidence_x +
                                positionConfidence_y * positionConfidence_y);
  return confidence;
}

double Geolocation::calculateHeadingConfidence(bool& a_filterSuccess)
{
  if (!a_filterSuccess){
      return -1;
  }
  auto covSR = m_ekf.getCovariance();
  double confidence = std::abs(covSR(4,4));
  return std::sqrt(confidence);
}

double Geolocation::calculateHeadingRateConfidence(bool& a_filterSuccess)
{
  if (!a_filterSuccess){
      return -1;
  }
  auto covSR = m_ekf.getCovariance();
  double confidence = std::abs(covSR(5,5));
  return std::sqrt(confidence);
}

double Geolocation::calculateSpeedConfidence(bool& a_filterSuccess)
{
  if (!a_filterSuccess){
      return -1;
  }
  auto covSR = m_ekf.getCovariance();
  double speedConfidence_x = std::sqrt(std::abs(covSR(1,1)));
  double speedConfidence_y = std::sqrt(std::abs(covSR(3,3)));
  double confidence = std::sqrt(speedConfidence_x * speedConfidence_x +
                                speedConfidence_y * speedConfidence_y);
  return confidence;
}


void Geolocation::filterReset(opendlv::data::environment::Point3& a_currentCartesianLocation,
                              opendlv::core::sensors::trimble::GpsReading& a_currentWGS84Location )
{


  State<double> state;
  state <<  a_currentCartesianLocation.getX(),
            0.0, //a_currentWGS84Location.getSpeed() * std::cos (a_currentWGS84Location.getNorthHeading()),
            a_currentCartesianLocation.getY(),
            0.0, //a_currentWGS84Location.getSpeed() * std::sin (a_currentWGS84Location.getNorthHeading()),
            a_currentWGS84Location.getNorthHeading(),
            0.0, //(a_currentWGS84Location.getSpeed() / 3.4 ) * std::tan(a_currentWGS84Location.getNorthHeading());

  m_ekf.init(state);
  m_ekf.setCovariance(Eigen::Matrix<double, 6, 6>::Identity());

}

}
}
}
