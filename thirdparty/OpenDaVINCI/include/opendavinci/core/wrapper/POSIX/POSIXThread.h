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

#ifndef OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXTHREAD_H_
#define OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXTHREAD_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"
#include "core/wrapper/Runnable.h"
#include "core/wrapper/Thread.h"
#include "core/wrapper/SystemLibraryProducts.h"
#include "core/wrapper/ConcurrencyFactoryWorker.h"

namespace core {
    namespace wrapper {
        namespace POSIX {

            using namespace std;

            /**
             * This class is an implementation of the thread interface
             * using pthread.
             *
             * @See Thread
             */
            class POSIXThread : public Thread {
                private:
                friend class ConcurrencyFactoryWorker<SystemLibraryPosix>;

                    /**
                     * Constructor.
                     *
                     * @param r Runnable to be threadified.
                     */
                    POSIXThread(Runnable &r);

                private:
                    enum THREAD_STATE {
                        INITIALIZED,
                        RUNNING,
                        STOPPED
                    };

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    POSIXThread(const POSIXThread &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    POSIXThread& operator=(const POSIXThread &);

                public:
                    virtual ~POSIXThread();

                    virtual bool start();

                    virtual bool stop();

                    virtual bool isRunning() const;

                private:
                    auto_ptr<Mutex> m_threadStateMutex;
                    THREAD_STATE m_threadState;

                    Runnable &m_runnable;

                    pthread_t m_threadWrapper;
            };

        }
    }
} // core::wrapper::POSIX

#endif /*OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXTHREAD_H_*/
