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

#ifndef OPENDAVINCI_CORE_IO_PACKET_H_
#define OPENDAVINCI_CORE_IO_PACKET_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace io {

        using namespace std;

        /**
         * This class provides information about a newly received packet.
         */
        class OPENDAVINCI_API Packet {
            public:
                Packet();

                /**
                 * Constructor.
                 *
                 * @param s Sender.
                 * @param d Data.
                 */
                Packet(const string &s, const string &d);

                virtual ~Packet();

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Packet(const Packet &obj);

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Packet& operator=(const Packet &obj);

                /**
                 * This method returns the sender.
                 *
                 * @return Sender.
                 */
                const string getSender() const;

                /**
                 * This method sets the sender.
                 *
                 * @param s Sender.
                 */
                void setSender(const string &s);

                /**
                 * This method returns the data.
                 *
                 * @return Data.
                 */
                const string getData() const;

                /**
                 * This method sets the data.
                 *
                 * @param d Data.
                 */
                void setData(const string &d);

            private:
                string m_sender;
                string m_data;
        };

    }
} // core::io

#endif /*OPENDAVINCI_CORE_IO_PACKET_H_*/
