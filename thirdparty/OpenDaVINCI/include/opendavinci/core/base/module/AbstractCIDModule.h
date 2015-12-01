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

#ifndef OPENDAVINCI_CORE_BASE_MODULE_ABSTRACTCIDMODULE_H_
#define OPENDAVINCI_CORE_BASE_MODULE_ABSTRACTCIDMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/module/InterruptibleModule.h"
#include "core/base/module/Periodic.h"
#include "core/exceptions/Exceptions.h"

namespace core {
    namespace base {
        namespace module {

            using namespace std;

            /**
             * This class is the abstract base for every module consisting to
             * one specific CID. For concrete classes, use either ClientModule or
             * MasterModule. ClientModules use DMCP client requests for getting
             * configuration data. MasterModules however must provide DMCP services.
             */
            class OPENDAVINCI_API AbstractCIDModule : public InterruptibleModule, public Periodic {
                protected:
                    /**
                     * Constructor for any module.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @throw InvalidArgumentException if the signal handler could not be registered or if --id was found but the identifier contains white spaces.
                     */
                    AbstractCIDModule(const int32_t &argc, char **argv) throw (exceptions::InvalidArgumentException);

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    AbstractCIDModule(const AbstractCIDModule&);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    AbstractCIDModule& operator=(const AbstractCIDModule&);

                public:
                    virtual ~AbstractCIDModule();

                    virtual float getFrequency() const;

                    /**
                     * This method returns this module's identifier as
                     * specified in the command line.
                     *
                     * @return This module's identifier.
                     */
                    const string getIdentifier() const;

                    /**
                     * This method returns the module's multicast group
                     * as specified in the command line
                     */
                    const string getMultiCastGroup() const;

                    /**
                     * This method returns the CID.
                     *
                     * @return CID.
                     */
                    uint32_t getCID() const;

                    /**
                     * This method returns true, if --verbose is enabled.
                     *
                     * @return true if verbose.
                     */
                    static bool isVerbose();

                    /**
                     * This method returns the level of verbosity.
                     *
                     * @return Level of verbosity.
                     */
                    static int32_t getVerbosity();

                    /**
                     * This method returns true, if --profiling is enabled.
                     *
                     * @return true if profiling is enabled.
                     */
                    inline bool isProfiling() const {
                        return m_profiling;
                    }

                    /**
                     * This method returns true, if --realtime is enabled.
                     *
                     * @return true if realtime is enabled.
                     */
                    inline bool isRealtime() const {
                        return m_realtime;
                    }

                    /**
                     * This method returns the priority, if --realtime is enabled.
                     *
                     * @return Scheduling priority.
                     */
                    uint32_t getRealtimePriority() const;

                    virtual void waitForNextFullSecond(const uint32_t &secondsIncrement);

                private:
                    static int32_t m_verbose;

                    float m_frequency;
                    string m_identifier;
                    string m_multicastGroup;
                    uint32_t m_CID;
                    bool m_profiling;
                    bool m_realtime;
                    uint32_t m_realtimePriority;

                    /**
                     * This method tries to parse the identifier.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @throw InvalidArgumentException if --id was found but the identifier contains white spaces.
                     */
                    void parseCommandLine(const int32_t &argc, char **argv) throw (exceptions::InvalidArgumentException);
            };

        }
    }
} // core::base::module

#endif /*OPENDAVINCI_CORE_BASE_MODULE_ABSTRACTCIDMODULE_H_*/
