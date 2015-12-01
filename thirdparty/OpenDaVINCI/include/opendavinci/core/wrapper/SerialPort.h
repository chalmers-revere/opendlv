/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2014 - 2015 Christian Berger
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

#ifndef OPENDAVINCI_CORE_WRAPPER_SERIALPORT_H_
#define OPENDAVINCI_CORE_WRAPPER_SERIALPORT_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/io/ConnectionObserver.h"
#include "core/io/StringListener.h"
#include "core/io/StringSender.h"
#include "core/wrapper/Mutex.h"
#include "core/wrapper/Runnable.h"
#include "core/wrapper/Thread.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This class provides a platform-independent serial port encapsulation.
         */
        class OPENDAVINCI_API SerialPort : public Runnable, public core::io::StringSender, public core::io::ConnectionObserver {
            public:
                /**
                 * Constructor for enforcing subclasses for this class.
                 *
                 * @param port Serial port.
                 * @param baudRate Baud rate.
                 */
                SerialPort(const string &port, const uint32_t &baudRate);

                virtual ~SerialPort();

                /**
                 * This method registers a ConnectionListener that will be
                 * informed about connection errors.
                 *
                 * @param cl The ConnectionListener
                 */
                void setConnectionListener(core::io::ConnectionListener* cl);

                /**
                 * This method sets the StringListener that will realize the protocol.
                 *
                 * @param sl StringListener that will handle incoming data.
                 */
                void setStringListener(core::io::StringListener *sl);

                /**
                 * This method must be called to start the connection.
                 */
                void start();

                /**
                 * This method closes a connection.
                 */
                void stop();

                /**
                 * This method sends data.
                 *
                 * param data Data to be sent.
                 */
                void send(const string &data);

            protected:
                virtual bool isRunning();

                virtual void run();

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
                 * This method is called by subclasses to invoke
                 * the connection listener.
                 */
                void invokeConnectionListener();

            private:
                auto_ptr<Thread> m_thread;

                auto_ptr<Mutex> m_connectionListenerMutex;
                core::io::ConnectionListener* m_connectionListener;

                auto_ptr<Mutex> m_stringListenerMutex;
                core::io::StringListener *m_stringListener;

                void *m_serial;

                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                SerialPort(const SerialPort &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                SerialPort& operator=(const SerialPort &);
        };
    }
}

#endif /* OPENDAVINCI_CORE_WRAPPER_SERIALPORT_H_ */
