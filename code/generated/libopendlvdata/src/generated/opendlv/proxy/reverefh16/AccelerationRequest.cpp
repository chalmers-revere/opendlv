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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/AccelerationRequest.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				AccelerationRequest::AccelerationRequest() :
				    SerializableData(), Visitable()
					, m_enableRequest(false)
					, m_acceleration(0.0)
				{
				}
			
				AccelerationRequest::AccelerationRequest(
					const bool &val0, 
					const double &val1
				) :
				    SerializableData(), Visitable()
					, m_enableRequest(val0)
					, m_acceleration(val1)
				{
				}
			
				AccelerationRequest::AccelerationRequest(const AccelerationRequest &obj) :
				    SerializableData(), Visitable()
					, m_enableRequest(obj.m_enableRequest)
					, m_acceleration(obj.m_acceleration)
				{
				}
				
				AccelerationRequest::~AccelerationRequest() {
				}
			
				AccelerationRequest& AccelerationRequest::operator=(const AccelerationRequest &obj) {
					m_enableRequest = obj.m_enableRequest;
					m_acceleration = obj.m_acceleration;
					return (*this);
				}
			
				int32_t AccelerationRequest::ID() {
					return 192;
				}
			
				const string AccelerationRequest::ShortName() {
					return "AccelerationRequest";
				}
			
				const string AccelerationRequest::LongName() {
					return "opendlv.proxy.reverefh16.AccelerationRequest";
				}
			
				int32_t AccelerationRequest::getID() const {
					return AccelerationRequest::ID();
				}
			
				const string AccelerationRequest::getShortName() const {
					return AccelerationRequest::ShortName();
				}
			
				const string AccelerationRequest::getLongName() const {
					return AccelerationRequest::LongName();
				}
			
				bool AccelerationRequest::getEnableRequest() const {
					return m_enableRequest;
				}
				
				void AccelerationRequest::setEnableRequest(const bool &val) {
					m_enableRequest = val;
				}
				double AccelerationRequest::getAcceleration() const {
					return m_acceleration;
				}
				
				void AccelerationRequest::setAcceleration(const double &val) {
					m_acceleration = val;
				}
			
				void AccelerationRequest::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'e', CharList<'n', CharList<'a', CharList<'b', CharList<'l', CharList<'e', CharList<'R', CharList<'e', CharList<'q', CharList<'u', CharList<'e', CharList<'s', CharList<'t', NullType> > > > > > > > > > > > >  >::RESULT, 1, "AccelerationRequest.enableRequest", "enableRequest", m_enableRequest);
					v.visit(CRC32 < CharList<'a', CharList<'c', CharList<'c', CharList<'e', CharList<'l', CharList<'e', CharList<'r', CharList<'a', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > >  >::RESULT, 2, "AccelerationRequest.acceleration", "acceleration", m_acceleration);
					v.endVisit();
				}
			
				const string AccelerationRequest::toString() const {
					stringstream s;
			
			
					s << "EnableRequest: " << getEnableRequest() << " ";
					s << "Acceleration: " << getAcceleration() << " ";
			
					return s.str();
				}
			
				ostream& AccelerationRequest::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_enableRequest);
					s->write(2,
							m_acceleration);
					return out;
				}
			
				istream& AccelerationRequest::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_enableRequest);
					d->read(2,
							m_acceleration);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
