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

#ifndef OPENDAVINCI_CORE_IO_URL_H_
#define OPENDAVINCI_CORE_IO_URL_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/io/URLProtocol.h"

namespace core {
    namespace io {

        using namespace std;

        /**
         * This class represents a URL:
         *
         * URL u("file:///tmp/file.txt");
         */
        class OPENDAVINCI_API URL {
            public:
                /**
                 * Constructor.
                 *
                 * @param connection Connection string.
                 */
                URL(const string &connection);

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                URL(const URL &obj);

                virtual ~URL();

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                URL& operator=(const URL &obj);

                /**
                * This method returns true if the connection string is valid.
                *
                * @return true if the connection string is valid.
                */
                bool isValid() const;

                /**
                 * This method returns the protocol type.
                 *
                 * @return Protocol type.
                 */
                URLProtocol::PROTOCOL getProtocol() const;

                /**
                 * This method returns the resource.
                 *
                 * @return Resource.
                 */
                const string getResource() const;

                /**
                 * This method returns a string representation of the URL.
                 *
                 * @return string representation.
                 */
                const string toString() const;

            private:
                bool m_valid;
                URLProtocol::PROTOCOL m_protocol;
                string m_resource;
        };

    }
} // core::io

#endif /*OPENDAVINCI_CORE_IO_URL_H_*/
