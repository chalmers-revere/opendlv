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

#ifndef CONTEXT_BASE_RUNNER_H_
#define CONTEXT_BASE_RUNNER_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/base/module/Periodic.h"
#include "core/wrapper/Time.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class is the base class for periodic executions.
         */
        class OPENDAVINCI_API Runner : public core::base::module::Periodic {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                Runner(const Runner&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                Runner& operator=(const Runner&);

            protected:
                /**
                 * Protected constructor to enforce subclasses.
                 */
                Runner();

            public:
                virtual ~Runner();

                /**
                 * This method returns true if the app  needs to be executed.
                 *
                 * @param t Time.
                 * @return True if the app needs to be executed at time t.
                 */
                bool needsExecution(const core::wrapper::Time &t) const;

                /**
                 * This method should be overridden in subclasses to add an additional
                 * condition to the time needsExecution indicating whether an
                 * application has finished. This implementation simply
                 * returns always true.
                 *
                 * @return true.
                 */
                virtual bool hasFinished() const;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_RUNNER_H_*/
