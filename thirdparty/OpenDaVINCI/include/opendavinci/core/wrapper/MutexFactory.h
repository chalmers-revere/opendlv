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

#ifndef OPENDAVINCI_CORE_WRAPPER_MUTEXFACTORY_H_
#define OPENDAVINCI_CORE_WRAPPER_MUTEXFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * Factory for creating wrapped mutexes (i.e.
         * mutexes based on WIN32, pthread, ...).
         *
         * It can be used as follows:
         *
         * @code
         * auto_ptr<Mutex> m = MutexFactory::createMutex();
         *
         * ...
         *
         * if (m.get() != NULL) {
         *     m->lock();
         * }
         *
         * ...
         * // Do some things in a critical section.
         * ...
         *
         * if (m.get() != NULL) {
         *     m->unlock();
         * }
         *
         * @endcode
         */
        struct OPENDAVINCI_API MutexFactory {
            /**
             * This method creates the mutex.
             *
             * @return mutex based on the type of instance this factory is.
             */
            static Mutex* createMutex();
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_MUTEXFACTORY_H_*/
