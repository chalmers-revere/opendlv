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

#include <stdint.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <opendavinci/odcore/base/KeyValueConfiguration.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/io/tcp/TCPFactory.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odtools/recorder/Recorder.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "gps/gps.hpp"
#include "gps/device.hpp"

namespace opendlv {
namespace proxy {
namespace gps {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Gps::Gps(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "proxy-gps")
    , m_device()
    , m_trimble()
    , m_gpsStringDecoder()
    , m_recorderGpsReadings()
    , m_CSVFile()
{
}

Gps::~Gps()
{
  // Flush output to ASC file.
  if (m_CSVFile.get() != NULL) {
    m_CSVFile->flush();
    m_CSVFile->close();
  }
}

void Gps::setUp()
{
  using namespace std;
  using namespace odcore::io::tcp;

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

//  std::string const type = kv.getValue<std::string>("proxy-gps.type");
//  float const mountX = kv.getValue<float>("proxy-gps.mount.x");
//  float const mountY = kv.getValue<float>("proxy-gps.mount.y");
//  float const mountZ = kv.getValue<float>("proxy-gps.mount.z");

  const bool RECORD_GPS = (getKeyValueConfiguration().getValue<int>("proxy-gps.record") == 1);
  if (RECORD_GPS) {
    // Automatically record all received raw CAN messages.
    odcore::data::TimeStamp now;
    vector<string> timeStampNoSpace = odcore::strings::StringToolbox::split(now.getYYYYMMDD_HHMMSS(), ' ');
    stringstream strTimeStampNoSpace;
    strTimeStampNoSpace << timeStampNoSpace.at(0);
    if (timeStampNoSpace.size() == 2) {
      strTimeStampNoSpace << "_" << timeStampNoSpace.at(1);
    }
    const string TIMESTAMP = strTimeStampNoSpace.str();

    // URL for storing containers containing GenericCANMessages.
    stringstream recordingUrl;
    recordingUrl << "file://"
                 << "CID-" << getCID() << "_"
                 << "gps_" << TIMESTAMP << ".rec";
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
    m_recorderGpsReadings = shared_ptr<odtools::recorder::Recorder>(new odtools::recorder::Recorder(recordingUrl.str(),
    MEMORY_SEGMENT_SIZE, NUMBER_OF_SEGMENTS, THREADING, DUMP_SHARED_DATA));

    // Create a file to dump CAN data in ASC format.
    stringstream fileName;
    fileName << "CID-" << getCID() << "_"
           << "gps_" << TIMESTAMP << ".csv";
    m_CSVFile = shared_ptr<fstream>(new fstream(fileName.str(), ios::out));
  }


  {
    string RECEIVER = kv.getValue<std::string>("proxy-gps.trimble.ip");
    uint32_t PORT = kv.getValue<uint32_t>("proxy-gps.trimble.port");

    // Separated string decoding from GPS messages from this class.
    // Therefore, we need to pass the getConference() reference to the other instance so that it can send containers.
    m_gpsStringDecoder = std::unique_ptr<GpsStringDecoder>(new GpsStringDecoder(getConference()));
    m_gpsStringDecoder->setDataSinks(m_recorderGpsReadings, m_CSVFile);

    try {
        m_trimble = shared_ptr<TCPConnection>(TCPFactory::createTCPConnectionTo(RECEIVER, PORT));
        m_trimble->setRaw(true);
        // The other instance is handling strings.
        m_trimble->setStringListener(m_gpsStringDecoder.get());
        m_trimble->start();
    }
    catch(string &exception) {
        cerr << "[" << getName() << "] Could not connect to Trimble: " << exception << endl;
    }
  }
}

void Gps::tearDown()
{
  m_trimble->stop();
  m_trimble->setStringListener(NULL);
}

void Gps::nextContainer(odcore::data::Container &c) {
  (void)c;
}

} // gps
} // proxy
} // opendlv
