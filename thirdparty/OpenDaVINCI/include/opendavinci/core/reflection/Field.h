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

#ifndef OPENDAVINCI_CORE_REFLECTION_FIELD_H_
#define OPENDAVINCI_CORE_REFLECTION_FIELD_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "GeneratedHeaders_CoreData.h"

namespace core {
    namespace reflection {

        using namespace std;

        /**
         * This class describes a concrete Field in a Message.
         */
        template<typename T>
        class OPENDAVINCI_API Field : public coredata::reflection::AbstractField {
            public:
                Field() : m_value() {}

                /**
                 * Constructor.
                 *
                 * @param v Value to be directly assigned to this field.
                 */ 
                Field(T v) : m_value(v) {}

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Field(const Field<T> &obj) : 
                    coredata::reflection::AbstractField(obj),
                    m_value(obj.m_value) {}

                virtual ~Field() {}

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Field& operator=(const Field<T> &obj) {
                    m_value = obj.getValue();
                    return *this;
                }

                /**
                 * This method sets the value for this data field.
                 *
                 * @param val Value for this data field.
                 */
                void setValue(const T &val) {
                    m_value = val;
                }

                /**
                 * Thie method returns the value for this data field.
                 *
                 * @return Value for this data field.
                 */
                T getValue() const {
                    return m_value;
                }

            private:
                T m_value;
        };

    }
} // core::reflection

#endif /*OPENDAVINCI_CORE_REFLECTION_FIELD_H*/
