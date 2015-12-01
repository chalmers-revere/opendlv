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

#ifndef CONTEXT_BASE_SENDCONTAINERTOSYSTEMSUNDERTEST_H_
#define CONTEXT_BASE_SENDCONTAINERTOSYSTEMSUNDERTEST_H_

// native.h must be included right before boost/asio.hpp because of the definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/data/Container.h"
#include "core/io/conference/ContainerListener.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This interface provides methods to send data to a
         * ControlledContainerConferenceForSystemsUnderTest.
         */
        class OPENDAVINCI_API SendContainerToSystemsUnderTest {
            public:
                virtual ~SendContainerToSystemsUnderTest();

                /**
                 * This method sends a Container to all created
                 * ControlledContainerConferences and thus to all
                 * running systems under test.
                 *
                 * @param c Container to send.
                 */
                virtual void sendToSystemsUnderTest(core::data::Container &c) = 0;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_SENDCONTAINERTOSYSTEMSUNDERTEST_H_*/
