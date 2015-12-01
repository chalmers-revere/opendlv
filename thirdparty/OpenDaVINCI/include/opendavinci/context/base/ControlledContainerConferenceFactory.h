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

#ifndef CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFACTORY_H_
#define CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFACTORY_H_

#include <string>
#include <vector>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/data/Container.h"
#include "core/io/conference/ContainerConference.h"
#include "core/io/conference/ContainerConferenceFactory.h"
#include "core/io/conference/ContainerListener.h"
#include "context/base/BlockableContainerReceiver.h"
#include "context/base/ContainerDeliverer.h"
#include "context/base/ControlledContainerConference.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides controlled ContainerConferences.
         */
        class OPENDAVINCI_API ControlledContainerConferenceFactory : public core::io::conference::ContainerConferenceFactory, public ControlledContainerConference, public core::io::conference::ContainerListener {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ControlledContainerConferenceFactory(const ControlledContainerConferenceFactory &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ControlledContainerConferenceFactory& operator=(const ControlledContainerConferenceFactory &);

            public:
                ControlledContainerConferenceFactory();

                virtual ~ControlledContainerConferenceFactory();

                /**
                 * This method returns a new ContainerConference.
                 *
                 * @param address Use address for joining.
                 * @param port Use port for joining.  If omitted, MULTICAST_PORT will be used.
                 * @return ContainerConference or NULL.
                 */
                virtual core::SharedPointer<core::io::conference::ContainerConference> getContainerConference(const string &address, const uint32_t &port = core::io::conference::ContainerConferenceFactory::MULTICAST_PORT);

                virtual void sendToSystemsUnderTest(core::data::Container &c);

                virtual void add(core::io::conference::ContainerListener *cl);

                // This method is called by BlockableContainerReceiver to send data FROM a specific System Under Test to all SystemParts.
                // Furthermore, every container send from a System Under Test is also dispatched to all Systems Under Test using sendToSystemsUnderTest
                virtual void nextContainer(core::data::Container &c);

            private:
                /**
                 * This method sends the given container to all systems under test
                 * without modifying the sent timestamp.
                 *
                 * @param c Container to send.
                 */
                void sendToSUD(core::data::Container &c);

                /**
                 * This method sends the given container to all SystemContextComponents
                 * without modifying the sent timestamp.
                 *
                 * @param c Container to send.
                 */
                void sendToSCC(core::data::Container &c);

            private:
                core::base::Mutex m_listOfContainerListenersToReceiveContainersFromSystemsUnderTestMutex;
                vector<core::io::conference::ContainerListener*> m_listOfContainerListenersToReceiveContainersFromSystemsUnderTest;

                // Using ContainerDeliverers, synchronous communication is enforced.
                core::base::Mutex m_listOfContainerDelivererToSystemUnderTestMutex;
                vector<ContainerDeliverer*> m_listOfContainerDelivererToSystemUnderTest;

                core::base::Mutex m_listOfContainerDelivererFromSystemUnderTestMutex;
                vector<BlockableContainerReceiver*> m_listOfContainerDelivererFromSystemUnderTest;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_CONTROLLEDCONTAINERCONFERENCEFACTORY_H_*/
