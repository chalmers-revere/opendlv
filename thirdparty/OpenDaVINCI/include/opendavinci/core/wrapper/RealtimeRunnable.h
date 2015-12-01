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

#ifndef OPENDAVINCI_CORE_WRAPPER_REALTIMERUNNABLE_H_
#define OPENDAVINCI_CORE_WRAPPER_REALTIMERUNNABLE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Runnable.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This class provides a threadable interface for realtime,
         * periodic tasks. Only a POSIX implementation for Linux
         * with rt-preempt is available at the moment.
         *
         * It can be used as follows:
         *
         * @code
         * class MyRealtimeRunnable : public RealtimeRunnable {
         *      public:
         *          MyRealtimeRunnable() : RealtimeRunnable(100 * 1000)
         *          {}
         *
         *          virtual void nextTimeSlice() {
         *              // Do some things right here.
         *          }
         * };
         * ...
         *
         * MyRealtimeRunnable rtr;
         * auto_ptr<Thread> t;
         *
         * try {
         *     t = auto_ptr<Thread>(ConcurrencyFactory::getInstance().createThread(rtr));
         * }
         * catch(string &s) {
         *    clog << "Failed: " << s << endl;
         * }
         *
         * if (t.get() != NULL) {
         *     t->start();
         * }
         *
         * ...
         * // Do some different things.
         * ...
         *
         * if (t.get() != NULL) {
         *     t->stop();
         * }
         *
         * @endcode
         */
        class OPENDAVINCI_API RealtimeRunnable : public Runnable {
            private:
                // Internal contants.
                enum {
                    MICROSECOND = 1000,                 // 1000 nanoseconds are one microsecond.
                    MILLISECOND = 1000 * MICROSECOND,   // 1000 microseconds are one millisecond.
                    SECOND      = 1000 * MILLISECOND,   // 1000 milliseconds are one second.

                    REALTIME_PRIORITY = 49 // PREMPT_RT use 50 for kernel tasklets and interrupt handler.
                };

                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                RealtimeRunnable(const RealtimeRunnable &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                RealtimeRunnable& operator=(const RealtimeRunnable &);

            protected:
                /**
                 * Constructor.
                 *
                 * @param periodInMicroseconds Duration for the slice.
                 */
                RealtimeRunnable(const uint32_t &periodInMicroseconds);

                /**
                 * This method is called periodically and must
                 * be implemented in derived classes.
                 */
                virtual void nextTimeSlice() = 0;

            public:
                virtual ~RealtimeRunnable();

            private:
                uint32_t m_periodInMicroseconds;

                virtual void run();
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_REALTIMERUNNABLE_H_*/
