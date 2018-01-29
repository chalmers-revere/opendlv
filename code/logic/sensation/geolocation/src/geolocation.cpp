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


//#include <odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h>

#include "geolocation.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

Geolocation::Geolocation(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocation")
    , m_gpsTimeStamp()
    , m_accTimeStamp()
    , m_gyrTimeStamp()
    , m_groundSpeedTimeStamp()
    , m_sensorMutex()
    , m_accXYReading()
    , m_yawReading()
    , m_gpsReading()
    , m_gpsReference()
    , m_Q()
    , m_R()
    , m_stateCovP()
    , m_vehicleModelParameters()


{

	std::cout << "T1" << std::endl;
	m_gpsReference = opendlv::data::environment::WGS84Coordinate(0.0, 0.0); //How to collect???
	std::cout << "T1.1" << std::endl;
	m_gpsReading = MatrixXd::Zero(2,1);
	m_Q = MatrixXd::Zero(6,6); //Six states
	m_R = MatrixXd::Zero(7,7); //Seven Measurements
	m_stateCovP = MatrixXd::Identity(6,6); //Initialize P
	m_stateCovP = m_stateCovP*0.1;
	m_vehicleModelParameters = MatrixXd::Zero(7,1);
	std::cout << "T1.2" << std::endl;



}

Geolocation::~Geolocation()
{
}

void Geolocation::nextContainer(odcore::data::Container &a_container)
{



	//m_groundSpeedTimeStamp = a_container.getReceivedTimeStamp();
	//Accelerometer
	 if (a_container.getDataType() == opendlv::proxy::AccelerometerReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	m_accTimeStamp = a_container.getReceivedTimeStamp();
	 	auto accReading = a_container.getData<opendlv::proxy::AccelerometerReading>();
	 	m_accXYReading(0,0) = accReading.getAccelerationX();
	 	m_accXYReading(1,0) = accReading.getAccelerationY();

	 }

	 //Gyro
	 if (a_container.getDataType() == opendlv::proxy::GyroscopeReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
		m_gyrTimeStamp = a_container.getReceivedTimeStamp();
		auto gyrReading = a_container.getData<opendlv::proxy::GyroscopeReading>();
		m_yawReading = gyrReading.getAngularVelocityZ();
	 }

	 //GPS
	 if (a_container.getDataType() == opendlv::core::sensors::trimble::GpsReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	m_gpsTimeStamp = a_container.getReceivedTimeStamp();
		auto gpsReading = a_container.getData<opendlv::core::sensors::trimble::GpsReading>();
		double longitude = gpsReading.getLongitude();
		double latitude = gpsReading.getLatitude();
		opendlv::data::environment::WGS84Coordinate gpsCurrent = opendlv::data::environment::WGS84Coordinate(latitude, longitude);
		opendlv::data::environment::Point3 gpsTransform = m_gpsReference.transform(gpsCurrent);
		m_gpsReading << gpsTransform.getX(),
						gpsTransform.getY(); 

		/*

		WGS84Reference = WGS84Coordinate(latitude, longitude);
		WGS84Coordinate WGS84current = c.getData<WGS84Coordinate>();
		Point3 currentPosition = WGS84Reference.transform(WGS84current);*/


	 }


	 /*if (a_container.getDataType() == opendlv::proxy::GeodeticHeadingReading::ID()){
	 	
	 } */
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
  	//Kolla alla sensor timestamps, om för lång tid gått ändra Q till att lite på modellen


   	

  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocation::setUp()
{
  	//%%%%%%%%%%%%%%%%%%%%%%%CONFIG STUFF%%%%%%%%%%%%%%%%%%%%%%
	  auto kv = getKeyValueConfiguration();
	  //Model uncertainties params
	  double const qX = kv.getValue<double>("logic-sensation-geolocation.Q.X");
	  double const qY = kv.getValue<double>("logic-sensation-geolocation.Q.Y");
	  double const qVelX = kv.getValue<double>("logic-sensation-geolocation.Q.VelX");
	  double const qVelY = kv.getValue<double>("logic-sensation-geolocation.Q.VelY");
	  double const qYaw = kv.getValue<double>("logic-sensation-geolocation.Q.Yaw");
	  double const qHeading= kv.getValue<double>("logic-sensation-geolocation.Q.Heading");

	  m_Q << qX,0,0,0,0,0,
	  		 0,qY,0,0,0,0,
	  		 0,0,qVelX,0,0,0,
	  		 0,0,0,qVelY,0,0,
	  		 0,0,0,0,qYaw,0,
	  		 0,0,0,0,0,qHeading;

	  //Measurement uncertainies params
	  double const rX = kv.getValue<double>("logic-sensation-geolocation.R.X");
	  double const rY = kv.getValue<double>("logic-sensation-geolocation.R.Y");
	  double const rVelX = kv.getValue<double>("logic-sensation-geolocation.R.VelX");
	  double const rAccX = kv.getValue<double>("logic-sensation-geolocation.R.AccX");
	  double const rAccY = kv.getValue<double>("logic-sensation-geolocation.R.AccY");
	  double const rYaw = kv.getValue<double>("logic-sensation-geolocation.R.Yaw");
	  double const rHeading = kv.getValue<double>("logic-sensation-geolocation.R.Heading");
	  m_R << rX,0,0,0,0,0,0,
	  		 0,rY,0,0,0,0,0,
	  		 0,0,rVelX,0,0,0,0,
	  		 0,0,0,rAccX,0,0,0,
	  		 0,0,0,0,rAccY,0,0,
	  		 0,0,0,0,0,rYaw,0,
	  		 0,0,0,0,0,0,rHeading;

	//%%%%%%%%%%%%VEHICLE MODEL PARAMETERS%%%%%%%%%%%%%%%%%%%

	double const vM = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.m");
	double const vIz = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.Iz");
	double const vG = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.g");
	double const vL = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.l");
	double const vLf = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.lf");
	double const vLr = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.lr");
	double const vMu = kv.getValue<double>("logic-sensation-geolocation.vehicle-parameter.mu");
	m_vehicleModelParameters << vM,vIz,vG,vL,vLf,vLr,vMu;

	std::cout << "UKF initialized!" << std::endl;

	Eigen::MatrixXd states = MatrixXd::Zero(6,1);
	states << 0,0,17.96,0,0.45,0.5205;
	Eigen::MatrixXd H = UKFWeights();
	Eigen::MatrixXd F = sigmaPoints(states);

	std::cout << H << std::endl;
	std::cout << "--------------" << std::endl;
	std::cout << F << std::endl;

}

void Geolocation::tearDown()
{
}


MatrixXd Geolocation::UKFWeights()
{

 	double const n = 6; //Amount of states
	double const alpha = 0.5;
	double const beta = 2;
	double const kappa = 3 - n;
	double const lambda = std::pow(alpha,2)*(n + kappa) - n;


	//Calculate weights to sigma points

	double Wm0 = lambda/(n+lambda);
	double Wc0 = lambda / (n + lambda) + (1 - std::pow(alpha,2) + beta);;

	Eigen::MatrixXd Wmc = MatrixXd::Zero(2,2*n+1);

	Wmc(0,0) = Wm0;
	Wmc(1,0) = Wc0;

	for(int i = 0; i < 2*n; i++){

			Wmc(0,i+1) = 1 / (2 * (n + lambda));
			Wmc(1,i+1) = Wmc(0,i+1);

	}


	return Wmc;

}
MatrixXd Geolocation::sigmaPoints(MatrixXd &x)
{

	double const n = 6; //Amount of states
	double const alpha = 0.5;
	//double const beta = 2;
	double const kappa = 3 - n;
	double const lambda = std::pow(alpha,2)*(n + kappa) - n;


	double c = std::sqrt(n + lambda);

	Eigen::MatrixXd Pchol(m_stateCovP.llt().matrixL());
	Eigen::MatrixXd A = c*Pchol;

	Eigen::MatrixXd SP = MatrixXd::Zero(n,2*n+1);

	Eigen::MatrixXd Ap = MatrixXd::Zero(n,n);
	Eigen::MatrixXd An = MatrixXd::Zero(n,n);

	for(int j = 0; j < A.cols(); j++){

		Ap.col(j) = x+A.col(j);
		An.col(j) = x-A.col(j);


	}


	SP << x, Ap, An;

	
	return SP;

}

MatrixXd Geolocation::UKFPrediction(MatrixXd &states)
{
	std::cout << states << std::endl;
	return MatrixXd::Zero(4,1);
}

MatrixXd Geolocation::UKFUpdate(MatrixXd &states)
{

	std::cout << states << std::endl;
	return MatrixXd::Zero(4,1);
}

}
}
}
