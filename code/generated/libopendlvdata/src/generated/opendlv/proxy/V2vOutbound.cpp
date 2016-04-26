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


#include "opendlvdata/generated/opendlv/proxy/V2vOutbound.h"

namespace opendlv {
	namespace proxy {
			using namespace std;
			using namespace odcore::base;
		
		
			V2vOutbound::V2vOutbound() :
			    SerializableData(), Visitable()
				, m_size(0)
				, m_data("")
			{
			}
		
			V2vOutbound::V2vOutbound(
				const uint32_t &val0, 
				const std::string &val1
			) :
			    SerializableData(), Visitable()
				, m_size(val0)
				, m_data(val1)
			{
			}
		
			V2vOutbound::V2vOutbound(const V2vOutbound &obj) :
			    SerializableData(), Visitable()
				, m_size(obj.m_size)
				, m_data(obj.m_data)
			{
			}
			
			V2vOutbound::~V2vOutbound() {
			}
		
			V2vOutbound& V2vOutbound::operator=(const V2vOutbound &obj) {
				m_size = obj.m_size;
				m_data = obj.m_data;
				return (*this);
			}
		
			int32_t V2vOutbound::ID() {
				return 163;
			}
		
			const string V2vOutbound::ShortName() {
				return "V2vOutbound";
			}
		
			const string V2vOutbound::LongName() {
				return "opendlv.proxy.V2vOutbound";
			}
		
			int32_t V2vOutbound::getID() const {
				return V2vOutbound::ID();
			}
		
			const string V2vOutbound::getShortName() const {
				return V2vOutbound::ShortName();
			}
		
			const string V2vOutbound::getLongName() const {
				return V2vOutbound::LongName();
			}
		
			uint32_t V2vOutbound::getSize() const {
				return m_size;
			}
			
			void V2vOutbound::setSize(const uint32_t &val) {
				m_size = val;
			}
			std::string V2vOutbound::getData() const {
				return m_data;
			}
			
			void V2vOutbound::setData(const std::string &val) {
				m_data = val;
			}
		
			void V2vOutbound::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'s', CharList<'i', CharList<'z', CharList<'e', NullType> > > >  >::RESULT, 1, "V2vOutbound.size", "size", m_size);
				v.visit(CRC32 < CharList<'d', CharList<'a', CharList<'t', CharList<'a', NullType> > > >  >::RESULT, 2, "V2vOutbound.data", "data", m_data);
				v.endVisit();
			}
		
			const string V2vOutbound::toString() const {
				stringstream s;
		
		
				s << "Size: " << getSize() << " ";
				s << "Data: " << getData() << " ";
		
				return s.str();
			}
		
			ostream& V2vOutbound::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_size);
				s->write(2,
						m_data);
				return out;
			}
		
			istream& V2vOutbound::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_size);
				d->read(2,
						m_data);
				return in;
			}
	} // proxy
} // opendlv
