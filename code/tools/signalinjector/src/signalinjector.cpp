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
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "signalinjector.hpp"



namespace opendlv {
namespace tools {
namespace signalinjector {


Signalinjector::Signalinjector(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-signalinjector"),
      m_initialized(false),
      m_throttle(),
      m_brake(),
      m_steering()
{
}

Signalinjector::~Signalinjector()
{
}

void Signalinjector::setUp()
{
  m_initialized = true;
}

void Signalinjector::tearDown()
{
}


void Signalinjector::nextContainer(odcore::data::Container &)
{
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Signalinjector::body(){
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING){
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}


void Signalinjector::ImportData(std::vector<std::pair<double,double>> &, std::string a_fileName)
{
  std::ifstream file(a_fileName);

  if(file.is_open()){
    // for(uint i = 0; i < a_vector.size(); i++){
    //   file >> a_vector.at(i).x;
    //   file >> a_vector.at(i).y;
    // }

  }

  file.close();
}



} // signalinjector
} // tools
} // opendlv
