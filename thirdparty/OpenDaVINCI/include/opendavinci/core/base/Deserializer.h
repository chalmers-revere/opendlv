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

#ifndef OPENDAVINCI_CORE_BASE_DESERIALIZER_H_
#define OPENDAVINCI_CORE_BASE_DESERIALIZER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace base {

        using namespace std;

        class Serializable;

        /**
         * This class is the interface for any deserializer.
         *
         * @See Serializable
         */
        class OPENDAVINCI_API Deserializer {
            public:
                Deserializer();

                virtual ~Deserializer();

                /**
                 * The method sets the istream from which the data shall
                 * be deserialized. Any existing deserialized data will
                 * be reset.
                 *
                 * @param in Input stream containing the data.
                 */
                virtual void deserializeDataFrom(istream &in) = 0;

                /**
                 * This method deserializes a Serializable.
                 *
                 * @param id Identifier for the s to be deserialized.
                 * @param s Serializable to be deserialized.
                 */
                virtual void read(const uint32_t &id, Serializable &s) = 0;

                /**
                 * This method deserializes a bool.
                 *
                 * @param id Identifier for the b to be deserialized.
                 * @param b Bool to be deserialized.
                 */
                virtual void read(const uint32_t &id, bool &b) = 0;

                /**
                 * This method deserializes a char.
                 *
                 * @param id Identifier for the c to be deserialized.
                 * @param c Char to be deserialized.
                 */
                virtual void read(const uint32_t &id, char &c) = 0;

                /**
                 * This method deserializes an unsigned char.
                 *
                 * @param id Identifier for the uc to be deserialized.
                 * @param uc Unsigned char to be deserialized.
                 */
                virtual void read(const uint32_t &id, unsigned char &uc) = 0;

                /**
                 * This method deserializes an int.
                 *
                 * @param id Identifier for the i to be deserialized.
                 * @param i Int to be deserialized.
                 */
                virtual void read(const uint32_t &id, int8_t &i) = 0;

                /**
                 * This method deserializes an int.
                 *
                 * @param id Identifier for the i to be deserialized.
                 * @param i Int to be deserialized.
                 */
                virtual void read(const uint32_t &id, int16_t &i) = 0;

                /**
                 * This method deserializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be deserialized.
                 * @param ui Unsigned int32_t to be deserialized.
                 */
                virtual void read(const uint32_t &id, uint16_t &ui) = 0;

                /**
                 * This method deserializes an int.
                 *
                 * @param id Identifier for the i to be deserialized.
                 * @param i Int to be deserialized.
                 */
                virtual void read(const uint32_t &id, int32_t &i) = 0;

                /**
                 * This method deserializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be deserialized.
                 * @param ui Unsigned int32_t to be deserialized.
                 */
                virtual void read(const uint32_t &id, uint32_t &ui) = 0;

                /**
                 * This method deserializes an int.
                 *
                 * @param id Identifier for the i to be deserialized.
                 * @param i Int to be deserialized.
                 */
                virtual void read(const uint32_t &id, int64_t &i) = 0;

                /**
                 * This method deserializes an uint32_t.
                 *
                 * @param id Identifier for the ui to be deserialized.
                 * @param ui Unsigned int32_t to be deserialized.
                 */
                virtual void read(const uint32_t &id, uint64_t &ui) = 0;

                /**
                 * This method deserializes a float.
                 *
                 * @param id Identifier for the f to be deserialized.
                 * @param f Float to be deserialized.
                 */
                virtual void read(const uint32_t &id, float &f) = 0;

                /**
                 * This method deserializes a double.
                 *
                 * @param id Identifier for the d to be deserialized.
                 * @param d Double to be deserialized.
                 */
                virtual void read(const uint32_t &id, double &d) = 0;

                /**
                 * This method deserializes a string.
                 *
                 * @param id Identifier for the s to be deserialized.
                 * @param s String to be deserialized.
                 */
                virtual void read(const uint32_t &id, string &s) = 0;

                /**
                 * This method deserializes undefined data of length size.
                 *
                 * @param id Identifier for the data to be deserialized.
                 * @param data Data to be deserialized.
                 * @param size Length of the data to be deserialized.
                 */
                virtual void read(const uint32_t &id, void *data, const uint32_t &size) = 0;

            public:
                /**
                 * This method converts a float from
                 * network byte order to host byte order.
                 *
                 * @param f float to be converted.
                 * @return f in HBO.
                 */
                static float ntohf(float f);

                /**
                 * This method converts a double from
                 * network byte order to host byte order.
                 *
                 * @param d double to be converted.
                 * @return d in HBO.
                 */
                static double ntohd(double d);

                /**
                 * This method converts a uint64_t from
                 * network byte order to host byte order.
                 *
                 * @param ui uint64_t to be converted.
                 * @return ui in HBO.
                 */
                static uint64_t __ntohll(uint64_t ui);

                /**
                 * This method converts an int64_t from
                 * network byte order to host byte order.
                 *
                 * @param i int64_t to be converted.
                 * @return i in HBO.
                 */
                static int64_t __ntohll(int64_t i);
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_DESERIALIZER_H_*/
