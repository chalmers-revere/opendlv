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

#ifndef CONTEXT_BASE_BLOCKABLECONTAINERLISTENER_H_
#define CONTEXT_BASE_BLOCKABLECONTAINERLISTENER_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/io/conference/ContainerListener.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides methods for blocking a ContainerListener.
         */
        class OPENDAVINCI_API BlockableContainerListener : public core::io::conference::ContainerListener {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                BlockableContainerListener(const BlockableContainerListener&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                BlockableContainerListener& operator=(const BlockableContainerListener&);

            public:
                BlockableContainerListener();

                virtual ~BlockableContainerListener();

                /**
                 * This method enables or disables the processing of
                 * containers using nextContainer(...).
                 *
                 * @param allowed True to allow nextContainer(...).
                 */
                void setNextContainerAllowed(const bool &allowed);

            protected:
                /**
                 * This method returns true if nextContainer(...) is allowed.
                 */
                bool isNextContainerAllowed() const;

            private:
                mutable core::base::Mutex m_nextContainerAllowedMutex;
                bool m_nextContainerAllowed;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_BLOCKABLECONTAINERLISTENER_H_*/
