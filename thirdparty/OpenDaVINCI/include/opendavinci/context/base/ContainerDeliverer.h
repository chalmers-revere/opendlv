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

#ifndef CONTEXT_BASE_CONTAINERDELIVERER_H_
#define CONTEXT_BASE_CONTAINERDELIVERER_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/base/Mutex.h"
#include "core/data/Container.h"
#include "core/io/conference/ContainerListener.h"
#include "core/io/conference/ContainerObserver.h"
#include "core/wrapper/Disposable.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class delivers Containers to ControlledContainerConferenceForSystemUnderTest.
         * Every ControlledContainerConferenceForSystemUnderTest has its own
         * ContainerDeliverer to ensure synchronous delivery.
         */
        class OPENDAVINCI_API ContainerDeliverer : public core::io::conference::ContainerObserver, public core::io::conference::ContainerListener, public core::wrapper::Disposable {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ContainerDeliverer(const ContainerDeliverer&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ContainerDeliverer& operator=(const ContainerDeliverer&);

            public:
                ContainerDeliverer();

                virtual ~ContainerDeliverer();

                virtual void setContainerListener(core::io::conference::ContainerListener *cl);

                // This method is called by ControlledContainerConferenceFactory to send c to the registered ContainerListener from an app.
                virtual void nextContainer(core::data::Container &c);

            private:
                core::base::Mutex m_containerListenerMutex;
                core::io::conference::ContainerListener *m_containerListener;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_CONTAINERDELIVERER_H_*/
