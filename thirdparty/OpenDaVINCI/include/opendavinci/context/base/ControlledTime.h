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

#ifndef CONTEXT_BASE_CONTROLLEDTIME_H_
#define CONTEXT_BASE_CONTROLLEDTIME_H_

#include <stdint.h>

// native.h must be included right before boost/asio.hpp because of the definition of _WIN32_WINNT.
#include "core/native.h"

#include "core/wrapper/Time.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides a controlled time.
         */
        class OPENDAVINCI_API ControlledTime : public core::wrapper::Time {
            public:
                ControlledTime();

                /**
                 * Constructor.
                 *
                 * @param s Seconds.
                 * @param ps Partial microseconds.
                 */
                ControlledTime(const uint32_t &s, const uint32_t &ps);

                /**
                 * Copy constructor.
                 *
                 * @param ct Object from same class.
                 */
                ControlledTime(const ControlledTime &ct);

                virtual ~ControlledTime();

                /**
                 * Assignment operator.
                 *
                 * @param ct Object from same class.
                 * @return (*this).
                 */
                ControlledTime& operator=(const ControlledTime &ct);

                virtual int32_t getSeconds() const;

                virtual int32_t getPartialMicroseconds() const;

                /**
                 * This method sets the seconds.
                 *
                 * @param s Seconds.
                 */
                void setSeconds(const int32_t &s);

                /**
                 * This method sets the partial microseconds.
                 *
                 * @param partialMS Partial microseconds.
                 */
                void setPartialMicroseconds(const int32_t &partialMS);

            private:
                int32_t m_seconds;
                int32_t m_partialMicroseconds;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_CONTROLLEDTIME_H_*/
