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

#ifndef CONTEXT_BASE_RUNTIMECONTROL_H_
#define CONTEXT_BASE_RUNTIMECONTROL_H_

#include <vector>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/SharedPointer.h"
#include "core/base/module/AbstractModule.h"
#include "core/base/Mutex.h"
#include "core/wrapper/Time.h"
#include "core/wrapper/TimeFactory.h"
#include "context/base/TimeTriggeredConferenceClientModuleRunner.h"
#include "context/base/ControlledContainerConferenceFactory.h"
#include "context/base/ControlledTimeFactory.h"
#include "context/base/SuperComponent.h"
#include "context/base/RuntimeControlInterface.h"
#include "context/base/RuntimeEnvironment.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class controls the system's context.
         */
        class OPENDAVINCI_API RuntimeControl : public core::base::module::AbstractModule {
            public:
                enum RUNTIMECONTROL {
                    UNSPECIFIED,
                    DONT_TAKE_CONTROL,
                    TAKE_CONTROL,
                };

                enum ERRORCODES {
                    NO_ERROR_OCCURRED,
                    APPLICATIONS_FINISHED,
                    RUNTIME_TIMEOUT,
                    RUNTIMECONTROL_FORCED_TO_QUIT,
                    SETUP_NOT_CALLED,
                    EXCEPTION_CAUGHT,
                    STRING_EXCEPTION_CAUGHT,
                    UNKNOWN_EXCEPTION_CAUGHT,
                };

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                RuntimeControl(const RuntimeControl&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                RuntimeControl& operator=(const RuntimeControl&);

            public:
                /**
                 * Contructor.
                 *
                 * @param sci RuntimeControlInterface to be used.
                 */
                RuntimeControl(const RuntimeControlInterface &sci);

                virtual ~RuntimeControl();

                /**
                 * This method must be called to setup the system context.
                 *
                 * @param takeControl any of the enum, if the RuntimeControl
                 *                    should take control for
                 *                    ContainerConference and TimeFactory for
                 *                    example.
                 */
                void setup(const enum RUNTIMECONTROL &takeControl);

                /**
                 * This method actually runs the system's context.
                 *
                 * @return Any errorcode.
                 */
                enum ERRORCODES run();

                /**
                 * This method actually runs the system's context for the
                 * given application. Therefore, the system's context takes
                 * control over the ContainerConference, Time, and the
                 * ControlFlow of the application.
                 *
                 * @param rte RuntimeEnvironment.
                 * @param maxRunningTimeInSeconds Maximum running time in seconds to avoid infinite runs. Hereby, seconds are in simulation time.
                 * @return Any errorcode.
                 */
                enum ERRORCODES run(RuntimeEnvironment &rte, const uint32_t &maxRunningTimeInSeconds);

                /**
                 * This method must be called to tear down the system's context.
                 */
                void tearDown();

            protected:
                /**
                 * This method actually runs the system's context for standalone system simulations.
                 * It is overridden in StandaloneRuntimeControl. In this class this method actually
                 * does nothing.
                 *
                 * @return Any errorcode.
                 */
                virtual enum ERRORCODES runStandalone();

                /**
                 * This method calls the setup-method of all
                 * SystemContextComponents registered at the
                 * given RuntimeEnvironment.
                 *
                 * @param rte RuntimeEnvironment.
                 */
                void setupSystemContextComponents(RuntimeEnvironment &rte);

                /**
                 * This method calls the tearDown-method of all
                 * SystemContextComponents registered at the
                 * given RuntimeEnvironment.
                 *
                 * @param rte RuntimeEnvironment.
                 */
                void tearDownSystemContextComponents(RuntimeEnvironment &rte);

            private:
                // This method comes from AbstractModule and will be simply disabled.
                virtual void waitForNextFullSecond(const uint32_t &secondsIncrement);

                /**
                 * This method calls the setup-method of all
                 * SystemContextComponents and SystemReportingComponents
                 * registered at the given RuntimeEnvironment.
                 *
                 * @param rte RuntimeEnvironment.
                 */
                void setupSystemContext(RuntimeEnvironment &rte);

                /**
                 * This method calls the setup-method of all
                 * SystemContextComponents registered at the
                 * given RuntimeEnvironment.
                 *
                 * @param rte RuntimeEnvironment.
                 */
                void setupSystemReportingComponents(RuntimeEnvironment &rte);

                /**
                 * This method removes any existing ContainerConferenceFactory.
                 */
                void removeExistingContainerConferenceFactory();

                /**
                 * This method removes any existing TimeFactory.
                 */
                void removeExistingTimeFactory();

                /**
                 * This method registers all SystemReportingComponents
                 * and SystemFeedbackComponents as receivers at
                 * ControlledClientConferenceFactory.
                 *
                 * @param rte RuntimeEnvironment containing all lists.
                 */
                void registerSystemContextComponentsAddControlledContainerConferenceFactory(RuntimeEnvironment &rte);

                /**
                 * This method wraps all ConferenceClientModules into
                 * TimeTriggeredConferenceClientModuleRunners.
                 *
                 * @param rte RuntimeEnvironment
                 * @return List of wrapped ConferenceClientModules.
                 */
                vector<core::SharedPointer<TimeTriggeredConferenceClientModuleRunner> > createListOfTimeTriggeredConferenceClientModuleRunners(RuntimeEnvironment &rte);

                /**
                 * This method calls all reporting components.
                 *
                 * @param rte RuntimeEnvironment.
                 */
                void doReporting(RuntimeEnvironment &rte, const core::wrapper::Time &time);

            public:
                /**
                 * This class is used to disable to TimeFactory.
                 */
                class DisableTimeFactory : public core::wrapper::TimeFactory {
                    public:
                        void disable();

                        virtual core::SharedPointer<core::wrapper::Time> now();
                };

            private:
                core::base::Mutex m_controlMutex;
                enum RUNTIMECONTROL m_control;
                bool m_tearDownCalled;
                const RuntimeControlInterface &m_runtimeControlInterface;
                SuperComponent *m_superComponent;
                ControlledContainerConferenceFactory *m_controlledContainerConferenceFactory;
                ControlledTimeFactory *m_controlledTimeFactory;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_RUNTIMECONTROL_H_*/
