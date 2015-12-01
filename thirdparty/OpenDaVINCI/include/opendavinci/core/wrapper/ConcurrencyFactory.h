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

#ifndef OPENDAVINCI_CORE_WRAPPER_CONCURRENCYFACTORY_H_
#define OPENDAVINCI_CORE_WRAPPER_CONCURRENCYFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Runnable.h"
#include "core/wrapper/Thread.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * Abstract factory for creating wrapped threads based on pthread or C++11.
         *
         * @See Thread, ConcurrencyFactoryWorker
         */
        struct OPENDAVINCI_API ConcurrencyFactory {
            /**
             * This method creates a new thread for a given Runnable object
             *
             * @param runnable The Runnable that should be threadified.
             * @return Thread based on the type of instance this factory is.
             */
            static Thread* createThread(Runnable &runnable);

            /**
             * This method causes the calling to sleep for the specified
             * amount of time.
             *
             * @param microseconds Time to sleep in ms.
             */
            static void usleepFor(const long &microseconds);

            /**
             * This methods sleeps until the specified TimeStamp or returns
             * immediately if the specified time is in the past.
             *
             * @param seconds Time point in seconds until which the execution of the current thread is suspended.
             * @param partialMicroseconds Partial microseconds.
             */
            static void usleepUntil(const long &seconds, const long &partialMicroseconds);
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_CONCURRENCYFACTORY_H_*/
