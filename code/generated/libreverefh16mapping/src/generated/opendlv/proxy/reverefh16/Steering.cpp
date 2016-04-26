/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.Steering

/*
signals of interest:

steering.roadwheelangle
steering.steeringwheelangle


CANID       : 0x107
FQDN        : steering.roadwheelangle
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.000058751
add         : -1.925
rangeStart  : -1.925
rangeEnd    : 1.925246785

CANID       : 0x107
FQDN        : steering.steeringwheelangle
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.0009765625
add         : -31.374
rangeStart  : -31.374
rangeEnd    : 32.6250234375
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/Steering.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				Steering::Steering() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_steeringRoadwheelangle(0.0),
					m_steeringSteeringwheelangle(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Steering()
				{
					m_neededCanMessages.push_back(0x107);
				}
				
				Steering::Steering(double parameter0, double parameter1) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_steeringRoadwheelangle(parameter0),
					m_steeringSteeringwheelangle(parameter1),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Steering()
				{
					m_neededCanMessages.push_back(0x107);
				}
				
				Steering::~Steering() {}
				
				const string Steering::toString() const {
					stringstream result;
					result << "Class : Steering"<<endl
							<<" steering.roadwheelangle : "<< m_steeringRoadwheelangle<<endl
							<<" steering.steeringwheelangle : "<< m_steeringSteeringwheelangle<<endl;
					return result.str();
				}
				
				double Steering::getSteeringRoadwheelangle() const{
					return m_steeringRoadwheelangle;
				}
				
				void Steering::setSteeringRoadwheelangle(const double &steeringRoadwheelangle) {
					m_steeringRoadwheelangle=steeringRoadwheelangle;
				}
				
				double Steering::getSteeringSteeringwheelangle() const{
					return m_steeringSteeringwheelangle;
				}
				
				void Steering::setSteeringSteeringwheelangle(const double &steeringSteeringwheelangle) {
					m_steeringSteeringwheelangle=steeringSteeringwheelangle;
				}
				
				
				ostream& Steering::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_steeringRoadwheelangle);
					s->write(static_cast<uint32_t>(2), m_steeringSteeringwheelangle);
					return out;
				}
				
				istream& Steering::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_steeringRoadwheelangle);
					id=2;
					s->read(static_cast<uint32_t>(id), m_steeringSteeringwheelangle);
					return in;
				}
				
				int32_t Steering::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Steering::ShortName() {
					return "Steering";
				}
				
				const string Steering::LongName() {
					return "Steering";
				}
				
				int32_t Steering::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Steering::getShortName() const {
					return Steering::ShortName();
				}
				
				const string Steering::getLongName() const {
					return Steering::LongName();
				}
				
				void Steering::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "steering.roadwheelangle", "roadwheelangle", m_steeringRoadwheelangle);
					v.visit(static_cast<uint32_t>(2), 2, "steering.steeringwheelangle", "steeringwheelangle", m_steeringSteeringwheelangle);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage Steering::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x107;
				GCM_0x107.setIdentifier(0x107);
				uint64_t GCMPayload_0x107=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::Steering::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::Steering tempSteering=c.getData<::opendlv::proxy::reverefh16::Steering>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempSteering.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal steering.roadwheelangle
					found=extracted=false;
					double rawSteering_roadwheelangle = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawSteering_roadwheelangle<-1.925)
							rawSteering_roadwheelangle=-1.925;
						if(rawSteering_roadwheelangle>1.925246785)
							rawSteering_roadwheelangle=1.925246785;
						
						const double SCALE = -1.925 ;
						const double OFFSET = 0.000058751 ;
						
						double transformedSteering_roadwheelangle=static_cast<double>((rawSteering_roadwheelangle - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalSteering_roadwheelangle=static_cast<uint16_t>(round(transformedSteering_roadwheelangle)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLSteering_roadwheelangle=0x0;
						tempPLSteering_roadwheelangle=static_cast<uint64_t>(finalSteering_roadwheelangle);
						tempPLSteering_roadwheelangle=tempPLSteering_roadwheelangle<<(0);
						GCMPayload_0x107=GCMPayload_0x107 | tempPLSteering_roadwheelangle;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal steering.steeringwheelangle
					found=extracted=false;
					double rawSteering_steeringwheelangle = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawSteering_steeringwheelangle<-31.374)
							rawSteering_steeringwheelangle=-31.374;
						if(rawSteering_steeringwheelangle>32.6250234375)
							rawSteering_steeringwheelangle=32.6250234375;
						
						const double SCALE = -31.374 ;
						const double OFFSET = 0.0009765625 ;
						
						double transformedSteering_steeringwheelangle=static_cast<double>((rawSteering_steeringwheelangle - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalSteering_steeringwheelangle=static_cast<uint16_t>(round(transformedSteering_steeringwheelangle)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLSteering_steeringwheelangle=0x0;
						tempPLSteering_steeringwheelangle=static_cast<uint64_t>(finalSteering_steeringwheelangle);
						tempPLSteering_steeringwheelangle=tempPLSteering_steeringwheelangle<<(16);
						GCMPayload_0x107=GCMPayload_0x107 | tempPLSteering_steeringwheelangle;
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
						rolledPayload |= GCMPayload_0x107 & 0xFF ;
						GCMPayload_0x107=GCMPayload_0x107>>8;
					}
				
					GCM_0x107.setData(rolledPayload);
					GCM_0x107.setLength(static_cast<uint8_t>(4.0));
				}
				return GCM_0x107;
				}
				
				odcore::data::Container Steering::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x107 : 
				
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
				
					// addressing signal steering.roadwheelangle
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x107];
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
						uint64_t raw_steering_roadwheelangle=0x0;
						raw_steering_roadwheelangle=data;
				
						// reset right-hand side of bit field
						raw_steering_roadwheelangle=raw_steering_roadwheelangle >> (0);
				
						raw_steering_roadwheelangle=raw_steering_roadwheelangle & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_steering_roadwheelangle=static_cast<double>(raw_steering_roadwheelangle);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.000058751;
						const double OFFSET = -1.925;
						
						// scaling and adding
						transformed_steering_roadwheelangle=(transformed_steering_roadwheelangle*SCALE)+OFFSET;
						
						// clamping
						if(transformed_steering_roadwheelangle<-1.925)
							transformed_steering_roadwheelangle=-1.925;
						else if(transformed_steering_roadwheelangle>1.925246785)
							transformed_steering_roadwheelangle=1.925246785;
						
						m_steeringRoadwheelangle=transformed_steering_roadwheelangle;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_steeringRoadwheelangle);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("steering.roadwheelangle");
						f->setShortFieldName("roadwheelangle");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_steeringRoadwheelangle));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal steering.steeringwheelangle
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x107];
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
						uint64_t raw_steering_steeringwheelangle=0x0;
						raw_steering_steeringwheelangle=data;
				
						// reset right-hand side of bit field
						raw_steering_steeringwheelangle=raw_steering_steeringwheelangle >> (16);
				
						raw_steering_steeringwheelangle=raw_steering_steeringwheelangle & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_steering_steeringwheelangle=static_cast<double>(raw_steering_steeringwheelangle);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.0009765625;
						const double OFFSET = -31.374;
						
						// scaling and adding
						transformed_steering_steeringwheelangle=(transformed_steering_steeringwheelangle*SCALE)+OFFSET;
						
						// clamping
						if(transformed_steering_steeringwheelangle<-31.374)
							transformed_steering_steeringwheelangle=-31.374;
						else if(transformed_steering_steeringwheelangle>32.6250234375)
							transformed_steering_steeringwheelangle=32.6250234375;
						
						m_steeringSteeringwheelangle=transformed_steering_steeringwheelangle;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_steeringSteeringwheelangle);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("steering.steeringwheelangle");
						f->setShortFieldName("steeringwheelangle");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_steeringSteeringwheelangle));
				
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
				        	m_opendlv_proxy_reverefh16_Steering.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_Steering);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
