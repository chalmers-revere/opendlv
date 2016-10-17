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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opendavinci/odcore/strings/StringToolbox.h"

#include "gcdc16/rule/intersectionright/intersectionright.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace rule {
namespace intersectionright {

/**
* Constructor.
*
* @param a_argc Number of command line arguments.
* @param a_argv Command line arguments.
*/
IntersectionRight::IntersectionRight(int32_t const &a_argc, char **a_argv)
: TimeTriggeredConferenceClientModule(
  a_argc, a_argv, "knowledge-gcdc16-intersectionright"),
  m_desiredGroundSpeed()
{
}

IntersectionRight::~IntersectionRight()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode IntersectionRight::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  } 
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
   
}

void IntersectionRight::nextContainer(odcore::data::Container &a_container)
{
  if (a_container.getDataType() == (opendlv::knowledge::Insight::ID() + 300)) {
    opendlv::knowledge::Insight insight = a_container.getData<opendlv::knowledge::Insight>();
    std::string whatInsight = insight.getInsight();


    if (whatInsight == "scenarioReady") {
      // set desired speed of 30 km/h
      /*
      opendlv::knowledge::DesiredOpticalFlow desired(30/3.6f);
      odcore::data::Container objectContainerDesiredOpticalFlow(desired);
      getConference().send(objectContainerDesiredOpticalFlow);
      */
    }
  }
}

void IntersectionRight::setUp()
{
    m_desiredGroundSpeed = static_cast<float>(kv.getValue<double>("knowledge-gcdc16-rule-intersection-right.desiredAngularSize"));
}

void IntersectionRight::tearDown()
{
}

} // intersectionright
} // rule
} // gcdc16
} // knowledge
} // opendlv
