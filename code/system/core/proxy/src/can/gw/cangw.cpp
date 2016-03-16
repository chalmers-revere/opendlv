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

#include <iostream>
#include <string>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odtools/recorder/Recorder.h>
#include <odcantools/MessageToCANDataStore.h>
#include <odcantools/CANDevice.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include <fh16mapping/GeneratedHeaders_FH16Mapping.h>

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "can/gw/cangw.hpp"
#include "can/gw/canmessagedatastore.hpp"

namespace opendlv {
namespace proxy {
namespace can {
namespace gw {

CANGW::CANGW(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-can-gw")
    , automotive::odcantools::GenericCANMessageListener()
    , m_fifo()
    , m_recorder()
    , m_device()
    , m_canMessageDataStore()
{
}

CANGW::~CANGW()
{
}

void CANGW::setUp()
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odtools::recorder;
  using namespace automotive::odcantools;

  string const DEVICE_NODE =
  getKeyValueConfiguration().getValue<string>("proxy-can-gw.devicenode");

  // Try to open CAN device and register this instance as receiver for
  // GenericCANMessages.
  m_device = shared_ptr<CANDevice>(new CANDevice(DEVICE_NODE, *this));

  // If the device could be successfully opened, create a recording file with a
  // dump of the data.
  if (m_device.get() && m_device->isOpen()) {
    cout << "Successfully opened CAN device '" << DEVICE_NODE << "'." << endl;

    // Automatically record all received CAN messages.
    {
      // URL for storing containers containing GenericCANMessages.
      stringstream recordingURL;
      recordingURL << "file://"
                   << "can-gw_" << TimeStamp().getYYYYMMDD_HHMMSS() << ".rec";
      // Size of memory segments (not needed for recording GenericCANMessages).
      const uint32_t MEMORY_SEGMENT_SIZE = 0;
      // Number of memory segments (not needed for recording
      // GenericCANMessages).
      const uint32_t NUMBER_OF_SEGMENTS = 0;
      // Run recorder in asynchronous mode to allow real-time recording in
      // background.
      const bool THREADING = true;
      // Dump shared images and shared data (not needed for recording
      // GenericCANMessages)?
      const bool DUMP_SHARED_DATA = false;

      // Create a recorder instance.
      m_recorder = unique_ptr<Recorder>(new Recorder(recordingURL.str(),
      MEMORY_SEGMENT_SIZE, NUMBER_OF_SEGMENTS, THREADING, DUMP_SHARED_DATA));
    }

    // Create a data sink that automatically receives all Containers and
    // selectively relays them based on the Container type to the CAN device.
    m_canMessageDataStore =
    unique_ptr<CANMessageDataStore>(new CANMessageDataStore(m_device));
    addDataStoreFor(*m_canMessageDataStore);

    // Start the wrapped CAN device to receive CAN messages concurrently.
    m_device->start();
  }
}

void CANGW::tearDown()
{
  if (m_device.get()) {
    // Stop the wrapper CAN device.
    m_device->stop();
  }
}

void CANGW::nextGenericCANMessage(const automotive::GenericCANMessage &gcm)
{
  std::cout << "Received CAN message " << gcm.toString() << std::endl;

  // Map CAN message to high-level data structure.
  {
    // TODO: Glue this method to the CAN mapping function using
    // odCANDataStructureGenerator.
    // TODO: Transform the received CAN message to high-level data structure.
    // TODO: Emit high-level messages therefrom afterwards.
  }

  // Enqueue CAN message wrapped as Container to be recorded if we have a valid
  // recorder.
  if (m_recorder.get() != NULL) {
    odcore::data::Container c(gcm);
    m_fifo.add(c);
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode CANGW::body()
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    if (m_recorder.get() != NULL) {
      const uint32_t ENTRIES = m_fifo.getSize();
      for (uint32_t i = 0; i < ENTRIES; i++) {
        odcore::data::Container c = m_fifo.leave();

        // Store container to dump file.
        m_recorder->store(c);
      }
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // gw
} // can
} // proxy
} // opendlv
