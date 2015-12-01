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

#ifndef CONTEXT_BASE_RUNTIMEENVIRONMENT_H_
#define CONTEXT_BASE_RUNTIMEENVIRONMENT_H_

#include <vector>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/base/Mutex.h"
#include "core/base/module/TimeTriggeredConferenceClientModule.h"
#include "context/base/SystemFeedbackComponent.h"
#include "context/base/SystemReportingComponent.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class describes a runtime environment. A runtime environment
         * consists of several ConferenceClientModules and a SystemContext.
         */
        class OPENDAVINCI_API RuntimeEnvironment {
            private:
                enum FALLBACK_TIMESTEP {
                    DEFAULT_TIMESTEP = 1000,
                };

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                RuntimeEnvironment(const RuntimeEnvironment&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                RuntimeEnvironment& operator=(const RuntimeEnvironment&);

            public:
                RuntimeEnvironment();

                virtual ~RuntimeEnvironment();

                /**
                 * This method adds another TimeTriggeredConferenceClientModule to be
                 * executed.
                 *
                 * @param ttccm TimeTriggeredConferenceClientModule to be executed.
                 */
                void add(core::base::module::TimeTriggeredConferenceClientModule &ttccm);

                /**
                 * This method adds another SystemFeedbackComponent to be
                 * executed.
                 *
                 * @param sfc SystemFeedbackComponent to be executed.
                 */
                void add(SystemFeedbackComponent &sfc);

                /**
                 * This method adds another SystemReportingComponent to be
                 * executed.
                 *
                 * @param src SystemReportingComponent to be executed.
                 */
                void add(SystemReportingComponent &src);

                /**
                 * This method returns the greatest possible time step or
                 * DEFAULT_TIMESTEP if all getFrequency()-calls return 0.
                 *
                 * @return Greatest possible time step among all added
                 *         ConferenceClientModules and SystemContexts.
                 */
                uint32_t getGreatestTimeStep() const;

                /**
                 * This method returns the list of added TimeTriggeredConferenceClientModules.
                 *
                 * @return List of added TimeTriggeredConferenceClientModules.
                 */
                vector<core::base::module::TimeTriggeredConferenceClientModule*> getListOfTimeTriggeredConferenceClientModules();

                /**
                 * This method returns the list of added SystemFeedbackComponents.
                 *
                 * @return List of added SystemFeedbackComponents.
                 */
                vector<SystemFeedbackComponent*> getListOfSystemFeedbackComponents();

                /**
                 * This method returns the list of added SystemReportingComponents.
                 *
                 * @return List of added SystemReportingComponents.
                 */
                vector<SystemReportingComponent*> getListOfSystemReportingComponents();

                /**
                 * This method disallows the further adding of
                 * ConferenceClientModules or SystemContexts.
                 */
                void beginExecution();

                /**
                 * This method returns true, if the lists of ConferenceClientModules
                 * and SystemContexts contain at least one entry, respectively.
                 *
                 * @return true or false (see above).
                 */
                bool isValid() const;

                /**
                 * This method returns true if this runtime environment is
                 * currently executed.
                 *
                 * @return True if this RuntimeEnvironment is executed.
                 */
                bool isExecuting() const;

            private:
                /**
                 * This method computes the greatest common divisor.
                 *
                 * @param a
                 * @param b
                 * @return Greatest common divisor.
                 */
                uint32_t getGreatestCommonDivisor(const uint32_t &a, const uint32_t &b) const;

            private:
                mutable core::base::Mutex m_listsMutex;

                vector<core::base::module::TimeTriggeredConferenceClientModule*> m_listOfTimeTriggeredConferenceClientModules;
                vector<SystemFeedbackComponent*> m_listOfSystemFeedbackComponents;
                vector<SystemReportingComponent*> m_listOfSystemReportingComponents;

                mutable core::base::Mutex m_executingMutex;
                bool m_executing;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_RUNTIMEENVIRONMENT_H_*/
