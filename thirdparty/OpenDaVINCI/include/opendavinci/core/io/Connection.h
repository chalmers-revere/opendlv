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

#ifndef OPENDAVINCI_CORE_IO_CONNECTION_H_
#define OPENDAVINCI_CORE_IO_CONNECTION_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/data/Container.h"
#include "core/io/conference/ContainerObserver.h"
#include "core/io/ConnectionErrorListener.h"
#include "core/io/conference/ContainerObserver.h"
#include "core/io/StringListener.h"
#include "core/io/tcp/TCPConnection.h"

namespace core {
    namespace io {

        using namespace std;

        class OPENDAVINCI_API Connection : public core::io::conference::ContainerObserver,
                                           protected StringListener,
                                           protected ConnectionListener
        {
            private:
                /**
                 * Forbidden copy constructor
                 */
                Connection(Connection& obj);

                /**
                 * Forbidden assignement operator
                 */
                Connection& operator=(Connection& obj);

            public:
                Connection(const std::string &ip, const uint32_t &port);
                virtual ~Connection();

                void send(core::data::Container &container);
                virtual void setContainerListener(core::io::conference::ContainerListener *cl);
                void setErrorListener(ConnectionErrorListener* el);

                void start();
                void stop();

                bool isConnected();

            protected:
                friend class ConnectionAcceptor;
                Connection(core::SharedPointer<core::io::tcp::TCPConnection> connection);

                virtual void nextString(const std::string &s);
                virtual void handleConnectionError();

            private:
                core::base::Mutex m_listenerMutex;
                core::io::conference::ContainerListener* m_listener;

                core::base::Mutex m_errorHandlerMutex;
                ConnectionErrorListener* m_errorHandler;

                core::SharedPointer<core::io::tcp::TCPConnection> m_connection;

                core::base::Mutex m_connectedMutex;
                bool m_connected;
        };
    }
}

#endif /* OPENDAVINCI_CORE_IO_CONNECTION_H_ */
