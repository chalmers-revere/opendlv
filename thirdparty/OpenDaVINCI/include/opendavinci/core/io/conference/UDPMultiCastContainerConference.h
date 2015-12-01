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

#ifndef OPENDAVINCI_CORE_IO_CONFERENCE_UDPMULTICASTCONTAINERCONFERENCE_H_
#define OPENDAVINCI_CORE_IO_CONFERENCE_UDPMULTICASTCONTAINERCONFERENCE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/exceptions/Exceptions.h"
#include "core/io/conference/ContainerConference.h"
#include "core/io/StringListener.h"
#include "core/io/udp/UDPSender.h"
#include "core/io/udp/UDPReceiver.h"

namespace core {
    namespace io {
        namespace conference {

            using namespace std;

            /**
             * This class encapsulates a conference about containers.
             * Therefore, it uses UDP multicast sending and receiving for
             * sending and receiving containers. Therefore, it implements
             * a StringListener for getting informed about new strings from
             * the UDPReceiver and informs any connected ContainerListener.
             */
            class OPENDAVINCI_API UDPMultiCastContainerConference : public ContainerConference, public core::io::StringListener {
                private:
                    friend class ContainerConferenceFactory;

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    UDPMultiCastContainerConference(const UDPMultiCastContainerConference &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    UDPMultiCastContainerConference& operator=(const UDPMultiCastContainerConference &);

                protected:
                    /**
                     * Constructor.
                     *
                     * @param address Use address for joining.
                     * @param port Use port for joining.
                     * @throws ConferenceException if the conference could not be created.
                     */
                    UDPMultiCastContainerConference(const string &address, const uint32_t &port) throw (exceptions::ConferenceException);

                public:
                    virtual ~UDPMultiCastContainerConference();

                    virtual void nextString(const string &s);

                    virtual void send(core::data::Container &container) const;

                private:
                    core::SharedPointer<core::io::udp::UDPSender> m_sender;
                    core::SharedPointer<core::io::udp::UDPReceiver> m_receiver;
            };

        }
    }
} // core::io::conference

#endif /*OPENDAVINCI_CORE_IO_CONFERENCE_UDPMULTICASTCONTAINERCONFERENCE_H_*/
