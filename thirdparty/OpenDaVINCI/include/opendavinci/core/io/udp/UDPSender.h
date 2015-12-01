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

#ifndef OPENDAVINCI_CORE_IO_UDP_UDPSENDER_H_
#define OPENDAVINCI_CORE_IO_UDP_UDPSENDER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace io {
        namespace udp {

            using namespace std;

            /**
             * This interface encapsulates the necessary methods
             * for sending data using UDP. This interface
             * is implemented by Boost or plain POSIX system
             * calls.
             *
             * It can be used as follows:
             *
             * @code
             * const string address = "192.168.0.22";
             * const uint32_t port = 12345;
             * SharedPointer<UDPSender> s;
             *
             * try {
             *     s = UDPFactory::createUDPSender(address, port);
             * }
             * catch(string &st) {
             *    clog << "Failed: " << st << endl;
             * }
             *
             * if (s.isValid()) {
             *     s->send("ABCD");
             * }
             *
             * ...
             * @endcode
             */
            class OPENDAVINCI_API UDPSender {
                public:
                    virtual ~UDPSender();

                    /**
                     * This method sends data using UDP.
                     *
                     * @param data Data to be sent.
                     */
                    virtual void send(const string &data) const = 0;
            };

        }
    }
} // core::io::udp

#endif /*OPENDAVINCI_CORE_IO_UDP_UDPSENDER_H_*/
