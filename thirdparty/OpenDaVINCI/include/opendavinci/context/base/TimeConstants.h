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

#ifndef CONTEXT_BASE_TIMECONSTANTS_H_
#define CONTEXT_BASE_TIMECONSTANTS_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"

namespace context {
    namespace base {

        /**
         * This class contains constants.
         */
        class OPENDAVINCI_API TimeConstants {
            public:
                enum EXECUTIONTIMES {
                    ONE_MILLISECOND = 1,
                    ONE_SECOND_IN_MILLISECONDS = 1000 * ONE_MILLISECOND,
                };

                enum CONTROLTIMES {
                    ONE_MILLISECOND_IN_MICROSECONDS = 1000,
                    ONE_SECOND_IN_MICROSECONDS = 1000 * ONE_MILLISECOND_IN_MICROSECONDS,
                    MAX_WAIT_FOR_REACHING_BREAKPOINT_PER_CYCLE = 15 * ONE_SECOND_IN_MICROSECONDS,
                };
        };

    }
} // context::base

#endif /*CONTEXT_BASE_TIMECONSTANTS_H_*/
