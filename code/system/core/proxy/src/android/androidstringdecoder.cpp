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

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "android/android.hpp"
#include "android/device.hpp"

namespace opendlv {
namespace proxy {
namespace android {

AndroidStringDecoder::AndroidStringDecoder(odcore::io::conference::ContainerConference &conference) :
    m_conference(conference)
    , m_connection()
{
}

AndroidStringDecoder::~AndroidStringDecoder()
{
    if (m_connection.get() != NULL) {
        // Stop this connection.
        m_connection->stop();

        // Unregister the listeners.
        m_connection->setStringListener(NULL);
        m_connection->setConnectionListener(NULL);
        m_connection.reset();
    }
}

void AndroidStringDecoder::handleConnectionError() {
    cout << "Connection terminated." << endl;
}

void AndroidStringDecoder::nextString(const std::string &s) {
    cout << "Received " << s.length() << " bytes containing '" << s << "'" << endl;
}

void AndroidStringDecoder::onNewConnection(std::shared_ptr<odcore::io::tcp::TCPConnection> connection) {
    if (connection.get()) {
        cout << "Handle a new connection." << endl;

        if (m_connection.get() != NULL) {
            // Stop this connection.
            m_connection->stop();

            // Unregister the listeners.
            m_connection->setStringListener(NULL);
            m_connection->setConnectionListener(NULL);
            m_connection.reset();
        }

        m_connection = connection;

        // Do not interpret the bytes in lower levels.
        m_connection->setRaw(true);

        // Set this class as StringListener to receive
        // data from this connection.
        m_connection->setStringListener(this);

        // Set this class also as ConnectionListener to
        // handle errors originating from this connection.
        m_connection->setConnectionListener(this);

        // Start receiving data from this connection.
        m_connection->start();
    }
}

} // android
} // proxy
} // opendlv
