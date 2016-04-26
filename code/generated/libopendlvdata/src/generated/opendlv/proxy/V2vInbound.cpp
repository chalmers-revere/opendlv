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


#include "opendlvdata/generated/opendlv/proxy/V2vInbound.h"

namespace opendlv {
	namespace proxy {
			using namespace std;
			using namespace odcore::base;
		
		
			V2vInbound::V2vInbound() :
			    SerializableData(), Visitable()
				, m_size(0)
				, m_data("")
			{
			}
		
			V2vInbound::V2vInbound(
				const uint32_t &val0, 
				const std::string &val1
			) :
			    SerializableData(), Visitable()
				, m_size(val0)
				, m_data(val1)
			{
			}
		
			V2vInbound::V2vInbound(const V2vInbound &obj) :
			    SerializableData(), Visitable()
				, m_size(obj.m_size)
				, m_data(obj.m_data)
			{
			}
			
			V2vInbound::~V2vInbound() {
			}
		
			V2vInbound& V2vInbound::operator=(const V2vInbound &obj) {
				m_size = obj.m_size;
				m_data = obj.m_data;
				return (*this);
			}
		
			int32_t V2vInbound::ID() {
				return 162;
			}
		
			const string V2vInbound::ShortName() {
				return "V2vInbound";
			}
		
			const string V2vInbound::LongName() {
				return "opendlv.proxy.V2vInbound";
			}
		
			int32_t V2vInbound::getID() const {
				return V2vInbound::ID();
			}
		
			const string V2vInbound::getShortName() const {
				return V2vInbound::ShortName();
			}
		
			const string V2vInbound::getLongName() const {
				return V2vInbound::LongName();
			}
		
			uint32_t V2vInbound::getSize() const {
				return m_size;
			}
			
			void V2vInbound::setSize(const uint32_t &val) {
				m_size = val;
			}
			std::string V2vInbound::getData() const {
				return m_data;
			}
			
			void V2vInbound::setData(const std::string &val) {
				m_data = val;
			}
		
			void V2vInbound::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'s', CharList<'i', CharList<'z', CharList<'e', NullType> > > >  >::RESULT, 1, "V2vInbound.size", "size", m_size);
				v.visit(CRC32 < CharList<'d', CharList<'a', CharList<'t', CharList<'a', NullType> > > >  >::RESULT, 2, "V2vInbound.data", "data", m_data);
				v.endVisit();
			}
		
			const string V2vInbound::toString() const {
				stringstream s;
		
		
				s << "Size: " << getSize() << " ";
				s << "Data: " << getData() << " ";
		
				return s.str();
			}
		
			ostream& V2vInbound::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_size);
				s->write(2,
						m_data);
				return out;
			}
		
			istream& V2vInbound::operator>>(istream &in) {
		
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
