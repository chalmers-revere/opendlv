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

#ifndef OPENDAVINCI_CORE_WRAPPER_THREAD_H_
#define OPENDAVINCI_CORE_WRAPPER_THREAD_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This class controls a thread. It can be used
         * to start different types of Runnables.
         *
         * It can be used as follows:
         *
         * @code
         * class MyRunnable : public Runnable {
         *      public:
         *          bool running;
         *          virtual bool isRunning() {
         *              // Implement some logic for controlling the running state.
         *              return running;
         *          }
         *
         *          virtual void run() {
         *              // Do some things right here.
         *          }
         * };
         * ...
         *
         * MyRunnable r;
         * r.running = false;
         * auto_ptr<Thread> t;
         *
         * try {
         *     t = auto_ptr<Thread>(ConcurrencyFactory::createThread(r));
         * }
         * catch(string &s) {
         *    clog << "Failed: " << s << endl;
         * }
         *
         * if (t.get() != NULL) {
         *     r.running = true;
         *     t->start();
         * }
         *
         * ...
         * // Do some different things.
         * ...
         *
         * if (t.get() != NULL) {
         *     r.running = false;
         *     t->stop();
         * }
         *
         * @endcode
         */
        class Thread {
            public:
                virtual ~Thread();

                /**
                 * This method starts a thread.
                 *
                 * @return true, iff the thread could be started.
                 */
                virtual bool start() = 0;

                /**
                 * This method stops a thread.
                 * A stopped thread cannot be restarted.
                 *
                 * @return true, iff the thread could be stopped.
                 */
                virtual bool stop() = 0;

                /**
                 * This method returns true, iff the thread is running.
                 *
                 * @return true, iff the thread is running.
                 */
                virtual bool isRunning() const = 0;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_THREAD_H_*/
