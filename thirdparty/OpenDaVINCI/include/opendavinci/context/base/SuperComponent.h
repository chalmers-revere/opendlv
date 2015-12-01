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

#ifndef CONTEXT_BASE_SUPERCOMPONENT_H_
#define CONTEXT_BASE_SUPERCOMPONENT_H_

#include <string>
#include <vector>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/SharedPointer.h"
#include "core/base/KeyValueConfiguration.h"
#include "core/io/conference/ContainerConference.h"
#include "core/dmcp/ModuleConfigurationProvider.h"
#include "core/dmcp/ModuleStateListener.h"
#include "core/dmcp/connection/ConnectionHandler.h"
#include "core/dmcp/connection/ModuleConnection.h"
#include "core/dmcp/connection/Server.h"
#include "core/dmcp/discoverer/Server.h"

#include "GeneratedHeaders_CoreData.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides supercomponent functionality.
         */
        class OPENDAVINCI_API SuperComponent : protected core::dmcp::connection::ConnectionHandler,
                                               protected core::dmcp::ModuleStateListener,
                                               public core::dmcp::ModuleConfigurationProvider {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                SuperComponent(const SuperComponent&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                SuperComponent& operator=(const SuperComponent&);

            public:
                /**
                 * Constructor for constructing a supercomponent using REAL UDP/TCP
                 * for the given CID and the given configuration.
                 *
                 * @param multicastGroup Multicast group to create supercomponent for.
                 * @param cid CID for the supercomponent.
                 * @param configuration Global configuration.
                 */
                SuperComponent(const string &multicastGroup, const uint32_t &CID, const core::base::KeyValueConfiguration &configuration);

                virtual ~SuperComponent();

                /**
                 * This method returns the real UDPMulticastClientConference.
                 *
                 * @return UDPMulticastClientConference.
                 */
                core::io::conference::ContainerConference& getContainerConference();

            protected:
                virtual core::base::KeyValueConfiguration getConfiguration(const coredata::dmcp::ModuleDescriptor& md);

                virtual core::base::KeyValueConfiguration getGlobalConfiguration() const;

                virtual void onNewModule(core::SharedPointer<core::dmcp::connection::ModuleConnection> mc);

                virtual void handleChangeState(const coredata::dmcp::ModuleDescriptor& md,
                                               const coredata::dmcp::ModuleStateMessage::ModuleState &ms);

                virtual void handleExitCode(const coredata::dmcp::ModuleDescriptor& md,
                                            const coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode &me);

                virtual void handleRuntimeStatistics(const coredata::dmcp::ModuleDescriptor& md,
                                                     const coredata::dmcp::RuntimeStatistic& rs);

                virtual void handleConnectionLost(const coredata::dmcp::ModuleDescriptor& md);

                virtual void handleUnkownContainer(const coredata::dmcp::ModuleDescriptor& md,
                                                   const core::data::Container& container);

            private:
                core::base::Mutex m_configurationMutex;
                core::base::KeyValueConfiguration m_configuration;

                auto_ptr<core::dmcp::discoverer::Server> m_discovererServer;
                auto_ptr<core::dmcp::connection::Server> m_connectionServer;

                core::SharedPointer<core::io::conference::ContainerConference> m_conference;

                vector<core::SharedPointer<core::dmcp::connection::ModuleConnection> > m_listOfModuleConnections;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_SUPERCOMPONENT_H_*/
