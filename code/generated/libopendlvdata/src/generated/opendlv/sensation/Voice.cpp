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


#include "opendlvdata/generated/opendlv/sensation/Voice.h"

namespace opendlv {
	namespace sensation {
			using namespace std;
			using namespace odcore::base;
		
		
			Voice::Voice() :
			    SerializableData(), Visitable()
				, m_type("")
				, m_size(0)
				, m_data("")
			{
			}
		
			Voice::Voice(
				const std::string &val0, 
				const uint32_t &val1, 
				const std::string &val2
			) :
			    SerializableData(), Visitable()
				, m_type(val0)
				, m_size(val1)
				, m_data(val2)
			{
			}
		
			Voice::Voice(const Voice &obj) :
			    SerializableData(), Visitable()
				, m_type(obj.m_type)
				, m_size(obj.m_size)
				, m_data(obj.m_data)
			{
			}
			
			Voice::~Voice() {
			}
		
			Voice& Voice::operator=(const Voice &obj) {
				m_type = obj.m_type;
				m_size = obj.m_size;
				m_data = obj.m_data;
				return (*this);
			}
		
			int32_t Voice::ID() {
				return 166;
			}
		
			const string Voice::ShortName() {
				return "Voice";
			}
		
			const string Voice::LongName() {
				return "opendlv.sensation.Voice";
			}
		
			int32_t Voice::getID() const {
				return Voice::ID();
			}
		
			const string Voice::getShortName() const {
				return Voice::ShortName();
			}
		
			const string Voice::getLongName() const {
				return Voice::LongName();
			}
		
			std::string Voice::getType() const {
				return m_type;
			}
			
			void Voice::setType(const std::string &val) {
				m_type = val;
			}
			uint32_t Voice::getSize() const {
				return m_size;
			}
			
			void Voice::setSize(const uint32_t &val) {
				m_size = val;
			}
			std::string Voice::getData() const {
				return m_data;
			}
			
			void Voice::setData(const std::string &val) {
				m_data = val;
			}
		
			void Voice::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'t', CharList<'y', CharList<'p', CharList<'e', NullType> > > >  >::RESULT, 1, "Voice.type", "type", m_type);
				v.visit(CRC32 < CharList<'s', CharList<'i', CharList<'z', CharList<'e', NullType> > > >  >::RESULT, 2, "Voice.size", "size", m_size);
				v.visit(CRC32 < CharList<'d', CharList<'a', CharList<'t', CharList<'a', NullType> > > >  >::RESULT, 3, "Voice.data", "data", m_data);
				v.endVisit();
			}
		
			const string Voice::toString() const {
				stringstream s;
		
		
				s << "Type: " << getType() << " ";
				s << "Size: " << getSize() << " ";
				s << "Data: " << getData() << " ";
		
				return s.str();
			}
		
			ostream& Voice::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_type);
				s->write(2,
						m_size);
				s->write(3,
						m_data);
				return out;
			}
		
			istream& Voice::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_type);
				d->read(2,
						m_size);
				d->read(3,
						m_data);
				return in;
			}
	} // sensation
} // opendlv
