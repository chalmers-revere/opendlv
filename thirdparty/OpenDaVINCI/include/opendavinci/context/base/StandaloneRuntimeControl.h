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

#ifndef CONTEXT_BASE_STANDALONERUNTIMECONTROL_H_
#define CONTEXT_BASE_STANDALONERUNTIMECONTROL_H_

#include <vector>

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/io/conference/ContainerConference.h"
#include "core/io/conference/ContainerListener.h"

#include "context/base/RuntimeEnvironment.h"
#include "context/base/RuntimeControl.h"
#include "context/base/SendContainerToSystemsUnderTest.h"
#include "context/base/SystemFeedbackComponent.h"

namespace context {
    namespace base {

		using namespace std;

		/**
		 * This class is the abstract base class for all standalone simulation.
		 */
		class OPENDAVINCI_API StandaloneRuntimeControl : public RuntimeControl, public core::io::conference::ContainerListener, public SendContainerToSystemsUnderTest {
			public:
				enum VEHICLECONTEXTMODULES {
					SIMPLIFIEDBICYCLEMODEL,
				};

			private:
				/**
				 * "Forbidden" copy constructor. Goal: The compiler should warn
				 * already at compile time for unwanted bugs caused by any misuse
				 * of the copy constructor.
				 */
				StandaloneRuntimeControl(const StandaloneRuntimeControl&);

				/**
				 * "Forbidden" assignment operator. Goal: The compiler should warn
				 * already at compile time for unwanted bugs caused by any misuse
				 * of the assignment operator.
				 */
				StandaloneRuntimeControl& operator=(const StandaloneRuntimeControl&);

			public:
				/**
				 * Constructor.
				 *
				 * @param sci RuntimeControlInterface to be used.
				 */
				StandaloneRuntimeControl(const RuntimeControlInterface &sci);

				virtual ~StandaloneRuntimeControl();

				virtual void sendToSystemsUnderTest(core::data::Container &c);

			protected:
				/**
				 * This method is called right before executing the actual
				 * system simulation to modify the RuntimeEnvironment to be used.
				 */
				virtual void configureRuntimeEnvironment() = 0;

				/**
				 * This method is used to add a SystemFeedbackComponent to react
				 * on incoming Containers.
				 *
				 * @param sfc SystemFeedbackComponent to be added to the RuntimeEnvironment.
				 */
				void add(SystemFeedbackComponent *sfc);

				/**
				 * This method destroys all added SystemFeedbackComponents.
				 * If they are managed outside this class, simply override this
				 * method with no method body.
				 */
				virtual void destroySystemFeedbackComponents();

			private:
				core::SharedPointer<core::io::conference::ContainerConference> m_conference;

				RuntimeEnvironment m_rte;

				core::base::Mutex m_listOfSystemFeedbackComponentsMutex;
				vector<SystemFeedbackComponent*> m_listOfSystemFeedbackComponents;

                virtual enum ERRORCODES runStandalone();

				virtual void nextContainer(core::data::Container &c);
		};

    }
} // context::base

#endif /*CONTEXT_BASE_STANDALONERUNTIMECONTROL_H_*/
