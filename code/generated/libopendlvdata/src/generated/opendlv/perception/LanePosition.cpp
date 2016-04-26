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


#include "opendlvdata/generated/opendlv/perception/LanePosition.h"

namespace opendlv {
	namespace perception {
			using namespace std;
			using namespace odcore::base;
		
		
			LanePosition::LanePosition() :
			    SerializableData(), Visitable()
				, m_offset(0)
				, m_heading(0)
			{
			}
		
			LanePosition::LanePosition(
				const float &val0, 
				const float &val1
			) :
			    SerializableData(), Visitable()
				, m_offset(val0)
				, m_heading(val1)
			{
			}
		
			LanePosition::LanePosition(const LanePosition &obj) :
			    SerializableData(), Visitable()
				, m_offset(obj.m_offset)
				, m_heading(obj.m_heading)
			{
			}
			
			LanePosition::~LanePosition() {
			}
		
			LanePosition& LanePosition::operator=(const LanePosition &obj) {
				m_offset = obj.m_offset;
				m_heading = obj.m_heading;
				return (*this);
			}
		
			int32_t LanePosition::ID() {
				return 186;
			}
		
			const string LanePosition::ShortName() {
				return "LanePosition";
			}
		
			const string LanePosition::LongName() {
				return "opendlv.perception.LanePosition";
			}
		
			int32_t LanePosition::getID() const {
				return LanePosition::ID();
			}
		
			const string LanePosition::getShortName() const {
				return LanePosition::ShortName();
			}
		
			const string LanePosition::getLongName() const {
				return LanePosition::LongName();
			}
		
			float LanePosition::getOffset() const {
				return m_offset;
			}
			
			void LanePosition::setOffset(const float &val) {
				m_offset = val;
			}
			float LanePosition::getHeading() const {
				return m_heading;
			}
			
			void LanePosition::setHeading(const float &val) {
				m_heading = val;
			}
		
			void LanePosition::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'o', CharList<'f', CharList<'f', CharList<'s', CharList<'e', CharList<'t', NullType> > > > > >  >::RESULT, 1, "LanePosition.offset", "offset", m_offset);
				v.visit(CRC32 < CharList<'h', CharList<'e', CharList<'a', CharList<'d', CharList<'i', CharList<'n', CharList<'g', NullType> > > > > > >  >::RESULT, 2, "LanePosition.heading", "heading", m_heading);
				v.endVisit();
			}
		
			const string LanePosition::toString() const {
				stringstream s;
		
		
				s << "Offset: " << getOffset() << " ";
				s << "Heading: " << getHeading() << " ";
		
				return s.str();
			}
		
			ostream& LanePosition::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_offset);
				s->write(2,
						m_heading);
				return out;
			}
		
			istream& LanePosition::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_offset);
				d->read(2,
						m_heading);
				return in;
			}
	} // perception
} // opendlv
