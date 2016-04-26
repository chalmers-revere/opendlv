/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.VehicleState

/*
signals of interest:

vehicledynamics.acceleration_x
vehicledynamics.acceleration_y
vehicledynamics.yawrate


CANID       : 0x104
FQDN        : vehicledynamics.acceleration_x
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00048828125
add         : -15.687
rangeStart  : -15.687
rangeEnd    : 16.31251171875

CANID       : 0x104
FQDN        : vehicledynamics.acceleration_y
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00048828125
add         : -15.687
rangeStart  : -15.687
rangeEnd    : 16.31251171875

CANID       : 0x104
FQDN        : vehicledynamics.yawrate
startBit    : 32
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.000122
add         : -3.92
rangeStart  : -3.92
rangeEnd    : 4.07527
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/VehicleState.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				VehicleState::VehicleState() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_vehicledynamicsAcceleration_x(0.0),
					m_vehicledynamicsAcceleration_y(0.0),
					m_vehicledynamicsYawrate(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_VehicleState()
				{
					m_neededCanMessages.push_back(0x104);
				}
				
				VehicleState::VehicleState(double parameter0, double parameter1, double parameter2) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_vehicledynamicsAcceleration_x(parameter0),
					m_vehicledynamicsAcceleration_y(parameter1),
					m_vehicledynamicsYawrate(parameter2),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_VehicleState()
				{
					m_neededCanMessages.push_back(0x104);
				}
				
				VehicleState::~VehicleState() {}
				
				const string VehicleState::toString() const {
					stringstream result;
					result << "Class : VehicleState"<<endl
							<<" vehicledynamics.acceleration_x : "<< m_vehicledynamicsAcceleration_x<<endl
							<<" vehicledynamics.acceleration_y : "<< m_vehicledynamicsAcceleration_y<<endl
							<<" vehicledynamics.yawrate : "<< m_vehicledynamicsYawrate<<endl;
					return result.str();
				}
				
				double VehicleState::getVehicledynamicsAcceleration_x() const{
					return m_vehicledynamicsAcceleration_x;
				}
				
				void VehicleState::setVehicledynamicsAcceleration_x(const double &vehicledynamicsAcceleration_x) {
					m_vehicledynamicsAcceleration_x=vehicledynamicsAcceleration_x;
				}
				
				double VehicleState::getVehicledynamicsAcceleration_y() const{
					return m_vehicledynamicsAcceleration_y;
				}
				
				void VehicleState::setVehicledynamicsAcceleration_y(const double &vehicledynamicsAcceleration_y) {
					m_vehicledynamicsAcceleration_y=vehicledynamicsAcceleration_y;
				}
				
				double VehicleState::getVehicledynamicsYawrate() const{
					return m_vehicledynamicsYawrate;
				}
				
				void VehicleState::setVehicledynamicsYawrate(const double &vehicledynamicsYawrate) {
					m_vehicledynamicsYawrate=vehicledynamicsYawrate;
				}
				
				
				ostream& VehicleState::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_vehicledynamicsAcceleration_x);
					s->write(static_cast<uint32_t>(2), m_vehicledynamicsAcceleration_y);
					s->write(static_cast<uint32_t>(3), m_vehicledynamicsYawrate);
					return out;
				}
				
				istream& VehicleState::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_vehicledynamicsAcceleration_x);
					id=2;
					s->read(static_cast<uint32_t>(id), m_vehicledynamicsAcceleration_y);
					id=3;
					s->read(static_cast<uint32_t>(id), m_vehicledynamicsYawrate);
					return in;
				}
				
				int32_t VehicleState::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string VehicleState::ShortName() {
					return "VehicleState";
				}
				
				const string VehicleState::LongName() {
					return "VehicleState";
				}
				
				int32_t VehicleState::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string VehicleState::getShortName() const {
					return VehicleState::ShortName();
				}
				
				const string VehicleState::getLongName() const {
					return VehicleState::LongName();
				}
				
				void VehicleState::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "vehicledynamics.acceleration_x", "acceleration_x", m_vehicledynamicsAcceleration_x);
					v.visit(static_cast<uint32_t>(2), 2, "vehicledynamics.acceleration_y", "acceleration_y", m_vehicledynamicsAcceleration_y);
					v.visit(static_cast<uint32_t>(3), 3, "vehicledynamics.yawrate", "yawrate", m_vehicledynamicsYawrate);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage VehicleState::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x104;
				GCM_0x104.setIdentifier(0x104);
				uint64_t GCMPayload_0x104=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::VehicleState::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::VehicleState tempVehicleState=c.getData<::opendlv::proxy::reverefh16::VehicleState>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempVehicleState.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal vehicledynamics.acceleration_x
					found=extracted=false;
					double rawVehicledynamics_acceleration_x = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawVehicledynamics_acceleration_x<-15.687)
							rawVehicledynamics_acceleration_x=-15.687;
						if(rawVehicledynamics_acceleration_x>16.31251171875)
							rawVehicledynamics_acceleration_x=16.31251171875;
						
						const double SCALE = -15.687 ;
						const double OFFSET = 0.00048828125 ;
						
						double transformedVehicledynamics_acceleration_x=static_cast<double>((rawVehicledynamics_acceleration_x - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalVehicledynamics_acceleration_x=static_cast<uint16_t>(round(transformedVehicledynamics_acceleration_x)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLVehicledynamics_acceleration_x=0x0;
						tempPLVehicledynamics_acceleration_x=static_cast<uint64_t>(finalVehicledynamics_acceleration_x);
						tempPLVehicledynamics_acceleration_x=tempPLVehicledynamics_acceleration_x<<(0);
						GCMPayload_0x104=GCMPayload_0x104 | tempPLVehicledynamics_acceleration_x;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal vehicledynamics.acceleration_y
					found=extracted=false;
					double rawVehicledynamics_acceleration_y = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawVehicledynamics_acceleration_y<-15.687)
							rawVehicledynamics_acceleration_y=-15.687;
						if(rawVehicledynamics_acceleration_y>16.31251171875)
							rawVehicledynamics_acceleration_y=16.31251171875;
						
						const double SCALE = -15.687 ;
						const double OFFSET = 0.00048828125 ;
						
						double transformedVehicledynamics_acceleration_y=static_cast<double>((rawVehicledynamics_acceleration_y - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalVehicledynamics_acceleration_y=static_cast<uint16_t>(round(transformedVehicledynamics_acceleration_y)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLVehicledynamics_acceleration_y=0x0;
						tempPLVehicledynamics_acceleration_y=static_cast<uint64_t>(finalVehicledynamics_acceleration_y);
						tempPLVehicledynamics_acceleration_y=tempPLVehicledynamics_acceleration_y<<(16);
						GCMPayload_0x104=GCMPayload_0x104 | tempPLVehicledynamics_acceleration_y;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal vehicledynamics.yawrate
					found=extracted=false;
					double rawVehicledynamics_yawrate = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/3, found, extracted);
					
					if(found && extracted){
						if(rawVehicledynamics_yawrate<-3.92)
							rawVehicledynamics_yawrate=-3.92;
						if(rawVehicledynamics_yawrate>4.07527)
							rawVehicledynamics_yawrate=4.07527;
						
						const double SCALE = -3.92 ;
						const double OFFSET = 0.000122 ;
						
						double transformedVehicledynamics_yawrate=static_cast<double>((rawVehicledynamics_yawrate - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalVehicledynamics_yawrate=static_cast<uint16_t>(round(transformedVehicledynamics_yawrate)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLVehicledynamics_yawrate=0x0;
						tempPLVehicledynamics_yawrate=static_cast<uint64_t>(finalVehicledynamics_yawrate);
						tempPLVehicledynamics_yawrate=tempPLVehicledynamics_yawrate<<(32);
						GCMPayload_0x104=GCMPayload_0x104 | tempPLVehicledynamics_yawrate;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
				
				if(abort){
					// discard and return
					::automotive::GenericCANMessage gcm;
					gcm.setData(0x0);
					gcm.setLength(0);
					return gcm;
				}
				
				// set payload of GenericCANMessage and return
				// the rolling is due to Vector's dbc file numeration convention
				{
					uint64_t rolledPayload=0x0;
					for(uint8_t i=0;i<static_cast<uint8_t>(6.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x104 & 0xFF ;
						GCMPayload_0x104=GCMPayload_0x104>>8;
					}
				
					GCM_0x104.setData(rolledPayload);
					GCM_0x104.setLength(static_cast<uint8_t>(6.0));
				}
				return GCM_0x104;
				}
				
				odcore::data::Container VehicleState::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x104 : 
				
				// since the order doesn't matter, store the payload in a map for future use replacing the current content held there
				m_payloads[gcm.getIdentifier()] = gcm.getData();
				    	break;
				
					default : return c; // valid id not found
				}
				
					// if we don't have all the needed CAN messages, return 
					if(m_payloads.size()!=m_neededCanMessages.size())
						return c;
				
					// Create a generic message.
					odcore::reflection::Message message;
				
					// addressing signal vehicledynamics.acceleration_x
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x104];
						// the rolling is due to Vector's dbc file numeration convention
						{
							uint64_t rolledPayload=0x0;
							for(uint8_t i=0;i<gcm.getLength(); ++i)
							{
								rolledPayload=rolledPayload<<8;
								rolledPayload |= data & 0xFF ;
								data=data>>8;
							}
						
							data=rolledPayload;
						}
						
						// Get raw value from attribute.
						uint64_t raw_vehicledynamics_acceleration_x=0x0;
						raw_vehicledynamics_acceleration_x=data;
				
						// reset right-hand side of bit field
						raw_vehicledynamics_acceleration_x=raw_vehicledynamics_acceleration_x >> (0);
				
						raw_vehicledynamics_acceleration_x=raw_vehicledynamics_acceleration_x & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_vehicledynamics_acceleration_x=static_cast<double>(raw_vehicledynamics_acceleration_x);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00048828125;
						const double OFFSET = -15.687;
						
						// scaling and adding
						transformed_vehicledynamics_acceleration_x=(transformed_vehicledynamics_acceleration_x*SCALE)+OFFSET;
						
						// clamping
						if(transformed_vehicledynamics_acceleration_x<-15.687)
							transformed_vehicledynamics_acceleration_x=-15.687;
						else if(transformed_vehicledynamics_acceleration_x>16.31251171875)
							transformed_vehicledynamics_acceleration_x=16.31251171875;
						
						m_vehicledynamicsAcceleration_x=transformed_vehicledynamics_acceleration_x;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_vehicledynamicsAcceleration_x);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("vehicledynamics.acceleration_x");
						f->setShortFieldName("acceleration_x");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_vehicledynamicsAcceleration_x));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal vehicledynamics.acceleration_y
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x104];
						// the rolling is due to Vector's dbc file numeration convention
						{
							uint64_t rolledPayload=0x0;
							for(uint8_t i=0;i<gcm.getLength(); ++i)
							{
								rolledPayload=rolledPayload<<8;
								rolledPayload |= data & 0xFF ;
								data=data>>8;
							}
						
							data=rolledPayload;
						}
						
						// Get raw value from attribute.
						uint64_t raw_vehicledynamics_acceleration_y=0x0;
						raw_vehicledynamics_acceleration_y=data;
				
						// reset right-hand side of bit field
						raw_vehicledynamics_acceleration_y=raw_vehicledynamics_acceleration_y >> (16);
				
						raw_vehicledynamics_acceleration_y=raw_vehicledynamics_acceleration_y & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_vehicledynamics_acceleration_y=static_cast<double>(raw_vehicledynamics_acceleration_y);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00048828125;
						const double OFFSET = -15.687;
						
						// scaling and adding
						transformed_vehicledynamics_acceleration_y=(transformed_vehicledynamics_acceleration_y*SCALE)+OFFSET;
						
						// clamping
						if(transformed_vehicledynamics_acceleration_y<-15.687)
							transformed_vehicledynamics_acceleration_y=-15.687;
						else if(transformed_vehicledynamics_acceleration_y>16.31251171875)
							transformed_vehicledynamics_acceleration_y=16.31251171875;
						
						m_vehicledynamicsAcceleration_y=transformed_vehicledynamics_acceleration_y;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_vehicledynamicsAcceleration_y);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("vehicledynamics.acceleration_y");
						f->setShortFieldName("acceleration_y");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_vehicledynamicsAcceleration_y));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal vehicledynamics.yawrate
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x104];
						// the rolling is due to Vector's dbc file numeration convention
						{
							uint64_t rolledPayload=0x0;
							for(uint8_t i=0;i<gcm.getLength(); ++i)
							{
								rolledPayload=rolledPayload<<8;
								rolledPayload |= data & 0xFF ;
								data=data>>8;
							}
						
							data=rolledPayload;
						}
						
						// Get raw value from attribute.
						uint64_t raw_vehicledynamics_yawrate=0x0;
						raw_vehicledynamics_yawrate=data;
				
						// reset right-hand side of bit field
						raw_vehicledynamics_yawrate=raw_vehicledynamics_yawrate >> (32);
				
						raw_vehicledynamics_yawrate=raw_vehicledynamics_yawrate & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_vehicledynamics_yawrate=static_cast<double>(raw_vehicledynamics_yawrate);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.000122;
						const double OFFSET = -3.92;
						
						// scaling and adding
						transformed_vehicledynamics_yawrate=(transformed_vehicledynamics_yawrate*SCALE)+OFFSET;
						
						// clamping
						if(transformed_vehicledynamics_yawrate<-3.92)
							transformed_vehicledynamics_yawrate=-3.92;
						else if(transformed_vehicledynamics_yawrate>4.07527)
							transformed_vehicledynamics_yawrate=4.07527;
						
						m_vehicledynamicsYawrate=transformed_vehicledynamics_yawrate;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_vehicledynamicsYawrate);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(3); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("vehicledynamics.yawrate");
						f->setShortFieldName("yawrate");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_vehicledynamicsYawrate));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// Depending on the CAN message specification, we are either ready here
					// (i.e. mapping one CAN message to one high-level C++ message), or we have
					// to wait for more low-level CAN messages to complete this high-level C++ message.
					// Thus, our state machine would have to store this (incomplete) "message"
					// variable internally to continue decoding later.
					{
						// As we are ready here, we create a visitor to traverse the 
						// unnamed message and create a named message (i.e. an instance
						// of a high-level C++ message) to be distributed as a Container.
						odcore::reflection::MessageToVisitableVisitor mtvv(message);
						// Letting the high-level message accept the visitor to set the values.
				        	m_opendlv_proxy_reverefh16_VehicleState.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_VehicleState);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
