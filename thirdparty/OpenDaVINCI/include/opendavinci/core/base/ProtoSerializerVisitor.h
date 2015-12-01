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

#ifndef OPENDAVINCI_CORE_BASE_PROTOSERIALIZERVISITOR_H_
#define OPENDAVINCI_CORE_BASE_PROTOSERIALIZERVISITOR_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Serializer.h"
#include "core/base/Visitor.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class provides a serialization visitor to encode data
         * in Google's Protobuf format.
         */
        class ProtoSerializerVisitor : public Serializer, public Visitor {
            public:
                // Protobuf type specification.
                enum PROTOBUF_TYPE { VARINT           = 0,
                                     EIGHT_BYTES      = 1, 
                                     LENGTH_DELIMITED = 2, 
                                     FOUR_BYTES       = 5, 
                                     OTHER            = 255 };

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                ProtoSerializerVisitor(const ProtoSerializerVisitor &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                ProtoSerializerVisitor& operator=(const ProtoSerializerVisitor &);

            public:
                ProtoSerializerVisitor();

                virtual ~ProtoSerializerVisitor();

                virtual void getSerializedData(ostream &o);

                /**
                 * This method serializes the data to the given ostream
                 * without adding header information.
                 *
                 * @param o Stream to serialize data to.
                 */
                void getSerializedDataNoHeader(ostream &o);

                /**
                 * This method encodes a given value using the varint encoding.
                 *
                 * @param out Output stream to be written to.
                 * @param value Value to be encoded.
                 * @return size Number of bytes written.
                 */
                uint8_t encodeVarInt(ostream &out, uint64_t value);

            private:
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

            public:
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, Serializable &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, bool &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, char &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, unsigned char &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, int8_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, int16_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, uint16_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, int32_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, uint32_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, int64_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, uint64_t &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, float &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, double &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, string &v);
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, void *data, const uint32_t &size);

            private:
                /**
                 * This method writes a given value using the provided identifier
                 * and type into the Protobuf format.
                 *
                 * @param id Identifier
                 * @param type Type identifier according to Protobuf specification
                 * @param value to be written
                 * @return Number of bytes written
                 */
                uint32_t write(const uint32_t &id, const PROTOBUF_TYPE &type, uint64_t value);

                /**
                 * This method creates the key for the Protobuf format.
                 *
                 * @param fieldId Field identifier
                 * @param protoType Protobuf type identifier
                 * @return Protobuf compliant key.
                 */
                uint32_t getKey(const uint32_t &fieldNumber, const uint8_t &protoType);

            private:
                uint32_t m_size;
                stringstream m_buffer;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_PROTOSERIALIZERVISITOR_H_*/
