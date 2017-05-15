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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Thread.h"


#include "identity.hpp"

namespace opendlv {
namespace logic {
namespace knowledge {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Identity::Identity(int32_t const &a_argc, char **a_argv)
    : TimeTriggeredConferenceClientModule(a_argc, a_argv, "logic-knowledge-identity"),
    m_stationId(),
    m_stationType(),
    m_vehicleLength(),
    m_vehicleWidth(),
    m_vehicleRole(),
    m_rearAxleLocation()
{
}

Identity::~Identity()
{
}

/**
 * Receives .
 * Sends .
 */
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Identity::body()
{  
  
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
      odcore::data::dmcp::ModuleStateMessage::RUNNING){
    odcore::data::TimeStamp now;
    opendlv::knowledge::Insight stationIdInsight(now,"stationId="+std::to_string(m_stationId));
    SendContainer(stationIdInsight);

    opendlv::knowledge::Insight stationTypeInsight(now,"stationType="+std::to_string(m_stationType));
    SendContainer(stationTypeInsight);

    opendlv::knowledge::Insight vehicleLengthInsight(now,"vehicleLength="+std::to_string(m_vehicleLength));
    SendContainer(vehicleLengthInsight);
    
    opendlv::knowledge::Insight vehicleWidthInsight(now,"vehicleWidth="+std::to_string(m_vehicleWidth));
    SendContainer(vehicleWidthInsight);
    
    opendlv::knowledge::Insight vehicleRoleInsight(now,"vehicleRole="+std::to_string(m_vehicleRole));
    SendContainer(vehicleRoleInsight);
    
    opendlv::knowledge::Insight rearAxleLocationInsight(now,"rearAxleLocation="+std::to_string(m_rearAxleLocation));
    SendContainer(rearAxleLocationInsight);
  
  }  
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Identity::SendContainer(opendlv::knowledge::Insight &a_insight)
{
  odcore::data::Container c(a_insight);
  getConference().send(c);

  // Allow sending packets.
  odcore::base::Thread::usleepFor(100);

  // Resend to v2vcam
  odcore::data::Container c2(a_insight, opendlv::knowledge::Insight::ID() + 400);
  getConference().send(c2);
}

void Identity::setUp()
{

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_stationId = kv.getValue<uint32_t>("logic-knowledge-identity.stationId");
  m_stationType = kv.getValue<uint32_t>("logic-knowledge-identity.stationType");
  m_vehicleLength = kv.getValue<double>("logic-knowledge-identity.vehicleLength");
  m_vehicleWidth = kv.getValue<double>("logic-knowledge-identity.vehicleWidth");
  m_vehicleRole = kv.getValue<uint32_t>("logic-knowledge-identity.vehicleRole");
  m_rearAxleLocation = kv.getValue<double>("logic-knowledge-identity.rearAxleLocation");


}

void Identity::tearDown()
{
}

}
}
}
