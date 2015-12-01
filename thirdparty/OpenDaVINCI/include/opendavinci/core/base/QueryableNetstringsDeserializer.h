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
 * License along with this library; if not, read to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSDESERIALIZER_H_
#define OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSDESERIALIZER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Deserializer.h"

namespace core {
    namespace base {

        using namespace std;

        class SerializationFactory;

        /**
         * This class implements the interface Deserializer for queryable
         * Netstrings. The original version (found at:
         * http://cr.yp.to/proto/netstrings.txt ) has been modified:
         *
         * '0xAA' '0xCF' 'binary length (as uint32_t)' 'PAYLOAD' ','
         *
         * @See Serializable
         */
        class OPENDAVINCI_API QueryableNetstringsDeserializer : public Deserializer {
            private:
                // Only the SerializationFactory or its subclasses are allowed to create instances of this Deserializer using the non-standard constructor.
                friend class SerializationFactory;

                /**
                 * Constructor.
                 *
                 * @param in Input stream containing the data.
                 */
                QueryableNetstringsDeserializer(istream &in);

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                QueryableNetstringsDeserializer(const QueryableNetstringsDeserializer &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                QueryableNetstringsDeserializer& operator=(const QueryableNetstringsDeserializer &);


            public:
                /**
                 * Default constructor. When a QueryableNetstringsDeserializer is created
                 * using this constructor, the method setSerializedData(istream &in) needs
                 * to be called before any read(...) method will return meaningful data.
                 */
                QueryableNetstringsDeserializer();

                virtual void deserializeDataFrom(istream &in);

                virtual ~QueryableNetstringsDeserializer();

                virtual void read(const uint32_t &id, Serializable &s);

                virtual void read(const uint32_t &id, bool &b);

                virtual void read(const uint32_t &id, char &c);

                virtual void read(const uint32_t &id, unsigned char &uc);

                virtual void read(const uint32_t &id, int8_t &i);

                virtual void read(const uint32_t &id, int16_t &i);

                virtual void read(const uint32_t &id, uint16_t &ui);

                virtual void read(const uint32_t &id, int32_t &i);

                virtual void read(const uint32_t &id, uint32_t &ui);

                virtual void read(const uint32_t &id, int64_t &i);

                virtual void read(const uint32_t &id, uint64_t &ui);

                virtual void read(const uint32_t &id, float &f);

                virtual void read(const uint32_t &id, double &d);

                virtual void read(const uint32_t &id, string &s);

                virtual void read(const uint32_t &id, void *data, const uint32_t &size);

            private:
                stringstream m_buffer;
                map<uint32_t, streampos> m_values;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSDESERIALIZER_H_*/
