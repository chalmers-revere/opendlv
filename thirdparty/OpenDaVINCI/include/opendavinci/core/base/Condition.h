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

#ifndef OPENDAVINCI_CORE_BASE_CONDITION_H_
#define OPENDAVINCI_CORE_BASE_CONDITION_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/wrapper/Condition.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class implements a condition for waiting on signals:
         *
         * Thread 1:
         * [...]
         * @code
         * Condition c;
         * Lock l(c);
         * if (sleep) {
         *    c.waitOnSignal(); // Now, the execution of the current thread is suspended.
         * }
         * @endcode
         *
         * Thread 2:
         * [...]
         * @code
         * Condition &c = thread1.getCondition();
         * Lock l(c);
         * sleep = false
         * c.wakeOne(); // Wake up sleeping thread 1.
         *
         * // or:
         * Lock l(c);
         * sleep = false
         * c.wakeAll(); // Wake up all threads sleeping on this condition.
         * @endcode
         *
         * It is important that a condition is locked before waitOnSignal,
         * wakeOne or wakeAll are called. Otherwise you will get an assertion
         * error. The lock is released before the condition is put asleep or
         * woken up and locked again before the mehtod returns. This way, sleep
         * can be modified thread-safe by different threads.
         */
        class OPENDAVINCI_API Condition : public Mutex {
            private:
                friend class Lock;

                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                Condition(const Condition&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                Condition& operator=(const Condition &);

            public:
                Condition();

                virtual ~Condition();

                /**
                 * This method suspends the execution of the
                 * current thread.
                 */
                void waitOnSignal();

                /**
                 * This methods suspends the execution of the current
                 * thread with a timeout
                 *
                 * @return TRUE if condition awakes, FALSE if timeout occurs
                 */
                bool waitOnSignalWithTimeout(const unsigned long& ms);

                /**
                 * This method awakes only one sleeping thread.
                 */
                void wakeOne();

                /**
                 * This method awakes all threads sleeping on
                 * this condition.
                 */
                void wakeAll();

            protected:
                /**
                 * This method locks the condition's mutex.
                 */
                virtual void lock();

                /**
                 * This method tries to lock the condition's mutex.
                 *
                 * @return true, if the mutex could be locked.
                 */
                virtual bool tryLock();

                /**
                 * This method unlocks the condition's mutex.
                 */
                virtual void unlock();

            private:
                SharedPointer<wrapper::Condition> m_condition;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_CONDITION_H_*/
