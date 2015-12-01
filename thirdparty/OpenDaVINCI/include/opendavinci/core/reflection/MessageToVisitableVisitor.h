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

#ifndef OPENDAVINCI_CORE_REFLECTION_MESSAGETOVISITABLEVISITOR_H_
#define OPENDAVINCI_CORE_REFLECTION_MESSAGETOVISITABLEVISITOR_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"
#include "core/base/Visitor.h"
#include "core/reflection/Field.h"
#include "core/reflection/Message.h"

namespace core {
    namespace reflection {

        using namespace std;

        /**
         * This class is a Visitor setting values in a visitable class from a generic Message representation.
         */
        class OPENDAVINCI_API MessageToVisitableVisitor : public core::base::Visitor {
            private:
                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                MessageToVisitableVisitor(const MessageToVisitableVisitor &obj);

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                MessageToVisitableVisitor& operator=(const MessageToVisitableVisitor &obj);

            public:
                MessageToVisitableVisitor(const Message &m);

                virtual ~MessageToVisitableVisitor();

            public:
                virtual void visit(const uint32_t &longId, const uint8_t &shortId, const string &longName, const string &shortName, core::base::Serializable &v);
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
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic push
# endif
# pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
                /**
                 * This method retrieves the current value from the list of
                 * fields, visits the value, and updates it in the if (f->getFieldDataType() == that
                 * the Visitor might have altered the value.
                 *
                 * @param v Visitor.
                 * @param f Field to visit.
                 */
                template<typename T>
                void visitPrimitiveDataType(core::SharedPointer<coredata::reflection::AbstractField> &f, T &v) {
                    // If T is != double but f->getFieldDataType() == double, we require a compiler cast.
                    double _v = 0;
                    if (f->getFieldDataType() == coredata::reflection::AbstractField::DOUBLE_T) {
                        _v = dynamic_cast<core::reflection::Field<double>*>(f.operator->())->getValue();
                    }

                    // Set value from f to v (default case).
                    if (f->getFieldDataType() != coredata::reflection::AbstractField::DOUBLE_T) {
                        v = dynamic_cast<core::reflection::Field<T>*>(f.operator->())->getValue();
                    }
                    else { v = _v; }
                }
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic pop
# endif
#endif

            private:
                Message m_message;
        };

    }
} // core::reflection

#endif /*OPENDAVINCI_CORE_REFLECTION_MESSAGETOVISITABLEVISITOR_H_*/
