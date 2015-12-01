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

#ifndef OPENDAVINCI_CORE_WRAPPER_WIN32_WIN32CONCURRENCYFACTORYWORKER_H_
#define OPENDAVINCI_CORE_WRAPPER_WIN32_WIN32CONCURRENCYFACTORYWORKER_H_

// Using c++11 standard.
#include <chrono>
#include <thread>

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/SystemLibraryProducts.h"
#include "core/wrapper/ConcurrencyFactoryWorker.h"

#include "core/wrapper/WIN32/WIN32Thread.h"

namespace core {
    namespace wrapper {

        using namespace std;

        template <> class OPENDAVINCI_API ConcurrencyFactoryWorker<SystemLibraryWin32> {
            public:
                static Thread* createThread(Runnable &runnable) {
                    return new core::wrapper::WIN32Impl::WIN32Thread(runnable);
                };

				static void usleepFor(const long &microseconds) {
					std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::microseconds(microseconds));
                };

				static void usleepUntil(const long &seconds, const long &partialMicroseconds) {
                    using Clock = std::chrono::high_resolution_clock;
                    using TimePoint = std::chrono::time_point<Clock>;

                    const Clock::duration sleepUntilDuration = std::chrono::seconds(seconds) + std::chrono::microseconds(partialMicroseconds);
                    const TimePoint timePoint(sleepUntilDuration);

					std::this_thread::sleep_until(timePoint);
                };
        };
    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_WIN32_WIN32CONCURRENCYFACTORY_H_*/
