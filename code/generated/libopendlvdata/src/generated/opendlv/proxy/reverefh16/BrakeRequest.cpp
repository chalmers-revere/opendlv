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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/BrakeRequest.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				BrakeRequest::BrakeRequest() :
				    SerializableData(), Visitable()
					, m_enableRequest(false)
					, m_brake(0.0)
				{
				}
			
				BrakeRequest::BrakeRequest(
					const bool &val0, 
					const double &val1
				) :
				    SerializableData(), Visitable()
					, m_enableRequest(val0)
					, m_brake(val1)
				{
				}
			
				BrakeRequest::BrakeRequest(const BrakeRequest &obj) :
				    SerializableData(), Visitable()
					, m_enableRequest(obj.m_enableRequest)
					, m_brake(obj.m_brake)
				{
				}
				
				BrakeRequest::~BrakeRequest() {
				}
			
				BrakeRequest& BrakeRequest::operator=(const BrakeRequest &obj) {
					m_enableRequest = obj.m_enableRequest;
					m_brake = obj.m_brake;
					return (*this);
				}
			
				int32_t BrakeRequest::ID() {
					return 193;
				}
			
				const string BrakeRequest::ShortName() {
					return "BrakeRequest";
				}
			
				const string BrakeRequest::LongName() {
					return "opendlv.proxy.reverefh16.BrakeRequest";
				}
			
				int32_t BrakeRequest::getID() const {
					return BrakeRequest::ID();
				}
			
				const string BrakeRequest::getShortName() const {
					return BrakeRequest::ShortName();
				}
			
				const string BrakeRequest::getLongName() const {
					return BrakeRequest::LongName();
				}
			
				bool BrakeRequest::getEnableRequest() const {
					return m_enableRequest;
				}
				
				void BrakeRequest::setEnableRequest(const bool &val) {
					m_enableRequest = val;
				}
				double BrakeRequest::getBrake() const {
					return m_brake;
				}
				
				void BrakeRequest::setBrake(const double &val) {
					m_brake = val;
				}
			
				void BrakeRequest::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'e', CharList<'n', CharList<'a', CharList<'b', CharList<'l', CharList<'e', CharList<'R', CharList<'e', CharList<'q', CharList<'u', CharList<'e', CharList<'s', CharList<'t', NullType> > > > > > > > > > > > >  >::RESULT, 1, "BrakeRequest.enableRequest", "enableRequest", m_enableRequest);
					v.visit(CRC32 < CharList<'b', CharList<'r', CharList<'a', CharList<'k', CharList<'e', NullType> > > > >  >::RESULT, 2, "BrakeRequest.brake", "brake", m_brake);
					v.endVisit();
				}
			
				const string BrakeRequest::toString() const {
					stringstream s;
			
			
					s << "EnableRequest: " << getEnableRequest() << " ";
					s << "Brake: " << getBrake() << " ";
			
					return s.str();
				}
			
				ostream& BrakeRequest::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_enableRequest);
					s->write(2,
							m_brake);
					return out;
				}
			
				istream& BrakeRequest::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_enableRequest);
					d->read(2,
							m_brake);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
