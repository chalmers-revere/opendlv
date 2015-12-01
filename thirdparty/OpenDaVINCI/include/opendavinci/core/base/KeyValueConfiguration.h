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

#ifndef OPENDAVINCI_CORE_BASE_KEYVALUECONFIGURATION_H_
#define OPENDAVINCI_CORE_BASE_KEYVALUECONFIGURATION_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/data/SerializableData.h"
#include "core/exceptions/Exceptions.h"
#include "core/strings/StringComparator.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class is the key/value-based configuration.
         * It reads configuration data using istream:
         *
         * @code
         * KeyValueConfiguration kvc;
         * istream i = ...;
         * kvc.readFrom(i);
         *
         * @endcode
         *
         * Format for the configuration data:
         * # Comment
         * key=value
         * #key=value Disabled key.
         * anotherKey=anotherValue # Commented key-value-pair.
         */
        class OPENDAVINCI_API KeyValueConfiguration : public core::data::SerializableData {
            public:
                KeyValueConfiguration();

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                KeyValueConfiguration(const KeyValueConfiguration &obj);

                virtual ~KeyValueConfiguration();

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                KeyValueConfiguration& operator=(const KeyValueConfiguration &obj);

                virtual const string toString() const;

                virtual ostream& operator<<(ostream &out) const;

                virtual istream& operator>>(istream &in);

                /**
                 * This method is used to write configuration data using iostreams.
                 * It is necessary to allow human readable data on disks.
                 *
                 * @param out Stream to which the configuration data will be written.
                 * @return Stream.
                 */
                ostream& writeTo(ostream &out) const;

                /**
                 * This method is used to read configuration data using iostreams.
                 * It is necessary to allow human readable data on disks.
                 *
                 * @param in Stream from which the configuration data will be read.
                 * @return Stream.
                 */
                istream& readFrom(istream &in);

                /**
                 * This method returns a configuration value
                 *
                 * @code
                 * KeyValueConfiguration kvc;
                 * ...
                 * T t = kvc.getValue<T>();
                 * @endcode
                 *
                 * @param key Key for retrieving the value.
                 * @return Value.
                 * @throws ValueForKeyNotFoundException is the value for the given key does not exist.
                 */
                template<class T>
                inline T getValue(const string &key) const throw (exceptions::ValueForKeyNotFoundException) {
                    string stringValue(getValueFor(key));
                    if (stringValue == "") {
                        stringstream s;
                        s << "Value for key '" << key << "' not found.";
                        errno = 0;
                        OPENDAVINCI_CORE_THROW_EXCEPTION(ValueForKeyNotFoundException, s.str());
                    }
                    stringstream s(stringValue);
                    T value;
                    s >> value;
                    return value;
                };

                /**
                 * This method returns a subset of this key/value-configuration, i.e.
                 * all key/value-pairs starting with "section" and removes the leading
                 * "section" from the resulting subset.
                 *
                 * @param section Build subset key/value-configuration for this section without "section".
                 * @return (Empty) subset key/value-configuration.
                 */
                KeyValueConfiguration getSubsetForSectionRemoveLeadingSectionName(const string &section) const;

                /**
                 * This method returns a subset of this key/value-configuration, i.e.
                 * all key/value-pairs starting with "section".
                 *
                 * @param section Build subset key/value-configuration for this section.
                 * @return (Empty) subset key/value-configuration.
                 */
                KeyValueConfiguration getSubsetForSection(const string &section) const;

                /**
                 * This method returns a vector of keys.
                 *
                 * @return Vector of contained keys.
                 */
                const vector<string> getListOfKeys() const;

            private:
                map<string, string, core::strings::StringComparator> m_keyValueConfiguration;

                /**
                 * This method returns the string for given key or ""
                 * if the key does not exist.
                 *
                 * @param key Key for which the value is queried for.
                 * @return value for the key or "".
                 */
                string getValueFor(string key) const;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_KEYVALUECONFIGURATION_H_*/
