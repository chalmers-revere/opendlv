/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.SteeringRequest

/*
signals of interest:

steerrequest.enable_steerreq
steerrequest.steerreq_rwa
steerrequest.steerreq_deltatrq


CANID       : 0x201
FQDN        : steerrequest.enable_steerreq
startBit    : 32
length      : 1
lengthBytes : 
endian      : little
multiplyBy  : 1
add         : 0
rangeStart  : 0
rangeEnd    : 1

CANID       : 0x201
FQDN        : steerrequest.steerreq_rwa
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.000192
add         : -3.2
rangeStart  : -3.2
rangeEnd    : 3.2

CANID       : 0x201
FQDN        : steerrequest.steerreq_deltatrq
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.001
add         : -32
rangeStart  : -32
rangeEnd    : 33.535
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/SteeringRequest.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				SteeringRequest::SteeringRequest() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_steerrequestEnable_steerreq(0.0),
					m_steerrequestSteerreq_rwa(0.0),
					m_steerrequestSteerreq_deltatrq(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_SteeringRequest()
				{
					m_neededCanMessages.push_back(0x201);
				}
				
				SteeringRequest::SteeringRequest(double parameter0, double parameter1, double parameter2) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_steerrequestEnable_steerreq(parameter0),
					m_steerrequestSteerreq_rwa(parameter1),
					m_steerrequestSteerreq_deltatrq(parameter2),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_SteeringRequest()
				{
					m_neededCanMessages.push_back(0x201);
				}
				
				SteeringRequest::~SteeringRequest() {}
				
				const string SteeringRequest::toString() const {
					stringstream result;
					result << "Class : SteeringRequest"<<endl
							<<" steerrequest.enable_steerreq : "<< m_steerrequestEnable_steerreq<<endl
							<<" steerrequest.steerreq_rwa : "<< m_steerrequestSteerreq_rwa<<endl
							<<" steerrequest.steerreq_deltatrq : "<< m_steerrequestSteerreq_deltatrq<<endl;
					return result.str();
				}
				
				double SteeringRequest::getSteerrequestEnable_steerreq() const{
					return m_steerrequestEnable_steerreq;
				}
				
				void SteeringRequest::setSteerrequestEnable_steerreq(const double &steerrequestEnable_steerreq) {
					m_steerrequestEnable_steerreq=steerrequestEnable_steerreq;
				}
				
				double SteeringRequest::getSteerrequestSteerreq_rwa() const{
					return m_steerrequestSteerreq_rwa;
				}
				
				void SteeringRequest::setSteerrequestSteerreq_rwa(const double &steerrequestSteerreq_rwa) {
					m_steerrequestSteerreq_rwa=steerrequestSteerreq_rwa;
				}
				
				double SteeringRequest::getSteerrequestSteerreq_deltatrq() const{
					return m_steerrequestSteerreq_deltatrq;
				}
				
				void SteeringRequest::setSteerrequestSteerreq_deltatrq(const double &steerrequestSteerreq_deltatrq) {
					m_steerrequestSteerreq_deltatrq=steerrequestSteerreq_deltatrq;
				}
				
				
				ostream& SteeringRequest::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_steerrequestEnable_steerreq);
					s->write(static_cast<uint32_t>(2), m_steerrequestSteerreq_rwa);
					s->write(static_cast<uint32_t>(3), m_steerrequestSteerreq_deltatrq);
					return out;
				}
				
				istream& SteeringRequest::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_steerrequestEnable_steerreq);
					id=2;
					s->read(static_cast<uint32_t>(id), m_steerrequestSteerreq_rwa);
					id=3;
					s->read(static_cast<uint32_t>(id), m_steerrequestSteerreq_deltatrq);
					return in;
				}
				
				int32_t SteeringRequest::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string SteeringRequest::ShortName() {
					return "SteeringRequest";
				}
				
				const string SteeringRequest::LongName() {
					return "SteeringRequest";
				}
				
				int32_t SteeringRequest::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string SteeringRequest::getShortName() const {
					return SteeringRequest::ShortName();
				}
				
				const string SteeringRequest::getLongName() const {
					return SteeringRequest::LongName();
				}
				
				void SteeringRequest::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "steerrequest.enable_steerreq", "enable_steerreq", m_steerrequestEnable_steerreq);
					v.visit(static_cast<uint32_t>(2), 2, "steerrequest.steerreq_rwa", "steerreq_rwa", m_steerrequestSteerreq_rwa);
					v.visit(static_cast<uint32_t>(3), 3, "steerrequest.steerreq_deltatrq", "steerreq_deltatrq", m_steerrequestSteerreq_deltatrq);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage SteeringRequest::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x201;
				GCM_0x201.setIdentifier(0x201);
				uint64_t GCMPayload_0x201=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::SteeringRequest::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::SteeringRequest tempSteeringRequest=c.getData<::opendlv::proxy::reverefh16::SteeringRequest>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempSteeringRequest.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal steerrequest.enable_steerreq
					found=extracted=false;
					double rawSteerrequest_enable_steerreq = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawSteerrequest_enable_steerreq<0)
							rawSteerrequest_enable_steerreq=0;
						if(rawSteerrequest_enable_steerreq>1)
							rawSteerrequest_enable_steerreq=1;
						
						const double SCALE = 0 ;
						const double OFFSET = 1 ;
						
						double transformedSteerrequest_enable_steerreq=static_cast<double>((rawSteerrequest_enable_steerreq - SCALE) / (double) OFFSET);
				
						
						// signal length is 1 bit long and the value will be casted to uint8_t
						uint8_t finalSteerrequest_enable_steerreq=static_cast<uint8_t>(round(transformedSteerrequest_enable_steerreq)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLSteerrequest_enable_steerreq=0x0;
						tempPLSteerrequest_enable_steerreq=static_cast<uint64_t>(finalSteerrequest_enable_steerreq);
						tempPLSteerrequest_enable_steerreq=tempPLSteerrequest_enable_steerreq<<(32);
						GCMPayload_0x201=GCMPayload_0x201 | tempPLSteerrequest_enable_steerreq;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal steerrequest.steerreq_rwa
					found=extracted=false;
					double rawSteerrequest_steerreq_rwa = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawSteerrequest_steerreq_rwa<-3.2)
							rawSteerrequest_steerreq_rwa=-3.2;
						if(rawSteerrequest_steerreq_rwa>3.2)
							rawSteerrequest_steerreq_rwa=3.2;
						
						const double SCALE = -3.2 ;
						const double OFFSET = 0.000192 ;
						
						double transformedSteerrequest_steerreq_rwa=static_cast<double>((rawSteerrequest_steerreq_rwa - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalSteerrequest_steerreq_rwa=static_cast<uint16_t>(round(transformedSteerrequest_steerreq_rwa)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLSteerrequest_steerreq_rwa=0x0;
						tempPLSteerrequest_steerreq_rwa=static_cast<uint64_t>(finalSteerrequest_steerreq_rwa);
						tempPLSteerrequest_steerreq_rwa=tempPLSteerrequest_steerreq_rwa<<(16);
						GCMPayload_0x201=GCMPayload_0x201 | tempPLSteerrequest_steerreq_rwa;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal steerrequest.steerreq_deltatrq
					found=extracted=false;
					double rawSteerrequest_steerreq_deltatrq = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/3, found, extracted);
					
					if(found && extracted){
						if(rawSteerrequest_steerreq_deltatrq<-32)
							rawSteerrequest_steerreq_deltatrq=-32;
						if(rawSteerrequest_steerreq_deltatrq>33.535)
							rawSteerrequest_steerreq_deltatrq=33.535;
						
						const double SCALE = -32 ;
						const double OFFSET = 0.001 ;
						
						double transformedSteerrequest_steerreq_deltatrq=static_cast<double>((rawSteerrequest_steerreq_deltatrq - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalSteerrequest_steerreq_deltatrq=static_cast<uint16_t>(round(transformedSteerrequest_steerreq_deltatrq)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLSteerrequest_steerreq_deltatrq=0x0;
						tempPLSteerrequest_steerreq_deltatrq=static_cast<uint64_t>(finalSteerrequest_steerreq_deltatrq);
						tempPLSteerrequest_steerreq_deltatrq=tempPLSteerrequest_steerreq_deltatrq<<(0);
						GCMPayload_0x201=GCMPayload_0x201 | tempPLSteerrequest_steerreq_deltatrq;
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
					for(uint8_t i=0;i<static_cast<uint8_t>(5.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x201 & 0xFF ;
						GCMPayload_0x201=GCMPayload_0x201>>8;
					}
				
					GCM_0x201.setData(rolledPayload);
					GCM_0x201.setLength(static_cast<uint8_t>(5.0));
				}
				return GCM_0x201;
				}
				
				odcore::data::Container SteeringRequest::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x201 : 
				
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
				
					// addressing signal steerrequest.enable_steerreq
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x201];
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
						uint64_t raw_steerrequest_enable_steerreq=0x0;
						raw_steerrequest_enable_steerreq=data;
				
						// reset right-hand side of bit field
						raw_steerrequest_enable_steerreq=raw_steerrequest_enable_steerreq >> (32);
				
						raw_steerrequest_enable_steerreq=raw_steerrequest_enable_steerreq & 0b01;
				
						// Field too short for endianness adjustment, skipping this step.
				
						// variable holding the transformed value
						double transformed_steerrequest_enable_steerreq=static_cast<double>(raw_steerrequest_enable_steerreq);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 1;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_steerrequest_enable_steerreq=(transformed_steerrequest_enable_steerreq*SCALE)+OFFSET;
						
						// clamping
						if(transformed_steerrequest_enable_steerreq<0)
							transformed_steerrequest_enable_steerreq=0;
						else if(transformed_steerrequest_enable_steerreq>1)
							transformed_steerrequest_enable_steerreq=1;
						
						m_steerrequestEnable_steerreq=transformed_steerrequest_enable_steerreq;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_steerrequestEnable_steerreq);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("steerrequest.enable_steerreq");
						f->setShortFieldName("enable_steerreq");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_steerrequestEnable_steerreq));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal steerrequest.steerreq_rwa
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x201];
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
						uint64_t raw_steerrequest_steerreq_rwa=0x0;
						raw_steerrequest_steerreq_rwa=data;
				
						// reset right-hand side of bit field
						raw_steerrequest_steerreq_rwa=raw_steerrequest_steerreq_rwa >> (16);
				
						raw_steerrequest_steerreq_rwa=raw_steerrequest_steerreq_rwa & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_steerrequest_steerreq_rwa=static_cast<double>(raw_steerrequest_steerreq_rwa);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.000192;
						const double OFFSET = -3.2;
						
						// scaling and adding
						transformed_steerrequest_steerreq_rwa=(transformed_steerrequest_steerreq_rwa*SCALE)+OFFSET;
						
						// clamping
						if(transformed_steerrequest_steerreq_rwa<-3.2)
							transformed_steerrequest_steerreq_rwa=-3.2;
						else if(transformed_steerrequest_steerreq_rwa>3.2)
							transformed_steerrequest_steerreq_rwa=3.2;
						
						m_steerrequestSteerreq_rwa=transformed_steerrequest_steerreq_rwa;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_steerrequestSteerreq_rwa);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("steerrequest.steerreq_rwa");
						f->setShortFieldName("steerreq_rwa");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_steerrequestSteerreq_rwa));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal steerrequest.steerreq_deltatrq
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x201];
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
						uint64_t raw_steerrequest_steerreq_deltatrq=0x0;
						raw_steerrequest_steerreq_deltatrq=data;
				
						// reset right-hand side of bit field
						raw_steerrequest_steerreq_deltatrq=raw_steerrequest_steerreq_deltatrq >> (0);
				
						raw_steerrequest_steerreq_deltatrq=raw_steerrequest_steerreq_deltatrq & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_steerrequest_steerreq_deltatrq=static_cast<double>(raw_steerrequest_steerreq_deltatrq);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.001;
						const double OFFSET = -32;
						
						// scaling and adding
						transformed_steerrequest_steerreq_deltatrq=(transformed_steerrequest_steerreq_deltatrq*SCALE)+OFFSET;
						
						// clamping
						if(transformed_steerrequest_steerreq_deltatrq<-32)
							transformed_steerrequest_steerreq_deltatrq=-32;
						else if(transformed_steerrequest_steerreq_deltatrq>33.535)
							transformed_steerrequest_steerreq_deltatrq=33.535;
						
						m_steerrequestSteerreq_deltatrq=transformed_steerrequest_steerreq_deltatrq;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_steerrequestSteerreq_deltatrq);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(3); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("steerrequest.steerreq_deltatrq");
						f->setShortFieldName("steerreq_deltatrq");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_steerrequestSteerreq_deltatrq));
				
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
				        	m_opendlv_proxy_reverefh16_SteeringRequest.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_SteeringRequest);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
