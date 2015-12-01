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

#ifndef OPENDAVINCI_DMCP_CONNECTION_MODULECONNECTION_H_
#define OPENDAVINCI_DMCP_CONNECTION_MODULECONNECTION_H_

#include <vector>

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/base/Condition.h"
#include "core/data/Container.h"
#include "core/dmcp/ModuleConfigurationProvider.h"
#include "core/dmcp/ModuleStateListener.h"
#include "core/io/Connection.h"
#include "core/io/ConnectionErrorListener.h"
#include "core/io/conference/ContainerListener.h"

#include "GeneratedHeaders_CoreData.h"

namespace core {
    namespace dmcp {
        namespace connection {

            using namespace std;

            class OPENDAVINCI_API ModuleConnection : protected core::io::ConnectionErrorListener,
                                                     protected core::io::conference::ContainerListener
            {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ModuleConnection(const ModuleConnection &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ModuleConnection& operator=(const ModuleConnection &);

                public:
                    ModuleConnection(core::SharedPointer<core::io::Connection> connection,
                                     ModuleConfigurationProvider &configProvider);
                    virtual ~ModuleConnection();

                    void setModuleStateListener(ModuleStateListener* listener);

                    void waitForModuleDescription();

                    /**
                     * This method sends a pulse to the connected module.
                     *
                     * @param pm Pulse to be sent.
                     */
                    void pulse(const coredata::dmcp::PulseMessage &pm);

                    /**
                     * This method sends a pulse to the connected module and
                     * requires an ACK confirmation sent from the respective,
                     * dependent module that the PULSE has been processed.
                     *
                     * @param pm Pulse to be sent.
                     * @param timeout Timeout in milliseconds to wait for the ACK message.
                     */
                    void pulse_ack(const coredata::dmcp::PulseMessage &pm, const uint32_t &timeout);

                    /**
                     * This method sends a pulse to the connected module and
                     * requires an ACK confirmation sent from the respective,
                     * dependent module that the PULSE has been processed. In
                     * addition to the original pulse_ack method, this one
                     * also receives all newly created containers from the
                     * dependent module to be transferred to supercomponent.
                     *
                     * @param pm Pulse to be sent.
                     * @param timeout Timeout in milliseconds to wait for the ACK message.
                     * @return Containers to be transferred to supercomponent.
                     */
                    vector<core::data::Container> pulse_ack_containers(const coredata::dmcp::PulseMessage &pm, const uint32_t &timeout);

                    const coredata::dmcp::ModuleDescriptor getModuleDescriptor() const;

                protected:
                    virtual void nextContainer(core::data::Container &c);
                    virtual void handleConnectionError();

                    core::SharedPointer<core::io::Connection> m_connection;
                    ModuleConfigurationProvider& m_configurationProvider;

                    core::base::Condition m_descriptorCondition;
                    coredata::dmcp::ModuleDescriptor m_descriptor;
                    bool m_hasDescriptor;

                    core::base::Condition m_pulseAckCondition;
                    bool m_hasReceivedPulseAck;

                    core::base::Condition m_pulseAckContainersCondition;
                    bool m_hasReceivedPulseAckContainers;

                    core::base::Mutex m_connectionLostMutex;
                    bool m_connectionLost;

                    ModuleStateListener* m_stateListener;
                    core::base::Mutex m_stateListenerMutex;

                    vector<core::data::Container> m_containersToBeTransferredToSupercomponent;
            };
        }
    }
} // core::dmcp

#endif /*OPENDAVINCI_DMCP_CONNECTION_MODULECONNECTION_H_*/
