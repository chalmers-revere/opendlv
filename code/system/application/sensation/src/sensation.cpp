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

#include <chrono>
#include <fstream>
#include <iostream>

#include <opendavinci/odcore/data/Container.h>
#include <opendlv/data/environment/Point3.h>


#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendavinci/odcore/reflection/CSVFromVisitableVisitor.h"

#include "sensation.hpp"

namespace opendlv {
namespace system {
namespace application {
namespace sensation {



/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Sensation::Sensation(int32_t const &a_argc, char **a_argv) :
    TimeTriggeredConferenceClientModule(a_argc, a_argv, "sensation"),
    X(),
    U(),
    sys(),
    Xdyn(),
    Udyn(),
    sys_dyn(),
    observationModel(0.0, 0.0,  0.0, 0.0 ), // clarify the numbers !
    dynObservationModel(0.0, 0.0,  0.0, 0.0 ),
    m_ekf(),
    m_dyn_ekf(),
    generator(),
    noise(0, 1),
    systemNoise(0),
    measurementNoise_x(0.05),
    measurementNoise_y(0.5),
    measurementNoise_yaw(0.001),
    measurementNoise_yawRate(0.0001),
    run_vse_test(false),
    m_saveToFile (false),
    EKF_initialized(false),
    m_timeBefore(),
    m_timeNow(),
    m_GPSreference(57.71278, opendlv::data::environment::WGS84Coordinate::NORTH, 11.94581583, opendlv::data::environment::WGS84Coordinate::EAST), // initialize to some coordinate in Sweden
    GPSreferenceSET(false)

{
    // Exit if the supercomponent is not running  --- should this statement be in all constructors (ACT-PERCEPTION ect.) to prevent possible errors ??
    //if (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::NOT_RUNNING)
    //{
    //    std::cout << getName() << " odSupercomponent is not running - run first " << std::endl;
    //    return ;
    //}

    initializeEKF();

}

Sensation::~Sensation()
{
}

void Sensation::setUp()
{
  // This method will be call automatically _before_ running body().
    // TODO should all the initializations go here ?? including ekf
    // initializeEKF();
}

void Sensation::tearDown()
{
  // This method will be call automatically _after_ return from body().
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Sensation::body() {
    // Example on how to use the type WGS84Coordinate:

        using namespace opendlv::data::environment;






        // First, you need to declare a lat/lon coordinate to be used
        // as reference (i.e. origin (0, 0) of a Cartesian coordinate
        // frame); in our example, we use one located at AstaZero.
        //WGS84Coordinate reference(57.77284043, WGS84Coordinate::NORTH, 12.76996356, WGS84Coordinate::EAST);
          WGS84Coordinate reference(57.71278, WGS84Coordinate::NORTH, 11.94581583, WGS84Coordinate::EAST);
        // Let's assume you have another lat/lon coordinate at hand.
        WGS84Coordinate WGS84_p2(57.71278, WGS84Coordinate::NORTH, 11.94581583, WGS84Coordinate::EAST);

        // Now, you can transform this new lat/lon coordinate to the
        // previously specified Cartesian reference frame.
        Point3 cartesian_p2 = reference.transform(WGS84_p2);
        std::cout << "WGS84 reference: " << reference.toString()
                  << ", other WGS84 coordinate: " << WGS84_p2.toString()
                  << ", transformed cartesian coordinate: " << cartesian_p2.toString()
                  << std::endl;
        // You can access the X, Y coordinates (Z==0) as follows:
        double p2_x = cartesian_p2.getX();
        double p2_y = cartesian_p2.getY();
        std::cout << "X = " << p2_x << ", Y = " << p2_y << std::endl;


    // To dump data structures into a CSV file, you create an output file first.
    std::ofstream fout("../Exp_data/output.csv");
    std::ofstream fout_ekfState("../Exp_data/output_ekf.csv");
    fout_ekfState << "% HEADER: Output of the Extended Kalman Filter, data format : \n"
                  << "% timestamp (s), ground truth: x (m),  y (m), theta (rad), theta_dot(rad/s), commands : velocity (m/s) steering angle (rad), noisy data: x (m), y (m), theta (rad), theta_dot (rad/s), ekf estimation vector: x (m), x_dot (m/s), y (m), y_dot (ms), theta (rad), theta_dot(rad/s)  " << endl;

    // You can optionally dump a header (i.e. first line with information).
    const bool WITH_HEADER = true;
    // You can choose the delimiter character between the fields.
    const char DELIMITER = ',';
    // For every data structure that you want to export in a CSV file, you need to create a new CSVFromVisitableVisitor.
    odcore::reflection::CSVFromVisitableVisitor csvExporter1(fout, WITH_HEADER, DELIMITER);


    double delta_t = 0;
    double time_stamp = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        odcore::data::Container c1 = getKeyValueDataStore().get(opendlv::system::actuator::Commands::ID());
        opendlv::system::actuator::Commands commands = c1.getData<opendlv::system::actuator::Commands>();

        odcore::data::Container c2 = getKeyValueDataStore().get(opendlv::system::sensor::TruckLocation::ID());
        opendlv::system::sensor::TruckLocation truckLocation = c2.getData<opendlv::system::sensor::TruckLocation>();


        if (c1.getReceivedTimeStamp().getSeconds() > 0)//if we are actually getting data !
        {
            if ( !GPSreferenceSET )// the GPS reference is not set, set the GPSreference to the current position
            {
            m_GPSreference = opendlv::data::environment::WGS84Coordinate(truckLocation.getX(), WGS84Coordinate::NORTH, truckLocation.getY(), WGS84Coordinate::EAST);
            GPSreferenceSET = true;
            m_timeBefore = c1.getReceivedTimeStamp();
             Udyn.v_y() = truckLocation.getLat_acc();
            // TODO: now this variable is set only ones using the first data,
            //       it is necessary to write a function able to reset this value to a new reference frame
            }

        m_timeNow = c1.getReceivedTimeStamp();
        odcore::data::TimeStamp duration = m_timeNow - m_timeBefore;
        cout << getName() << ": <<message>> : time step in microseconds = " << duration.toMicroseconds() << endl;
        m_timeBefore = c1.getReceivedTimeStamp();
        delta_t = duration.toMicroseconds()/1000000.0;

        // let me out our signal for now to check if we are doing the right processing
        cout << getName() << ": " << commands.toString() << ", " << truckLocation.toString() << endl;

        // Try to convert coordinates
        WGS84Coordinate WGS84_ptruck(truckLocation.getX(), WGS84Coordinate::NORTH, truckLocation.getY(), WGS84Coordinate::EAST);
        Point3 _p2 = m_GPSreference.transform(WGS84_ptruck);

        // The csvExporter1 will "visit" the data structure "commands" and iterate
        // through its fields that will be stored in the output file fout.
        commands.accept(csvExporter1);

         // set the commands from the opendavinci to the ekf state space
         // cout << getName() << " << message >> \n   CONTROL SIGNALS : u.v = " << U.v() << "  u.phi  = " << U.phi() << endl;
         U.v() = commands.getLongitudinalVelocity();
         U.phi() = commands.getSteeringAngle();
         Udyn.v() = commands.getLongitudinalVelocity();
         Udyn.phi() = commands.getSteeringAngle();
         Udyn.yaw_rate() = truckLocation.getYawRate();
         Udyn.v_y() = (truckLocation.getLat_acc() - Udyn.v_y())/delta_t;


         // System measurements
         m_tkmObservationVector Z = observationModel.h(X);
         m_tdmObservationVector Zdyn = dynObservationModel.h(Xdyn);

         // set the commands from the opendavinci to the ekf state space
         Z.Z_x()         =   _p2.getX();//truckLocation.getX();
         Z.Z_y()         =   _p2.getY();//truckLocation.getY();
         Z.Z_theta()     =   truckLocation.getYaw();
         Z.Z_theta_dot( )=   truckLocation.getYawRate();
         Zdyn.Z_x()         =   _p2.getX();//truckLocation.getX();
         Zdyn.Z_y()         =   _p2.getY();//truckLocation.getY();
         Zdyn.Z_theta()     =   truckLocation.getYaw();
         Zdyn.Z_theta_dot( )=   truckLocation.getYawRate();
         //cout << getName() << " << message >> \n   MEASURES : " << " Z.Z_x()  = " << Z.Z_x() << " Z.Z_y()  = " << Z.Z_y()
         //                  << " Z.Z_theta()  = " << Z.Z_theta() << " Z.Z_theta_dot()  = " << Z.Z_theta_dot()  << endl;

run_vse_test = false;
         if (run_vse_test) // if run test is true we are running a test and it will add noise to the measures
         {
             Z.Z_x() += measurementNoise_x * noise(generator);
             Z.Z_y() += measurementNoise_y * noise(generator);
             Z.Z_theta() += measurementNoise_yaw * noise(generator);
             Z.Z_theta_dot() += measurementNoise_yawRate * noise(generator);
         }


         if (EKF_initialized) // if the filter is not initialze - initialize it first
         {
             std::cout << "Sensation::initializeEKF  << message >> Filter initialized " << std::endl;

             // update the timestamp - give the time in seconds
             sys.updateDeltaT(delta_t);
             sys_dyn.updateDeltaT(delta_t);

             // Predict state for current time-step using the filters
             X = m_ekf.predict(sys, U);
             Xdyn = m_dyn_ekf.predict(sys_dyn, Udyn);


             // update stage of the EKF
             X = m_ekf.update(observationModel, Z);
            Xdyn = m_dyn_ekf.update(dynObservationModel, Zdyn);

 time_stamp+=sys.getDeltaT();
         // Print to stdout as csv format
            std::cout   << getName() << " << message >> STATE \n"
                        << "timestamp = " << time_stamp << "\n"
                        << "           x " << X.x() << ", y " << X.y() << ", theta " << X.theta()  << "\n"
                        << "   dyn     x " << Xdyn.x() << ", y " << Xdyn.y() << ", theta " << Xdyn.theta()  << "\n"
                        << std::endl;

            //save data to file
m_saveToFile = true;
            if (m_saveToFile){
            fout_ekfState << time_stamp << " "
                          << truckLocation.getX() << " " << truckLocation.getY() << " " << truckLocation.getYaw() << " " << truckLocation.getYawRate() << " "
                          << U.v() << " " << U.phi() << " "
                          << Z.Z_x() << " " << Z.Z_y() << " " << Z.Z_theta() << " " << Z.Z_theta_dot() << " "
                          << X.x() << " " << X.x_dot() << " "  << X.y() << " " << X.y_dot() << " " << X.theta() << " " << X.theta_dot() << " "
                          << Xdyn.x() << " " << Xdyn.x_dot() << " " << Xdyn.y() << " "  << Xdyn.y_dot() << " " <<  Xdyn.theta() << " " << Xdyn.theta_dot()
                          << endl;
            }

         }
         else
         {
              std::cout << getName() << "  << message >> Filter NOT initialized " << std::endl;
         }

      }// end if we are getting data
    }// end while
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Sensation::initializeEKF()
{

if (!EKF_initialized)
{
    std::cout << "Sensation::initializeEKF  << message >> initialize the kalman filter " << std::endl;

    X.setZero();  // initialize the state vector
    Xdyn.setZero();
    generator.seed( std::chrono::system_clock::now().time_since_epoch().count() );


// all the necessary initializations should go here


    EKF_initialized = true;  // last operation if everything success
}
else
{
     std::cout << getName() << " << message >> Filter initialized " << std::endl;
}

}




} // sensation
} // application
} // system
} // opendlv




      /*
        cout << "[perception] received container of type " << c.getDataType() <<
                                               " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() <<
                                           " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << endl;

        if (c.getDataType() == Container::USER_DATA_2) {
            revere::generic::ExampleMessageForPerception payload = c.getData<revere::generic::ExampleMessageForPerception>();
            cout << "[perception]: " << payload.toString() << endl;

            // Example: Create message revere.generic.ExampleMessageForSafety.
            revere::generic::ExampleMessageForDecision nextMessage;
            nextMessage.setData1(5.2345);

            Container c2(Container::USER_DATA_3, nextMessage);
            getConference().send(c2);
        }
        */
