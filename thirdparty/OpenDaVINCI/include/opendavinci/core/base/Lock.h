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

#ifndef OPENDAVINCI_CORE_BASE_LOCK_H_
#define OPENDAVINCI_CORE_BASE_LOCK_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Mutex.h"
#include "core/SharedPointer.h"
#include "core/wrapper/SharedMemory.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class implements an automatic locking mechanism for
         * mutexes. If the control leaves the scope wherein this lock
         * was defined, the mutex is automatically released.
         *
         * @code
         * void foo() {
         *     Lock l(m_mutex);
         *     ...
         * }
         * @endcode
         */
        class OPENDAVINCI_API Lock {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Lock(const Lock&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Lock& operator=(const Lock&);

            public:
                /**
                 * Constructor.
                 *
                 * @param mutex Mutex to be automagically locked.
                 */
                Lock(Mutex &mutex);

                virtual ~Lock();

                /**
                 * Constructor.
                 *
                 * @param mutex Mutex to be automagically locked.
                 */
                Lock(core::SharedPointer<core::wrapper::SharedMemory> sp);

            private:
                Mutex *m_mutex;
                core::SharedPointer<core::wrapper::SharedMemory> m_sharedPointerToSharedMemory;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_LOCK_H_*/
