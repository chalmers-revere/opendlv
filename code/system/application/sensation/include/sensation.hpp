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

#ifndef SENSATION_SENSATION_HPP_
#define SENSATION_SENSATION_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendlv/data/environment/WGS84Coordinate.h>

//include headers for the EKF


// disable the maximum warning level for some headers
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic push
# endif
# pragma GCC diagnostic ignored "-Weffc++"
# pragma GCC diagnostic ignored "-Wshadow"
#endif
    #include "ExtendedKalmanFilter.hpp"
    #include "Types.hpp"
    #include "vehicleModels/truckModel.hpp"
    #include "vehicleModels/truckObservationModel.hpp"
    #include "vehicleModels/combinedTruckModel.hpp"
    #include "vehicleModels/combinedTruckObservationModel.hpp"
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic pop
# endif
#endif


namespace opendlv {
namespace system {
namespace application {
namespace sensation {

/**
  *  @brief Sensation -
  *
  *  DESCRIBE the sensation concept:
  *
  *  Inside the sensation, the vehicle state estimator will take as imput the
  *  commands (FROM ACTION ???)  and measurements (FROM PERCEPTION ??? ),
  *  and provide an estimation of the state of the system.
  *
  *  The implementation of an Extended Kalman Filtering for a vehicle
  *  pose estimation follows the classical equations and notation
  *  which can be found everywhere on the web.
  *
  *  State Equation:
  *      X(k+1) = f(X(k),u(k)) + w(k)
  *      the state X has the dimension N-by-1
  *      the input U has the dimension N-by-1
  *      w ~ N(0,Q) is gaussian noise with covariance Q
  *
  *  Observation Equation:
  *      Z(k) = h(X(k)) + v(k)
  *      the observation Z has the dimension M-by-1
  *      v ~ N(0,R) is gaussian noise with covariance R
  *
  *  @param [in] control signals in current time-step
  *  @param [in] current measurements in the current time-step
  *  @param [in] the structure of the system
  *  @returns The (estimated) system state in the next time-step
  */
//    m_tkmState vehicleStateEstimator( m_tkmControl _U, m_tkmObservationVector _Z, m_tkmSys _sys );
class Sensation : public odcore::base::module::TimeTriggeredConferenceClientModule {
  public:
    Sensation(int32_t const &, char **);
    Sensation(Sensation const &) = delete;
    Sensation &operator=(Sensation const &) = delete;
    virtual ~Sensation();
    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  /**
    * use this to run the VSE in test, _run_vse_test = true -- add noise generator,   _run_vse_test = no noise generator
    */
    inline void runVSEtest (bool _run_vse_test) {run_vse_test = _run_vse_test; }

  private:
      // This method will be call automatically _before_ running body().
    // add details !
    virtual void setUp();

    // This method will be call automatically _after_ return from body().
    // add details !
    virtual void tearDown();

    //
//    void GPSreferenceReset(opendlv::data::environment::WGS84Coordinate _newGPSreference){ m_GPSreference = _newGPSreference;}

    // define some shourtcut to be used in the file --- tkm = Truck Kinematic Model
    typedef opendlv::system::application::sensation::truckKinematicModel::State<double> m_tkmState;     ///--> def truck kinematic model state space
    typedef opendlv::system::application::sensation::truckKinematicModel::Control<double> m_tkmControl; ///--> def truck kinematic model control space
    typedef opendlv::system::application::sensation::truckKinematicModel::SystemModel<double> m_tkmSys; ///--> def truck kinematic model system
    typedef opendlv::system::application::sensation::truckObservationModel::truckObservationModel<double> m_tkmObservationModel;
    typedef opendlv::system::application::sensation::truckObservationModel::truckObservationVector<double> m_tkmObservationVector;

    // define some shourtcut to be used in the file --- tdm = Truck dynamic Model
    typedef opendlv::system::application::sensation::combinedTruckModel::State<double> m_tdmState;     ///--> def truck dynamic model state space
    typedef opendlv::system::application::sensation::combinedTruckModel::Control<double> m_tdmControl; ///--> def truck dynamic model control space
    typedef opendlv::system::application::sensation::combinedTruckModel::SystemModel<double> m_tdmSys; ///--> def truck dynamic model system
    typedef opendlv::system::application::sensation::combinedTruckObservationModel::combinedTruckObservationModel<double> m_tdmObservationModel;
    typedef opendlv::system::application::sensation::combinedTruckObservationModel::combinedTruckObservationVector<double> m_tdmObservationVector;

  /**  Initialize the Extended Kalman filter for the vehicle state estimator,
    *  this method should be called only once, maybe after, or inside, the constructor.
    */
    void initializeEKF();

   m_tkmState X;    ///--> state vector for our model
   m_tkmControl U;  ///--> input vector for the truck
   m_tkmSys sys;    ///--> system model



   m_tdmState Xdyn;    ///--> state vector for our model
   m_tdmControl Udyn;  ///--> input vector for the truck
   m_tdmSys sys_dyn;    ///--> system model


   m_tkmObservationModel observationModel;  ///--> observation model
   m_tdmObservationModel dynObservationModel; ///--> observation model


   opendlv::system::libs::kalman::ExtendedKalmanFilter< opendlv::system::application::sensation::truckKinematicModel::State<double> > m_ekf;   // extended kalman filter
   opendlv::system::libs::kalman::ExtendedKalmanFilter< opendlv::system::application::sensation::combinedTruckModel::State<double> > m_dyn_ekf; // extended kalman filter


    // Random number generation (for noise simulation)
    std::default_random_engine generator;   ///--> set the random engine to generate noise
    std::normal_distribution<double> noise; ///--> generate a guassian distributed noise with N~(0, 1);

    double systemNoise;               ///--> Standard-Deviation of noise added to all state vector components during state transition = 0 by default //0.1;
    double measurementNoise_x;        ///--> Standard-Deviation of noise added to the measurement vector component 'x', longitudinal direction. Default value 0.05;
    double measurementNoise_y;        ///--> Standard-Deviation of noise added to the measurement vector component 'y', lateral direction. Default value 0.5;
    double measurementNoise_yaw;      ///--> Standard-Deviation of noise added to the measurement vector component 'yaw', truck orientation. Default value 0.001;
    double measurementNoise_yawRate;  ///--> Standard-Deviation of noise added to the measurement vector component 'yaw', truck orientation. Default value 0.0001;

    bool run_vse_test;        ///--> false by default, if true it enables the noise generator to add noise on measurements to test the VSE
    bool m_saveToFile;        ///--> false by default, set to true save logs to file
    bool EKF_initialized;     ///--> false by default, it is set to true after the initialization
    odcore::data::TimeStamp   m_timeBefore;  ///--> contains the time in microsecons between two consecutive steps of EKF, default value = 0.05s -- 20 Hz
    odcore::data::TimeStamp   m_timeNow;     ///--> contains the time in microsecons between two consecutive steps of EKF, default value = 0.05s -- 20 Hz

    opendlv::data::environment::WGS84Coordinate m_GPSreference;
    bool GPSreferenceSET;      ///--> false by default, set to true when the variable m_GPSreference is set to a new reference poit considered as origin of the axis

};
} // sensation
} // application
} // system
} // opendlv

#endif
