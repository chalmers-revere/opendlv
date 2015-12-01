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

#ifndef OPENDAVINCI_CORE_IO_CONNECTIONOBSERVER_H_
#define OPENDAVINCI_CORE_IO_CONNECTIONOBSERVER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/io/ConnectionListener.h"

namespace core {
    namespace io {

        using namespace std;

        /**
         * This class provides an interface for registering
         * an ConnectionListener to handle connection error
         */
        class OPENDAVINCI_API ConnectionObserver {
            public:
                virtual ~ConnectionObserver();

                /**
                 * This method sets or unsets a connection listener.
                 *
                 * @param sl Connection listener to be set. If set to NULL, error handling is suspended.
                 */
                virtual void setConnectionListener(ConnectionListener *cl) = 0;
        };

    }
} // core::io

#endif /*OPENDAVINCI_CORE_IO_CONNECTIONOBSERVER_H_*/
