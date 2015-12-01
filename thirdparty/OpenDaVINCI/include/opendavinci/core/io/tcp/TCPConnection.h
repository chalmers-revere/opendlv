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

#ifndef OPENDAVINCI_CORE_IO_TCP_TCPCONNECTION_H_
#define OPENDAVINCI_CORE_IO_TCP_TCPCONNECTION_H_

#include <sstream>

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Mutex.h"
#include "core/io/StringObserver.h"
#include "core/io/ConnectionObserver.h"

namespace core {
    namespace io {
        namespace tcp {

            using namespace std;

            class OPENDAVINCI_API TCPConnection : public StringObserver, public ConnectionObserver {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    TCPConnection(const TCPConnection &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    TCPConnection& operator=(const TCPConnection &);

                protected:
                    /**
                     * Protected constructor for enforcing subclasses for this class.
                     */
                    TCPConnection();

                public:
                    virtual ~TCPConnection();

                    /**
                     * This method is used to send data using this TCP connection.
                     *
                     * @param data Data to send.
                     */
                    void send(const string &data);

                    /**
                     * This method registers a ConnectionListener that will be
                     * informed about connection errors.
                     *
                     * @param cl The ConnectionListener
                     */
                    void setConnectionListener(ConnectionListener* cl);

                    /**
                     * This method sets the StringListener that will receive
                     * incoming data.
                     *
                     * @param listener StringListener that will receive incoming data.
                     */
                    void setStringListener(StringListener* listener);

                    /**
                     * This method must be called to start the connection.
                     */
                    virtual void start() = 0;

                    /**
                     * This method closes a connection.
                     */
                    virtual void stop() = 0;

                protected:
                    /**
                     * This method has to be called by subclasses whenever
                     * new (partial) data was received. This method is
                     * responsible for gathering partial data and invoking
                     * the registered StringListener when a complete data
                     * packet was gathered.
                     *
                     * @param partialData Partially received data that will
                     *                    be gathered until the complete
                     *                    data can be passed to the registered
                     *                    StringListener.
                     */
                    void receivedString(const string &partialData);

                    /**
                     * This method has to be implemented in subclasses
                     * to send data. It is called from within the send()-
                     * method.
                     *
                     * param data Data with prepended size information.
                     */
                    virtual void sendImplementation(const string &data) = 0;

                    /**
                     * This method is called be subclasses to invoke
                     * the connection listener.
                     */
                    void invokeConnectionListener();

                private:
                    bool hasCompleteData();

                    /**
                     * This method is used to pass received data thread-safe
                     * to the registered StringListener.
                     */
                    void invokeStringListener(const string& data);

                    core::base::Mutex m_connectionListenerMutex;
                    ConnectionListener* m_connectionListener;

                    core::base::Mutex m_stringListenerMutex;
                    StringListener *m_stringListener;

                    core::base::Mutex m_partialDataMutex;
                    stringstream m_partialData;
            };

        }
    }
}

#endif /* OPENDAVINCI_CORE_IO_TCP_TCPCONNECTION_H_ */
