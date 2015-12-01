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

#ifndef OPENDAVINCI_CORE_WRAPPER_SIMPLEDB_SIMPLEDB_H_
#define OPENDAVINCI_CORE_WRAPPER_SIMPLEDB_SIMPLEDB_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/wrapper/KeyValueDatabase.h"
#include "core/wrapper/Mutex.h"

#include "core/wrapper/KeyValueDatabaseFactoryWorker.h"
#include "core/wrapper/KeyValueDatabaseLibraryProducts.h"

namespace core {
    namespace wrapper {
        namespace SimpleDB {

            using namespace std;

            class SimpleDB : public KeyValueDatabase {
                friend class KeyValueDatabaseFactoryWorker<KeyValueDatabaseSimpleDB>;
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    SimpleDB(const SimpleDB &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    SimpleDB& operator=(const SimpleDB &);

                protected:
                    SimpleDB();

                public:
                    virtual ~SimpleDB();

                    virtual void put(const int32_t &key, const string &value);

                    virtual const string get(const int32_t &key) const;

                protected:
                    auto_ptr<Mutex> m_mutex;
                    mutable map<int, string> m_entries;
            };

        }
    }
} // core::wrapper::SimpleDB

#endif /*OPENDAVINCI_CORE_WRAPPER_SIMPLEDB_SIMPLEDB_H_*/
