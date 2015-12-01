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

#ifndef OPENDAVINCI_CORE_BASE_KEYVALUEDATASTORE_H_
#define OPENDAVINCI_CORE_BASE_KEYVALUEDATASTORE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/data/Container.h"
#include "core/exceptions/Exceptions.h"
#include "core/wrapper/KeyValueDatabase.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class is a key/value-based implementation for a datastore.
         * It can be used as follows:
         *
         * @code
         * KeyValueDataStore &kv = new KeyValueDataStore(wrapper::KeyValueDatabaseFactory::createKeyValueDatabase());
         * int32_t key = 1;
         * TimeStamp ts;
         * Container c(TIMESTAMP, ts);
         * kv.put(key, c);
         * @endcode
         */
        class OPENDAVINCI_API KeyValueDataStore {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                KeyValueDataStore(const KeyValueDataStore&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                KeyValueDataStore& operator=(const KeyValueDataStore&);

            public:
                /**
                 * Constructor.
                 *
                 * @param keyValueDatabase Associated key/value database.
                 * @throws NoDatabaseAvailableException if keyValueDatabase is NULL.
                 */
                KeyValueDataStore(SharedPointer<wrapper::KeyValueDatabase> keyValueDatabase) throw (core::exceptions::NoDatabaseAvailableException);

                virtual ~KeyValueDataStore();

                /**
                 * This method puts a key/value pair into the data store.
                 *
                 * @param key The key.
                 * @param value The value.
                 */
                virtual void put(const int32_t &key, const data::Container &value);

                /**
                 * This method returns the value for a key.
                 *
                 * @param key The key for which the value has to be returned.
                 * @return The value.
                 */
                data::Container get(const int32_t &key) const;

            private:
                SharedPointer<wrapper::KeyValueDatabase> m_keyValueDatabase;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_KEYVALUEDATASTORE_H_*/
