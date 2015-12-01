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

#ifndef OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONCURRENCYFACTORYWORKER_H_
#define OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONCURRENCYFACTORYWORKER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/SystemLibraryProducts.h"
#include "core/wrapper/ConcurrencyFactoryWorker.h"

#include "core/wrapper/POSIX/POSIXThread.h"

namespace core {
    namespace wrapper {

        using namespace std;

        template <> class OPENDAVINCI_API ConcurrencyFactoryWorker<SystemLibraryPosix> {
            public:
                static Thread* createThread(Runnable &runnable) {
                    return new core::wrapper::POSIX::POSIXThread(runnable);
                };

                static void usleepFor(const long &microseconds) {
                    struct timespec delay;

                    delay.tv_sec = 0;
                    delay.tv_nsec = 0;

                    const long NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;
                    long nanoseconds = microseconds * 1000;
                    while (nanoseconds >= NANOSECONDS_PER_SECOND) {
                        nanoseconds -= NANOSECONDS_PER_SECOND;
                        delay.tv_sec++;
                    }
                    // Add remaining nanoseconds.
                    delay.tv_nsec += nanoseconds;

#if defined(__FreeBSD__) || (__NetBSD__) || (__OpenBSD__) || (__APPLE__) || (__DragonFly__)
                    nanosleep(&delay, NULL);
#else
                    // Same call as nanosleep(&delay, NULL);
                    clock_nanosleep(CLOCK_REALTIME, 0, &delay, NULL);
#endif
                };

                static void usleepUntil(const long &seconds, const long &partialMicroseconds) {
#if defined(__FreeBSD__) || (__NetBSD__) || (__OpenBSD__) || (__APPLE__) || (__DragonFly__)
                    struct timeval now;
                    gettimeofday(&now, NULL);

                    long waitSec = seconds - now.tv_sec;
                    long waitPartialMicrosec = partialMicroseconds - now.tv_usec;

                    // Check for partial microseconds being negative (waiting not a full second but just a remainder).
                    if (waitPartialMicrosec < 0) {
                        waitSec--;
                        waitPartialMicrosec += (1000 * 1000); 
                    }

                    if ((waitSec > 0) && (waitPartialMicrosec > 0)) {
                        struct timespec delay;
                        delay.tv_sec = waitSec;
                        delay.tv_nsec = waitPartialMicrosec;

                        const long NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;
                        long nanoseconds = delay.tv_nsec * 1000;
                        while (nanoseconds >= NANOSECONDS_PER_SECOND) {
                            nanoseconds -= NANOSECONDS_PER_SECOND;
                            delay.tv_sec++;
                        }
                        // Add remaining nanoseconds.
                        delay.tv_nsec += nanoseconds;

                        // Use regular nanosleep.
                        nanosleep(&delay, NULL);
                    }
#else
                    struct timespec timePoint;
                    timePoint.tv_sec = seconds;
                    timePoint.tv_nsec = 0;

                    const long NANOSECONDS_PER_SECOND = 1000 * 1000 * 1000;
                    long nanoseconds = partialMicroseconds * 1000;
                    while (nanoseconds >= NANOSECONDS_PER_SECOND) {
                        nanoseconds -= NANOSECONDS_PER_SECOND;
                        timePoint.tv_sec++;
                    }
                    // Add remaining nanoseconds.
                    timePoint.tv_nsec += nanoseconds;

                    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &timePoint, NULL);
#endif
                };
        };
    }
} // core::wrapper::POSIX

#endif /*OPENDAVINCI_CORE_WRAPPER_POSIX_POSIXCONCURRENCYFACTORY_H_*/
