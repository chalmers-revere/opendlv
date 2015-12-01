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

#ifndef OPENDAVINCI_CORE_WRAPPER_TIMEFACTORY_H_
#define OPENDAVINCI_CORE_WRAPPER_TIMEFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/wrapper/Libraries.h"
#include "core/wrapper/ConfigurationTraits.h"
#include "core/wrapper/Mutex.h"

#include "core/wrapper/TimeFactoryWorker.h"

#ifdef WIN32
    #include "core/wrapper/WIN32/WIN32TimeFactoryWorker.h"
#endif
#ifndef WIN32
    #include "core/wrapper/POSIX/POSIXTimeFactoryWorker.h"
#endif

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * Factory for creating wrapped times (i.e.
         * time data structures based on Win32, POSIX, ...).
         *
            * It can be used as follows:
            *
            * @code
            * SharedPointer<Time> t(TimeFactory::getInstance().now());
            *
            * ...
            *
            * @endcode
         */
        class OPENDAVINCI_API TimeFactory {
            public:
                virtual ~TimeFactory();
                virtual core::SharedPointer<core::wrapper::Time> now();
                static TimeFactory& getInstance();

            protected:
                TimeFactory();
                static void setSingleton(TimeFactory *tf);
                static TimeFactory *instance;
                static TimeFactory *controlledInstance;

            private:
                static auto_ptr<Mutex> m_singletonMutex;
        };

        class OPENDAVINCI_API SystemTimeFactory {
            public:
                // Product type created by factory.
                typedef SystemLibraryProducts product_type;

                // Configuration for factory.
                typedef ConfigurationTraits<product_type>::configuration configuration_value;

                // Factory method to create a concrete product
                typedef TimeFactoryWorker<configuration_value::value> worker_type;

                // Wrapper to preserve current functionality.
                static worker_type getInstance() { return instance; }

            protected:
                // Wrapper to preserve current functionality.
                static worker_type instance;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_TIMEFACTORY_H_*/
