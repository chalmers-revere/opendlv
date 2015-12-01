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

#ifndef CONTEXT_BASE_DIRECTINTERFACE_H_
#define CONTEXT_BASE_DIRECTINTERFACE_H_

#include <string>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "context/base/RuntimeControlInterface.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides access to the RuntimeControl using elsewhere
         * prepared data.
         */
        class OPENDAVINCI_API DirectInterface : public RuntimeControlInterface {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                DirectInterface(const DirectInterface&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                DirectInterface& operator=(const DirectInterface&);

            public:
                /**
                 * Constructor.
                 *
                 * @param multicastGroup Multicast group to be used for communication.
                 * @param CID Conference ID.
                 * @param configuration Configuration data.
                 */
                DirectInterface(const string &multicastGroup, const uint32_t &CID, const string &configuration);

                virtual ~DirectInterface();

                virtual const core::base::KeyValueConfiguration getConfiguration() const;

                virtual const string getMulticastGroup() const;

                virtual uint32_t getCID() const;

                virtual bool isVerbose() const;

                virtual bool isSupercomponent() const;

            private:
                core::base::KeyValueConfiguration m_configuration;
                string m_multicastGroup;
                uint32_t m_CID;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_DIRECTINTERFACE_H_*/
