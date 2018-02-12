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



{


}

Geolocationtest::~Geolocationtest()
{
}

void Geolocationtest::nextContainer(odcore::data::Container &a_container)
{
	auto datatype = a_container.getDataType();
	std::cout << datatype << std::endl;
} 

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Geolocationtest::body()
{
  

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING && m_iterator<500) {
   	
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

	std::cout << m_lons[m_iterator] << std::endl;
	m_iterator++;
	
	
  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
void Geolocationtest::setUp()
{	
	std::cout << "Setting up" << std::endl;
	getDataFromFile();
}

void Geolocationtest::tearDown()
{
}

void Geolocationtest::getDataFromFile(){
	int nLine = 500;
	int i = 0;
	std::string line;
	std::string const HOME = "/opt/opendlv.data/";
	std::string infile = HOME + "myFile.txt";
	std::ifstream myFile(infile, std::ifstream::in);
	std::cout << infile << std::endl;
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
		std::cout << outfile << std::endl;
		std::ofstream afile(outfile, std::ios::app);
		if(afile.is_open()){
			std::cout << "Sad" << std::endl;
			afile << "Sad" << std::endl;
		}
		afile.close();
	}

}
}
}
}
