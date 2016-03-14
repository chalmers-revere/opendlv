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
#include <opendlv/data/environment/WGS84Coordinate.h>

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"
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
    x(),
    u(),
    sys(),
    observationModel(0.0, 0.0,  0.0, 0.0 ), // clarify the numbers !
    m_ekf(),
    generator(),
    noise(0, 1),
    systemNoise(0),
    orientationNoise(0),
    distanceNoise(0),
    run_vse_test(false),
    EKF_initialized(false)
{

    initializeEKF();

}

Sensation::~Sensation()
{
}

void Sensation::setUp()
{
  // This method will be call automatically _before_ running body().
}

void Sensation::tearDown()
{
  // This method will be call automatically _after_ return from body().
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Sensation::body() {
    // Example on how to use the type WGS84Coordinate:
    {
        using namespace opendlv::data::environment;

        // First, you need to declare a lat/lon coordinate to be used
        // as reference (i.e. origin (0, 0) of a Cartesian coordinate
        // frame); in our example, we use one located at AstaZero.
        WGS84Coordinate reference(57.77284043, WGS84Coordinate::NORTH, 12.76996356, WGS84Coordinate::EAST);

        // Let's assume you have another lat/lon coordinate at hand.
        WGS84Coordinate WGS84_p2(57.7730612, WGS84Coordinate::NORTH, 12.77008208, WGS84Coordinate::EAST);

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
    }

    // To dump data structures into a CSV file, you create an output file first.
    std::ofstream fout("../Exp_data/output.csv");
    std::ofstream fout_command("../Exp_data/output_commands.csv");
    std::ofstream fout_ekfState("../Exp_data/output_ekf.csv");

    fout_ekfState << "% HEADER: Output of the Extended Kalman Filter, data format : \n"
                  << "% ground truth x (m)  ground truth y (m) ground truth theta (rad) noisy x (m)  noisy y (m) noisy theta (rad)  ekf x (m) ekf y (m) ekf theta (rad) " << endl;

    // You can optionally dump a header (i.e. first line with information).
    const bool WITH_HEADER = true;
    // You can choose the delimiter character between the fields.
    const char DELIMITER = ',';
    // For every data structure that you want to export in a CSV file, you need to create a new CSVFromVisitableVisitor.
    odcore::reflection::CSVFromVisitableVisitor csvExporter1(fout, WITH_HEADER, DELIMITER);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        odcore::data::Container c1 = getKeyValueDataStore().get(opendlv::system::actuator::Commands::ID());
        opendlv::system::actuator::Commands commands = c1.getData<opendlv::system::actuator::Commands>();

        odcore::data::Container c2 = getKeyValueDataStore().get(opendlv::system::sensor::TruckLocation::ID());
        opendlv::system::sensor::TruckLocation truckLocation = c2.getData<opendlv::system::sensor::TruckLocation>();

        cout << getName() << ": " << commands.toString() << ", " << truckLocation.toString() << endl;


        // The csvExporter1 will "visit" the data structure "commands" and iterate
        // through its fields that will be stored in the output file fout.
        commands.accept(csvExporter1);


//all this part should be moved into the vehicle state estimator function

         u.v() = commands.getLongitudinalVelocity();//3.6;   /// from km/h to m/s
         u.phi() = commands.getSteeringAngle();//*180/M_PI;

         fout_command << u.v() << " " << u.phi() << endl;
         ///fout_measures <<truckLocation.getX() << " " << truckLocation.getY() << " " << truckLocation.getYaw() << " " << truckLocation.getYawRate() << endl;


        // Add noise: Our robot move is affected by noise (due to actuator failures)
        //x.x() += systemNoise*noise(generator);
        //x.y() += systemNoise*noise(generator);
        //x.theta() += systemNoise*noise(generator);


        // Predict state for current time-step using the filters
        opendlv::system::application::sensation::truckKinematicModel::State<double>  x_ekf_pred = m_ekf.predict(sys, u);  // TODO: change auto type for compatibility !

        // Orientation measurement
            //opendlv::system::application::sensation::truckObservationModel::OrientationMeasurement<double> orientation = OrientationModel.h(x);
            opendlv::system::application::sensation::truckObservationModel::truckObservationVector<double> measurement = observationModel.h(x);


            // Measurement is affected by noise as well
            //orientation.theta() += orientationNoise * noise(generator);

            measurement.Z_x()         =   truckLocation.getX()+0.05*noise(generator);
            measurement.Z_y()         =   truckLocation.getY()+0.5*noise(generator);
            measurement.Z_theta()     =   truckLocation.getYaw()+0.001*noise(generator);
            measurement.Z_theta_dot( )=   truckLocation.getYawRate();
            // Update EKF
            //x_ekf = m_ekf.update(OrientationModel, orientation);
            opendlv::system::application::sensation::truckKinematicModel::State<double> x_ekf = m_ekf.update(observationModel, measurement);





        // Print to stdout as csv format
        std::cout   << "Sensation::body << message >> \n"
                    << "      x_ekf_pred " << x_ekf_pred.x() << ", y_ekf_pred " << x_ekf_pred.y() << ", theta_ekf_pred " << x_ekf_pred.theta()  << "\n"
                    << "      x_ekf      " << x_ekf.x() << ", y_ekf " << x_ekf.y() << ", theta_ekf " << x_ekf.theta()  << "\n"
                    << std::endl;
//save data to file
fout_ekfState << truckLocation.getX() << " " << truckLocation.getY() << " " << truckLocation.getYaw() << " "
              << measurement.Z_x() << " " << measurement.Z_y() << " " << measurement.Z_theta() << " "
              << x_ekf.x() << " " << x_ekf.y() << " " << x_ekf.theta() << " "
              <<endl;


    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Sensation::initializeEKF()
{

if (!EKF_initialized)
{
     std::cout << "Sensation::initializeEKF  << message >> initialize the kalman filter " << std::endl;

    x.setZero();  // initialize the state vector
    generator.seed( std::chrono::system_clock::now().time_since_epoch().count() );


// all the necessary initializations should go here


    EKF_initialized = true;  // last operation if everything success
}
else
{
     std::cout << "Sensation::initializeEKF  << message >> Filter initialized " << std::endl;
}

}

void Sensation::vehicleStateEstimator( opendlv::system::application::sensation::truckKinematicModel::Control<double> _u,
                                       opendlv::system::application::sensation::truckKinematicModel::State<double> _x )
{

  _x.setZero();   // just to avoid unused error
  _u.setZero();

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
