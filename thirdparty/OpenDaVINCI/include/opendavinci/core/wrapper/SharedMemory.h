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

#ifndef OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORY_H_
#define OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This interface encapsulates all methods necessary to
         * realize a condition.
         *
         * @See SharedMemoryFactory
         */
        class SharedMemory {
            public:
                virtual ~SharedMemory();

                /**
                 * This method returns true if the shared memory is valid.
                 *
                 * @return true if the shared memory is valid.
                 */
                virtual bool isValid() const = 0;

                /**
                 * This method returns the name for the shared memory.
                 *
                 * @return name for the shared memory.
                 */
                virtual const string getName() const = 0;

                /**
                 * This method tries to lock the shared memory.
                 */
                virtual void lock() = 0;

                /**
                 * This method unlocks the shared memory.
                 */
                virtual void unlock() = 0;

                /**
                 * This method returns a pointer to the beginning of the
                 * shared memory.
                 *
                 * @return Pointer to the beginning of the shared memory.
                 */
                virtual void* getSharedMemory() const = 0;

                /**
                 * This method returns the size of the shared memory.
                 *
                 * @return Size of the shared memory.
                 */
                virtual uint32_t getSize() const = 0;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORY_H_*/
