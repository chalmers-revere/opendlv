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

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "odcantools/MessageToCANDataStore.h"


#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "can/cangw.hpp"

namespace opendlv {
namespace proxy {
namespace can {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
CANGW::CANGW(int32_t const &a_argc, char **a_argv) :
    TimeTriggeredConferenceClientModule(a_argc, a_argv, "proxy-cangw"),
    m_device()
{
}

CANGW::~CANGW() 
{
}

void CANGW::nextGenericCANMessage(const automotive::GenericCANMessage &gcm) {
    std::cout << "Received CAN message " << gcm.toString() << std::endl;
    // TODO: Glue this method to the CAN mapping function.
    // TODO: Transform the received CAN message to high-level data structure.
    // TODO: Emit high-level messages therefrom afterwards.
}

void CANGW::setUp() 
{
    using namespace std;
    using namespace odcore::base;
    using namespace automotive::odcantools;

    string const deviceNode = getKeyValueConfiguration().getValue<string>("proxy-cangw.devicenode");

    // Try to open CAN device and register this instance as receiver for GenericCANMessages.
    m_device = unique_ptr<CANDevice>(new CANDevice(deviceNode, *this));

    // If the device could be successfully opened, create a recording file with a dump of the data.
    if (m_device->isOpen()) {
        cout << "Successfully opened " << deviceNode << endl;

        // Register the CAN device as receiver for all Containers to be
        // potentially written to the CAN bus.
        addDataStoreFor(m_device->getMessageToCANDataStore());

        // Start the wrapped CAN device to receive CAN messages concurrently.
        m_device->start();
    }
}

void CANGW::tearDown() 
{
    m_device->stop();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode CANGW::body() {
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        // TODO: Add recorder functionality.
    }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // can
} // proxy
} // opendlv
