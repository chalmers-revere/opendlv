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

#ifndef OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXMUTEX_H_
#define OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXMUTEX_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"
#include "core/wrapper/MutexFactoryWorker.h"

namespace core {
    namespace wrapper {
        namespace POSIX {

            using namespace std;

            class POSIXCondition;

            /**
             * This class implements a mutex for protecting parts
             * using pthread.
             *
             * @See Mutex
             */
            class POSIXMutex : public Mutex {
                private:
                    friend class MutexFactoryWorker<SystemLibraryPosix>;
                    friend class POSIXCondition;
                protected:
                    POSIXMutex();

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    POSIXMutex(const POSIXMutex &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    POSIXMutex& operator=(const POSIXMutex &);

                public:
                    virtual ~POSIXMutex();

                    virtual void lock();

                    virtual bool tryLock();

                    virtual void unlock();

                    pthread_mutex_t& getNativeMutex();

                private:
                    pthread_mutex_t m_mutex;
            };

        }
    }
} // core::wrapper::POSIX

#endif /*OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXMUTEX_H_*/
