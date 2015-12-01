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

#ifndef CONTEXT_BASE_SYSTEMREPORTINGCOMPONENT_H_
#define CONTEXT_BASE_SYSTEMREPORTINGCOMPONENT_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "context/base/SystemContextComponent.h"
#include "core/wrapper/Time.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This interface can be used to compute reporting data.
         */
        class OPENDAVINCI_API SystemReportingComponent : public SystemContextComponent {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                SystemReportingComponent(const SystemReportingComponent&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                SystemReportingComponent& operator=(const SystemReportingComponent&);

            protected:
                /**
                 * Protected constructor to enforce subclasses.
                 */
                SystemReportingComponent();

            public:
                virtual ~SystemReportingComponent();

                /**
                 * This method is called whenever any application component
                 * was activated to validate the new situation.
                 *
                 * @param t Current system time.
                 */
                virtual void report(const core::wrapper::Time &t) = 0;

            private:
                /**
                 * This method returns the fixed frequency of 0 since
                 * reporting components are unscheduled but called
                 * right after any application was executed.
                 */
                virtual float getFrequency() const;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_SYSTEMREPORTINGCOMPONENT_H_*/
