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

#ifndef OPENDAVINCI_CORE_BASE_SERVICE_H_
#define OPENDAVINCI_CORE_BASE_SERVICE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Condition.h"
#include "core/base/Mutex.h"
#include "core/exceptions/Exceptions.h"
#include "core/wrapper/Runnable.h"
#include "core/wrapper/Thread.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class combines wrapper::Runnable and wrapper::Thread
         * for convenience:
         *
         * @code
         * class MyService : public Service {
         *     virtual void beforeStop() {
         *         // This block is executed right before the thread will be stopped.
         *     }
         *
         *     virtual void run() {
         *         // Do some initialization
         *         serviceReady();
         *         // Do something.
         *     }
         * };
         *
         * int32_t main(int32_t argc, char **argv) {
         *     MyService s;
         *     s.start();
         *     s.stop();
         * }
         * @endcode
         */
        class OPENDAVINCI_API Service : public wrapper::Runnable {
            private:
                enum SERVICE_STATE {
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
                Service(const Service &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                Service& operator=(const Service &);

            public:
                Service() throw (exceptions::ThreadException) ;

                virtual ~Service();

                /**
                 * This method starts the service and will block the calling
                 * thread until serviceReady() is called.
                 */
                void start();

                /**
                 * This method stops the service.
                 * A stopped service CANNOT get restarted.
                 */
                void stop();

                /**
                 * This method has to be called by the run() method to
                 * unblock the start() method when the service is fully
                 * initialized.
                 */
                void serviceReady();

            protected:
                /**
                 * This method is called right before this service gets stopped.
                 */
                virtual void beforeStop() = 0;

                virtual void run() = 0;

                virtual bool isRunning();

            private:
                auto_ptr<core::wrapper::Thread> m_thread;

                Mutex m_serviceStateMutex;
                SERVICE_STATE m_serviceState;
                Condition m_serviceReadyCondition;
                bool m_serviceReady;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_SERVICE_H_*/
