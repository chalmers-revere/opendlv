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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/Axles.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				Axles::Axles() :
				    SerializableData(), Visitable()
					, m_loadAxle11(0.0)
					, m_loadAxle12(0.0)
					, m_loadAxle13(0.0)
				{
				}
			
				Axles::Axles(
					const double &val0, 
					const double &val1, 
					const double &val2
				) :
				    SerializableData(), Visitable()
					, m_loadAxle11(val0)
					, m_loadAxle12(val1)
					, m_loadAxle13(val2)
				{
				}
			
				Axles::Axles(const Axles &obj) :
				    SerializableData(), Visitable()
					, m_loadAxle11(obj.m_loadAxle11)
					, m_loadAxle12(obj.m_loadAxle12)
					, m_loadAxle13(obj.m_loadAxle13)
				{
				}
				
				Axles::~Axles() {
				}
			
				Axles& Axles::operator=(const Axles &obj) {
					m_loadAxle11 = obj.m_loadAxle11;
					m_loadAxle12 = obj.m_loadAxle12;
					m_loadAxle13 = obj.m_loadAxle13;
					return (*this);
				}
			
				int32_t Axles::ID() {
					return 195;
				}
			
				const string Axles::ShortName() {
					return "Axles";
				}
			
				const string Axles::LongName() {
					return "opendlv.proxy.reverefh16.Axles";
				}
			
				int32_t Axles::getID() const {
					return Axles::ID();
				}
			
				const string Axles::getShortName() const {
					return Axles::ShortName();
				}
			
				const string Axles::getLongName() const {
					return Axles::LongName();
				}
			
				double Axles::getLoadAxle11() const {
					return m_loadAxle11;
				}
				
				void Axles::setLoadAxle11(const double &val) {
					m_loadAxle11 = val;
				}
				double Axles::getLoadAxle12() const {
					return m_loadAxle12;
				}
				
				void Axles::setLoadAxle12(const double &val) {
					m_loadAxle12 = val;
				}
				double Axles::getLoadAxle13() const {
					return m_loadAxle13;
				}
				
				void Axles::setLoadAxle13(const double &val) {
					m_loadAxle13 = val;
				}
			
				void Axles::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'l', CharList<'o', CharList<'a', CharList<'d', CharList<'A', CharList<'x', CharList<'l', CharList<'e', CharList<'1', CharList<'1', NullType> > > > > > > > > >  >::RESULT, 1, "Axles.loadAxle11", "loadAxle11", m_loadAxle11);
					v.visit(CRC32 < CharList<'l', CharList<'o', CharList<'a', CharList<'d', CharList<'A', CharList<'x', CharList<'l', CharList<'e', CharList<'1', CharList<'2', NullType> > > > > > > > > >  >::RESULT, 2, "Axles.loadAxle12", "loadAxle12", m_loadAxle12);
					v.visit(CRC32 < CharList<'l', CharList<'o', CharList<'a', CharList<'d', CharList<'A', CharList<'x', CharList<'l', CharList<'e', CharList<'1', CharList<'3', NullType> > > > > > > > > >  >::RESULT, 3, "Axles.loadAxle13", "loadAxle13", m_loadAxle13);
					v.endVisit();
				}
			
				const string Axles::toString() const {
					stringstream s;
			
			
					s << "LoadAxle11: " << getLoadAxle11() << " ";
					s << "LoadAxle12: " << getLoadAxle12() << " ";
					s << "LoadAxle13: " << getLoadAxle13() << " ";
			
					return s.str();
				}
			
				ostream& Axles::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_loadAxle11);
					s->write(2,
							m_loadAxle12);
					s->write(3,
							m_loadAxle13);
					return out;
				}
			
				istream& Axles::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_loadAxle11);
					d->read(2,
							m_loadAxle12);
					d->read(3,
							m_loadAxle13);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
