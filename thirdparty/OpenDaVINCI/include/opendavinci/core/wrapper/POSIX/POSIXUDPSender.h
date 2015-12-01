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

#ifndef OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXUDPSENDER_H_
#define OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXUDPSENDER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"
#include "core/io/udp/UDPSender.h"

#include "core/wrapper/NetworkLibraryProducts.h"
#include "core/wrapper/UDPFactoryWorker.h"

namespace core {
    namespace wrapper {
        namespace POSIX {

            using namespace std;

            /**
             * This class implements a UDP sender for sending data using POSIX.
             *
             * @See UDPSender
             */
            class POSIXUDPSender : public core::io::udp::UDPSender {
                private:
                    enum {
                        MAX_UDP_PACKET_SIZE = 65507
                    };

                private:
                    friend class UDPFactoryWorker<NetworkLibraryPosix>;

                    /**
                     * Constructor.
                     *
                     * @param address Address of the receiver.
                     * @param port Port of the receiver.
                     */
                    POSIXUDPSender(const string &address, const uint32_t &port);

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    POSIXUDPSender(const POSIXUDPSender &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    POSIXUDPSender& operator=(const POSIXUDPSender &);

                public:
                    virtual ~POSIXUDPSender();

                    virtual void send(const string &data) const;

                private:
                    struct sockaddr_in m_address;
                    int32_t m_fd;

                    mutable auto_ptr<Mutex> m_socketMutex;
            };

        }
    }
} // core::wrapper::POSIX

#endif /*OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXUDPSENDER_H_*/
