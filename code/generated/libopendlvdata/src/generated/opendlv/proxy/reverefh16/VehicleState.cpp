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


#include "opendlvdata/generated/opendlv/proxy/reverefh16/VehicleState.h"

namespace opendlv {
	namespace proxy {
		namespace reverefh16 {
				using namespace std;
				using namespace odcore::base;
			
			
				VehicleState::VehicleState() :
				    SerializableData(), Visitable()
					, m_accelerationX(0.0)
					, m_accelerationY(0.0)
					, m_yawRate(0.0)
				{
				}
			
				VehicleState::VehicleState(
					const double &val0, 
					const double &val1, 
					const double &val2
				) :
				    SerializableData(), Visitable()
					, m_accelerationX(val0)
					, m_accelerationY(val1)
					, m_yawRate(val2)
				{
				}
			
				VehicleState::VehicleState(const VehicleState &obj) :
				    SerializableData(), Visitable()
					, m_accelerationX(obj.m_accelerationX)
					, m_accelerationY(obj.m_accelerationY)
					, m_yawRate(obj.m_yawRate)
				{
				}
				
				VehicleState::~VehicleState() {
				}
			
				VehicleState& VehicleState::operator=(const VehicleState &obj) {
					m_accelerationX = obj.m_accelerationX;
					m_accelerationY = obj.m_accelerationY;
					m_yawRate = obj.m_yawRate;
					return (*this);
				}
			
				int32_t VehicleState::ID() {
					return 197;
				}
			
				const string VehicleState::ShortName() {
					return "VehicleState";
				}
			
				const string VehicleState::LongName() {
					return "opendlv.proxy.reverefh16.VehicleState";
				}
			
				int32_t VehicleState::getID() const {
					return VehicleState::ID();
				}
			
				const string VehicleState::getShortName() const {
					return VehicleState::ShortName();
				}
			
				const string VehicleState::getLongName() const {
					return VehicleState::LongName();
				}
			
				double VehicleState::getAccelerationX() const {
					return m_accelerationX;
				}
				
				void VehicleState::setAccelerationX(const double &val) {
					m_accelerationX = val;
				}
				double VehicleState::getAccelerationY() const {
					return m_accelerationY;
				}
				
				void VehicleState::setAccelerationY(const double &val) {
					m_accelerationY = val;
				}
				double VehicleState::getYawRate() const {
					return m_yawRate;
				}
				
				void VehicleState::setYawRate(const double &val) {
					m_yawRate = val;
				}
			
				void VehicleState::accept(odcore::base::Visitor &v) {
					v.beginVisit();
					v.visit(CRC32 < CharList<'a', CharList<'c', CharList<'c', CharList<'e', CharList<'l', CharList<'e', CharList<'r', CharList<'a', CharList<'t', CharList<'i', CharList<'o', CharList<'n', CharList<'X', NullType> > > > > > > > > > > > >  >::RESULT, 1, "VehicleState.accelerationX", "accelerationX", m_accelerationX);
					v.visit(CRC32 < CharList<'a', CharList<'c', CharList<'c', CharList<'e', CharList<'l', CharList<'e', CharList<'r', CharList<'a', CharList<'t', CharList<'i', CharList<'o', CharList<'n', CharList<'Y', NullType> > > > > > > > > > > > >  >::RESULT, 2, "VehicleState.accelerationY", "accelerationY", m_accelerationY);
					v.visit(CRC32 < CharList<'y', CharList<'a', CharList<'w', CharList<'R', CharList<'a', CharList<'t', CharList<'e', NullType> > > > > > >  >::RESULT, 3, "VehicleState.yawRate", "yawRate", m_yawRate);
					v.endVisit();
				}
			
				const string VehicleState::toString() const {
					stringstream s;
			
			
					s << "AccelerationX: " << getAccelerationX() << " ";
					s << "AccelerationY: " << getAccelerationY() << " ";
					s << "YawRate: " << getYawRate() << " ";
			
					return s.str();
				}
			
				ostream& VehicleState::operator<<(ostream &out) const {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Serializer> s = sf.getSerializer(out);
			
					s->write(1,
							m_accelerationX);
					s->write(2,
							m_accelerationY);
					s->write(3,
							m_yawRate);
					return out;
				}
			
				istream& VehicleState::operator>>(istream &in) {
			
					SerializationFactory& sf = SerializationFactory::getInstance();
			
					std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
			
					d->read(1,
							m_accelerationX);
					d->read(2,
							m_accelerationY);
					d->read(3,
							m_yawRate);
					return in;
				}
		} // reverefh16
	} // proxy
} // opendlv
