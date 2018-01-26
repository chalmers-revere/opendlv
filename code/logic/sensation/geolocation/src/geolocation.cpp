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
#include "odvdopendlvstandardmessageset/GeneratedHeaders_ODVDOpenDLVStandardMessageSet.h"


//#include <odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h>

#include "geolocation.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

Geolocation::Geolocation(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocation")
    , m_recievedGroundSpeed()
    , m_recievedAccReading()
    , m_receivedGyrReading()
    , m_recievedGpsReading()
    , m_recievedHeadingReading()
    , m_refTimeStamp()
    , m_sensorDataToFilterSynch()

{
	m_recievedGroundSpeed = false;
    m_recievedAccReading = false;
    m_receivedGyrReading = false;
    m_recievedGpsReading = false;
    m_recievedHeadingReading = false;
    m_sensorDataToFilterSynch = false;

}

Geolocation::~Geolocation()
{
}

void Geolocation::nextContainer(odcore::data::Container &a_container)
{

 //Check id and timestamp on sensor readings
 //For each new reading , set true
	 /*if (a_container.getDataType() == opendlv::proxy::GroundSpeedReading::ID()){

	
	 }*/

	//Collect data from sensors


	//Accelerometer
	 if (a_container.getDataType() == opendlv::proxy::AccelerometerReading::ID() && !m_recievedAccReading && m_recievedGpsReading){

	 	 odcore::data::TimeStamp accTimeStamp = a_container.getReceivedTimeStamp();

	 	 if(accTimeStamp.toMicroseconds() - m_refTimeStamp.toMicroseconds() < 1000){

	 	 	m_recievedAccReading = true;
	 	 }


	 }

	 //Gyroscope
	 if (a_container.getDataType() == opendlv::proxy::GyroscopeReading::ID() && !m_receivedGyrReading && m_recievedGpsReading ){

	 	 odcore::data::TimeStamp gyrTimeStamp = a_container.getReceivedTimeStamp();

	 	 if(gyrTimeStamp.toMicroseconds() - m_refTimeStamp.toMicroseconds() < 1000){

	 	 	m_receivedGyrReading = true;
	 	 }

	 }

	 //GPS
	 if (a_container.getDataType() == opendlv::core::sensors::trimble::GpsReading::ID() && !m_recievedGpsReading){


	 	m_refTimeStamp = a_container.getReceivedTimeStamp();

	 	m_recievedGpsReading = true;
	 }

	 if(m_recievedGpsReading && m_receivedGyrReading && m_recievedAccReading){


	 	//filterBool = true
	 	m_sensorDataToFilterSynch = true;
	 }
	 /*if (a_container.getDataType() == opendlv::proxy::GeodeticHeadingReading::ID()){
	 	
	 } */
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocation::body()
{
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
   
   if(m_sensorDataToFilterSynch){

   	 //START HERE

   	 m_sensorDataToFilterSynch = false;
   	 m_recievedGpsReading = false;
   	 m_receivedGyrReading = false;
   	 m_recievedAccReading = false;
   }


   	

  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocation::setUp()
{
  std::cout << "Welcome" << std::endl;
}

void Geolocation::tearDown()
{
}

MatrixXd Geolocation::sigmaPoints(MatrixXd &states, MatrixXd &P)
{

	std::cout << states << P << std::endl;
	return MatrixXd::Zero(4,1);

}

MatrixXd Geolocation::UKFPrediction(MatrixXd &states, MatrixXd &P,double &T,MatrixXd &Q)
{
	std::cout << states << P << T << Q << std::endl;
	return MatrixXd::Zero(4,1);
}

MatrixXd Geolocation::UKFUpdate(MatrixXd &states, MatrixXd &P, MatrixXd &R)
{

	std::cout << states << P << R << std::endl;
	return MatrixXd::Zero(4,1);
}

}
}
}
