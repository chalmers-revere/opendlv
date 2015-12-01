/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2015 Christian Berger
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

#ifndef OPENDAVINCI_CORE_WRAPPER_ZLIB_ZLIB_H_
#define OPENDAVINCI_CORE_WRAPPER_ZLIB_ZLIB_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {
        namespace zlib {

            using namespace std;

            /**
             * This class implements an abstract object to compress
             * a string up to 16,384 bytes.
             */
            class Zlib {
                private:
                    enum {
                        BUFFER_SIZE = 16384,
                    };

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    Zlib(const Zlib &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    Zlib& operator=(const Zlib &);

                    Zlib();
                    
                public:
                    virtual ~Zlib();

                    static string compress(const string &s);
                    static string decompress(const string &s);
            };

        }
    }
} // core::wrapper::zlib

#endif /*OPENDAVINCI_CORE_WRAPPER_ZLIB_ZLIB_H_*/
