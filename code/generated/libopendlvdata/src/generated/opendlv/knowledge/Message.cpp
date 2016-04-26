/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#include <memory>

#include "opendavinci/odcore/base/Hash.h"
#include "opendavinci/odcore/base/Deserializer.h"
#include "opendavinci/odcore/base/SerializationFactory.h"
#include "opendavinci/odcore/base/Serializer.h"


#include "opendlvdata/generated/opendlv/knowledge/Message.h"

namespace opendlv {
	namespace knowledge {
			using namespace std;
			using namespace odcore::base;
		
		
			Message::Message() :
			    SerializableData(), Visitable()
				, m_size(0)
				, m_data("")
			{
			}
		
			Message::Message(
				const uint32_t &val0, 
				const std::string &val1
			) :
			    SerializableData(), Visitable()
				, m_size(val0)
				, m_data(val1)
			{
			}
		
			Message::Message(const Message &obj) :
			    SerializableData(), Visitable()
				, m_size(obj.m_size)
				, m_data(obj.m_data)
			{
			}
			
			Message::~Message() {
			}
		
			Message& Message::operator=(const Message &obj) {
				m_size = obj.m_size;
				m_data = obj.m_data;
				return (*this);
			}
		
			int32_t Message::ID() {
				return 185;
			}
		
			const string Message::ShortName() {
				return "Message";
			}
		
			const string Message::LongName() {
				return "opendlv.knowledge.Message";
			}
		
			int32_t Message::getID() const {
				return Message::ID();
			}
		
			const string Message::getShortName() const {
				return Message::ShortName();
			}
		
			const string Message::getLongName() const {
				return Message::LongName();
			}
		
			uint32_t Message::getSize() const {
				return m_size;
			}
			
			void Message::setSize(const uint32_t &val) {
				m_size = val;
			}
			std::string Message::getData() const {
				return m_data;
			}
			
			void Message::setData(const std::string &val) {
				m_data = val;
			}
		
			void Message::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'s', CharList<'i', CharList<'z', CharList<'e', NullType> > > >  >::RESULT, 1, "Message.size", "size", m_size);
				v.visit(CRC32 < CharList<'d', CharList<'a', CharList<'t', CharList<'a', NullType> > > >  >::RESULT, 2, "Message.data", "data", m_data);
				v.endVisit();
			}
		
			const string Message::toString() const {
				stringstream s;
		
		
				s << "Size: " << getSize() << " ";
				s << "Data: " << getData() << " ";
		
				return s.str();
			}
		
			ostream& Message::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_size);
				s->write(2,
						m_data);
				return out;
			}
		
			istream& Message::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_size);
				d->read(2,
						m_data);
				return in;
			}
	} // knowledge
} // opendlv
