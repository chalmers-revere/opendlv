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

#include <opendavinci/odcore/data/TimeStamp.h>

#include "geolocator.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

Geolocator::Geolocator(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocator")
    , m_measurementsTimeStamp()
    , m_paramVecR()
    , m_sensorMutex()
    , m_accXYReading()
    , m_yawReading()
    , m_gpsReading()
    , m_gpsReference()
    , m_Q()
    , m_R()
    , m_stateCovP()
    , m_vehicleModelParameters()
    , m_delta()
    , m_sampleTime()
    , m_headingReading()
    , m_groundSpeedReading()
    , m_states()


{
	m_measurementsTimeStamp = MatrixXd::Zero(7,1);
	m_paramVecR = MatrixXd::Zero(7,1);
	m_gpsReading = MatrixXd::Zero(2,1);
	m_accXYReading = MatrixXf::Zero(2,1);
	m_yawReading = 0;
	m_delta = 0;
	m_headingReading = 0.0;
	m_groundSpeedReading = 0;
	m_Q = MatrixXd::Zero(6,6); //Six states
	m_R = MatrixXd::Zero(7,7); //Seven Measurements
	m_stateCovP = MatrixXd::Identity(6,6); //Initialize P
	m_states = MatrixXd::Zero(6,1);
	m_vehicleModelParameters = MatrixXd::Zero(7,1);
}

Geolocator::~Geolocator()
{
}

void Geolocator::nextContainer(odcore::data::Container &a_container)
{

	//Groundspeed	
	if (a_container.getDataType() == opendlv::proxy::GroundSpeedReading::ID()){
		odcore::base::Lock l(m_sensorMutex);
	 	odcore::data::TimeStamp containerStamp = a_container.getReceivedTimeStamp();
	 	m_measurementsTimeStamp(2,0) = containerStamp.toMicroseconds();

	 	auto groundspeed = a_container.getData<opendlv::proxy::GroundSpeedReading>();
	 	m_groundSpeedReading = groundspeed.getGroundSpeed();
	}
	//Accelerometer
	 if (a_container.getDataType() == opendlv::proxy::AccelerationReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	odcore::data::TimeStamp containerStamp = a_container.getReceivedTimeStamp();
	 	m_measurementsTimeStamp(3,0) = containerStamp.toMicroseconds();
	 	m_measurementsTimeStamp(4,0) = containerStamp.toMicroseconds();
	 	auto accReading = a_container.getData<opendlv::proxy::AccelerationReading>();
		m_accXYReading << accReading.getAccelerationX(),
						  accReading.getAccelerationY();


	 }
	 //Gyro
	 if (a_container.getDataType() == opendlv::proxy::AngularVelocityReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	odcore::data::TimeStamp containerStamp = a_container.getReceivedTimeStamp();
	 	m_measurementsTimeStamp(5,0) = containerStamp.toMicroseconds();
		auto gyrReading = a_container.getData<opendlv::proxy::AngularVelocityReading>();
		m_yawReading = gyrReading.getAngularVelocityZ();
	 }

	 //GPS Position
	 if (a_container.getDataType() == opendlv::proxy::GeodeticWgs84Reading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	odcore::data::TimeStamp containerStamp = a_container.getReceivedTimeStamp();
	 	m_measurementsTimeStamp(0,0) = containerStamp.toMicroseconds();
	 	m_measurementsTimeStamp(1,0) = containerStamp.toMicroseconds();

		auto gpsReading = a_container.getData<opendlv::proxy::GeodeticWgs84Reading>();
		double longitude = gpsReading.getLongitude();
		double latitude = gpsReading.getLatitude();
		opendlv::data::environment::WGS84Coordinate gpsCurrent = opendlv::data::environment::WGS84Coordinate(latitude, longitude);
		opendlv::data::environment::Point3 gpsTransform = m_gpsReference.transform(gpsCurrent);
		m_gpsReading << gpsTransform.getX(),
						gpsTransform.getY();
	 }

	 //Heading

	 if (a_container.getDataType() == opendlv::proxy::GeodeticHeadingReading::ID()){
	 	odcore::base::Lock l(m_sensorMutex);
	 	odcore::data::TimeStamp containerStamp = a_container.getReceivedTimeStamp();
	 	m_measurementsTimeStamp(6,0) = containerStamp.toMicroseconds();


		auto gpsHeadingReading = a_container.getData<opendlv::proxy::GeodeticHeadingReading>();
		m_headingReading = gpsHeadingReading.getNorthHeading();

	 }

	 //Racktravel
	 if (a_container.getDataType() == opendlv::proxy::GroundSteeringReading::ID()){

	 	odcore::base::Lock l(m_sensorMutex);
		auto racktravel = a_container.getData<opendlv::proxy::GroundSteeringReading>();

		double rT = racktravel.getGroundSteering();
		m_delta = rackTravelToFrontWheelSteering(rT);
	 }
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocator::body()
{
  
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
  	odcore::base::Lock l(m_sensorMutex);
  	odcore::data::TimeStamp currentTime;
	
	//Check last recieved measurement from each sensor, if longer than 1 sec, start trusting the model more  	
  	for(int i = 0; i < 7; i++){
  		
  		if(currentTime.toMicroseconds()-m_measurementsTimeStamp(i,0) > 1000000){
  			
  			m_R(i,i) = m_paramVecR(i,0)*1000;
			std::cout << "Not trusting sensor " << i << std::endl;

  		}else
  		{
  			m_R(i,i) = m_paramVecR(i,0);
  		}
  	}
  	
  	//prediction
  	m_states = UKFPrediction(m_states);
  	//update
  	m_states = UKFUpdate(m_states);

  	stateSender(m_states);
  	
   	

  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocator::setUp()
{
  	//%%%%%%%%%%%%%%%%%%%%%%%CONFIG STUFF%%%%%%%%%%%%%%%%%%%%%%
	  auto kv = getKeyValueConfiguration();
	  //Model uncertainties params
	  double const qX = kv.getValue<double>("logic-sensation-geolocator.Q.X");
	  double const qY = kv.getValue<double>("logic-sensation-geolocator.Q.Y");
	  double const qVelX = kv.getValue<double>("logic-sensation-geolocator.Q.VelX");
	  double const qVelY = kv.getValue<double>("logic-sensation-geolocator.Q.VelY");
	  double const qYaw = kv.getValue<double>("logic-sensation-geolocator.Q.Yaw");
	  double const qHeading= kv.getValue<double>("logic-sensation-geolocator.Q.Heading");

	  m_Q << qX,0,0,0,0,0,
	  		 0,qY,0,0,0,0,
	  		 0,0,qVelX,0,0,0,
	  		 0,0,0,qVelY,0,0,
	  		 0,0,0,0,qYaw,0,
	  		 0,0,0,0,0,qHeading;

	  //Measurement uncertainies params
	  double const rX = kv.getValue<double>("logic-sensation-geolocator.R.X");
	  double const rY = kv.getValue<double>("logic-sensation-geolocator.R.Y");
	  double const rVelX = kv.getValue<double>("logic-sensation-geolocator.R.VelX");
	  double const rAccX = kv.getValue<double>("logic-sensation-geolocator.R.AccX");
	  double const rAccY = kv.getValue<double>("logic-sensation-geolocator.R.AccY");
	  double const rYaw = kv.getValue<double>("logic-sensation-geolocator.R.Yaw");
	  double const rHeading = kv.getValue<double>("logic-sensation-geolocator.R.Heading");
	  m_paramVecR << rX,rY,rVelX,rAccX,rAccY,rYaw,rHeading;
	  m_R << rX,0,0,0,0,0,0,
	  		 0,rY,0,0,0,0,0,
	  		 0,0,rVelX,0,0,0,0,
	  		 0,0,0,rAccX,0,0,0,
	  		 0,0,0,0,rAccY,0,0,
	  		 0,0,0,0,0,rYaw,0,
	  		 0,0,0,0,0,0,rHeading;

	//%%%%%%%%%%%%VEHICLE MODEL PARAMETERS%%%%%%%%%%%%%%%%%%%
	double const vM = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.m");
	double const vIz = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.Iz");
	double const vG = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.g");
	double const vL = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.l");
	double const vLf = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.lf");
	double const vLr = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.lr");
	double const vMu = kv.getValue<double>("logic-sensation-geolocator.vehicle-parameter.mu");

	m_vehicleModelParameters << vM,vIz,vG,vL,vLf,vLr,vMu;

	//%%%%%%%%%%%%System Params%%%%%%%%%%%%%%%%%%%%%%

	double const latitude = getKeyValueConfiguration().getValue<double>("logic-sensation-geolocator.GPSreference.latitude");
	double const longitude = getKeyValueConfiguration().getValue<double>("logic-sensation-geolocator.GPSreference.longitude");
	m_gpsReference = opendlv::data::environment::WGS84Coordinate(latitude,longitude);

	m_sampleTime = kv.getValue<double>("logic-sensation-geolocator.T");

	std::cout << "UKF initialized!" << std::endl;

}

void Geolocator::tearDown()
{
}


MatrixXd Geolocator::UKFWeights()
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
MatrixXd Geolocator::sigmaPoints(MatrixXd &x)
{

	double const n = x.rows(); //Amount of states
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

MatrixXd Geolocator::UKFPrediction(MatrixXd &x)
{	

	MatrixXd Wmc = UKFWeights();
	MatrixXd SP = sigmaPoints(x);
	double const n = x.rows();

	//Predictions
	MatrixXd x_hat = MatrixXd::Zero(n,1);
	MatrixXd sigmaPoint, sigmaStates;
	//Calculate Mean

	for(int i = 0; i < 2*n+1; i++){

		sigmaPoint = SP.col(i);
		sigmaStates = vehicleModel(sigmaPoint);
		x_hat = x_hat + sigmaStates*Wmc(0,i);

	}

	//Calculate Covariance

	MatrixXd P_temp = MatrixXd::Zero(n,n); 
	for(int i = 0; i < 2*n+1; i++){

		sigmaPoint = SP.col(i);
		sigmaStates = vehicleModel(sigmaPoint);


		P_temp = P_temp + (sigmaStates-x_hat)*(sigmaStates-x_hat).transpose()*Wmc(1,i);

	}

	P_temp = P_temp + m_Q;

	//Dirty trick to keep numerical stability
	m_stateCovP = (P_temp + P_temp.transpose())/2;

	return x_hat;
}

MatrixXd Geolocator::UKFUpdate(MatrixXd &x)
{


		MatrixXd Wmc = UKFWeights();
		MatrixXd SP = sigmaPoints(x);
		double const n = x.rows();

		MatrixXd y_hat = MatrixXd::Zero(7,1);
		MatrixXd sigmaPoint, sigmaStates;
		//Calculate Mean
	for(int i = 0; i < 2*n+1;i++){

		sigmaPoint = SP.col(i);
		sigmaStates = measurementModel(sigmaPoint);
		y_hat = y_hat + sigmaStates*Wmc(0,i);

	}

	MatrixXd Pxy = MatrixXd::Zero(6,7);
	//Innovation covariance

	MatrixXd S = m_R;
	for(int i = 0; i < 2*n+1; i++){

		sigmaPoint = SP.col(i);
		sigmaStates = measurementModel(sigmaPoint);

		Pxy = Pxy + (sigmaPoint-x)*(sigmaStates-y_hat).transpose()*Wmc(1,i);
		S = S + (sigmaStates-y_hat)*(sigmaStates-y_hat).transpose()*Wmc(1,i);

	}

	//Collect measurements from sensor

	MatrixXd y = MatrixXd::Zero(7,1);
	y << m_gpsReading(0,0), 
		 m_gpsReading(1,0),
		 m_groundSpeedReading,
		 m_accXYReading(0,0),
		 m_accXYReading(1,0),
		 m_yawReading,
		 m_headingReading;

	//State update
	x = x + Pxy*S.inverse()*(y-y_hat);

	m_stateCovP = m_stateCovP - Pxy*S.inverse()*Pxy.transpose();

	//Dirty trick to keep numerical stability
	m_stateCovP = (m_stateCovP + m_stateCovP.transpose())/2;
	
	return x;
}

MatrixXd Geolocator::vehicleModel(MatrixXd &x)
{


	if(x(2) < 0.0001){

		x(2) = 0.01;
	}

	double alphaF = std::atan((m_vehicleModelParameters(3)*x(4)) + x(3)/x(2)) - m_delta;
	double alphaR = std::atan((x(3)-m_vehicleModelParameters(4)*x(4))/x(2));

	//Non linear Tire Model

	double Fzf = m_vehicleModelParameters(0)*m_vehicleModelParameters(2)*(m_vehicleModelParameters(4)/(m_vehicleModelParameters(4)+m_vehicleModelParameters(3)));
	double Fzr = m_vehicleModelParameters(0)*m_vehicleModelParameters(2)*(m_vehicleModelParameters(3)/(m_vehicleModelParameters(4)+m_vehicleModelParameters(3)));

	double Fyf = magicFormula(alphaF,Fzf,m_vehicleModelParameters(5));
	double Fyr = magicFormula(alphaR,Fzr,m_vehicleModelParameters(5));

	Eigen::MatrixXd xdot = MatrixXd::Zero(6,1);

	xdot << x(2),
			x(3),
			-Fyf*std::sin(m_delta)/m_vehicleModelParameters(0) + x(4)*x(3),
			(Fyf*std::cos(m_delta)+Fyr)/m_vehicleModelParameters(0) - x(4)*x(3),
			(m_vehicleModelParameters(3)*Fyf*std::cos(m_delta)-m_vehicleModelParameters(4)*Fyr)/m_vehicleModelParameters(1),
			x(4);

	MatrixXd fx = x + xdot*m_sampleTime;

	return fx;
}
MatrixXd Geolocator::measurementModel(MatrixXd &x)
{

	MatrixXd hx = MatrixXd::Zero(7,1);
	double alphaF = std::atan((m_vehicleModelParameters(3)*x(4)) + x(3)/x(2)) - m_delta;
	double alphaR = std::atan((x(3)-m_vehicleModelParameters(4)*x(4))/x(2));

	//Non linear Tire Model

	double Fzf = m_vehicleModelParameters(0)*m_vehicleModelParameters(2)*(m_vehicleModelParameters(4)/(m_vehicleModelParameters(4)+m_vehicleModelParameters(3)));
	double Fzr = m_vehicleModelParameters(0)*m_vehicleModelParameters(2)*(m_vehicleModelParameters(3)/(m_vehicleModelParameters(4)+m_vehicleModelParameters(3)));

	double Fyf = magicFormula(alphaF,Fzf,m_vehicleModelParameters(5));
	double Fyr = magicFormula(alphaR,Fzr,m_vehicleModelParameters(5));

	hx << x(0),
		  x(1),
		  x(2),
		  -Fyf*std::sin(m_delta)/m_vehicleModelParameters(0),
		  (Fyf*std::cos(m_delta)+Fyr)/m_vehicleModelParameters(0),
		  x(4),
		  x(5);

	return hx;
}
double Geolocator::magicFormula(double &alpha, double &Fz, double const &mu)
{

	double const C = 1;
	double const c_alpha = 25229;
	double const B = c_alpha/C/mu/Fz;
	double const E = -2;
	double Fy = mu*Fz*std::sin(C*std::atan(B*alpha - E*(B*alpha - std::atan(B*alpha))));

	return Fy;
}

double Geolocator::rackTravelToFrontWheelSteering(double &rackTravel)
{

	double const rackTravelToSteeringAngleLineSlope = -1.225;
	rackTravel = (rackTravel-100.3)/5;

	double delta = rackTravelToSteeringAngleLineSlope*rackTravel*3.14159/180; 

	return delta;
}

void Geolocator::stateSender(MatrixXd &x)
{
	opendlv::data::environment::Point3 xyz;
	xyz.setX(x(0));
	xyz.setY(x(1));
	xyz.setZ(0);
	opendlv::data::environment::WGS84Coordinate gpsCurrent = m_gpsReference.transform(xyz);
	opendlv::logic::sensation::Geolocation geoState;
	geoState.setLatitude(gpsCurrent.getLatitude());
	geoState.setLongitude(gpsCurrent.getLongitude());
	geoState.setHeading(x(5));
	odcore::data::Container c1(geoState);
	getConference().send(c1);

	opendlv::logic::sensation::Equilibrioception kinematicState;
	kinematicState.setVx(x(2));
	kinematicState.setVy(x(3));
	kinematicState.setYawRate(x(4));
	odcore::data::Container c2(kinematicState);
	getConference().send(c2);

	std::cout << "States sent" << x.transpose() << std::endl;

}

}
}
}
