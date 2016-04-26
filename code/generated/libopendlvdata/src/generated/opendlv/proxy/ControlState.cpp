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


#include "opendlvdata/generated/opendlv/proxy/ControlState.h"

namespace opendlv {
	namespace proxy {
			using namespace std;
			using namespace odcore::base;
		
		
			ControlState::ControlState() :
			    SerializableData(), Visitable()
				, m_allowAutonomous(false)
			{
			}
		
			ControlState::ControlState(
				const bool &val0
			) :
			    SerializableData(), Visitable()
				, m_allowAutonomous(val0)
			{
			}
		
			ControlState::ControlState(const ControlState &obj) :
			    SerializableData(), Visitable()
				, m_allowAutonomous(obj.m_allowAutonomous)
			{
			}
			
			ControlState::~ControlState() {
			}
		
			ControlState& ControlState::operator=(const ControlState &obj) {
				m_allowAutonomous = obj.m_allowAutonomous;
				return (*this);
			}
		
			int32_t ControlState::ID() {
				return 165;
			}
		
			const string ControlState::ShortName() {
				return "ControlState";
			}
		
			const string ControlState::LongName() {
				return "opendlv.proxy.ControlState";
			}
		
			int32_t ControlState::getID() const {
				return ControlState::ID();
			}
		
			const string ControlState::getShortName() const {
				return ControlState::ShortName();
			}
		
			const string ControlState::getLongName() const {
				return ControlState::LongName();
			}
		
			bool ControlState::getAllowAutonomous() const {
				return m_allowAutonomous;
			}
			
			void ControlState::setAllowAutonomous(const bool &val) {
				m_allowAutonomous = val;
			}
		
			void ControlState::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'a', CharList<'l', CharList<'l', CharList<'o', CharList<'w', CharList<'A', CharList<'u', CharList<'t', CharList<'o', CharList<'n', CharList<'o', CharList<'m', CharList<'o', CharList<'u', CharList<'s', NullType> > > > > > > > > > > > > > >  >::RESULT, 1, "ControlState.allowAutonomous", "allowAutonomous", m_allowAutonomous);
				v.endVisit();
			}
		
			const string ControlState::toString() const {
				stringstream s;
		
		
				s << "AllowAutonomous: " << getAllowAutonomous() << " ";
		
				return s.str();
			}
		
			ostream& ControlState::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_allowAutonomous);
				return out;
			}
		
			istream& ControlState::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_allowAutonomous);
				return in;
			}
	} // proxy
} // opendlv
