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


#include "opendlvdata/generated/opendlv/perception/DesiredOpticalFlow.h"

namespace opendlv {
	namespace perception {
			using namespace std;
			using namespace odcore::base;
		
		
			DesiredOpticalFlow::DesiredOpticalFlow() :
			    SerializableData(), Visitable()
				, m_flow(0)
			{
			}
		
			DesiredOpticalFlow::DesiredOpticalFlow(
				const float &val0
			) :
			    SerializableData(), Visitable()
				, m_flow(val0)
			{
			}
		
			DesiredOpticalFlow::DesiredOpticalFlow(const DesiredOpticalFlow &obj) :
			    SerializableData(), Visitable()
				, m_flow(obj.m_flow)
			{
			}
			
			DesiredOpticalFlow::~DesiredOpticalFlow() {
			}
		
			DesiredOpticalFlow& DesiredOpticalFlow::operator=(const DesiredOpticalFlow &obj) {
				m_flow = obj.m_flow;
				return (*this);
			}
		
			int32_t DesiredOpticalFlow::ID() {
				return 189;
			}
		
			const string DesiredOpticalFlow::ShortName() {
				return "DesiredOpticalFlow";
			}
		
			const string DesiredOpticalFlow::LongName() {
				return "opendlv.perception.DesiredOpticalFlow";
			}
		
			int32_t DesiredOpticalFlow::getID() const {
				return DesiredOpticalFlow::ID();
			}
		
			const string DesiredOpticalFlow::getShortName() const {
				return DesiredOpticalFlow::ShortName();
			}
		
			const string DesiredOpticalFlow::getLongName() const {
				return DesiredOpticalFlow::LongName();
			}
		
			float DesiredOpticalFlow::getFlow() const {
				return m_flow;
			}
			
			void DesiredOpticalFlow::setFlow(const float &val) {
				m_flow = val;
			}
		
			void DesiredOpticalFlow::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'f', CharList<'l', CharList<'o', CharList<'w', NullType> > > >  >::RESULT, 1, "DesiredOpticalFlow.flow", "flow", m_flow);
				v.endVisit();
			}
		
			const string DesiredOpticalFlow::toString() const {
				stringstream s;
		
		
				s << "Flow: " << getFlow() << " ";
		
				return s.str();
			}
		
			ostream& DesiredOpticalFlow::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_flow);
				return out;
			}
		
			istream& DesiredOpticalFlow::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_flow);
				return in;
			}
	} // perception
} // opendlv
