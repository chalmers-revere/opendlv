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

#ifndef CONTEXT_BASE_SYSTEMFEEDBACKCOMPONENT_H_
#define CONTEXT_BASE_SYSTEMFEEDBACKCOMPONENT_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/wrapper/Time.h"
#include "context/base/SystemContextComponent.h"
#include "context/base/SendContainerToSystemsUnderTest.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class can be used to compute data for system's parts.
         */
        class OPENDAVINCI_API SystemFeedbackComponent : public SystemContextComponent {
            public:
                virtual ~SystemFeedbackComponent();

                /**
                 * This method is called whenever a step for this system's
                 * part must be computed.
                 *
                 * @param t Current system time.
                 * @param sender Sender for containers to be delivered to Systems Under Tests.
                 */
                virtual void step(const core::wrapper::Time &t, SendContainerToSystemsUnderTest &sender) = 0;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_SYSTEMFEEDBACKCOMPONENT_H_*/
