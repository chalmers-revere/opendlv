/**
 * Copyright (C) 2016 Chalmers REVERE
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

#include <algorithm>  
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/strings/StringToolbox.h" 

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "signalinjector.hpp"



namespace opendlv {
namespace tools {
namespace signalinjector {


Signalinjector::Signalinjector(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-signalinjector"),
      m_initialized(false),
      m_fileNames(),
      m_testNumber(),
      m_brake(),
      m_steering(),
      m_throttle()
{
}

Signalinjector::~Signalinjector()
{
}

void Signalinjector::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const filenames = kv.getValue<std::string>("tools-signalinjector.filenames");
  m_fileNames = odcore::strings::StringToolbox::split(filenames,',');
  // std::cout << m_fileNames[0] << std::endl;
  m_testNumber = kv.getValue<int32_t>("tools-signalinjector.testnumber"); 
  // std::cout << m_testNumber << std::endl;

  std::string path = "/opt/opendlv/var/tools/signalinjector/" + std::to_string(m_testNumber) + "/";

  ImportData(m_brake, path + m_fileNames[0]);
  ImportData(m_steering, path + m_fileNames[1]);
  ImportData(m_throttle, path + m_fileNames[2]);
  m_initialized = true;
}

void Signalinjector::tearDown()
{
}


void Signalinjector::nextContainer(odcore::data::Container &)
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Signalinjector::body(){
  std::cout << "Execute? [yn]:";
  char execute;
  std::cin >> execute;
  
  odcore::data::TimeStamp startTime;
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING && execute == 'y'){
    odcore::data::TimeStamp now;
    double time = static_cast<double>((now.toMicroseconds() - startTime.toMicroseconds()))/1000000;
    // std::cout << std::get<0>(m_brake[0]) << std::endl;
    while(std::get<0>(m_brake.back()) < time){
      //todo send the message to can
      m_brake.pop_back();
    }
    while(std::get<0>(m_steering.back()) < time){
      //todo send the message to can
      m_steering.pop_back();
    }
    while(std::get<0>(m_throttle.back()) < time){
      //todo send the message to can
      m_throttle.pop_back();
    }

    std::cout << "Brake: " << m_brake.size() <<  " Steering: " << m_steering.size() << " Throttle: " << m_throttle.size() << std::endl;
    //todo jump out when finished the vectors
  }


  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}


void Signalinjector::ImportData(std::vector<std::pair<double,double>> &a_vec, std::string a_fileName)
{
  std::cout << a_fileName << std::endl;
  std::ifstream file(a_fileName);
  std::string val;
  while(file.good()){
    std::getline(file, val);
    if(val != ""){
      std::vector<std::string> vstr = odcore::strings::StringToolbox::split(val,',');
      a_vec.push_back(std::make_pair(std::stod(vstr[0]),std::stod(vstr[1])));
    }
  }
  file.close();
  std::reverse(a_vec.begin(),a_vec.end());
}



} // signalinjector
} // tools
} // opendlv
