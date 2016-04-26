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


#include "opendlvdata/generated/opendlv/sensation/OpticalFlow.h"

namespace opendlv {
	namespace sensation {
			using namespace std;
			using namespace odcore::base;
		
		
			OpticalFlow::OpticalFlow() :
			    SerializableData(), Visitable()
				, m_flow(0)
			{
			}
		
			OpticalFlow::OpticalFlow(
				const float &val0
			) :
			    SerializableData(), Visitable()
				, m_flow(val0)
			{
			}
		
			OpticalFlow::OpticalFlow(const OpticalFlow &obj) :
			    SerializableData(), Visitable()
				, m_flow(obj.m_flow)
			{
			}
			
			OpticalFlow::~OpticalFlow() {
			}
		
			OpticalFlow& OpticalFlow::operator=(const OpticalFlow &obj) {
				m_flow = obj.m_flow;
				return (*this);
			}
		
			int32_t OpticalFlow::ID() {
				return 188;
			}
		
			const string OpticalFlow::ShortName() {
				return "OpticalFlow";
			}
		
			const string OpticalFlow::LongName() {
				return "opendlv.sensation.OpticalFlow";
			}
		
			int32_t OpticalFlow::getID() const {
				return OpticalFlow::ID();
			}
		
			const string OpticalFlow::getShortName() const {
				return OpticalFlow::ShortName();
			}
		
			const string OpticalFlow::getLongName() const {
				return OpticalFlow::LongName();
			}
		
			float OpticalFlow::getFlow() const {
				return m_flow;
			}
			
			void OpticalFlow::setFlow(const float &val) {
				m_flow = val;
			}
		
			void OpticalFlow::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'f', CharList<'l', CharList<'o', CharList<'w', NullType> > > >  >::RESULT, 1, "OpticalFlow.flow", "flow", m_flow);
				v.endVisit();
			}
		
			const string OpticalFlow::toString() const {
				stringstream s;
		
		
				s << "Flow: " << getFlow() << " ";
		
				return s.str();
			}
		
			ostream& OpticalFlow::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_flow);
				return out;
			}
		
			istream& OpticalFlow::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_flow);
				return in;
			}
	} // sensation
} // opendlv
