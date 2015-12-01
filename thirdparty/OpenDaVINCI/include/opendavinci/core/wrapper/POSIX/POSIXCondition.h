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

#ifndef OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONDITION_H_
#define OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONDITION_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Condition.h"
#include "core/wrapper/ConditionFactoryWorker.h"
#include "core/wrapper/SystemLibraryProducts.h"
#include "core/wrapper/POSIX/POSIXMutex.h"

namespace core {
    namespace wrapper {
        namespace POSIX {

            using namespace std;

            /**
             * This class implements a condition for protecting parts
             * using pthread. It is derived from POSIXMutex for getting
             * the native mutex handle in pthread_cond_wait(...) calls.
             *
             * @See Condition.
             */
            class POSIXCondition : public Condition {
                private:
                    friend class ConditionFactoryWorker<SystemLibraryPosix>;

                    POSIXCondition();

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    POSIXCondition(const POSIXCondition &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    POSIXCondition& operator=(const POSIXCondition &);

                public:
                    virtual ~POSIXCondition();

                    virtual void waitOnSignal();

                    virtual bool waitOnSignalWithTimeout(const unsigned long ms);

                    virtual void wakeOne();

                    virtual void wakeAll();

                    virtual void lock();

                    virtual bool tryLock();

                    virtual void unlock();

                private:
                    pthread_cond_t m_condition;
                    POSIXMutex m_mutex;
            };

        }
    }
} // core::wrapper::POSIX

#endif /*OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONDITION_H_*/
