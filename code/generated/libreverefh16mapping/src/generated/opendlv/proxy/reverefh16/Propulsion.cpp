/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.Propulsion

/*
signals of interest:

vehiclespeed.vehiclespeedpropshaft


CANID       : 0x103
FQDN        : vehiclespeed.vehiclespeedpropshaft
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/Propulsion.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				Propulsion::Propulsion() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_vehiclespeedVehiclespeedpropshaft(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Propulsion()
				{
					m_neededCanMessages.push_back(0x103);
				}
				
				Propulsion::Propulsion(double parameter0) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_vehiclespeedVehiclespeedpropshaft(parameter0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Propulsion()
				{
					m_neededCanMessages.push_back(0x103);
				}
				
				Propulsion::~Propulsion() {}
				
				const string Propulsion::toString() const {
					stringstream result;
					result << "Class : Propulsion"<<endl
							<<" vehiclespeed.vehiclespeedpropshaft : "<< m_vehiclespeedVehiclespeedpropshaft<<endl;
					return result.str();
				}
				
				double Propulsion::getVehiclespeedVehiclespeedpropshaft() const{
					return m_vehiclespeedVehiclespeedpropshaft;
				}
				
				void Propulsion::setVehiclespeedVehiclespeedpropshaft(const double &vehiclespeedVehiclespeedpropshaft) {
					m_vehiclespeedVehiclespeedpropshaft=vehiclespeedVehiclespeedpropshaft;
				}
				
				
				ostream& Propulsion::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_vehiclespeedVehiclespeedpropshaft);
					return out;
				}
				
				istream& Propulsion::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_vehiclespeedVehiclespeedpropshaft);
					return in;
				}
				
				int32_t Propulsion::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Propulsion::ShortName() {
					return "Propulsion";
				}
				
				const string Propulsion::LongName() {
					return "Propulsion";
				}
				
				int32_t Propulsion::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Propulsion::getShortName() const {
					return Propulsion::ShortName();
				}
				
				const string Propulsion::getLongName() const {
					return Propulsion::LongName();
				}
				
				void Propulsion::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "vehiclespeed.vehiclespeedpropshaft", "vehiclespeedpropshaft", m_vehiclespeedVehiclespeedpropshaft);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage Propulsion::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x103;
				GCM_0x103.setIdentifier(0x103);
				uint64_t GCMPayload_0x103=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::Propulsion::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::Propulsion tempPropulsion=c.getData<::opendlv::proxy::reverefh16::Propulsion>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempPropulsion.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal vehiclespeed.vehiclespeedpropshaft
					found=extracted=false;
					double rawVehiclespeed_vehiclespeedpropshaft = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawVehiclespeed_vehiclespeedpropshaft<0)
							rawVehiclespeed_vehiclespeedpropshaft=0;
						if(rawVehiclespeed_vehiclespeedpropshaft>255.99609375)
							rawVehiclespeed_vehiclespeedpropshaft=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedVehiclespeed_vehiclespeedpropshaft=static_cast<double>((rawVehiclespeed_vehiclespeedpropshaft - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalVehiclespeed_vehiclespeedpropshaft=static_cast<uint16_t>(round(transformedVehiclespeed_vehiclespeedpropshaft)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLVehiclespeed_vehiclespeedpropshaft=0x0;
						tempPLVehiclespeed_vehiclespeedpropshaft=static_cast<uint64_t>(finalVehiclespeed_vehiclespeedpropshaft);
						tempPLVehiclespeed_vehiclespeedpropshaft=tempPLVehiclespeed_vehiclespeedpropshaft<<(0);
						GCMPayload_0x103=GCMPayload_0x103 | tempPLVehiclespeed_vehiclespeedpropshaft;
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
					for(uint8_t i=0;i<static_cast<uint8_t>(2.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x103 & 0xFF ;
						GCMPayload_0x103=GCMPayload_0x103>>8;
					}
				
					GCM_0x103.setData(rolledPayload);
					GCM_0x103.setLength(static_cast<uint8_t>(2.0));
				}
				return GCM_0x103;
				}
				
				odcore::data::Container Propulsion::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x103 : 
				
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
				
					// addressing signal vehiclespeed.vehiclespeedpropshaft
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x103];
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
						uint64_t raw_vehiclespeed_vehiclespeedpropshaft=0x0;
						raw_vehiclespeed_vehiclespeedpropshaft=data;
				
						// reset right-hand side of bit field
						raw_vehiclespeed_vehiclespeedpropshaft=raw_vehiclespeed_vehiclespeedpropshaft >> (0);
				
						raw_vehiclespeed_vehiclespeedpropshaft=raw_vehiclespeed_vehiclespeedpropshaft & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_vehiclespeed_vehiclespeedpropshaft=static_cast<double>(raw_vehiclespeed_vehiclespeedpropshaft);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_vehiclespeed_vehiclespeedpropshaft=(transformed_vehiclespeed_vehiclespeedpropshaft*SCALE)+OFFSET;
						
						// clamping
						if(transformed_vehiclespeed_vehiclespeedpropshaft<0)
							transformed_vehiclespeed_vehiclespeedpropshaft=0;
						else if(transformed_vehiclespeed_vehiclespeedpropshaft>255.99609375)
							transformed_vehiclespeed_vehiclespeedpropshaft=255.99609375;
						
						m_vehiclespeedVehiclespeedpropshaft=transformed_vehiclespeed_vehiclespeedpropshaft;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_vehiclespeedVehiclespeedpropshaft);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("vehiclespeed.vehiclespeedpropshaft");
						f->setShortFieldName("vehiclespeedpropshaft");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_vehiclespeedVehiclespeedpropshaft));
				
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
				        	m_opendlv_proxy_reverefh16_Propulsion.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_Propulsion);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
