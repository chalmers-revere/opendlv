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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/Propulsion.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				Propulsion::Propulsion() :
				    SerializableData(), Visitable()
					, m_propulsionShaftVehicleSpeed(0.0)
				{
				}
			
				Propulsion::Propulsion(
					const double &val0
				) :
				    SerializableData(), Visitable()
					, m_propulsionShaftVehicleSpeed(val0)
				{
				}
			
				Propulsion::Propulsion(const Propulsion &obj) :
				    SerializableData(), Visitable()
					, m_propulsionShaftVehicleSpeed(obj.m_propulsionShaftVehicleSpeed)
				{
				}
				
				Propulsion::~Propulsion() {
				}
			
				Propulsion& Propulsion::operator=(const Propulsion &obj) {
					m_propulsionShaftVehicleSpeed = obj.m_propulsionShaftVehicleSpeed;
					return (*this);
				}
			
				int32_t Propulsion::ID() {
					return 196;
				}
			
				const string Propulsion::ShortName() {
					return "Propulsion";
				}
			
				const string Propulsion::LongName() {
					return "opendlv.proxy.reverefh16.Propulsion";
				}
			
				int32_t Propulsion::getID() const {
					return Propulsion::ID();
				}
			
				const string Propulsion::getShortName() const {
					return Propulsion::ShortName();
				}
			
				const string Propulsion::getLongName() const {
					return Propulsion::LongName();
				}
			
				double Propulsion::getPropulsionShaftVehicleSpeed() const {
					return m_propulsionShaftVehicleSpeed;
				}
				
				void Propulsion::setPropulsionShaftVehicleSpeed(const double &val) {
					m_propulsionShaftVehicleSpeed = val;
				}
			
				void Propulsion::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'p', CharList<'r', CharList<'o', CharList<'p', CharList<'u', CharList<'l', CharList<'s', CharList<'i', CharList<'o', CharList<'n', CharList<'S', CharList<'h', CharList<'a', CharList<'f', CharList<'t', CharList<'V', CharList<'e', CharList<'h', CharList<'i', CharList<'c', CharList<'l', CharList<'e', CharList<'S', CharList<'p', CharList<'e', CharList<'e', CharList<'d', NullType> > > > > > > > > > > > > > > > > > > > > > > > > > >  >::RESULT, 1, "Propulsion.propulsionShaftVehicleSpeed", "propulsionShaftVehicleSpeed", m_propulsionShaftVehicleSpeed);
					v.endVisit();
				}
			
				const string Propulsion::toString() const {
					stringstream s;
			
			
					s << "PropulsionShaftVehicleSpeed: " << getPropulsionShaftVehicleSpeed() << " ";
			
					return s.str();
				}
			
				ostream& Propulsion::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_propulsionShaftVehicleSpeed);
					return out;
				}
			
				istream& Propulsion::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_propulsionShaftVehicleSpeed);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
