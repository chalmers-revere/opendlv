/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.Pedals

/*
signals of interest:

manualdriver.accelerationpedalposition
manualdriver.brakepedalposition
manualdriver.torsionbartorque


CANID       : 0x106
FQDN        : manualdriver.accelerationpedalposition
startBit    : 0
length      : 8
lengthBytes : 1
endian      : little
multiplyBy  : 0.4
add         : 0
rangeStart  : 0
rangeEnd    : 102

CANID       : 0x106
FQDN        : manualdriver.brakepedalposition
startBit    : 8
length      : 8
lengthBytes : 1
endian      : little
multiplyBy  : 0.4
add         : 0
rangeStart  : 0
rangeEnd    : 102

CANID       : 0x106
FQDN        : manualdriver.torsionbartorque
startBit    : 23
length      : 16
lengthBytes : 2
endian      : big
multiplyBy  : 0.0009765625
add         : -32
rangeStart  : -32
rangeEnd    : 31.9990234375
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/Pedals.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				Pedals::Pedals() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_manualdriverAccelerationpedalposition(0.0),
					m_manualdriverBrakepedalposition(0.0),
					m_manualdriverTorsionbartorque(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Pedals()
				{
					m_neededCanMessages.push_back(0x106);
				}
				
				Pedals::Pedals(double parameter0, double parameter1, double parameter2) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_manualdriverAccelerationpedalposition(parameter0),
					m_manualdriverBrakepedalposition(parameter1),
					m_manualdriverTorsionbartorque(parameter2),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Pedals()
				{
					m_neededCanMessages.push_back(0x106);
				}
				
				Pedals::~Pedals() {}
				
				const string Pedals::toString() const {
					stringstream result;
					result << "Class : Pedals"<<endl
							<<" manualdriver.accelerationpedalposition : "<< m_manualdriverAccelerationpedalposition<<endl
							<<" manualdriver.brakepedalposition : "<< m_manualdriverBrakepedalposition<<endl
							<<" manualdriver.torsionbartorque : "<< m_manualdriverTorsionbartorque<<endl;
					return result.str();
				}
				
				double Pedals::getManualdriverAccelerationpedalposition() const{
					return m_manualdriverAccelerationpedalposition;
				}
				
				void Pedals::setManualdriverAccelerationpedalposition(const double &manualdriverAccelerationpedalposition) {
					m_manualdriverAccelerationpedalposition=manualdriverAccelerationpedalposition;
				}
				
				double Pedals::getManualdriverBrakepedalposition() const{
					return m_manualdriverBrakepedalposition;
				}
				
				void Pedals::setManualdriverBrakepedalposition(const double &manualdriverBrakepedalposition) {
					m_manualdriverBrakepedalposition=manualdriverBrakepedalposition;
				}
				
				double Pedals::getManualdriverTorsionbartorque() const{
					return m_manualdriverTorsionbartorque;
				}
				
				void Pedals::setManualdriverTorsionbartorque(const double &manualdriverTorsionbartorque) {
					m_manualdriverTorsionbartorque=manualdriverTorsionbartorque;
				}
				
				
				ostream& Pedals::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_manualdriverAccelerationpedalposition);
					s->write(static_cast<uint32_t>(2), m_manualdriverBrakepedalposition);
					s->write(static_cast<uint32_t>(3), m_manualdriverTorsionbartorque);
					return out;
				}
				
				istream& Pedals::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_manualdriverAccelerationpedalposition);
					id=2;
					s->read(static_cast<uint32_t>(id), m_manualdriverBrakepedalposition);
					id=3;
					s->read(static_cast<uint32_t>(id), m_manualdriverTorsionbartorque);
					return in;
				}
				
				int32_t Pedals::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Pedals::ShortName() {
					return "Pedals";
				}
				
				const string Pedals::LongName() {
					return "Pedals";
				}
				
				int32_t Pedals::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Pedals::getShortName() const {
					return Pedals::ShortName();
				}
				
				const string Pedals::getLongName() const {
					return Pedals::LongName();
				}
				
				void Pedals::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "manualdriver.accelerationpedalposition", "accelerationpedalposition", m_manualdriverAccelerationpedalposition);
					v.visit(static_cast<uint32_t>(2), 2, "manualdriver.brakepedalposition", "brakepedalposition", m_manualdriverBrakepedalposition);
					v.visit(static_cast<uint32_t>(3), 3, "manualdriver.torsionbartorque", "torsionbartorque", m_manualdriverTorsionbartorque);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage Pedals::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x106;
				GCM_0x106.setIdentifier(0x106);
				uint64_t GCMPayload_0x106=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::Pedals::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::Pedals tempPedals=c.getData<::opendlv::proxy::reverefh16::Pedals>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempPedals.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal manualdriver.accelerationpedalposition
					found=extracted=false;
					double rawManualdriver_accelerationpedalposition = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawManualdriver_accelerationpedalposition<0)
							rawManualdriver_accelerationpedalposition=0;
						if(rawManualdriver_accelerationpedalposition>102)
							rawManualdriver_accelerationpedalposition=102;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.4 ;
						
						double transformedManualdriver_accelerationpedalposition=static_cast<double>((rawManualdriver_accelerationpedalposition - SCALE) / (double) OFFSET);
				
						
						// signal length is 8 bits long and the value will be casted to uint8_t
						uint8_t finalManualdriver_accelerationpedalposition=static_cast<uint8_t>(round(transformedManualdriver_accelerationpedalposition)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLManualdriver_accelerationpedalposition=0x0;
						tempPLManualdriver_accelerationpedalposition=static_cast<uint64_t>(finalManualdriver_accelerationpedalposition);
						tempPLManualdriver_accelerationpedalposition=tempPLManualdriver_accelerationpedalposition<<(0);
						GCMPayload_0x106=GCMPayload_0x106 | tempPLManualdriver_accelerationpedalposition;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal manualdriver.brakepedalposition
					found=extracted=false;
					double rawManualdriver_brakepedalposition = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawManualdriver_brakepedalposition<0)
							rawManualdriver_brakepedalposition=0;
						if(rawManualdriver_brakepedalposition>102)
							rawManualdriver_brakepedalposition=102;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.4 ;
						
						double transformedManualdriver_brakepedalposition=static_cast<double>((rawManualdriver_brakepedalposition - SCALE) / (double) OFFSET);
				
						
						// signal length is 8 bits long and the value will be casted to uint16_t
						uint16_t finalManualdriver_brakepedalposition=static_cast<uint16_t>(round(transformedManualdriver_brakepedalposition)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLManualdriver_brakepedalposition=0x0;
						tempPLManualdriver_brakepedalposition=static_cast<uint64_t>(finalManualdriver_brakepedalposition);
						tempPLManualdriver_brakepedalposition=tempPLManualdriver_brakepedalposition<<(8);
						GCMPayload_0x106=GCMPayload_0x106 | tempPLManualdriver_brakepedalposition;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal manualdriver.torsionbartorque
					found=extracted=false;
					double rawManualdriver_torsionbartorque = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/3, found, extracted);
					
					if(found && extracted){
						if(rawManualdriver_torsionbartorque<-32)
							rawManualdriver_torsionbartorque=-32;
						if(rawManualdriver_torsionbartorque>31.9990234375)
							rawManualdriver_torsionbartorque=31.9990234375;
						
						const double SCALE = -32 ;
						const double OFFSET = 0.0009765625 ;
						
						double transformedManualdriver_torsionbartorque=static_cast<double>((rawManualdriver_torsionbartorque - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalManualdriver_torsionbartorque=static_cast<uint16_t>(round(transformedManualdriver_torsionbartorque)); // avoid truncation errors
				
						// endian      : big
							finalManualdriver_torsionbartorque=htons(finalManualdriver_torsionbartorque);
				
						uint64_t tempPLManualdriver_torsionbartorque=0x0;
						tempPLManualdriver_torsionbartorque=static_cast<uint64_t>(finalManualdriver_torsionbartorque);
						tempPLManualdriver_torsionbartorque=tempPLManualdriver_torsionbartorque<<(23);
						GCMPayload_0x106=GCMPayload_0x106 | tempPLManualdriver_torsionbartorque;
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
					for(uint8_t i=0;i<static_cast<uint8_t>(4.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x106 & 0xFF ;
						GCMPayload_0x106=GCMPayload_0x106>>8;
					}
				
					GCM_0x106.setData(rolledPayload);
					GCM_0x106.setLength(static_cast<uint8_t>(4.0));
				}
				return GCM_0x106;
				}
				
				odcore::data::Container Pedals::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x106 : 
				
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
				
					// addressing signal manualdriver.accelerationpedalposition
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x106];
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
						uint64_t raw_manualdriver_accelerationpedalposition=0x0;
						raw_manualdriver_accelerationpedalposition=data;
				
						// reset right-hand side of bit field
						raw_manualdriver_accelerationpedalposition=raw_manualdriver_accelerationpedalposition >> (0);
				
						raw_manualdriver_accelerationpedalposition=raw_manualdriver_accelerationpedalposition & 0b011111111;
				
						// Field too short for endianness adjustment, skipping this step.
				
						// variable holding the transformed value
						double transformed_manualdriver_accelerationpedalposition=static_cast<double>(raw_manualdriver_accelerationpedalposition);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.4;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_manualdriver_accelerationpedalposition=(transformed_manualdriver_accelerationpedalposition*SCALE)+OFFSET;
						
						// clamping
						if(transformed_manualdriver_accelerationpedalposition<0)
							transformed_manualdriver_accelerationpedalposition=0;
						else if(transformed_manualdriver_accelerationpedalposition>102)
							transformed_manualdriver_accelerationpedalposition=102;
						
						m_manualdriverAccelerationpedalposition=transformed_manualdriver_accelerationpedalposition;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_manualdriverAccelerationpedalposition);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("manualdriver.accelerationpedalposition");
						f->setShortFieldName("accelerationpedalposition");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_manualdriverAccelerationpedalposition));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal manualdriver.brakepedalposition
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x106];
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
						uint64_t raw_manualdriver_brakepedalposition=0x0;
						raw_manualdriver_brakepedalposition=data;
				
						// reset right-hand side of bit field
						raw_manualdriver_brakepedalposition=raw_manualdriver_brakepedalposition >> (8);
				
						raw_manualdriver_brakepedalposition=raw_manualdriver_brakepedalposition & 0b0111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_manualdriver_brakepedalposition=static_cast<double>(raw_manualdriver_brakepedalposition);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.4;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_manualdriver_brakepedalposition=(transformed_manualdriver_brakepedalposition*SCALE)+OFFSET;
						
						// clamping
						if(transformed_manualdriver_brakepedalposition<0)
							transformed_manualdriver_brakepedalposition=0;
						else if(transformed_manualdriver_brakepedalposition>102)
							transformed_manualdriver_brakepedalposition=102;
						
						m_manualdriverBrakepedalposition=transformed_manualdriver_brakepedalposition;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_manualdriverBrakepedalposition);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("manualdriver.brakepedalposition");
						f->setShortFieldName("brakepedalposition");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_manualdriverBrakepedalposition));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal manualdriver.torsionbartorque
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x106];
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
						uint64_t raw_manualdriver_torsionbartorque=0x0;
						raw_manualdriver_torsionbartorque=data;
				
						// reset right-hand side of bit field
						raw_manualdriver_torsionbartorque=raw_manualdriver_torsionbartorque >> (23);
				
						raw_manualdriver_torsionbartorque=raw_manualdriver_torsionbartorque & 0b01111111111111111;
				
						// Optional: Fix endianness depending on CAN message specification.
							uint16_t temp_cast=static_cast<uint16_t>(raw_manualdriver_torsionbartorque);
							temp_cast=ntohs(temp_cast);
							raw_manualdriver_torsionbartorque = static_cast<uint64_t>(temp_cast);
				
						// variable holding the transformed value
						double transformed_manualdriver_torsionbartorque=static_cast<double>(raw_manualdriver_torsionbartorque);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.0009765625;
						const double OFFSET = -32;
						
						// scaling and adding
						transformed_manualdriver_torsionbartorque=(transformed_manualdriver_torsionbartorque*SCALE)+OFFSET;
						
						// clamping
						if(transformed_manualdriver_torsionbartorque<-32)
							transformed_manualdriver_torsionbartorque=-32;
						else if(transformed_manualdriver_torsionbartorque>31.9990234375)
							transformed_manualdriver_torsionbartorque=31.9990234375;
						
						m_manualdriverTorsionbartorque=transformed_manualdriver_torsionbartorque;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_manualdriverTorsionbartorque);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(3); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("manualdriver.torsionbartorque");
						f->setShortFieldName("torsionbartorque");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_manualdriverTorsionbartorque));
				
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
				        	m_opendlv_proxy_reverefh16_Pedals.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_Pedals);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
