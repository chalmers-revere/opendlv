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

#ifndef OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSSERIALIZER_H_
#define OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSSERIALIZER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Serializer.h"

namespace core {
    namespace base {

        using namespace std;

        class SerializationFactory;

        /**
         * This class implements the interface Serializer for queryable
         * Netstrings. The original version (found at:
         * http://cr.yp.to/proto/netstrings.txt ) has been modified:
         *
         * '0xAA' '0xCF' 'binary length (as uint32_t)' 'PAYLOAD' ','
         *
         * @See Serializable
         */
        class QueryableNetstringsSerializer : public Serializer {
            private:
                // Only the SerializationFactory or its subclasses are allowed to create instances of this Serializer using non-standard constructors.
                friend class SerializationFactory;

                /**
                 * Constructor.
                 *
                 * @param out Output stream for the data.
                 */
                QueryableNetstringsSerializer(ostream &out);

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                QueryableNetstringsSerializer(const QueryableNetstringsSerializer &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                QueryableNetstringsSerializer& operator=(const QueryableNetstringsSerializer &);

            public:
                /**
                 * Default Constructor where the serialized data needs to be
                 * retrieved by calling the method getSerializedData().
                 */
                QueryableNetstringsSerializer();

                virtual ~QueryableNetstringsSerializer();

                virtual void getSerializedData(ostream &o);

                virtual void write(const uint32_t &id, const Serializable &s);

                virtual void write(const uint32_t &id, const bool &b);

                virtual void write(const uint32_t &id, const char &c);

                virtual void write(const uint32_t &id, const unsigned char &uc);

                virtual void write(const uint32_t &id, const int8_t &i);

                virtual void write(const uint32_t &id, const int16_t &i);

                virtual void write(const uint32_t &id, const uint16_t &ui);

                virtual void write(const uint32_t &id, const int32_t &i);

                virtual void write(const uint32_t &id, const uint32_t &ui);

                virtual void write(const uint32_t &id, const int64_t &i);

                virtual void write(const uint32_t &id, const uint64_t &ui);

                virtual void write(const uint32_t &id, const float &f);

                virtual void write(const uint32_t &id, const double &d);

                virtual void write(const uint32_t &id, const string &s);

                virtual void write(const uint32_t &id, const void *data, const uint32_t &size);

            private:
                ostream *m_out; // We have a pointer here that we derive from a reference parameter in our non-standard constructor; thus, the other class is responsible for the lifecycle of the variable to which we point to.
                stringstream m_buffer;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_QUERYABLENETSTRINGSSERIALIZER_H_*/
