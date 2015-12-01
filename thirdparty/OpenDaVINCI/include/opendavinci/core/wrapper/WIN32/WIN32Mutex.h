/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2014 - 2015 Christian Berger
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

#ifndef OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32MUTEX_H_
#define OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32MUTEX_H_

// Using c++11 standard.
#include <mutex>

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"
#include "core/wrapper/MutexFactoryWorker.h"

namespace core {
    namespace wrapper {
        namespace WIN32Impl {

            using namespace std;

            class WIN32Condition;

            /**
             * This class implements a mutex for protecting parts
             * using pthread.
             *
             * @See Mutex
             */
            class WIN32Mutex : public Mutex {
                private:
                    friend class MutexFactoryWorker<SystemLibraryWin32>;
                    friend class WIN32Condition;

                protected:
                    WIN32Mutex();

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    WIN32Mutex(const WIN32Mutex &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    WIN32Mutex& operator=(const WIN32Mutex &);

                public:
                    virtual ~WIN32Mutex();

                    virtual void lock();

                    virtual bool tryLock();

                    virtual void unlock();

                    std::mutex& getNativeMutex();

                private:
                    std::mutex m_mutex;
            };

        }
    }
} // core::wrapper::WIN32Impl

#endif /*OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32MUTEX_H_*/
