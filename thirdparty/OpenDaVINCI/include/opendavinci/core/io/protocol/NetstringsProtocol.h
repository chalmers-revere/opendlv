/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_IO_PROTOCOL_NETSTRINGSPROTOCOL_H_
#define OPENDAVINCI_CORE_IO_PROTOCOL_NETSTRINGSPROTOCOL_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include <sstream>

#include "core/base/Mutex.h"
#include "core/io/protocol/AbstractProtocol.h"
#include "core/io/StringObserver.h"

namespace core {
    namespace io {
        namespace protocol {

            using namespace std;

            /**
             * This class realizes a thread-safe Netstrings protocol implementation.
             *
             * It can be used as follows:
             *
             * @code
             * // Your class needs to implement the interface StringListener to get
             * // informed about newly received payloads extracted from a Netstring.
             *
             * NetstringsProtocol np;
             * np.setStringListener(this); // We are listening for byte sequences.
             * np.setStringSender(<your sender, could be your class that receives the Netstrings encoded data>);
             * ...
             * // Your byte sequence receiving thread triggering the Netstrings decoding:
             * np.nextString(partialData);
             * ...
             * // Encoding a byte sequence as a Netstring.
             * string payload = "Hello World!";
             * np.send(payload);
             *
             * @endcode
             */
            class OPENDAVINCI_API NetstringsProtocol : public StringObserver, public AbstractProtocol {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    NetstringsProtocol(const NetstringsProtocol &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    NetstringsProtocol& operator=(const NetstringsProtocol &);

                public:
                    /**
                     * Constructor.
                     */
                    NetstringsProtocol();

                    virtual ~NetstringsProtocol();

                    /**
                     * This method sends the data in the following format:
                     *
                     * Size : Data ,
                     *
                     * Size: data.length() as string
                     *
                     * @param data Data to be sent.
                     */
                    void send(const string& data);

                    /**
                     * This method sets the StringListener that will receive
                     * incoming data.
                     *
                     * @param listener StringListener that will receive incoming data.
                     */
                    void setStringListener(StringListener *listener);

                    virtual void nextString(const string &s);

                private:
                    void decodeNetstring();

                    /**
                     * This method is used to pass received data thread-safe
                     * to the registered StringListener.
                     */
                    void invokeStringListener(const string& data);

                    core::base::Mutex m_stringListenerMutex;
                    StringListener *m_stringListener;

                    core::base::Mutex m_partialDataMutex;
                    stringstream m_partialData;
            };

        }
    }
}

#endif /* OPENDAVINCI_CORE_IO_PROTOCOL_NETSTRINGSPROTOCOL_H_ */

