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
    , m_delta()
    , m_sampleTime()
    , m_headingReading()
    , m_groundSpeedReading()


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

	//Groundspeed

	
	if (a_container.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()){
		odcore::base::Lock l(m_sensorMutex);
	 	m_groundSpeedTimeStamp = a_container.getReceivedTimeStamp();
	 	auto groundspeed = a_container.getData<opendlv::proxy::reverefh16::Propulsion>();
	 	m_groundSpeedReading = groundspeed.getPropulsionShaftVehicleSpeed();
	}
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
		m_headingReading = gpsReading.getNorthHeading();
		opendlv::data::environment::WGS84Coordinate gpsCurrent = opendlv::data::environment::WGS84Coordinate(latitude, longitude);
		opendlv::data::environment::Point3 gpsTransform = m_gpsReference.transform(gpsCurrent);
		m_gpsReading << gpsTransform.getX(),
						gpsTransform.getY();

		/*

		WGS84Reference = WGS84Coordinate(latitude, longitude);
		WGS84Coordinate WGS84current = c.getData<WGS84Coordinate>();
		Point3 currentPosition = WGS84Reference.transform(WGS84current);*/


	 }

	 //Racktravel

	 if (a_container.getDataType() == opendlv::proxy::reverefh16::Steering::ID()){

	 	odcore::base::Lock l(m_sensorMutex);
		auto racktravel = a_container.getData<opendlv::proxy::reverefh16::Steering>();

		double rT = racktravel.getSteeringwheelangle();
		m_delta = rackTravelToFrontWheelSteering(rT);

		//RACKTRAVELTODELTA??


		//

	 }

	 /*if (a_container.getDataType() == opendlv::proxy::GeodeticHeadingReading::ID()){
	 	
	 } */
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
  	//Kolla alla sensor timestamps, om för lång tid gått ändra Q till att lite på modellen

  	odcore::base::Lock l(m_sensorMutex);

  	//prediction

  	//update
   	

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

	m_sampleTime = kv.getValue<double>("logic-sensation-geolocation.T");

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

MatrixXd Geolocation::UKFPrediction(MatrixXd &x)
{	

	MatrixXd Wmc = UKFWeights();
	MatrixXd SP = sigmaPoints(x);
	double const n = x.rows();
	

	 
	//Predictions

	MatrixXd x_hat = MatrixXd::Zero(n,1);
	MatrixXd sigmaPoint, sigmaStates;

	//Calculate Mean
	for(int i = 0; i < 2*n+1;){

		sigmaPoint = SP.col(i);
		sigmaStates = vehicleModel(sigmaPoint);
		x_hat = x_hat + sigmaStates*Wmc(0,i);

	}

	//Calculate Covariance

	MatrixXd P_temp = MatrixXd::Zero(n,n); 
	for(int i = 0; i < 2*n+1;){

		sigmaPoint = SP.col(i);
		sigmaStates = vehicleModel(sigmaPoint);


		P_temp = P_temp + (sigmaStates-x_hat)*(sigmaStates-x_hat).transpose()*Wmc(1,i);

	}

	P_temp = P_temp + m_Q;

	m_stateCovP = (P_temp + P_temp.transpose())/2;
		/*
	        %Generate sigma points
        [SP,Wm,Wc] = sigmaPoints(x, P, type);
        %Calculate state prediction
        for i = 1:(2*n+1)
            x_hat = x_hat+f(SP(:,i),T,u,param)*Wm(i);
        end
        x = x_hat;
        %Calculate Error Covariance prediction
        for i = 1:(2*n+1)
           P_t = P_t + (f(SP(:,i),T,u,param)-x_hat)*(f(SP(:,i),T,u,param)-x_hat)'*Wc(i);
        end
         P = P_t+Q;
         P = (P+P')/2;*/



	std::cout << x << std::endl;
	return x_hat;
}

