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


#include "opendlvdata/generated/opendlv/perception/LeadVehicleSize.h"

namespace opendlv {
	namespace perception {
			using namespace std;
			using namespace odcore::base;
		
		
			LeadVehicleSize::LeadVehicleSize() :
			    SerializableData(), Visitable()
				, m_size(0)
			{
			}
		
			LeadVehicleSize::LeadVehicleSize(
				const float &val0
			) :
			    SerializableData(), Visitable()
				, m_size(val0)
			{
			}
		
			LeadVehicleSize::LeadVehicleSize(const LeadVehicleSize &obj) :
			    SerializableData(), Visitable()
				, m_size(obj.m_size)
			{
			}
			
			LeadVehicleSize::~LeadVehicleSize() {
			}
		
			LeadVehicleSize& LeadVehicleSize::operator=(const LeadVehicleSize &obj) {
				m_size = obj.m_size;
				return (*this);
			}
		
			int32_t LeadVehicleSize::ID() {
				return 187;
			}
		
			const string LeadVehicleSize::ShortName() {
				return "LeadVehicleSize";
			}
		
			const string LeadVehicleSize::LongName() {
				return "opendlv.perception.LeadVehicleSize";
			}
		
			int32_t LeadVehicleSize::getID() const {
				return LeadVehicleSize::ID();
			}
		
			const string LeadVehicleSize::getShortName() const {
				return LeadVehicleSize::ShortName();
			}
		
			const string LeadVehicleSize::getLongName() const {
				return LeadVehicleSize::LongName();
			}
		
			float LeadVehicleSize::getSize() const {
				return m_size;
			}
			
			void LeadVehicleSize::setSize(const float &val) {
				m_size = val;
			}
		
			void LeadVehicleSize::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'s', CharList<'i', CharList<'z', CharList<'e', NullType> > > >  >::RESULT, 1, "LeadVehicleSize.size", "size", m_size);
				v.endVisit();
			}
		
			const string LeadVehicleSize::toString() const {
				stringstream s;
		
		
				s << "Size: " << getSize() << " ";
		
				return s.str();
			}
		
			ostream& LeadVehicleSize::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_size);
				return out;
			}
		
			istream& LeadVehicleSize::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_size);
				return in;
			}
	} // perception
} // opendlv
