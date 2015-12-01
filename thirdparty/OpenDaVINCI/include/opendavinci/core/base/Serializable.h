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

#ifndef OPENDAVINCI_CORE_BASE_SERIALIZABLE_H_
#define OPENDAVINCI_CORE_BASE_SERIALIZABLE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class is the interface for any serializable.
         *
         * It can be used as follows:
         *
         * @code
         * #include "base/Hash.h"
         *
         * class MyData : Serializable {
         *     private:
         *        int32_t m_data;
         *
         *     public:
         *        ostream& operator<<(ostream &out) const {
         *            SerializationFactory sf;
         *            SharedPointer<Serializer> s = sf.getSerializer(out);
         *            s->write(CRC32<OPENDAVINCI_CORE_STRINGLITERAL2('I', 'D')>::RESULT, m_data);
         *            return out;
         *        }
         *
         *        istream& operator>>(istream &in) {
         *            SerializationFactory sf;
         *            SharedPointer<Deserializer> d = sf.getDeserializer(in);
         *            d->read(CRC32<OPENDAVINCI_CORE_STRINGLITERAL2('I', 'D')>::RESULT, m_data);
         *            return in;
         *        }
         * }
         * @endcode
         */
        class OPENDAVINCI_API Serializable {
            public:
                virtual ~Serializable();

                /**
                 * This method needs to be used to serialize data.
                 *
                 * @param out ostream to serialize data to.
                 * @return The ostream.
                 */
                virtual ostream& operator<<(ostream &out) const = 0;

                /**
                 * This method needs to be used to deserialize data.
                 *
                 * @param in istream to deserialize data from.
                 * @return The istream.
                 */
                virtual istream& operator>>(istream &in) = 0;
        };

    }
} // core::base

namespace std {
    // The following methods are declarations for convenient usage.
    ostream OPENDAVINCI_API &operator<<(ostream &out, const core::base::Serializable &s);
    istream OPENDAVINCI_API &operator>>(istream &in, core::base::Serializable &s);
}

#endif /*OPENDAVINCI_CORE_BASE_SERIALIZEABLE_H_*/
