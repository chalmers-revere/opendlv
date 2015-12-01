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

#ifndef CONTEXT_BASE_CONTROLLEDTIMEFACTORY_H_
#define CONTEXT_BASE_CONTROLLEDTIMEFACTORY_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/wrapper/TimeFactory.h"

#include "context/base/ControlledTime.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides controlled time factory.
         */
        class OPENDAVINCI_API ControlledTimeFactory : public core::wrapper::TimeFactory {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ControlledTimeFactory(const ControlledTimeFactory &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ControlledTimeFactory& operator=(const ControlledTimeFactory &);

            public:
                ControlledTimeFactory();

                virtual ~ControlledTimeFactory();

                virtual core::SharedPointer<core::wrapper::Time> now();

                /**
                 * This method sets the time.
                 *
                 * @param t Time.
                 */
                void setTime(const ControlledTime &ct);

            private:
                core::base::Mutex m_timeMutex;
                ControlledTime m_time;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_CONTROLLEDTIMEFACTORY_H_*/
