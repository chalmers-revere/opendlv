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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/Steering.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				Steering::Steering() :
				    SerializableData(), Visitable()
					, m_roadwheelangle(0.0)
					, m_steeringwheelangle(0.0)
				{
				}
			
				Steering::Steering(
					const double &val0, 
					const double &val1
				) :
				    SerializableData(), Visitable()
					, m_roadwheelangle(val0)
					, m_steeringwheelangle(val1)
				{
				}
			
				Steering::Steering(const Steering &obj) :
				    SerializableData(), Visitable()
					, m_roadwheelangle(obj.m_roadwheelangle)
					, m_steeringwheelangle(obj.m_steeringwheelangle)
				{
				}
				
				Steering::~Steering() {
				}
			
				Steering& Steering::operator=(const Steering &obj) {
					m_roadwheelangle = obj.m_roadwheelangle;
					m_steeringwheelangle = obj.m_steeringwheelangle;
					return (*this);
				}
			
				int32_t Steering::ID() {
					return 199;
				}
			
				const string Steering::ShortName() {
					return "Steering";
				}
			
				const string Steering::LongName() {
					return "opendlv.proxy.reverefh16.Steering";
				}
			
				int32_t Steering::getID() const {
					return Steering::ID();
				}
			
				const string Steering::getShortName() const {
					return Steering::ShortName();
				}
			
				const string Steering::getLongName() const {
					return Steering::LongName();
				}
			
				double Steering::getRoadwheelangle() const {
					return m_roadwheelangle;
				}
				
				void Steering::setRoadwheelangle(const double &val) {
					m_roadwheelangle = val;
				}
				double Steering::getSteeringwheelangle() const {
					return m_steeringwheelangle;
				}
				
				void Steering::setSteeringwheelangle(const double &val) {
					m_steeringwheelangle = val;
				}
			
				void Steering::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'r', CharList<'o', CharList<'a', CharList<'d', CharList<'w', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'a', CharList<'n', CharList<'g', CharList<'l', CharList<'e', NullType> > > > > > > > > > > > > >  >::RESULT, 1, "Steering.roadwheelangle", "roadwheelangle", m_roadwheelangle);
					v.visit(CRC32 < CharList<'s', CharList<'t', CharList<'e', CharList<'e', CharList<'r', CharList<'i', CharList<'n', CharList<'g', CharList<'w', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'a', CharList<'n', CharList<'g', CharList<'l', CharList<'e', NullType> > > > > > > > > > > > > > > > > >  >::RESULT, 2, "Steering.steeringwheelangle", "steeringwheelangle", m_steeringwheelangle);
					v.endVisit();
				}
			
				const string Steering::toString() const {
					stringstream s;
			
			
					s << "Roadwheelangle: " << getRoadwheelangle() << " ";
					s << "Steeringwheelangle: " << getSteeringwheelangle() << " ";
			
					return s.str();
				}
			
				ostream& Steering::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_roadwheelangle);
					s->write(2,
							m_steeringwheelangle);
					return out;
				}
			
				istream& Steering::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_roadwheelangle);
					d->read(2,
							m_steeringwheelangle);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
