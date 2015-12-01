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

#ifndef OPENDAVINCI_CORE_REFLECTION_MESSAGEFROMVISITABLEVISITOR_H_
#define OPENDAVINCI_CORE_REFLECTION_MESSAGEFROMVISITABLEVISITOR_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"
#include "core/base/Visitor.h"
#include "core/reflection/Message.h"

namespace core {
    namespace reflection {

        using namespace std;

        /**
         * This class is a Visitor transforming a visitable class into a generic Message representation.
         */
        class OPENDAVINCI_API MessageFromVisitableVisitor : public core::base::Visitor {
            private:
                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                MessageFromVisitableVisitor(const MessageFromVisitableVisitor &obj);

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                MessageFromVisitableVisitor& operator=(const MessageFromVisitableVisitor &obj);

            public:
                MessageFromVisitableVisitor();

                virtual ~MessageFromVisitableVisitor();

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

            public:
                /**
                 * This method returns the message translate from a visitable.
                 *
                 * @return Message created from a Visitable.
                 */
                Message getMessage();

            private:
                Message m_message;
        };

    }
} // core::reflection

#endif /*OPENDAVINCI_CORE_REFLECTION_MESSAGEFROMVISITABLEVISITOR_H_*/
