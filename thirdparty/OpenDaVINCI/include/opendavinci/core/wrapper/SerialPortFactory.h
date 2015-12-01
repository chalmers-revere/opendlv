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

#ifndef OPENDAVINCI_CORE_WRAPPER_SERIALPORTFACTORY_H_
#define OPENDAVINCI_CORE_WRAPPER_SERIALPORTFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/SerialPort.h"

namespace core {
    namespace wrapper {

        using namespace std;

        class OPENDAVINCI_API SerialPortFactory {
            public:
                /**
                 * This method creates a serial port.
                 *
                 * @param port Serial port.
                 * @param baudRate Baud rate.
                 * @return A new serial port
                 */
                static SerialPort* createSerialPort(const string &port, const uint32_t &baudRate);
        };
    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_SERIALPORTFACTORY_H_*/
