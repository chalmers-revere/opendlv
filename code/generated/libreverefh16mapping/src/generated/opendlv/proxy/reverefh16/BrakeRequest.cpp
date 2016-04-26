/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.BrakeRequest

/*
signals of interest:

brakerequest.enable_brakerequest
brakerequest.brakerequest


CANID       : 0x202
FQDN        : brakerequest.enable_brakerequest
startBit    : 16
length      : 1
lengthBytes : 
endian      : little
multiplyBy  : 1
add         : 0
rangeStart  : 0
rangeEnd    : 1

CANID       : 0x202
FQDN        : brakerequest.brakerequest
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

#include "generated/opendlv/proxy/reverefh16/BrakeRequest.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				BrakeRequest::BrakeRequest() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_brakerequestEnable_brakerequest(0.0),
					m_brakerequestBrakerequest(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_BrakeRequest()
				{
					m_neededCanMessages.push_back(0x202);
				}
				
				BrakeRequest::BrakeRequest(double parameter0, double parameter1) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_brakerequestEnable_brakerequest(parameter0),
					m_brakerequestBrakerequest(parameter1),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_BrakeRequest()
				{
					m_neededCanMessages.push_back(0x202);
				}
				
				BrakeRequest::~BrakeRequest() {}
				
				const string BrakeRequest::toString() const {
					stringstream result;
					result << "Class : BrakeRequest"<<endl
							<<" brakerequest.enable_brakerequest : "<< m_brakerequestEnable_brakerequest<<endl
							<<" brakerequest.brakerequest : "<< m_brakerequestBrakerequest<<endl;
					return result.str();
				}
				
				double BrakeRequest::getBrakerequestEnable_brakerequest() const{
					return m_brakerequestEnable_brakerequest;
				}
				
				void BrakeRequest::setBrakerequestEnable_brakerequest(const double &brakerequestEnable_brakerequest) {
					m_brakerequestEnable_brakerequest=brakerequestEnable_brakerequest;
				}
				
				double BrakeRequest::getBrakerequestBrakerequest() const{
					return m_brakerequestBrakerequest;
				}
				
				void BrakeRequest::setBrakerequestBrakerequest(const double &brakerequestBrakerequest) {
					m_brakerequestBrakerequest=brakerequestBrakerequest;
				}
				
				
				ostream& BrakeRequest::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_brakerequestEnable_brakerequest);
					s->write(static_cast<uint32_t>(2), m_brakerequestBrakerequest);
					return out;
				}
				
				istream& BrakeRequest::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_brakerequestEnable_brakerequest);
					id=2;
					s->read(static_cast<uint32_t>(id), m_brakerequestBrakerequest);
					return in;
				}
				
				int32_t BrakeRequest::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string BrakeRequest::ShortName() {
					return "BrakeRequest";
				}
				
				const string BrakeRequest::LongName() {
					return "BrakeRequest";
				}
				
				int32_t BrakeRequest::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string BrakeRequest::getShortName() const {
					return BrakeRequest::ShortName();
				}
				
				const string BrakeRequest::getLongName() const {
					return BrakeRequest::LongName();
				}
				
				void BrakeRequest::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "brakerequest.enable_brakerequest", "enable_brakerequest", m_brakerequestEnable_brakerequest);
					v.visit(static_cast<uint32_t>(2), 2, "brakerequest.brakerequest", "brakerequest", m_brakerequestBrakerequest);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage BrakeRequest::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x202;
				GCM_0x202.setIdentifier(0x202);
				uint64_t GCMPayload_0x202=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::BrakeRequest::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::BrakeRequest tempBrakeRequest=c.getData<::opendlv::proxy::reverefh16::BrakeRequest>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempBrakeRequest.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal brakerequest.enable_brakerequest
					found=extracted=false;
					double rawBrakerequest_enable_brakerequest = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawBrakerequest_enable_brakerequest<0)
							rawBrakerequest_enable_brakerequest=0;
						if(rawBrakerequest_enable_brakerequest>1)
							rawBrakerequest_enable_brakerequest=1;
						
						const double SCALE = 0 ;
						const double OFFSET = 1 ;
						
						double transformedBrakerequest_enable_brakerequest=static_cast<double>((rawBrakerequest_enable_brakerequest - SCALE) / (double) OFFSET);
				
						
						// signal length is 1 bit long and the value will be casted to uint8_t
						uint8_t finalBrakerequest_enable_brakerequest=static_cast<uint8_t>(round(transformedBrakerequest_enable_brakerequest)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLBrakerequest_enable_brakerequest=0x0;
						tempPLBrakerequest_enable_brakerequest=static_cast<uint64_t>(finalBrakerequest_enable_brakerequest);
						tempPLBrakerequest_enable_brakerequest=tempPLBrakerequest_enable_brakerequest<<(16);
						GCMPayload_0x202=GCMPayload_0x202 | tempPLBrakerequest_enable_brakerequest;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal brakerequest.brakerequest
					found=extracted=false;
					double rawBrakerequest_brakerequest = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawBrakerequest_brakerequest<-15.687)
							rawBrakerequest_brakerequest=-15.687;
						if(rawBrakerequest_brakerequest>16.31251171875)
							rawBrakerequest_brakerequest=16.31251171875;
						
						const double SCALE = -15.687 ;
						const double OFFSET = 0.00048828125 ;
						
						double transformedBrakerequest_brakerequest=static_cast<double>((rawBrakerequest_brakerequest - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalBrakerequest_brakerequest=static_cast<uint16_t>(round(transformedBrakerequest_brakerequest)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLBrakerequest_brakerequest=0x0;
						tempPLBrakerequest_brakerequest=static_cast<uint64_t>(finalBrakerequest_brakerequest);
						tempPLBrakerequest_brakerequest=tempPLBrakerequest_brakerequest<<(0);
						GCMPayload_0x202=GCMPayload_0x202 | tempPLBrakerequest_brakerequest;
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
						rolledPayload |= GCMPayload_0x202 & 0xFF ;
						GCMPayload_0x202=GCMPayload_0x202>>8;
					}
				
					GCM_0x202.setData(rolledPayload);
					GCM_0x202.setLength(static_cast<uint8_t>(3.0));
				}
				return GCM_0x202;
				}
				
				odcore::data::Container BrakeRequest::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x202 : 
				
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
				
					// addressing signal brakerequest.enable_brakerequest
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x202];
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
						uint64_t raw_brakerequest_enable_brakerequest=0x0;
						raw_brakerequest_enable_brakerequest=data;
				
						// reset right-hand side of bit field
						raw_brakerequest_enable_brakerequest=raw_brakerequest_enable_brakerequest >> (16);
				
						raw_brakerequest_enable_brakerequest=raw_brakerequest_enable_brakerequest & 0b01;
				
						// Field too short for endianness adjustment, skipping this step.
				
						// variable holding the transformed value
						double transformed_brakerequest_enable_brakerequest=static_cast<double>(raw_brakerequest_enable_brakerequest);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 1;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_brakerequest_enable_brakerequest=(transformed_brakerequest_enable_brakerequest*SCALE)+OFFSET;
						
						// clamping
						if(transformed_brakerequest_enable_brakerequest<0)
							transformed_brakerequest_enable_brakerequest=0;
						else if(transformed_brakerequest_enable_brakerequest>1)
							transformed_brakerequest_enable_brakerequest=1;
						
						m_brakerequestEnable_brakerequest=transformed_brakerequest_enable_brakerequest;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_brakerequestEnable_brakerequest);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("brakerequest.enable_brakerequest");
						f->setShortFieldName("enable_brakerequest");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_brakerequestEnable_brakerequest));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal brakerequest.brakerequest
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x202];
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
						uint64_t raw_brakerequest_brakerequest=0x0;
						raw_brakerequest_brakerequest=data;
				
						// reset right-hand side of bit field
						raw_brakerequest_brakerequest=raw_brakerequest_brakerequest >> (0);
				
						raw_brakerequest_brakerequest=raw_brakerequest_brakerequest & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_brakerequest_brakerequest=static_cast<double>(raw_brakerequest_brakerequest);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00048828125;
						const double OFFSET = -15.687;
						
						// scaling and adding
						transformed_brakerequest_brakerequest=(transformed_brakerequest_brakerequest*SCALE)+OFFSET;
						
						// clamping
						if(transformed_brakerequest_brakerequest<-15.687)
							transformed_brakerequest_brakerequest=-15.687;
						else if(transformed_brakerequest_brakerequest>16.31251171875)
							transformed_brakerequest_brakerequest=16.31251171875;
						
						m_brakerequestBrakerequest=transformed_brakerequest_brakerequest;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_brakerequestBrakerequest);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("brakerequest.brakerequest");
						f->setShortFieldName("brakerequest");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_brakerequestBrakerequest));
				
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
				        	m_opendlv_proxy_reverefh16_BrakeRequest.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_BrakeRequest);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
