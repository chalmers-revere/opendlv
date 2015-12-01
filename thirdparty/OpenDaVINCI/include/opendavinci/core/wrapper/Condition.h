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

#ifndef OPENDAVINCI_CORE_WRAPPER_CONDITION_H_
#define OPENDAVINCI_CORE_WRAPPER_CONDITION_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This interface encapsulates all methods necessary to
         * realize a condition.
         *
         * @See ConditionFactory
         */
        class Condition {
            public:
                virtual ~Condition();

                /**
                 * This method suspends the execution of the
                 * current thread.
                 */
                virtual void waitOnSignal() = 0;

                /**
                 * This method suspends the execution of the
                 * current thread with a timeout.
                 */
                virtual bool waitOnSignalWithTimeout(const unsigned long ms) = 0;

                /**
                 * This method awakes only one sleeping thread.
                 */
                virtual void wakeOne() = 0;

                /**
                 * This method awakes all threads sleeping on
                 * this condition.
                 */
                virtual void wakeAll() = 0;

                /**
                 * This method locks the condition's mutex.
                 */
                virtual void lock() = 0;

                /**
                 * This method tries to lock the condition's mutex.
                 *
                 * @return true, if the mutex could be locked.
                 */
                virtual bool tryLock() = 0;

                /**
                 * This method unlocks the condition's mutex.
                 */
                virtual void unlock() = 0;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_CONDITION_H_*/
