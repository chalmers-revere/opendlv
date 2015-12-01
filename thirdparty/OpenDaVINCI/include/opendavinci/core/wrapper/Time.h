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

#ifndef OPENDAVINCI_CORE_WRAPPER_TIME_H_
#define OPENDAVINCI_CORE_WRAPPER_TIME_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This interface encapsulates all methods necessary for
         * getting the time since Jan. 1, 1970.
         *
         * @See TimeFactory
         */
        class Time {
            public:
                virtual ~Time();

                /**
                 * This method returns the seconds since
                 * Jan. 1, 1970.
                 *
                 * @return Seconds since Jan. 1, 1970.
                 */
                virtual int32_t getSeconds() const = 0;

                /**
                 * This method returns the partial microseconds from
                 * the next full second.
                 *
                 * @return Partial microseconds from the next full second.
                 */
                virtual int32_t getPartialMicroseconds() const = 0;

        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_TIME_H_*/
