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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/SteeringRequest.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				SteeringRequest::SteeringRequest() :
				    SerializableData(), Visitable()
					, m_enableRequest(false)
					, m_steeringRoadWheelAngle(0.0)
					, m_steeringDeltaTorque(0.0)
				{
				}
			
				SteeringRequest::SteeringRequest(
					const bool &val0, 
					const double &val1, 
					const double &val2
				) :
				    SerializableData(), Visitable()
					, m_enableRequest(val0)
					, m_steeringRoadWheelAngle(val1)
					, m_steeringDeltaTorque(val2)
				{
				}
			
				SteeringRequest::SteeringRequest(const SteeringRequest &obj) :
				    SerializableData(), Visitable()
					, m_enableRequest(obj.m_enableRequest)
					, m_steeringRoadWheelAngle(obj.m_steeringRoadWheelAngle)
					, m_steeringDeltaTorque(obj.m_steeringDeltaTorque)
				{
				}
				
				SteeringRequest::~SteeringRequest() {
				}
			
				SteeringRequest& SteeringRequest::operator=(const SteeringRequest &obj) {
					m_enableRequest = obj.m_enableRequest;
					m_steeringRoadWheelAngle = obj.m_steeringRoadWheelAngle;
					m_steeringDeltaTorque = obj.m_steeringDeltaTorque;
					return (*this);
				}
			
				int32_t SteeringRequest::ID() {
					return 194;
				}
			
				const string SteeringRequest::ShortName() {
					return "SteeringRequest";
				}
			
				const string SteeringRequest::LongName() {
					return "opendlv.proxy.reverefh16.SteeringRequest";
				}
			
				int32_t SteeringRequest::getID() const {
					return SteeringRequest::ID();
				}
			
				const string SteeringRequest::getShortName() const {
					return SteeringRequest::ShortName();
				}
			
				const string SteeringRequest::getLongName() const {
					return SteeringRequest::LongName();
				}
			
				bool SteeringRequest::getEnableRequest() const {
					return m_enableRequest;
				}
				
				void SteeringRequest::setEnableRequest(const bool &val) {
					m_enableRequest = val;
				}
				double SteeringRequest::getSteeringRoadWheelAngle() const {
					return m_steeringRoadWheelAngle;
				}
				
				void SteeringRequest::setSteeringRoadWheelAngle(const double &val) {
					m_steeringRoadWheelAngle = val;
				}
				double SteeringRequest::getSteeringDeltaTorque() const {
					return m_steeringDeltaTorque;
				}
				
				void SteeringRequest::setSteeringDeltaTorque(const double &val) {
					m_steeringDeltaTorque = val;
				}
			
				void SteeringRequest::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'e', CharList<'n', CharList<'a', CharList<'b', CharList<'l', CharList<'e', CharList<'R', CharList<'e', CharList<'q', CharList<'u', CharList<'e', CharList<'s', CharList<'t', NullType> > > > > > > > > > > > >  >::RESULT, 1, "SteeringRequest.enableRequest", "enableRequest", m_enableRequest);
					v.visit(CRC32 < CharList<'s', CharList<'t', CharList<'e', CharList<'e', CharList<'r', CharList<'i', CharList<'n', CharList<'g', CharList<'R', CharList<'o', CharList<'a', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'A', CharList<'n', CharList<'g', CharList<'l', CharList<'e', NullType> > > > > > > > > > > > > > > > > > > > > >  >::RESULT, 2, "SteeringRequest.steeringRoadWheelAngle", "steeringRoadWheelAngle", m_steeringRoadWheelAngle);
					v.visit(CRC32 < CharList<'s', CharList<'t', CharList<'e', CharList<'e', CharList<'r', CharList<'i', CharList<'n', CharList<'g', CharList<'D', CharList<'e', CharList<'l', CharList<'t', CharList<'a', CharList<'T', CharList<'o', CharList<'r', CharList<'q', CharList<'u', CharList<'e', NullType> > > > > > > > > > > > > > > > > > >  >::RESULT, 3, "SteeringRequest.steeringDeltaTorque", "steeringDeltaTorque", m_steeringDeltaTorque);
					v.endVisit();
				}
			
				const string SteeringRequest::toString() const {
					stringstream s;
			
			
					s << "EnableRequest: " << getEnableRequest() << " ";
					s << "SteeringRoadWheelAngle: " << getSteeringRoadWheelAngle() << " ";
					s << "SteeringDeltaTorque: " << getSteeringDeltaTorque() << " ";
			
					return s.str();
				}
			
				ostream& SteeringRequest::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_enableRequest);
					s->write(2,
							m_steeringRoadWheelAngle);
					s->write(3,
							m_steeringDeltaTorque);
					return out;
				}
			
				istream& SteeringRequest::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_enableRequest);
					d->read(2,
							m_steeringRoadWheelAngle);
					d->read(3,
							m_steeringDeltaTorque);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
