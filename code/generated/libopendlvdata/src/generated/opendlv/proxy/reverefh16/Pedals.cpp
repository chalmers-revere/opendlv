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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/Pedals.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				Pedals::Pedals() :
				    SerializableData(), Visitable()
					, m_accelerationPedalPosition(0.0)
					, m_brakePedalPosition(0.0)
					, m_torsionBarTorque(0.0)
				{
				}
			
				Pedals::Pedals(
					const double &val0, 
					const double &val1, 
					const double &val2
				) :
				    SerializableData(), Visitable()
					, m_accelerationPedalPosition(val0)
					, m_brakePedalPosition(val1)
					, m_torsionBarTorque(val2)
				{
				}
			
				Pedals::Pedals(const Pedals &obj) :
				    SerializableData(), Visitable()
					, m_accelerationPedalPosition(obj.m_accelerationPedalPosition)
					, m_brakePedalPosition(obj.m_brakePedalPosition)
					, m_torsionBarTorque(obj.m_torsionBarTorque)
				{
				}
				
				Pedals::~Pedals() {
				}
			
				Pedals& Pedals::operator=(const Pedals &obj) {
					m_accelerationPedalPosition = obj.m_accelerationPedalPosition;
					m_brakePedalPosition = obj.m_brakePedalPosition;
					m_torsionBarTorque = obj.m_torsionBarTorque;
					return (*this);
				}
			
				int32_t Pedals::ID() {
					return 191;
				}
			
				const string Pedals::ShortName() {
					return "Pedals";
				}
			
				const string Pedals::LongName() {
					return "opendlv.proxy.reverefh16.Pedals";
				}
			
				int32_t Pedals::getID() const {
					return Pedals::ID();
				}
			
				const string Pedals::getShortName() const {
					return Pedals::ShortName();
				}
			
				const string Pedals::getLongName() const {
					return Pedals::LongName();
				}
			
				double Pedals::getAccelerationPedalPosition() const {
					return m_accelerationPedalPosition;
				}
				
				void Pedals::setAccelerationPedalPosition(const double &val) {
					m_accelerationPedalPosition = val;
				}
				double Pedals::getBrakePedalPosition() const {
					return m_brakePedalPosition;
				}
				
				void Pedals::setBrakePedalPosition(const double &val) {
					m_brakePedalPosition = val;
				}
				double Pedals::getTorsionBarTorque() const {
					return m_torsionBarTorque;
				}
				
				void Pedals::setTorsionBarTorque(const double &val) {
					m_torsionBarTorque = val;
				}
			
				void Pedals::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'a', CharList<'c', CharList<'c', CharList<'e', CharList<'l', CharList<'e', CharList<'r', CharList<'a', CharList<'t', CharList<'i', CharList<'o', CharList<'n', CharList<'P', CharList<'e', CharList<'d', CharList<'a', CharList<'l', CharList<'P', CharList<'o', CharList<'s', CharList<'i', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > > > > > > > > > > > > > > >  >::RESULT, 1, "Pedals.accelerationPedalPosition", "accelerationPedalPosition", m_accelerationPedalPosition);
					v.visit(CRC32 < CharList<'b', CharList<'r', CharList<'a', CharList<'k', CharList<'e', CharList<'P', CharList<'e', CharList<'d', CharList<'a', CharList<'l', CharList<'P', CharList<'o', CharList<'s', CharList<'i', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > > > > > > > >  >::RESULT, 2, "Pedals.brakePedalPosition", "brakePedalPosition", m_brakePedalPosition);
					v.visit(CRC32 < CharList<'t', CharList<'o', CharList<'r', CharList<'s', CharList<'i', CharList<'o', CharList<'n', CharList<'B', CharList<'a', CharList<'r', CharList<'T', CharList<'o', CharList<'r', CharList<'q', CharList<'u', CharList<'e', NullType> > > > > > > > > > > > > > > >  >::RESULT, 3, "Pedals.torsionBarTorque", "torsionBarTorque", m_torsionBarTorque);
					v.endVisit();
				}
			
				const string Pedals::toString() const {
					stringstream s;
			
			
					s << "AccelerationPedalPosition: " << getAccelerationPedalPosition() << " ";
					s << "BrakePedalPosition: " << getBrakePedalPosition() << " ";
					s << "TorsionBarTorque: " << getTorsionBarTorque() << " ";
			
					return s.str();
				}
			
				ostream& Pedals::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_accelerationPedalPosition);
					s->write(2,
							m_brakePedalPosition);
					s->write(3,
							m_torsionBarTorque);
					return out;
				}
			
				istream& Pedals::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_accelerationPedalPosition);
					d->read(2,
							m_brakePedalPosition);
					d->read(3,
							m_torsionBarTorque);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
