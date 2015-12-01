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

#ifndef OPENDAVINCI_BASE_CLIENTMODULE_H_
#define OPENDAVINCI_BASE_CLIENTMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/module/AbstractCIDModule.h"
#include "core/base/KeyValueConfiguration.h"
#include "core/exceptions/Exceptions.h"
#include "core/dmcp/connection/Client.h"
#include "core/dmcp/SuperComponentStateListener.h"

#include "GeneratedHeaders_CoreData.h"

namespace core {
    namespace base {
        namespace module {

            using namespace std;

            /**
             * This class is the abstract superclass for every client module.
             * For using this class, simply derive your class from ConferenceClientModule.
             *
             * @See ConferenceClientModule
             */
            class OPENDAVINCI_API ClientModule : public core::base::module::AbstractCIDModule,
                                                 protected core::dmcp::SupercomponentStateListener {
                private:
                    friend class ManagedClientModule;

                    /**
                     * Private constructor to not allow any other subclasses than ManagedClientModule to instantiate an object.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @param name Name of this module. This parameter is necessary for identifying the corresponding parts in the configuration.
                     * @throw InvalidArgumentException if the signal handler could not be registered.
                     */
                    ClientModule(const int32_t &argc, char **argv, const string &name) throw (core::exceptions::InvalidArgumentException);

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ClientModule(const ClientModule&);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ClientModule& operator=(const ClientModule&);

                public:
                    virtual ~ClientModule();

                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModule();

                    /**
                     * This method returns the module's name.
                     *
                     * @return name of the module.
                     */
                    const string getName() const;

                protected:
                    /**
                     * This method is called right before the body is executed.
                     */
                    virtual void setUp() = 0;

                    /**
                     * This method is called after returning from body.
                     */
                    virtual void tearDown() = 0;

                    /**
                     * This method contains the real module body.
                     *
                     * @return The exit code of the real body.
                     */
                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body() = 0;

                    /**
                     * This method returns the key/value-configuration
                     * for this client module.
                     *
                     * @return Key/value-configuration.
                     */
                    const core::base::KeyValueConfiguration getKeyValueConfiguration() const;

                    /**
                     * This method returns the SharedPointer for the
                     * DMCP connection.
                     *
                     * @return SharedPointer for DMCP connection.
                     */
                    core::SharedPointer<core::dmcp::connection::Client>& getDMCPClient();

                    virtual void handleConnectionLost();

                    /**
                     * This method is called after the connection to supercomponent is lost.
                     */
                    virtual void DMCPconnectionLost() = 0;

                    /**
                     * This method calls the implementation logic setUp - body - tearDown.
                     *
                     * @return return status.
                     */
                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation() = 0;

                    /**
                     * This method returns the ServerInformation object describing the supercomponent.
                     *
                     * @return ServerInformation
                     */
                    const coredata::dmcp::ServerInformation getServerInformation() const;

                private:
                    string m_name;
                    core::base::KeyValueConfiguration m_keyValueConfiguration;
                    coredata::dmcp::ServerInformation m_serverInformation;
                    core::SharedPointer<core::dmcp::connection::Client> m_dmcpClient;
            };

        }
    }
} // core::base::module

#endif /*OPENDAVINCI_BASE_CLIENTMODULE_H_*/
