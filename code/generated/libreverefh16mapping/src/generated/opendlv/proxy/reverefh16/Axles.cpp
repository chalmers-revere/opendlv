/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.Axles

/*
signals of interest:

axleloads.frontaxle1load
axleloads.driveaxle1load
axleloads.driveaxle2load


CANID       : 0x105
FQDN        : axleloads.frontaxle1load
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.5
add         : 0
rangeStart  : 0
rangeEnd    : 32767.5

CANID       : 0x105
FQDN        : axleloads.driveaxle1load
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.5
add         : 0
rangeStart  : 0
rangeEnd    : 32767.5

CANID       : 0x105
FQDN        : axleloads.driveaxle2load
startBit    : 32
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.5
add         : 0
rangeStart  : 0
rangeEnd    : 32767.5
*/

#include <memory>
#include <iostream>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/MessageFromVisitableVisitor.h>
#include <opendavinci/odcore/base/SerializationFactory.h>
#include <opendavinci/odcore/base/Serializer.h>
#include <opendavinci/odcore/base/Deserializer.h>

#include "generated/opendlv/proxy/reverefh16/Axles.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				Axles::Axles() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_axleloadsFrontaxle1load(0.0),
					m_axleloadsDriveaxle1load(0.0),
					m_axleloadsDriveaxle2load(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Axles()
				{
					m_neededCanMessages.push_back(0x105);
				}
				
				Axles::Axles(double parameter0, double parameter1, double parameter2) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_axleloadsFrontaxle1load(parameter0),
					m_axleloadsDriveaxle1load(parameter1),
					m_axleloadsDriveaxle2load(parameter2),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Axles()
				{
					m_neededCanMessages.push_back(0x105);
				}
				
				Axles::~Axles() {}
				
				const string Axles::toString() const {
					stringstream result;
					result << "Class : Axles"<<endl
							<<" axleloads.frontaxle1load : "<< m_axleloadsFrontaxle1load<<endl
							<<" axleloads.driveaxle1load : "<< m_axleloadsDriveaxle1load<<endl
							<<" axleloads.driveaxle2load : "<< m_axleloadsDriveaxle2load<<endl;
					return result.str();
				}
				
				double Axles::getAxleloadsFrontaxle1load() const{
					return m_axleloadsFrontaxle1load;
				}
				
				void Axles::setAxleloadsFrontaxle1load(const double &axleloadsFrontaxle1load) {
					m_axleloadsFrontaxle1load=axleloadsFrontaxle1load;
				}
				
				double Axles::getAxleloadsDriveaxle1load() const{
					return m_axleloadsDriveaxle1load;
				}
				
				void Axles::setAxleloadsDriveaxle1load(const double &axleloadsDriveaxle1load) {
					m_axleloadsDriveaxle1load=axleloadsDriveaxle1load;
				}
				
				double Axles::getAxleloadsDriveaxle2load() const{
					return m_axleloadsDriveaxle2load;
				}
				
				void Axles::setAxleloadsDriveaxle2load(const double &axleloadsDriveaxle2load) {
					m_axleloadsDriveaxle2load=axleloadsDriveaxle2load;
				}
				
				
				ostream& Axles::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_axleloadsFrontaxle1load);
					s->write(static_cast<uint32_t>(2), m_axleloadsDriveaxle1load);
					s->write(static_cast<uint32_t>(3), m_axleloadsDriveaxle2load);
					return out;
				}
				
				istream& Axles::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_axleloadsFrontaxle1load);
					id=2;
					s->read(static_cast<uint32_t>(id), m_axleloadsDriveaxle1load);
					id=3;
					s->read(static_cast<uint32_t>(id), m_axleloadsDriveaxle2load);
					return in;
				}
				
				int32_t Axles::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Axles::ShortName() {
					return "Axles";
				}
				
				const string Axles::LongName() {
					return "Axles";
				}
				
				int32_t Axles::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Axles::getShortName() const {
					return Axles::ShortName();
				}
				
				const string Axles::getLongName() const {
					return Axles::LongName();
				}
				
				void Axles::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "axleloads.frontaxle1load", "frontaxle1load", m_axleloadsFrontaxle1load);
					v.visit(static_cast<uint32_t>(2), 2, "axleloads.driveaxle1load", "driveaxle1load", m_axleloadsDriveaxle1load);
					v.visit(static_cast<uint32_t>(3), 3, "axleloads.driveaxle2load", "driveaxle2load", m_axleloadsDriveaxle2load);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage Axles::encode(odcore::data::Container &c) {
				
				
				::automotive::GenericCANMessage GCM_0x105;
				GCM_0x105.setIdentifier(0x105);
				uint64_t GCMPayload_0x105=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::Axles::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::Axles tempAxles=c.getData<::opendlv::proxy::reverefh16::Axles>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempAxles.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal axleloads.frontaxle1load
					found=extracted=false;
					double rawAxleloads_frontaxle1load = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawAxleloads_frontaxle1load<0)
							rawAxleloads_frontaxle1load=0;
						if(rawAxleloads_frontaxle1load>32767.5)
							rawAxleloads_frontaxle1load=32767.5;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.5 ;
						
						double transformedAxleloads_frontaxle1load=static_cast<double>((rawAxleloads_frontaxle1load - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalAxleloads_frontaxle1load=static_cast<uint16_t>(round(transformedAxleloads_frontaxle1load)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLAxleloads_frontaxle1load=0x0;
						tempPLAxleloads_frontaxle1load=static_cast<uint64_t>(finalAxleloads_frontaxle1load);
						tempPLAxleloads_frontaxle1load=tempPLAxleloads_frontaxle1load<<(0);
						GCMPayload_0x105=GCMPayload_0x105 | tempPLAxleloads_frontaxle1load;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal axleloads.driveaxle1load
					found=extracted=false;
					double rawAxleloads_driveaxle1load = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawAxleloads_driveaxle1load<0)
							rawAxleloads_driveaxle1load=0;
						if(rawAxleloads_driveaxle1load>32767.5)
							rawAxleloads_driveaxle1load=32767.5;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.5 ;
						
						double transformedAxleloads_driveaxle1load=static_cast<double>((rawAxleloads_driveaxle1load - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalAxleloads_driveaxle1load=static_cast<uint16_t>(round(transformedAxleloads_driveaxle1load)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLAxleloads_driveaxle1load=0x0;
						tempPLAxleloads_driveaxle1load=static_cast<uint64_t>(finalAxleloads_driveaxle1load);
						tempPLAxleloads_driveaxle1load=tempPLAxleloads_driveaxle1load<<(16);
						GCMPayload_0x105=GCMPayload_0x105 | tempPLAxleloads_driveaxle1load;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal axleloads.driveaxle2load
					found=extracted=false;
					double rawAxleloads_driveaxle2load = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/3, found, extracted);
					
					if(found && extracted){
						if(rawAxleloads_driveaxle2load<0)
							rawAxleloads_driveaxle2load=0;
						if(rawAxleloads_driveaxle2load>32767.5)
							rawAxleloads_driveaxle2load=32767.5;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.5 ;
						
						double transformedAxleloads_driveaxle2load=static_cast<double>((rawAxleloads_driveaxle2load - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalAxleloads_driveaxle2load=static_cast<uint16_t>(round(transformedAxleloads_driveaxle2load)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLAxleloads_driveaxle2load=0x0;
						tempPLAxleloads_driveaxle2load=static_cast<uint64_t>(finalAxleloads_driveaxle2load);
						tempPLAxleloads_driveaxle2load=tempPLAxleloads_driveaxle2load<<(32);
						GCMPayload_0x105=GCMPayload_0x105 | tempPLAxleloads_driveaxle2load;
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
						rolledPayload |= GCMPayload_0x105 & 0xFF ;
						GCMPayload_0x105=GCMPayload_0x105>>8;
					}
				
					GCM_0x105.setData(rolledPayload);
					GCM_0x105.setLength(static_cast<uint8_t>(6.0));
				}
				return GCM_0x105;
				}
				
				odcore::data::Container Axles::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x105 : 
				
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
				
					// addressing signal axleloads.frontaxle1load
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x105];
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
						uint64_t raw_axleloads_frontaxle1load=0x0;
						raw_axleloads_frontaxle1load=data;
				
						// reset right-hand side of bit field
						raw_axleloads_frontaxle1load=raw_axleloads_frontaxle1load >> (0);
				
						raw_axleloads_frontaxle1load=raw_axleloads_frontaxle1load & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_axleloads_frontaxle1load=static_cast<double>(raw_axleloads_frontaxle1load);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.5;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_axleloads_frontaxle1load=(transformed_axleloads_frontaxle1load*SCALE)+OFFSET;
						
						// clamping
						if(transformed_axleloads_frontaxle1load<0)
							transformed_axleloads_frontaxle1load=0;
						else if(transformed_axleloads_frontaxle1load>32767.5)
							transformed_axleloads_frontaxle1load=32767.5;
						
						m_axleloadsFrontaxle1load=transformed_axleloads_frontaxle1load;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_axleloadsFrontaxle1load);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("axleloads.frontaxle1load");
						f->setShortFieldName("frontaxle1load");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_axleloadsFrontaxle1load));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal axleloads.driveaxle1load
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x105];
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
						uint64_t raw_axleloads_driveaxle1load=0x0;
						raw_axleloads_driveaxle1load=data;
				
						// reset right-hand side of bit field
						raw_axleloads_driveaxle1load=raw_axleloads_driveaxle1load >> (16);
				
						raw_axleloads_driveaxle1load=raw_axleloads_driveaxle1load & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_axleloads_driveaxle1load=static_cast<double>(raw_axleloads_driveaxle1load);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.5;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_axleloads_driveaxle1load=(transformed_axleloads_driveaxle1load*SCALE)+OFFSET;
						
						// clamping
						if(transformed_axleloads_driveaxle1load<0)
							transformed_axleloads_driveaxle1load=0;
						else if(transformed_axleloads_driveaxle1load>32767.5)
							transformed_axleloads_driveaxle1load=32767.5;
						
						m_axleloadsDriveaxle1load=transformed_axleloads_driveaxle1load;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_axleloadsDriveaxle1load);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("axleloads.driveaxle1load");
						f->setShortFieldName("driveaxle1load");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_axleloadsDriveaxle1load));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal axleloads.driveaxle2load
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x105];
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
						uint64_t raw_axleloads_driveaxle2load=0x0;
						raw_axleloads_driveaxle2load=data;
				
						// reset right-hand side of bit field
						raw_axleloads_driveaxle2load=raw_axleloads_driveaxle2load >> (32);
				
						raw_axleloads_driveaxle2load=raw_axleloads_driveaxle2load & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_axleloads_driveaxle2load=static_cast<double>(raw_axleloads_driveaxle2load);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.5;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_axleloads_driveaxle2load=(transformed_axleloads_driveaxle2load*SCALE)+OFFSET;
						
						// clamping
						if(transformed_axleloads_driveaxle2load<0)
							transformed_axleloads_driveaxle2load=0;
						else if(transformed_axleloads_driveaxle2load>32767.5)
							transformed_axleloads_driveaxle2load=32767.5;
						
						m_axleloadsDriveaxle2load=transformed_axleloads_driveaxle2load;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_axleloadsDriveaxle2load);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(3); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("axleloads.driveaxle2load");
						f->setShortFieldName("driveaxle2load");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_axleloadsDriveaxle2load));
				
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
				        	m_opendlv_proxy_reverefh16_Axles.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_Axles);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
