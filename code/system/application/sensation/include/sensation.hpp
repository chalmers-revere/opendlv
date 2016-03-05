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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SENSATION_HPP_
#define SENSATION_HPP_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

//include headers for the EKF

#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic push
# endif
# pragma GCC diagnostic ignored "-Weffc++"
# pragma GCC diagnostic ignored "-Wshadow"
#endif
    #include "ExtendedKalmanFilter.hpp"
    #include "Types.hpp"
    #include "truckModel.hpp"
    #include "observationModel.hpp"
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic pop
# endif
#endif


namespace opendlv {
namespace system {
namespace application {
namespace sensation{



/**
 * This class provides...
 */
class Sensation : public odcore::base::module::TimeTriggeredConferenceClientModule {
  public:
    Sensation(int32_t const &, char **);
    Sensation(Sensation const &) = delete;
    Sensation &operator=(Sensation const &) = delete;
    virtual ~Sensation();

  /**
    * Body of the sensation function, all sensing function will be run from here
    * TODO: Add description !
    */
    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  /**
    * use this to run the VSE in test, _run_vse_test = true -- add noise generator,   _run_vse_test = no noise generator
    */
    inline void runVSEtest (bool _run_vse_test) {run_vse_test = _run_vse_test; };

  private:
      // This method will be call automatically _before_ running body().
    // add details !
    virtual void setUp();

    // This method will be call automatically _after_ return from body().
    // add details !
    virtual void tearDown();


  /**  Initialize the Extended Kalman filter for the vehicle state estimator,
    *  this method should be called only once, maybe after, or inside, the constructor.
    */
    void initializeEKF();

    void vehicleStateEstimator( opendlv::system::application::sensation::truckKinematicModel::Control<double> _u,
                                opendlv::system::application::sensation::truckKinematicModel::State<double> _x );

   opendlv::system::application::sensation::truckKinematicModel::State<double> x;   ///--> state vector for our model
   opendlv::system::application::sensation::truckKinematicModel::Control<double> u;  ///--> input vector for the truck
   opendlv::system::application::sensation::truckKinematicModel::SystemModel<double> sys;  ///--> system model

   opendlv::system::application::sensation::observationModel::PositionMeasurement<double> PositionMeasurement;       ///--> x,y position vector
   opendlv::system::application::sensation::observationModel::OrientationMeasurement<double> OrientationMeasurement; ///--> theta orientation
   opendlv::system::application::sensation::observationModel::PositionMeasurementModel<double> PositionModel;
   opendlv::system::application::sensation::observationModel::OrientationMeasurementModel<double> OrientationModel;

   opendlv::system::libs::kalman::ExtendedKalmanFilter< opendlv::system::application::sensation::truckKinematicModel::State<double> > m_ekf;   // extended kalman filter


    // Random number generation (for noise simulation)
    std::default_random_engine generator;
    std::normal_distribution<double> noise;//(0, 1);

    double systemNoise; ///--> Standard-Deviation of noise added to all state vector components during state transition = 0 by default //0.1;
    double orientationNoise;///--> Standard-Deviation of noise added to all measurement vector components in orientation measurements = 0 by default //0.0025;
    double distanceNoise;///--> Standard-Deviation of noise added to all measurement vector components in distance measurements = 0; by default //0.025;

    bool run_vse_test;    ///--> false by default, if true it enables the noise generator to add noise on measurements to test the VSE
    bool EKF_initialized; ///--> false by default, it is set to true after the initialization

};
} // sensation
} // application
} // system
} // opendlv

#endif
