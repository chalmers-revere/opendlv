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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/Wheels.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				Wheels::Wheels() :
				    SerializableData(), Visitable()
					, m_speedWheel111(0.0)
					, m_speedWheel112(0.0)
					, m_speedWheel121(0.0)
					, m_speedWheel122(0.0)
					, m_speedWheel131(0.0)
					, m_speedWheel132(0.0)
				{
				}
			
				Wheels::Wheels(
					const double &val0, 
					const double &val1, 
					const double &val2, 
					const double &val3, 
					const double &val4, 
					const double &val5
				) :
				    SerializableData(), Visitable()
					, m_speedWheel111(val0)
					, m_speedWheel112(val1)
					, m_speedWheel121(val2)
					, m_speedWheel122(val3)
					, m_speedWheel131(val4)
					, m_speedWheel132(val5)
				{
				}
			
				Wheels::Wheels(const Wheels &obj) :
				    SerializableData(), Visitable()
					, m_speedWheel111(obj.m_speedWheel111)
					, m_speedWheel112(obj.m_speedWheel112)
					, m_speedWheel121(obj.m_speedWheel121)
					, m_speedWheel122(obj.m_speedWheel122)
					, m_speedWheel131(obj.m_speedWheel131)
					, m_speedWheel132(obj.m_speedWheel132)
				{
				}
				
				Wheels::~Wheels() {
				}
			
				Wheels& Wheels::operator=(const Wheels &obj) {
					m_speedWheel111 = obj.m_speedWheel111;
					m_speedWheel112 = obj.m_speedWheel112;
					m_speedWheel121 = obj.m_speedWheel121;
					m_speedWheel122 = obj.m_speedWheel122;
					m_speedWheel131 = obj.m_speedWheel131;
					m_speedWheel132 = obj.m_speedWheel132;
					return (*this);
				}
			
				int32_t Wheels::ID() {
					return 198;
				}
			
				const string Wheels::ShortName() {
					return "Wheels";
				}
			
				const string Wheels::LongName() {
					return "opendlv.proxy.reverefh16.Wheels";
				}
			
				int32_t Wheels::getID() const {
					return Wheels::ID();
				}
			
				const string Wheels::getShortName() const {
					return Wheels::ShortName();
				}
			
				const string Wheels::getLongName() const {
					return Wheels::LongName();
				}
			
				double Wheels::getSpeedWheel111() const {
					return m_speedWheel111;
				}
				
				void Wheels::setSpeedWheel111(const double &val) {
					m_speedWheel111 = val;
				}
				double Wheels::getSpeedWheel112() const {
					return m_speedWheel112;
				}
				
				void Wheels::setSpeedWheel112(const double &val) {
					m_speedWheel112 = val;
				}
				double Wheels::getSpeedWheel121() const {
					return m_speedWheel121;
				}
				
				void Wheels::setSpeedWheel121(const double &val) {
					m_speedWheel121 = val;
				}
				double Wheels::getSpeedWheel122() const {
					return m_speedWheel122;
				}
				
				void Wheels::setSpeedWheel122(const double &val) {
					m_speedWheel122 = val;
				}
				double Wheels::getSpeedWheel131() const {
					return m_speedWheel131;
				}
				
				void Wheels::setSpeedWheel131(const double &val) {
					m_speedWheel131 = val;
				}
				double Wheels::getSpeedWheel132() const {
					return m_speedWheel132;
				}
				
				void Wheels::setSpeedWheel132(const double &val) {
					m_speedWheel132 = val;
				}
			
				void Wheels::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'1', CharList<'1', NullType> > > > > > > > > > > > >  >::RESULT, 1, "Wheels.speedWheel111", "speedWheel111", m_speedWheel111);
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'1', CharList<'2', NullType> > > > > > > > > > > > >  >::RESULT, 2, "Wheels.speedWheel112", "speedWheel112", m_speedWheel112);
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'2', CharList<'1', NullType> > > > > > > > > > > > >  >::RESULT, 3, "Wheels.speedWheel121", "speedWheel121", m_speedWheel121);
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'2', CharList<'2', NullType> > > > > > > > > > > > >  >::RESULT, 4, "Wheels.speedWheel122", "speedWheel122", m_speedWheel122);
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'3', CharList<'1', NullType> > > > > > > > > > > > >  >::RESULT, 5, "Wheels.speedWheel131", "speedWheel131", m_speedWheel131);
					v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', CharList<'W', CharList<'h', CharList<'e', CharList<'e', CharList<'l', CharList<'1', CharList<'3', CharList<'2', NullType> > > > > > > > > > > > >  >::RESULT, 6, "Wheels.speedWheel132", "speedWheel132", m_speedWheel132);
					v.endVisit();
				}
			
				const string Wheels::toString() const {
					stringstream s;
			
			
					s << "SpeedWheel111: " << getSpeedWheel111() << " ";
					s << "SpeedWheel112: " << getSpeedWheel112() << " ";
					s << "SpeedWheel121: " << getSpeedWheel121() << " ";
					s << "SpeedWheel122: " << getSpeedWheel122() << " ";
					s << "SpeedWheel131: " << getSpeedWheel131() << " ";
					s << "SpeedWheel132: " << getSpeedWheel132() << " ";
			
					return s.str();
				}
			
				ostream& Wheels::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_speedWheel111);
					s->write(2,
							m_speedWheel112);
					s->write(3,
							m_speedWheel121);
					s->write(4,
							m_speedWheel122);
					s->write(5,
							m_speedWheel131);
					s->write(6,
							m_speedWheel132);
					return out;
				}
			
				istream& Wheels::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_speedWheel111);
					d->read(2,
							m_speedWheel112);
					d->read(3,
							m_speedWheel121);
					d->read(4,
							m_speedWheel122);
					d->read(5,
							m_speedWheel131);
					d->read(6,
							m_speedWheel132);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
