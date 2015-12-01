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

#ifndef OPENDAVINCI_CORE_WRAPPER_KEYVALUEDATABASE_H_
#define OPENDAVINCI_CORE_WRAPPER_KEYVALUEDATABASE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This interface encapsulates all methods necessary to
         * implement a key/value-database.
         *
         * @See KeyValueDatabaseFactory.
         */
        class KeyValueDatabase {
            public:
                virtual ~KeyValueDatabase();

                /**
                 * This method puts a key/value pair into the database. An
                 * existing entry is substituted.
                 *
                 * @param key The key.
                 * @param value The value.
                 */
                virtual void put(const int32_t &key, const string &value) = 0;

                /**
                 * This method returns the value for a key. The value for
                 * a non-existing key is "".
                 *
                 * @param key The key for which the value has to be returned.
                 * @return The value.
                 */
                virtual const string get(const int32_t &key) const = 0;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_KEYVALUEDATABASE_H_*/
