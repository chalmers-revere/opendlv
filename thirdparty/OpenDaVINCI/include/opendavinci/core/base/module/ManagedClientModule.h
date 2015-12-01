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

#ifndef OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_
#define OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/base/module/Breakpoint.h"
#include "core/base/module/ClientModule.h"
#include "core/base/module/ManagedClientModuleContainerConference.h"
#include "core/data/TimeStamp.h"
#include "core/exceptions/Exceptions.h"
#include "core/io/conference/ContainerConference.h"

#include "context/base/Clock.h"
#include "context/base/ControlledTimeFactory.h"

namespace core {
    namespace base {
        namespace module {

            using namespace std;

            /**
             * This class manages the local module:
             *  - unsupervised distributed execution
             *  - supervised distributed execution
             *
             * @See AbstractConferenceClientModule
             */
            class OPENDAVINCI_API ManagedClientModule : public core::base::module::ClientModule, public core::base::module::Breakpoint {
                private:
                    friend class AbstractConferenceClientModule;

                    /**
                     * Private constructor to not allow any other subclasses than ConferenceClientModule to instantiate an object.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @param name Name of this module. This parameter is necessary for identifying the corresponding parts in the configuration.
                     * @throw InvalidArgumentException if the signal handler could not be registered.
                     */
                    ManagedClientModule(const int32_t &argc, char **argv, const string &name) throw (core::exceptions::InvalidArgumentException);

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ManagedClientModule(const ManagedClientModule&);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ManagedClientModule& operator=(const ManagedClientModule&);

                public:
                    virtual ~ManagedClientModule();

                    /**
                     * @return Start time of the current execution cycle.
                     */
                    const core::data::TimeStamp getStartOfCurrentCycle() const;

                    /**
                     * @return End time of the last execution cycle.
                     */
                    const core::data::TimeStamp getStartOfLastCycle() const;

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

                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation();

                    virtual void reached();

                private:
                    virtual void wait();

                    virtual void DMCPconnectionLost();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_None();
                    void wait_ManagedLevel_None();
                    void wait_ManagedLevel_None_realtime();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_Pulse();
                    void wait_ManagedLevel_Pulse();
                    void reached_ManagedLevel_Pulse();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_Pulse_Shift();
                    void wait_ManagedLevel_Pulse_Shift();
                    void reached_ManagedLevel_Pulse_Shift();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_Pulse_Time();
                    void wait_ManagedLevel_Pulse_Time();
                    void reached_ManagedLevel_Pulse_Time();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_Pulse_Time_Ack();
                    void wait_ManagedLevel_Pulse_Time_Ack();
                    void reached_ManagedLevel_Pulse_Time_Ack();

                    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModuleImplementation_ManagedLevel_Pulse_Time_Ack_Containers();
                    void wait_ManagedLevel_Pulse_Time_Ack_Containers();
                    void reached_ManagedLevel_Pulse_Time_Ack_Containers();

                    /**
                     * This method calculates the waiting time in microseconds
                     * to complete this execution cycle. Furthermore, it updates
                     * RuntimeStatistics and local profiling data.
                     *
                     * @return waiting time in microseconds.
                     */
                    uint32_t getWaitingTimeAndUpdateRuntimeStatistics();

                    /**
                     * This method is used to log the time consumption (load)
                     * for this module into a profiling file.
                     */
                    void logProfilingData(const core::data::TimeStamp &current,
                                          const core::data::TimeStamp &lastCycle,
                                          const float &freq,
                                          const long &lastWaitTime,
                                          const long &timeConsumptionCurrent,
                                          const long &nominalDuration,
                                          const long &waitingTimeCurrent,
                                          const int32_t &cycleCounter);

                protected:
                    /**
                     * This method sets the ContainerConference to be used. In the case
                     * that we are not executed as managed level == ML_SIMULATION or ML_SIMULATION_RT,
                     * the deriving class ConferenceClientModule sets the UDP-based ContainerConference
                     * here.
                     *
                     * In the case the we are executed as managed level = ML_SIMULATION or
                     * ML_SIMULATION_RT, the UDP-based container conference will be replaced
                     * by a pure software implementation one.
                     *
                     * @param c Pointer to the ContainerConference to be used.
                     */
                    void setContainerConference(core::SharedPointer<core::io::conference::ContainerConference> c);

                    /**
                     * This method returns the ContainerConference to be used. In the case
                     * that we are executed as managed level == ML_SIMULATION or ML_SIMULATION_RT
                     * we replace the UDP-based one with a local ContainerConference.
                     *
                     * @return Pointer to the ContainerConference to be used.
                     */
                    core::SharedPointer<core::io::conference::ContainerConference> getContainerConference();

                private:
#ifndef WIN32
                    struct timespec m_waitForSlice;
#endif
                    core::data::TimeStamp m_startOfCurrentCycle;
                    core::data::TimeStamp m_startOfLastCycle;
                    core::data::TimeStamp m_lastCycle;
                    long m_lastWaitTime;
                    int32_t m_cycleCounter;
                    ofstream *m_profilingFile;

                    bool m_firstCallToBreakpoint_ManagedLevel_Pulse;

                    context::base::Clock m_time;
                    context::base::ControlledTimeFactory *m_controlledTimeFactory;

                    coredata::dmcp::PulseMessage m_pulseMessage;
                    core::SharedPointer<core::io::conference::ContainerConference> m_localContainerConference;
                    bool m_hasExternalContainerConference;
                    core::SharedPointer<core::io::conference::ContainerConference> m_containerConference;
            };

        }
    }
} // core::base::module

#endif /*OPENDAVINCI_BASE_MANAGEDCLIENTMODULE_H_*/
