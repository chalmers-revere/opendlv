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


#include "opendlvdata/generated/opendlv/proxy/Actuation.h"

namespace opendlv {
	namespace proxy {
			using namespace std;
			using namespace odcore::base;
		
		
			Actuation::Actuation() :
			    SerializableData(), Visitable()
				, m_acceleration(0)
				, m_steering(0)
				, m_isValid(false)
			{
			}
		
			Actuation::Actuation(
				const float &val0, 
				const float &val1, 
				const bool &val2
			) :
			    SerializableData(), Visitable()
				, m_acceleration(val0)
				, m_steering(val1)
				, m_isValid(val2)
			{
			}
		
			Actuation::Actuation(const Actuation &obj) :
			    SerializableData(), Visitable()
				, m_acceleration(obj.m_acceleration)
				, m_steering(obj.m_steering)
				, m_isValid(obj.m_isValid)
			{
			}
			
			Actuation::~Actuation() {
			}
		
			Actuation& Actuation::operator=(const Actuation &obj) {
				m_acceleration = obj.m_acceleration;
				m_steering = obj.m_steering;
				m_isValid = obj.m_isValid;
				return (*this);
			}
		
			int32_t Actuation::ID() {
				return 164;
			}
		
			const string Actuation::ShortName() {
				return "Actuation";
			}
		
			const string Actuation::LongName() {
				return "opendlv.proxy.Actuation";
			}
		
			int32_t Actuation::getID() const {
				return Actuation::ID();
			}
		
			const string Actuation::getShortName() const {
				return Actuation::ShortName();
			}
		
			const string Actuation::getLongName() const {
				return Actuation::LongName();
			}
		
			float Actuation::getAcceleration() const {
				return m_acceleration;
			}
			
			void Actuation::setAcceleration(const float &val) {
				m_acceleration = val;
			}
			float Actuation::getSteering() const {
				return m_steering;
			}
			
			void Actuation::setSteering(const float &val) {
				m_steering = val;
			}
			bool Actuation::getIsValid() const {
				return m_isValid;
			}
			
			void Actuation::setIsValid(const bool &val) {
				m_isValid = val;
			}
		
			void Actuation::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'a', CharList<'c', CharList<'c', CharList<'e', CharList<'l', CharList<'e', CharList<'r', CharList<'a', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > >  >::RESULT, 1, "Actuation.acceleration", "acceleration", m_acceleration);
				v.visit(CRC32 < CharList<'s', CharList<'t', CharList<'e', CharList<'e', CharList<'r', CharList<'i', CharList<'n', CharList<'g', NullType> > > > > > > >  >::RESULT, 2, "Actuation.steering", "steering", m_steering);
				v.visit(CRC32 < CharList<'i', CharList<'s', CharList<'V', CharList<'a', CharList<'l', CharList<'i', CharList<'d', NullType> > > > > > >  >::RESULT, 3, "Actuation.isValid", "isValid", m_isValid);
				v.endVisit();
			}
		
			const string Actuation::toString() const {
				stringstream s;
		
		
				s << "Acceleration: " << getAcceleration() << " ";
				s << "Steering: " << getSteering() << " ";
				s << "IsValid: " << getIsValid() << " ";
		
				return s.str();
			}
		
			ostream& Actuation::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_acceleration);
				s->write(2,
						m_steering);
				s->write(3,
						m_isValid);
				return out;
			}
		
			istream& Actuation::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_acceleration);
				d->read(2,
						m_steering);
				d->read(3,
						m_isValid);
				return in;
			}
	} // proxy
} // opendlv
