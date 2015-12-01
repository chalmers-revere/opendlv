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

#ifndef OPENDAVINCI_CORE_IO_PROTOCOL_ABSTRACTPROTOCOL_H_
#define OPENDAVINCI_CORE_IO_PROTOCOL_ABSTRACTPROTOCOL_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Mutex.h"
#include "core/io/StringListener.h"
#include "core/io/StringSender.h"

namespace core {
    namespace io {
        namespace protocol {

            using namespace std;

            /**
             * Base class for realizing byte sequence protocols.
             */
            class OPENDAVINCI_API AbstractProtocol : public StringListener {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    AbstractProtocol(const AbstractProtocol &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    AbstractProtocol& operator=(const AbstractProtocol &);

                protected:
                    /**
                     * Protected constructor to enforce subclasses.
                     */
                    AbstractProtocol();

                public:
                    virtual ~AbstractProtocol();

                    /**
                     * This method sets the StringSender (implemented by a connection like SerialPort)
                     * that can actually send the data.
                     *
                     * @param sender StringSender that will send the data.
                     */
                    void setStringSender(StringSender *sender);

                    /**
                     * This method needs to be called by subclasses to actually send
                     * some data according to the implemented protocol.
                     *
                     * @param data Data to be sent.
                     */
                    void sendByStringSender(const string &data);

                private:
                    core::base::Mutex m_stringSenderMutex;
                    StringSender *m_stringSender;
            };

        }
    }
}

#endif /* OPENDAVINCI_CORE_IO_PROTOCOL_ABSTRACTPROTOCOL_H_ */

