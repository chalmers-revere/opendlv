/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.AccelerationRequest

/*
signals of interest:

accelerationrequest.enable_accrequest
accelerationrequest.accelerationrequest


CANID       : 0x203
FQDN        : accelerationrequest.enable_accrequest
startBit    : 16
length      : 1
lengthBytes : 
endian      : little
multiplyBy  : 1
add         : 0
rangeStart  : 0
rangeEnd    : 1

CANID       : 0x203
FQDN        : accelerationrequest.accelerationrequest
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00048828125
add         : -15.687
rangeStart  : -15.687
rangeEnd    : 16.31251171875
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/AccelerationRequest.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				AccelerationRequest::AccelerationRequest() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_accelerationrequestEnable_accrequest(0.0),
					m_accelerationrequestAccelerationrequest(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_AccelerationRequest()
				{
					m_neededCanMessages.push_back(0x203);
				}
				
				AccelerationRequest::AccelerationRequest(double parameter0, double parameter1) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_accelerationrequestEnable_accrequest(parameter0),
					m_accelerationrequestAccelerationrequest(parameter1),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_AccelerationRequest()
				{
					m_neededCanMessages.push_back(0x203);
				}
				
				AccelerationRequest::~AccelerationRequest() {}
				
				const string AccelerationRequest::toString() const {
					stringstream result;
					result << "Class : AccelerationRequest"<<endl
							<<" accelerationrequest.enable_accrequest : "<< m_accelerationrequestEnable_accrequest<<endl
							<<" accelerationrequest.accelerationrequest : "<< m_accelerationrequestAccelerationrequest<<endl;
					return result.str();
				}
				
				double AccelerationRequest::getAccelerationrequestEnable_accrequest() const{
					return m_accelerationrequestEnable_accrequest;
				}
				
				void AccelerationRequest::setAccelerationrequestEnable_accrequest(const double &accelerationrequestEnable_accrequest) {
					m_accelerationrequestEnable_accrequest=accelerationrequestEnable_accrequest;
				}
				
				double AccelerationRequest::getAccelerationrequestAccelerationrequest() const{
					return m_accelerationrequestAccelerationrequest;
				}
				
				void AccelerationRequest::setAccelerationrequestAccelerationrequest(const double &accelerationrequestAccelerationrequest) {
					m_accelerationrequestAccelerationrequest=accelerationrequestAccelerationrequest;
				}
				
				
				ostream& AccelerationRequest::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_accelerationrequestEnable_accrequest);
					s->write(static_cast<uint32_t>(2), m_accelerationrequestAccelerationrequest);
					return out;
				}
				
				istream& AccelerationRequest::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_accelerationrequestEnable_accrequest);
					id=2;
					s->read(static_cast<uint32_t>(id), m_accelerationrequestAccelerationrequest);
					return in;
				}
				
				int32_t AccelerationRequest::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string AccelerationRequest::ShortName() {
					return "AccelerationRequest";
				}
				
				const string AccelerationRequest::LongName() {
					return "AccelerationRequest";
				}
				
				int32_t AccelerationRequest::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string AccelerationRequest::getShortName() const {
					return AccelerationRequest::ShortName();
				}
				
				const string AccelerationRequest::getLongName() const {
					return AccelerationRequest::LongName();
				}
				
				void AccelerationRequest::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "accelerationrequest.enable_accrequest", "enable_accrequest", m_accelerationrequestEnable_accrequest);
					v.visit(static_cast<uint32_t>(2), 2, "accelerationrequest.accelerationrequest", "accelerationrequest", m_accelerationrequestAccelerationrequest);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage AccelerationRequest::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x203;
				GCM_0x203.setIdentifier(0x203);
				uint64_t GCMPayload_0x203=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::AccelerationRequest::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::AccelerationRequest tempAccelerationRequest=c.getData<::opendlv::proxy::reverefh16::AccelerationRequest>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempAccelerationRequest.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal accelerationrequest.enable_accrequest
					found=extracted=false;
					double rawAccelerationrequest_enable_accrequest = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawAccelerationrequest_enable_accrequest<0)
							rawAccelerationrequest_enable_accrequest=0;
						if(rawAccelerationrequest_enable_accrequest>1)
							rawAccelerationrequest_enable_accrequest=1;
						
						const double SCALE = 0 ;
						const double OFFSET = 1 ;
						
						double transformedAccelerationrequest_enable_accrequest=static_cast<double>((rawAccelerationrequest_enable_accrequest - SCALE) / (double) OFFSET);
				
						
						// signal length is 1 bit long and the value will be casted to uint8_t
						uint8_t finalAccelerationrequest_enable_accrequest=static_cast<uint8_t>(round(transformedAccelerationrequest_enable_accrequest)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLAccelerationrequest_enable_accrequest=0x0;
						tempPLAccelerationrequest_enable_accrequest=static_cast<uint64_t>(finalAccelerationrequest_enable_accrequest);
						tempPLAccelerationrequest_enable_accrequest=tempPLAccelerationrequest_enable_accrequest<<(16);
						GCMPayload_0x203=GCMPayload_0x203 | tempPLAccelerationrequest_enable_accrequest;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal accelerationrequest.accelerationrequest
					found=extracted=false;
					double rawAccelerationrequest_accelerationrequest = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawAccelerationrequest_accelerationrequest<-15.687)
							rawAccelerationrequest_accelerationrequest=-15.687;
						if(rawAccelerationrequest_accelerationrequest>16.31251171875)
							rawAccelerationrequest_accelerationrequest=16.31251171875;
						
						const double SCALE = -15.687 ;
						const double OFFSET = 0.00048828125 ;
						
						double transformedAccelerationrequest_accelerationrequest=static_cast<double>((rawAccelerationrequest_accelerationrequest - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalAccelerationrequest_accelerationrequest=static_cast<uint16_t>(round(transformedAccelerationrequest_accelerationrequest)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLAccelerationrequest_accelerationrequest=0x0;
						tempPLAccelerationrequest_accelerationrequest=static_cast<uint64_t>(finalAccelerationrequest_accelerationrequest);
						tempPLAccelerationrequest_accelerationrequest=tempPLAccelerationrequest_accelerationrequest<<(0);
						GCMPayload_0x203=GCMPayload_0x203 | tempPLAccelerationrequest_accelerationrequest;
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
					for(uint8_t i=0;i<static_cast<uint8_t>(3.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x203 & 0xFF ;
						GCMPayload_0x203=GCMPayload_0x203>>8;
					}
				
					GCM_0x203.setData(rolledPayload);
					GCM_0x203.setLength(static_cast<uint8_t>(3.0));
				}
				return GCM_0x203;
				}
				
				odcore::data::Container AccelerationRequest::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x203 : 
				
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
				
					// addressing signal accelerationrequest.enable_accrequest
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x203];
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
						uint64_t raw_accelerationrequest_enable_accrequest=0x0;
						raw_accelerationrequest_enable_accrequest=data;
				
						// reset right-hand side of bit field
						raw_accelerationrequest_enable_accrequest=raw_accelerationrequest_enable_accrequest >> (16);
				
						raw_accelerationrequest_enable_accrequest=raw_accelerationrequest_enable_accrequest & 0b01;
				
						// Field too short for endianness adjustment, skipping this step.
				
						// variable holding the transformed value
						double transformed_accelerationrequest_enable_accrequest=static_cast<double>(raw_accelerationrequest_enable_accrequest);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 1;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_accelerationrequest_enable_accrequest=(transformed_accelerationrequest_enable_accrequest*SCALE)+OFFSET;
						
						// clamping
						if(transformed_accelerationrequest_enable_accrequest<0)
							transformed_accelerationrequest_enable_accrequest=0;
						else if(transformed_accelerationrequest_enable_accrequest>1)
							transformed_accelerationrequest_enable_accrequest=1;
						
						m_accelerationrequestEnable_accrequest=transformed_accelerationrequest_enable_accrequest;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_accelerationrequestEnable_accrequest);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("accelerationrequest.enable_accrequest");
						f->setShortFieldName("enable_accrequest");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_accelerationrequestEnable_accrequest));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal accelerationrequest.accelerationrequest
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x203];
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
						uint64_t raw_accelerationrequest_accelerationrequest=0x0;
						raw_accelerationrequest_accelerationrequest=data;
				
						// reset right-hand side of bit field
						raw_accelerationrequest_accelerationrequest=raw_accelerationrequest_accelerationrequest >> (0);
				
						raw_accelerationrequest_accelerationrequest=raw_accelerationrequest_accelerationrequest & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_accelerationrequest_accelerationrequest=static_cast<double>(raw_accelerationrequest_accelerationrequest);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00048828125;
						const double OFFSET = -15.687;
						
						// scaling and adding
						transformed_accelerationrequest_accelerationrequest=(transformed_accelerationrequest_accelerationrequest*SCALE)+OFFSET;
						
						// clamping
						if(transformed_accelerationrequest_accelerationrequest<-15.687)
							transformed_accelerationrequest_accelerationrequest=-15.687;
						else if(transformed_accelerationrequest_accelerationrequest>16.31251171875)
							transformed_accelerationrequest_accelerationrequest=16.31251171875;
						
						m_accelerationrequestAccelerationrequest=transformed_accelerationrequest_accelerationrequest;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_accelerationrequestAccelerationrequest);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("accelerationrequest.accelerationrequest");
						f->setShortFieldName("accelerationrequest");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_accelerationrequestAccelerationrequest));
				
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
				        	m_opendlv_proxy_reverefh16_AccelerationRequest.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_AccelerationRequest);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