MatrixXd Geolocation::UKFUpdate(MatrixXd &x)
{


		MatrixXd Wmc = UKFWeights();
		MatrixXd SP = sigmaPoints(x);
		double const n = x.rows();



		MatrixXd y_hat = MatrixXd::Zero(n,1);
		MatrixXd sigmaPoint, sigmaStates;

		//Calculate Mean
	for(int i = 0; i < 2*n+1;){

		sigmaPoint = SP.col(i);
		sigmaStates = measurementModel(sigmaPoint);
		y_hat = y_hat + sigmaStates*Wmc(0,i);

	}

	MatrixXd Pxy = MatrixXd::Zero(n,n);
	//Innovation covariance

	MatrixXd S = m_R;
	for(int i = 0; i < 2*n+1;){

		sigmaPoint = SP.col(i);
		sigmaStates = vehicleModel(sigmaPoint);


		Pxy = Pxy + (sigmaStates-x)*(sigmaStates-y_hat).transpose()*Wmc(1,i);

		S = S + (sigmaStates-y_hat)*(sigmaStates-y_hat).transpose()*Wmc(1,i);

	}

	//Collect measurements from sensor

	MatrixXd y = MatrixXd::Zero(7,1);

	y << m_gpsReading(0), 
		 m_gpsReading(1),
		 m_groundSpeedReading,
		 m_accXYReading(0),
		 m_accXYReading(1),
		 m_yawReading,
		 m_headingReading;

	//State update

	x = x + Pxy*S.inverse()*(y-y_hat);

	m_stateCovP = m_stateCovP - Pxy*S.inverse()*Pxy.transpose();

	m_stateCovP = (m_stateCovP + m_stateCovP.transpose())/2;

	/*        %Generate measurement models
        [hx, Hx] = h(x,s,delta,param);
        y_hat = zeros(numel(hx),1);
        %Generate Sigma Points
        [SP,Wm,Wc] = sigmaPoints(x, P, type);
        
        %Calculate measurement estimate
        for i = 1:2*n+1
            y_hat = y_hat + h(SP(:,i),s,delta,param)*Wm(i);
        end
        S = R;
        Pxy = zeros(n,numel(hx));
        for i = 1:2*n+1
            %Calculate Covariance
            Pxy = Pxy + (SP(:,i)-x)*(h(SP(:,i),s,delta,param)-y_hat)'*Wc(i);
            %Calculate innovation covariance
            S = S + (h(SP(:,i),s,delta,param)-y_hat)*(h(SP(:,i),s,delta,param)-y_hat)'*Wc(i);
        end
        Pxy*inv(S)
        %State update
        x = x + Pxy*inv(S)*(y-y_hat);
        %Error covariance matrix update
        P = P - Pxy*inv(S)*Pxy';
        P = (P+P')/2;*/

	
	return x;
}

MatrixXd Geolocation::vehicleModel(MatrixXd &x)
{


	if(x(2) < 0.0001){

		x(2) = 0.01;
	}
	/*
	double const m = m_vehicleModelParameters(1);
	double const Iz = m_vehicleModelParameters(2);
	double const g = m_vehicleModelParameters(3);
	double const lf = m_vehicleModelParameters(4);
	double const lr = m_vehicleModelParameters(5);
	double const mu = m_vehicleModelParameters(6);
	*/

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


	/*
	m = param(1);
	Iz = param(2);
	g = param(3);
	lf = param(4);
	lr = param(5);
	mu = param(6);

	if x(3) < 0.0001
    x(3) = 0.01;
	end

	%Create N.L motion model

	alphaF = atan((lf*x(5) + x(4))/x(3)) - delta;
	alphaR = atan((x(4)-lr*x(5))/x(3));

	%linear tire model
	% Ff = -cf*alphaF;
	% Fr = -cr*alphaR;
	%NL tire model
	Fzf = m*g*(lr/(lr+lf));
	Fzr = m*g*(lf/(lr+lf));
	Fyf = magicFormula(alphaF,Fzf,mu);
	Fyr = magicFormula(alphaR,Fzr,mu);

	xdot = [x(3);...
	    x(4);...
	    -Fyf*sin(delta)/m + x(5)*x(4);...
	    (Fyf*cos(delta)+Fyr)/m - x(5)*x(4);...
	    (lf*Fyf*cos(delta)-lr*Fyr)/Iz;...
	    x(5)];
	fx = x + xdot*T;
	Fx = 0;*/

	return fx;
}
MatrixXd Geolocation::measurementModel(MatrixXd &x)
{

	/*	double const m = m_vehicleModelParameters(1);
	double const Iz = m_vehicleModelParameters(2);
	double const g = m_vehicleModelParameters(3);
	double const lf = m_vehicleModelParameters(4);
	double const lr = m_vehicleModelParameters(5);
	double const mu = m_vehicleModelParameters(6);*/

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

	/*hx = [x(1);...
      x(2);...
      x(3);...
      -Fyf*sin(delta)/m;...
      (Fyf*cos(delta)+Fyr)/m;...
      x(5)
      x(6)];*/
	return hx;
}
double Geolocation::magicFormula(double &alpha, double &Fz, double const &mu)
{

	double const C = 1;
	double const c_alpha = 25229;
	double const B = c_alpha/C/mu/Fz;
	double const E = -2;
	double Fy = mu*Fz*std::sin(C*std::atan(B*alpha - E*(B*alpha - std::atan(B*alpha))));
	/*
		%mu = 0.9;
	C = 1;
	c_alpha = 2.5229e4;
	%Fz = 598.4142; 
	B = c_alpha/C/mu/Fz;
	E = -2;

	%alpha = 0:pi/180:pi/18;

	Fy = mu*Fz*sin(C*atan(B*alpha - E*(B*alpha - atan(B*alpha))));
	*/

	return Fy;
}

double Geolocation::rackTravelToFrontWheelSteering(double &rackTravel)
{

	double const rackTravelToSteeringAngleLineSlope = -1.225;
	rackTravel = (rackTravel-100.3)/5;

	double delta = rackTravelToSteeringAngleLineSlope*rackTravel*3.14159/180; 

	return delta;
}

}
}
}
