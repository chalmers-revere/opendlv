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
#include <opendavinci/odcore/base/Thread.h>
#include <opendavinci/odcore/io/tcp/TCPFactory.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "android/android.hpp"
#include "android/device.hpp"

namespace opendlv {
namespace proxy {
namespace android {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Android::Android(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "proxy-android")
    , m_tcpacceptor()
    , m_androidStringDecoder()
{
}

Android::~Android()
{
}

void Android::setUp()
{
  using namespace std;
  using namespace odcore::io::tcp;

//  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  {
    const uint32_t PORT = 1234;

    try {
        m_tcpacceptor = std::shared_ptr<TCPAcceptor>(TCPFactory::createTCPAcceptor(PORT));

        // This instance will handle any new connections.
        m_androidStringDecoder = unique_ptr<AndroidStringDecoder>(new AndroidStringDecoder(getConference()));
        m_tcpacceptor->setAcceptorListener(m_androidStringDecoder.get());

        // Start accepting new connections.
        m_tcpacceptor->start();
    }
    catch(string &exception) {
        cerr << "Error while creating TCP receiver: " << exception << endl;
    }
  }
}

void Android::tearDown()
{
  // Stop accepting new connections and unregister our handler.
  m_tcpacceptor->stop();
  m_tcpacceptor->setAcceptorListener(NULL);
}

void Android::nextContainer(odcore::data::Container &c) {
  (void)c;
}

} // android
} // proxy
} // opendlv
