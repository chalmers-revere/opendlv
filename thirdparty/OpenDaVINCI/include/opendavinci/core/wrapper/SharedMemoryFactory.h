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

#ifndef OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORYFACTORY_H_
#define OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORYFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/wrapper/SharedMemory.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * Abstract factory for creating shared memory between independent
         * processes using different implementations (i.e. WIN32 or POSIX).
         */
        struct OPENDAVINCI_API SharedMemoryFactory {
            /**
             * This method returns the shared memory.
             *
             * @param name Name of the shared memory to create.
             * @param size Required size for the new shared memory.
             * @return Shared memory based on the type of instance this factory is.
             */
            static SharedPointer<SharedMemory> createSharedMemory(const string &name, const uint32_t &size);

            /**
             * This method returns the shared memory.
             *
             * @param name Name of the shared memory to attach.
             * @return Shared memory based on the type of instance this factory is.
             */
            static SharedPointer<SharedMemory> attachToSharedMemory(const string &name);
        };
    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_SHAREDMEMORYFACTORY_H_*/
