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

#include "geolocationtest.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

Geolocationtest::Geolocationtest(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-geolocationtest")
    , m_sensorMutex()
    , m_states()
    , m_dataCollector()



{
 
 m_states = MatrixXd::Zero(6,1);
 m_dataCollector = MatrixXd::Zero(3000,6);
}

Geolocationtest::~Geolocationtest()
{
}

void Geolocationtest::nextContainer(odcore::data::Container &a_container)
{

	//Groundspeed	
	if (a_container.getDataType() == opendlv::logic::sensation::Geolocation::ID()){
		odcore::base::Lock l(m_sensorMutex);

	 	auto gpsState = a_container.getData<opendlv::logic::sensation::Geolocation>();
	 	m_states(0) = gpsState.getLatitude();
	 	m_states(1) = gpsState.getLongitude();
	 	m_states(5) = gpsState.getHeading();
	}

	if (a_container.getDataType() == opendlv::logic::sensation::Equilibrioception::ID()){
		odcore::base::Lock l(m_sensorMutex);

	 	auto eqState = a_container.getData<opendlv::logic::sensation::Equilibrioception>();
	 	m_states(2) = eqState.getVx();
	 	m_states(3) = eqState.getVy();
	 	m_states(4) = eqState.getYawRate();

	}

	/*
	opendlv::logic::sensation::Geolocation geoState;
	geoState.setLatitude(x(0));
	geoState.setLongitude(x(1));
	geoState.setHeading(x(5));
	odcore::data::Container c1(geoState);
	getConference().send(c1);

	opendlv::logic::sensation::Equilibrioception kinematicState;
	kinematicState.setVx(x(2));
	kinematicState.setVx(x(3));
	kinematicState.setYawRate(x(4));
	odcore::data::Container c2(kinematicState);
	getConference().send(c2);*/
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocationtest::body()
{
  
   
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING && m_iterator<3000) {
   	
   	opendlv::proxy::GroundSpeedReading propulsion;
	propulsion.setGroundSpeed(m_groundspeeds[m_iterator]);
	odcore::data::Container c1(propulsion);
	getConference().send(c1);
		
	opendlv::proxy::AccelerationReading accelerometerReading;
	accelerometerReading.setAccelerationX(m_ax[m_iterator]);
	accelerometerReading.setAccelerationY(m_ay[m_iterator]);
	odcore::data::Container c2(accelerometerReading);
	getConference().send(c2);

	opendlv::proxy::AngularVelocityReading gyroscopeReading;
	gyroscopeReading.setAngularVelocityZ(m_yawrate[m_iterator]);
	odcore::data::Container c3(gyroscopeReading);
	getConference().send(c3);

	opendlv::proxy::GeodeticWgs84Reading gnssReading;
	gnssReading.setLatitude(m_lats[m_iterator]);
	gnssReading.setLongitude(m_lons[m_iterator]);
	odcore::data::Container c4(gnssReading);
	getConference().send(c4);

	opendlv::proxy::GroundSteeringReading steering;
	steering.setGroundSteering(m_racktravel[m_iterator]);
	odcore::data::Container c5(steering);
	getConference().send(c5);

	opendlv::proxy::GeodeticHeadingReading heading;
	heading.setNorthHeading(m_heading[m_iterator]);
	odcore::data::Container c6(heading);
	getConference().send(c6);

	//std::cout << m_lons[m_iterator] << std::endl;
	m_iterator++;
    odcore::base::Lock l(m_sensorMutex);
    m_dataCollector.row(m_iterator-1) = m_states.transpose();
    std::cout << "------------------------------" << std::endl;
	std::cout << "recieved states:" << m_states.transpose() << std::endl;
	std::cout << "------------------------------" << std::endl;


  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocationtest::setUp()
{	
	std::cout << "Setting up test data" << std::endl;
	getDataFromFile();
}

void Geolocationtest::tearDown()
{
	std::cout << m_dataCollector << std::endl;
	writeDataToFile();
}

void Geolocationtest::getDataFromFile(){
	int nLine = 3000;
	int i = 0;
	std::string line;
	std::string const HOME = "/opt/opendlv.data/";
	std::string infile = HOME + "myFile.txt";
	std::ifstream myFile(infile, std::ifstream::in);
	//std::cout << infile << std::endl;
	if(myFile.is_open()){
		std::cout << "File open" << std::endl;
		while(i<nLine){
			if(getline(myFile,line)){
				m_times.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_lats.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_lons.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_groundspeeds.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_ax.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_ay.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_yawrate.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_heading.push_back(std::stod(line));
			}
			if(getline(myFile,line)){
				m_racktravel.push_back(std::stod(line));
			}			
			i++;			
		}
		std::cout << "NO" << std::endl;
		myFile.close();
	}
	else{
		std::string outfile = HOME + "vafan.txt";
		//std::cout << outfile << std::endl;
		std::ofstream afile(outfile, std::ios::app);
		if(afile.is_open()){
			std::cout << "Sad" << std::endl;
			afile << "Sad" << std::endl;
		}
		afile.close();
	}

}
void Geolocationtest::writeDataToFile()
{

	std::string const HOME = "/opt/opendlv.data/";
	std::string outfile = HOME + "filteredStateOldCarData.txt";
		//std::cout << outfile << std::endl;
		std::ofstream afile(outfile, std::ios::trunc);
		if(afile.is_open()){
			for(int i = 0; i< 3000; i++){				
				afile << m_dataCollector.row(i) << std::endl;
			}
		}
		afile.close();

}


}
}
}
