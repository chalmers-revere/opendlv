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

#ifndef OPENDAVINCI_CORE_BASE_MUTEX_H_
#define OPENDAVINCI_CORE_BASE_MUTEX_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/wrapper/Mutex.h"

namespace core {
    namespace base {

        using namespace std;

        // Forward declaration to prevent circular dependencies.
        class Lock;

        /**
         * This class implements a mutex for protecting parts
         * of the code for simultaneous access using the concept
         * of scoped locks:
         *
         * @code
         * // Declaration elsewhere.
         * Mutex m;
         *
         * ...
         * {
         *     Lock l(m);
         *     // Critical part.
         * }
         * ...
         * @endcode
         *
         * If a mutex gets destroyed it will be unlocked before automatically!
         */
        class OPENDAVINCI_API Mutex {
            private:
                friend class Lock;

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                Mutex(const Mutex&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                Mutex& operator=(const Mutex &);

            public:
                Mutex();

                virtual ~Mutex();

            protected:
                /**
                 * This method locks this mutex.
                 */
                virtual void lock();

                /**
                 * This method tries to lock this mutex.
                 *
                 * @return true, if the mutex could be locked.
                 */
                virtual bool tryLock();

                /**
                 * This method unlocks this mutex.
                 */
                virtual void unlock();

            private:
                SharedPointer<core::wrapper::Mutex> m_mutex;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_MUTEX_H_*/
