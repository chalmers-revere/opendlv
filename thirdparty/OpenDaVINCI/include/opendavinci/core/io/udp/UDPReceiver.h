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

#ifndef OPENDAVINCI_CORE_IO_UDP_UDPRECEIVER_H_
#define OPENDAVINCI_CORE_IO_UDP_UDPRECEIVER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Mutex.h"
#include "core/io/PacketObserver.h"
#include "core/io/PacketListener.h"
#include "core/io/StringObserver.h"
#include "core/io/StringPipeline.h"

namespace core {
    namespace io {
        namespace udp {

            using namespace std;

            /**
             * This class is the abstract superclass for receiving UDP packets
             * using UDP.
             *
             * It can be used for a stringlistener receiving asynchronously
             * decoupled strings or for a packetlistener receiving synchronously
             * packets.
             *
             * @code
             * class MyStringListener : public StringListener {
             *      public:
             *          virtual void nextString(const string &s) {
             *              clog << "Received: " << s << endl;
             *          }
             * };
             * ...
             *
             * MyStringListener msl;
             * const string address = "0.0.0.0";
             * const uint32_t port = 12345;
             * SharedPointer<UDPReceiver> r;
             *
             * try {
             *     r = UDPFactory::createUDPReceiver(address, port);
             * }
             * catch(string &s) {
             *    clog << "Failed: " << s << endl;
             * }
             *
             * if (r.isValid()) {
             *     r->setStringListener(&msl);
             *     r->start();
             * }
             *
             * ...
             * // Do some different things.
             * ...
             *
             * if (r.isValid()) {
             *     r->stop();
             * }
             *
             * @endcode
             */
            class UDPReceiver : public PacketObserver, public StringObserver {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    UDPReceiver(const UDPReceiver &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    UDPReceiver& operator=(const UDPReceiver &);

                protected:
                    /**
                     * Protected constructor for enforcing subclasses for this class.
                     */
                    UDPReceiver();

                public:
                    virtual ~UDPReceiver();

                    /**
                     * This method starts the asynchronous
                     * UDP receiving.
                     */
                    virtual void start() = 0;

                    /**
                     * This method stops the asynchronous
                     * UDP receiving. A stopped receiving cannot be restarted!
                     */
                    virtual void stop() = 0;

                    virtual void setStringListener(StringListener *sl);

                    virtual void setPacketListener(PacketListener *pl);

                protected:
                    /**
                     * This method is called from deriving classes to
                     * pass newly arrived packets.
                     */
                    void nextPacket(const Packet &p);

                private:
                    StringPipeline m_stringPipeline;

                    core::base::Mutex m_packetListenerMutex;
                    PacketListener *m_packetListener;
            };

        }
    }
} // core::io::udp

#endif /*OPENDAVINCI_CORE_IO_UDP_UDPRECEIVER_H_*/
