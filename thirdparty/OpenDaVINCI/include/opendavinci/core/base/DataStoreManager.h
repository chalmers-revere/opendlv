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

#ifndef OPENDAVINCI_BASE_DATASTOREMANAGER_H_
#define OPENDAVINCI_BASE_DATASTOREMANAGER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/AbstractDataStore.h"
#include "core/base/KeyValueDataStore.h"
#include "core/data/Container.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This interface manages access to the data store management.
         */
        class DataStoreManager {
            public:
                virtual ~DataStoreManager();

                /**
                 * This method adds a data store for a all data types.
                 *
                 * @param dataStore Data store to be added.
                 */
                virtual void addDataStoreFor(core::base::AbstractDataStore &dataStore) = 0;

                /**
                 * This method adds a data store for a given data type.
                 *
                 * @param datatype Datatype for which a datastore should be added.
                 * @param dataStore Data store to be added.
                 */
                virtual void addDataStoreFor(const core::data::Container::DATATYPE &datatype, core::base::AbstractDataStore &dataStore) = 0;

                /**
                 * This method returns a key/value-datastore for all
                 * captured data.
                 *
                 * @return Key/Value-data store containing all received data.
                 */
                virtual core::base::KeyValueDataStore& getKeyValueDataStore() = 0;
        };

    }
} // core::base

#endif /*OPENDAVINCI_BASE_DATASTOREMANAGER_H_*/
