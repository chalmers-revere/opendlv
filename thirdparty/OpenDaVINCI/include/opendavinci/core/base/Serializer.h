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

#ifndef OPENDAVINCI_CORE_BASE_SERIALIZER_H_
#define OPENDAVINCI_CORE_BASE_SERIALIZER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace base {

        using namespace std;

        class Serializable;

        /**
         * This class is the interface for any serializer.
         *
         * @See Serializable
         */
        class OPENDAVINCI_API Serializer {
            public:
                Serializer();

                virtual ~Serializer();

                /**
                 * This method returns the data in serialized format.
                 *
                 * @param o ostream object (for instance a stringstream) to serialize the data into.
                 */
                virtual void getSerializedData(ostream &o) = 0;

                /**
                 * This method serializes a Serializable.
                 *
                 * @param id Identifier for the s to be serialized.
                 * @param s Serializable to be serialized.
                 */
                virtual void write(const uint32_t &id, const Serializable &s) = 0;

                /**
                 * This method serializes a bool.
                 *
                 * @param id Identifier for the b to be serialized.
                 * @param b Bool to be serialized.
                 */
                virtual void write(const uint32_t &id, const bool &b) = 0;

                /**
                 * This method serializes a char.
                 *
                 * @param id Identifier for the c to be serialized.
                 * @param c Char to be serialized.
                 */
                virtual void write(const uint32_t &id, const char &c) = 0;

                /**
                 * This method serializes an unsigned char.
                 *
                 * @param id Identifier for the uc to be serialized.
                 * @param uc Unsigned char to be serialized.
                 */
                virtual void write(const uint32_t &id, const unsigned char &uc) = 0;

                /**
                 * This method serializes an int.
                 *
                 * @param id Identifier for the i to be serialized.
                 * @param i Int to be serialized.
                 */
                virtual void write(const uint32_t &id, const int8_t &i) = 0;

                /**
                 * This method serializes an int.
                 *
                 * @param id Identifier for the i to be serialized.
                 * @param i Int to be serialized.
                 */
                virtual void write(const uint32_t &id, const int16_t &i) = 0;

                /**
                 * This method serializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be serialized.
                 * @param ui Unsigned int32_t to be serialized.
                 */
                virtual void write(const uint32_t &id, const uint16_t &ui) = 0;

                /**
                 * This method serializes an int.
                 *
                 * @param id Identifier for the i to be serialized.
                 * @param i Int to be serialized.
                 */
                virtual void write(const uint32_t &id, const int32_t &i) = 0;

                /**
                 * This method serializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be serialized.
                 * @param ui Unsigned int32_t to be serialized.
                 */
                virtual void write(const uint32_t &id, const uint32_t &ui) = 0;

                /**
                 * This method serializes an int.
                 *
                 * @param id Identifier for the i to be serialized.
                 * @param i Int to be serialized.
                 */
                virtual void write(const uint32_t &id, const int64_t &i) = 0;

                /**
                 * This method serializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be serialized.
                 * @param ui Unsigned int32_t to be serialized.
                 */
                virtual void write(const uint32_t &id, const uint64_t &ui) = 0;

                /**
                 * This method serializes a float.
                 *
                 * @param id Identifier for the f to be serialized.
                 * @param f Float to be serialized.
                 */
                virtual void write(const uint32_t &id, const float &f) = 0;

                /**
                 * This method serializes a double.
                 *
                 * @param id Identifier for the d to be serialized.
                 * @param d Double to be serialized.
                 */
                virtual void write(const uint32_t &id, const double &d) = 0;

                /**
                 * This method serializes a string.
                 *
                 * @param id Identifier for the s to be serialized.
                 * @param s String to be serialized.
                 */
                virtual void write(const uint32_t &id, const string &s) = 0;

                /**
                 * This method serializes undefined data of length size.
                 *
                 * @param id Identifier for the data to be serialized.
                 * @param data Data to be serialized.
                 * @param size Length of the data to be serialized.
                 */
                virtual void write(const uint32_t &id, const void *data, const uint32_t &size) = 0;

            public:
                /**
                 * This method converts a float from
                 * host byte order to network byte order.
                 *
                 * @param f float to be converted.
                 * @return f in NBO.
                 */
                static float htonf(float f);

                /**
                 * This method converts a double from
                 * host byte order to network byte order.
                 *
                 * @param d double to be converted.
                 * @return d in NBO.
                 */
                static double htond(double d);

                /**
                 * This method converts a uint64_t from
                 * host byte order to network byte order.
                 *
                 * @param ui uint64_t to be converted.
                 * @return ui in NBO.
                 */
                static uint64_t __htonll(uint64_t ui);

                /**
                 * This method converts an int64_t from
                 * host byte order to network byte order.
                 *
                 * @param i int64_t to be converted.
                 * @return i in NBO.
                 */
                static int64_t __htonll(int64_t i);
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_SERIALIZER_H_*/
