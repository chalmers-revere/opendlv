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
    Measurement(),
    observationModel(0.0, 0.0, 0.0, 0.0 ), // clarify the numbers !
    //PositionMeasurement(),
    //OrientationMeasurement(),
    //PositionModel(-10, -10, 30, 75),//(0.0, 0.0, 0.0, 0.0),
    //OrientationModel(),
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
    // To dump data structures into a CSV file, you create an output file first.
    std::ofstream fout("output.csv");
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

         // Simulate system
         x = sys.f(x, u);

        // Add noise: Our robot move is affected by noise (due to actuator failures)
        //x.x() += systemNoise*noise(generator);
        //x.y() += systemNoise*noise(generator);
        //x.theta() += systemNoise*noise(generator);

         // wrong here ! the measurements should go into the observation model
         //x.x() = truckLocation.getX();
         //x.y() = truckLocation.getY();
         //x.theta() = truckLocation.getYaw();
         //x.theta_dot() = truckLocation.getYawRate();

        // Predict state for current time-step using the filters
        auto x_ekf = m_ekf.predict(sys, u);  // TODO: change auto type for compatibility !

        // Orientation measurement
        {
            // We can measure the orientation every 5th step
            //OrientationMeasurement orientation = OrientationModel.h(x);
            //opendlv::system::application::sensation::truckObservationModel::OrientationMeasurement<double> orientation = OrientationModel.h(x);
            opendlv::system::application::sensation::truckObservationModel::truckObservationVector<double> measurement = observationModel.h(x);


            // Measurement is affected by noise as well
            //orientation.theta() += orientationNoise * noise(generator);

            measurement.Z_x()=truckLocation.getX();
            measurement.Z_y()=truckLocation.getY();
            measurement.Z_theta()=truckLocation.getYaw();
            measurement.Z_theta_dot()=truckLocation.getYawRate();
            // Update EKF
            //x_ekf = m_ekf.update(OrientationModel, orientation);
            x_ekf = m_ekf.update(observationModel, measurement);

        }

        // Position measurement
        {
            // We can measure the position every 10th step
            //PositionMeasurement position = PositionModel.h(x);
            //opendlv::system::application::sensation::truckObservationModel::PositionMeasurement<double>  position;
            //position = PositionModel.h(x);

            // Measurement is affected by noise as well
            //position.d1() += distanceNoise * noise(generator);
            //position.d2() += distanceNoise * noise(generator);


            // Update EKF
            //x_ekf = m_ekf.update(PositionModel, position);

        }

        // Print to stdout as csv format
        std::cout   << "Sensation::body << message >> x " << x.x() << ", y " << x.y() << ", yaw " << x.theta() << ", x_ekf "
                    << x_ekf.x() << ", y_ekf " << x_ekf.y() << ", theta_ekf " << x_ekf.theta()  << ","
                    << std::endl;




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
