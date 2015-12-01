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

#ifndef OPENDAVINCI_CORE_WRAPPER_RUNNABLE_H_
#define OPENDAVINCI_CORE_WRAPPER_RUNNABLE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This class provides a threadable interface. Any class that
         * wants to be able to start a new thread must implement
         * this interface.
         *
         * @See Thread
         */
        class OPENDAVINCI_API Runnable {
            public:
                virtual ~Runnable();

                /**
                 * This method returns true, iff this runnable is in its
                 * internal state for running.
                 *
                 * @return true iff this runnable is in its internal state for running.
                 */
                virtual bool isRunning() = 0;

                /**
                 * This method implements the body of the function
                 * to be "threadified".
                 */
                virtual void run() = 0;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_RUNNABLE_H_*/
