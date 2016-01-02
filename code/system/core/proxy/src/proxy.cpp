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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "core/base/KeyValueConfiguration.h"
#include "core/data/Container.h"
#include "core/data/TimeStamp.h"

#include "GeneratedHeaders_OpenDLVData.h"

#include "opencvcamera.hpp"
#include "proxy.hpp"

namespace opendlv {
namespace system {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Proxy::Proxy(int32_t const &a_argc, char **a_argv):
    DataTriggeredConferenceClientModule(a_argc, a_argv, "proxy"),
    m_recorder(nullptr),
    m_camera(nullptr)
{
}

Proxy::~Proxy() 
{
}

// This method will do the main data processing job.
coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode Proxy::body()
{
  uint32_t captureCounter = 0;
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == coredata::dmcp::ModuleStateMessage::RUNNING)
  {
    // Capture frame.
    if (m_camera.get() != nullptr) {
      coredata::image::SharedImage si = m_camera->Capture();

      core::data::Container c(core::data::Container::SHARED_IMAGE, si);
      distribute(c);
      captureCounter++;
    }

    // Send an example message revere.generic.ExampleMessageForDiagnostics.
//    opendlv::generic::ExampleMessageForDiagnostics nextMessage;
//    nextMessage.setData1(42);
//    nextMessage.setData2(3.1415);
//    nextMessage.setData3("Hej Sverige!");

//    Container c2(Container::USER_DATA_0, nextMessage);
//    distribute(c2);
  }

  std::cout << "[proxy] Captured " << captureCounter << " frames." 
      << std::endl;

  return coredata::dmcp::ModuleExitCodeMessage::OKAY;
}

void Proxy::distribute(core::data::Container a_c)
{
  // Store data to recorder.
  if (m_recorder.get() != nullptr) {
    // Time stamp data before storing.
    a_c.setReceivedTimeStamp(core::data::TimeStamp());
    m_recorder->store(a_c);
  }

  // Share data.
  getConference().send(a_c);
}

void Proxy::nextContainer(core::data::Container &) 
{
}

void Proxy::setUp() 
{
  // This method will be call automatically _before_ running body().
  
  // Get configuration data.
  core::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  // Create built-in recorder.
  bool const useRecorder = (kv.getValue<uint32_t>("proxy.useRecorder") == 1);
  if (useRecorder) {
    // URL for storing containers.
    std::stringstream recordingUrl;
    recordingUrl << "file://" << "proxy_" 
        << core::data::TimeStamp().getYYYYMMDD_HHMMSS() << ".rec";
    
    // Size of memory segments.
    uint32_t const MEMORY_SEGMENT_SIZE = 
        getKeyValueConfiguration().getValue<uint32_t>(
        "global.buffer.memorySegmentSize");

    // Number of memory segments.
    uint32_t const NUMBER_OF_SEGMENTS = 
        getKeyValueConfiguration().getValue<uint32_t>(
        "global.buffer.numberOfMemorySegments");

    // Run recorder in asynchronous mode to allow real-time recording in background.
    bool const THREADING = true;
    
    // Dump shared images and shared data?
    bool const DUMP_SHARED_DATA = 
        (getKeyValueConfiguration().getValue<uint32_t>(
        "proxy.recorder.dumpshareddata") == 1);

    m_recorder = std::unique_ptr<tools::recorder::Recorder>(
        new tools::recorder::Recorder(recordingUrl.str(), MEMORY_SEGMENT_SIZE, 
        NUMBER_OF_SEGMENTS, THREADING, DUMP_SHARED_DATA));
  }

  // Example: Create the camera grabber.
  std::string const NAME = getKeyValueConfiguration().getValue<std::string>(
      "proxy.camera.name");
  std::string TYPE = getKeyValueConfiguration().getValue<std::string>(
      "proxy.camera.type");
  std::transform(TYPE.begin(), TYPE.end(), TYPE.begin(), ::tolower);

  uint32_t const ID = getKeyValueConfiguration().getValue<uint32_t>(
      "proxy.camera.id");
  uint32_t const WIDTH = getKeyValueConfiguration().getValue<uint32_t>(
      "proxy.camera.width");
  uint32_t const HEIGHT = getKeyValueConfiguration().getValue<uint32_t>(
      "proxy.camera.height");
  uint32_t const BPP = getKeyValueConfiguration().getValue<uint32_t>(
      "proxy.camera.bpp");

  if (TYPE.compare("opencv") == 0) {
      m_camera = std::unique_ptr<Camera>(
          new OpenCvCamera(NAME, ID, WIDTH, HEIGHT, BPP));
  }

  if (m_camera.get() == nullptr) {
    std::cerr << "[proxy] No valid camera type defined." << std::endl;
  }
}

void Proxy::tearDown() 
{
  // This method will be call automatically _after_ return from body().
}

} // system
} // opendlv
