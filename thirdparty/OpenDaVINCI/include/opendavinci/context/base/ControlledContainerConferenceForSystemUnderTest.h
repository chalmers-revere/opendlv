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

#ifndef CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFORSYSTEMUNDERTEST_H_
#define CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFORSYSTEMUNDERTEST_H_

// native.h must be included right before boost/asio.hpp because of the definition of _WIN32_WINNT.
#include "core/native.h"

#include "core/data/Container.h"
#include "core/io/conference/ContainerConference.h"
#include "core/io/conference/ContainerListener.h"
#include "core/io/conference/ContainerObserver.h"
#include "context/base/BlockableContainerReceiver.h"

namespace context {
    namespace base {

        using namespace std;

        class ControlledContainerConferenceFactory;

        /**
         * This class provides a controlled container conference.
         */
        class OPENDAVINCI_API ControlledContainerConferenceForSystemUnderTest : public core::io::conference::ContainerConference, public core::io::conference::ContainerListener {
            private:
                friend class ControlledContainerConferenceFactory;

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ControlledContainerConferenceForSystemUnderTest(const ControlledContainerConferenceForSystemUnderTest &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ControlledContainerConferenceForSystemUnderTest& operator=(const ControlledContainerConferenceForSystemUnderTest &);

            protected:
                /**
                 * Constructor.
                 *
                 * @param address Create controlled container conference for this address.
                 * @param port Create controlled container conference for this port.
                 * @param bcl BlockableContainerListener to which we send our Containers.
                 * @param receiveFromObserver ContainerObserver which delivers incoming Containers to us.
                 */
                ControlledContainerConferenceForSystemUnderTest(const string &address, const uint32_t &port, BlockableContainerReceiver &bcl, core::io::conference::ContainerObserver &receiveFromObserver);

            public:
                virtual ~ControlledContainerConferenceForSystemUnderTest();

                virtual void send(core::data::Container &container) const;

                virtual void nextContainer(core::data::Container &c);

                /**
                 * This method returns the BlockableContainerReceiver.
                 *
                 * @return BlockableContainerReceiver to which we send our data.
                 */
                BlockableContainerReceiver& getBlockableContainerReceiver();

            private:
                BlockableContainerReceiver &m_sendToListener;
                core::io::conference::ContainerObserver &m_receiveFromObserver;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFORSYSTEMUNDERTEST_H_*/
