/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Source file for: opendlv.proxy.reverefh16.Wheels

/*
signals of interest:

wheelspeeds1.frontaxle1wheelspeedleft
wheelspeeds1.frontaxle1wheelspeedright
wheelspeeds1.driveaxle1wheelspeedleft
wheelspeeds1.driveaxle1wheelspeedright
wheelspeeds2.driveaxle2wheelspeedleft
wheelspeeds2.driveaxle2wheelspeedright


CANID       : 0x101
FQDN        : wheelspeeds1.frontaxle1wheelspeedleft
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375

CANID       : 0x101
FQDN        : wheelspeeds1.frontaxle1wheelspeedright
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375

CANID       : 0x101
FQDN        : wheelspeeds1.driveaxle1wheelspeedleft
startBit    : 32
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375

CANID       : 0x101
FQDN        : wheelspeeds1.driveaxle1wheelspeedright
startBit    : 48
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375

CANID       : 0x102
FQDN        : wheelspeeds2.driveaxle2wheelspeedleft
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375

CANID       : 0x102
FQDN        : wheelspeeds2.driveaxle2wheelspeedright
startBit    : 16
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

#include "generated/opendlv/proxy/reverefh16/Wheels.h"

namespace canmapping {

	namespace opendlv {
		namespace proxy {
			namespace reverefh16 {
				
				using namespace std;
				
				
				Wheels::Wheels() :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_wheelspeeds1Frontaxle1wheelspeedleft(0.0),
					m_wheelspeeds1Frontaxle1wheelspeedright(0.0),
					m_wheelspeeds1Driveaxle1wheelspeedleft(0.0),
					m_wheelspeeds1Driveaxle1wheelspeedright(0.0),
					m_wheelspeeds2Driveaxle2wheelspeedleft(0.0),
					m_wheelspeeds2Driveaxle2wheelspeedright(0.0),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Wheels()
				{
					m_neededCanMessages.push_back(0x101);
					m_neededCanMessages.push_back(0x102);
				}
				
				Wheels::Wheels(double parameter0, double parameter1, double parameter2, double parameter3, double parameter4, double parameter5) :
					odcore::data::SerializableData(),
					odcore::base::Visitable(),
					m_wheelspeeds1Frontaxle1wheelspeedleft(parameter0),
					m_wheelspeeds1Frontaxle1wheelspeedright(parameter1),
					m_wheelspeeds1Driveaxle1wheelspeedleft(parameter2),
					m_wheelspeeds1Driveaxle1wheelspeedright(parameter3),
					m_wheelspeeds2Driveaxle2wheelspeedleft(parameter4),
					m_wheelspeeds2Driveaxle2wheelspeedright(parameter5),
					m_payloads(),
					m_neededCanMessages(),
					m_index(0),
					m_opendlv_proxy_reverefh16_Wheels()
				{
					m_neededCanMessages.push_back(0x101);
					m_neededCanMessages.push_back(0x102);
				}
				
				Wheels::~Wheels() {}
				
				const string Wheels::toString() const {
					stringstream result;
					result << "Class : Wheels"<<endl
							<<" wheelspeeds1.frontaxle1wheelspeedleft : "<< m_wheelspeeds1Frontaxle1wheelspeedleft<<endl
							<<" wheelspeeds1.frontaxle1wheelspeedright : "<< m_wheelspeeds1Frontaxle1wheelspeedright<<endl
							<<" wheelspeeds1.driveaxle1wheelspeedleft : "<< m_wheelspeeds1Driveaxle1wheelspeedleft<<endl
							<<" wheelspeeds1.driveaxle1wheelspeedright : "<< m_wheelspeeds1Driveaxle1wheelspeedright<<endl
							<<" wheelspeeds2.driveaxle2wheelspeedleft : "<< m_wheelspeeds2Driveaxle2wheelspeedleft<<endl
							<<" wheelspeeds2.driveaxle2wheelspeedright : "<< m_wheelspeeds2Driveaxle2wheelspeedright<<endl;
					return result.str();
				}
				
				double Wheels::getWheelspeeds1Frontaxle1wheelspeedleft() const{
					return m_wheelspeeds1Frontaxle1wheelspeedleft;
				}
				
				void Wheels::setWheelspeeds1Frontaxle1wheelspeedleft(const double &wheelspeeds1Frontaxle1wheelspeedleft) {
					m_wheelspeeds1Frontaxle1wheelspeedleft=wheelspeeds1Frontaxle1wheelspeedleft;
				}
				
				double Wheels::getWheelspeeds1Frontaxle1wheelspeedright() const{
					return m_wheelspeeds1Frontaxle1wheelspeedright;
				}
				
				void Wheels::setWheelspeeds1Frontaxle1wheelspeedright(const double &wheelspeeds1Frontaxle1wheelspeedright) {
					m_wheelspeeds1Frontaxle1wheelspeedright=wheelspeeds1Frontaxle1wheelspeedright;
				}
				
				double Wheels::getWheelspeeds1Driveaxle1wheelspeedleft() const{
					return m_wheelspeeds1Driveaxle1wheelspeedleft;
				}
				
				void Wheels::setWheelspeeds1Driveaxle1wheelspeedleft(const double &wheelspeeds1Driveaxle1wheelspeedleft) {
					m_wheelspeeds1Driveaxle1wheelspeedleft=wheelspeeds1Driveaxle1wheelspeedleft;
				}
				
				double Wheels::getWheelspeeds1Driveaxle1wheelspeedright() const{
					return m_wheelspeeds1Driveaxle1wheelspeedright;
				}
				
				void Wheels::setWheelspeeds1Driveaxle1wheelspeedright(const double &wheelspeeds1Driveaxle1wheelspeedright) {
					m_wheelspeeds1Driveaxle1wheelspeedright=wheelspeeds1Driveaxle1wheelspeedright;
				}
				
				double Wheels::getWheelspeeds2Driveaxle2wheelspeedleft() const{
					return m_wheelspeeds2Driveaxle2wheelspeedleft;
				}
				
				void Wheels::setWheelspeeds2Driveaxle2wheelspeedleft(const double &wheelspeeds2Driveaxle2wheelspeedleft) {
					m_wheelspeeds2Driveaxle2wheelspeedleft=wheelspeeds2Driveaxle2wheelspeedleft;
				}
				
				double Wheels::getWheelspeeds2Driveaxle2wheelspeedright() const{
					return m_wheelspeeds2Driveaxle2wheelspeedright;
				}
				
				void Wheels::setWheelspeeds2Driveaxle2wheelspeedright(const double &wheelspeeds2Driveaxle2wheelspeedright) {
					m_wheelspeeds2Driveaxle2wheelspeedright=wheelspeeds2Driveaxle2wheelspeedright;
				}
				
				
				ostream& Wheels::operator<<(ostream &out) const {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Serializer> s = sf.getSerializer(out);
				
					s->write(static_cast<uint32_t>(1), m_wheelspeeds1Frontaxle1wheelspeedleft);
					s->write(static_cast<uint32_t>(2), m_wheelspeeds1Frontaxle1wheelspeedright);
					s->write(static_cast<uint32_t>(3), m_wheelspeeds1Driveaxle1wheelspeedleft);
					s->write(static_cast<uint32_t>(4), m_wheelspeeds1Driveaxle1wheelspeedright);
					s->write(static_cast<uint32_t>(5), m_wheelspeeds2Driveaxle2wheelspeedleft);
					s->write(static_cast<uint32_t>(6), m_wheelspeeds2Driveaxle2wheelspeedright);
					return out;
				}
				
				istream& Wheels::operator>>(istream &in) {
					odcore::base::SerializationFactory& sf = odcore::base::SerializationFactory::getInstance();
					std::shared_ptr<odcore::base::Deserializer> s = sf.getDeserializer(in);
					
					uint32_t id;
					id=1;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds1Frontaxle1wheelspeedleft);
					id=2;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds1Frontaxle1wheelspeedright);
					id=3;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds1Driveaxle1wheelspeedleft);
					id=4;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds1Driveaxle1wheelspeedright);
					id=5;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds2Driveaxle2wheelspeedleft);
					id=6;
					s->read(static_cast<uint32_t>(id), m_wheelspeeds2Driveaxle2wheelspeedright);
					return in;
				}
				
				int32_t Wheels::ID() {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Wheels::ShortName() {
					return "Wheels";
				}
				
				const string Wheels::LongName() {
					return "Wheels";
				}
				
				int32_t Wheels::getID() const {
					return 0; // There is no valid message identifier specified as the CAN message will be mapped to a high-level data structure.
				}
				
				const string Wheels::getShortName() const {
					return Wheels::ShortName();
				}
				
				const string Wheels::getLongName() const {
					return Wheels::LongName();
				}
				
				void Wheels::accept(odcore::base::Visitor &v) {
				v.beginVisit();
					v.visit(static_cast<uint32_t>(1), 1, "wheelspeeds1.frontaxle1wheelspeedleft", "frontaxle1wheelspeedleft", m_wheelspeeds1Frontaxle1wheelspeedleft);
					v.visit(static_cast<uint32_t>(2), 2, "wheelspeeds1.frontaxle1wheelspeedright", "frontaxle1wheelspeedright", m_wheelspeeds1Frontaxle1wheelspeedright);
					v.visit(static_cast<uint32_t>(3), 3, "wheelspeeds1.driveaxle1wheelspeedleft", "driveaxle1wheelspeedleft", m_wheelspeeds1Driveaxle1wheelspeedleft);
					v.visit(static_cast<uint32_t>(4), 4, "wheelspeeds1.driveaxle1wheelspeedright", "driveaxle1wheelspeedright", m_wheelspeeds1Driveaxle1wheelspeedright);
					v.visit(static_cast<uint32_t>(5), 5, "wheelspeeds2.driveaxle2wheelspeedleft", "driveaxle2wheelspeedleft", m_wheelspeeds2Driveaxle2wheelspeedleft);
					v.visit(static_cast<uint32_t>(6), 6, "wheelspeeds2.driveaxle2wheelspeedright", "driveaxle2wheelspeedright", m_wheelspeeds2Driveaxle2wheelspeedright);
				v.endVisit();
				}
				
				::automotive::GenericCANMessage Wheels::encode(odcore::data::Container &c) {
				
				cerr<<"Warning: Mapping 'Wheels' is defined over more than one concrete CAN messages. Only one of those CAN messages will be returned."<<endl;
				
				::automotive::GenericCANMessage GCM_0x101;
				GCM_0x101.setIdentifier(0x101);
				uint64_t GCMPayload_0x101=0x0;
				::automotive::GenericCANMessage GCM_0x102;
				GCM_0x102.setIdentifier(0x102);
				uint64_t GCMPayload_0x102=0x0;
				
					bool found, extracted, abort=false;
				
					if(c.getDataType() != ::opendlv::proxy::reverefh16::Wheels::ID())
					{
						// something went wrong
						::automotive::GenericCANMessage gcm;
						gcm.setData(0x0);
						gcm.setLength(0);
						return gcm;
					}
					
					::opendlv::proxy::reverefh16::Wheels tempWheels=c.getData<::opendlv::proxy::reverefh16::Wheels>();
					odcore::reflection::MessageFromVisitableVisitor mfvv;
					tempWheels.accept(mfvv);
					odcore::reflection::Message msg=mfvv.getMessage();
				
				
					///////// manipulating signal wheelspeeds1.frontaxle1wheelspeedleft
					found=extracted=false;
					double rawWheelspeeds1_frontaxle1wheelspeedleft = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/1, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds1_frontaxle1wheelspeedleft<0)
							rawWheelspeeds1_frontaxle1wheelspeedleft=0;
						if(rawWheelspeeds1_frontaxle1wheelspeedleft>255.99609375)
							rawWheelspeeds1_frontaxle1wheelspeedleft=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds1_frontaxle1wheelspeedleft=static_cast<double>((rawWheelspeeds1_frontaxle1wheelspeedleft - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds1_frontaxle1wheelspeedleft=static_cast<uint16_t>(round(transformedWheelspeeds1_frontaxle1wheelspeedleft)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds1_frontaxle1wheelspeedleft=0x0;
						tempPLWheelspeeds1_frontaxle1wheelspeedleft=static_cast<uint64_t>(finalWheelspeeds1_frontaxle1wheelspeedleft);
						tempPLWheelspeeds1_frontaxle1wheelspeedleft=tempPLWheelspeeds1_frontaxle1wheelspeedleft<<(0);
						GCMPayload_0x101=GCMPayload_0x101 | tempPLWheelspeeds1_frontaxle1wheelspeedleft;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal wheelspeeds1.frontaxle1wheelspeedright
					found=extracted=false;
					double rawWheelspeeds1_frontaxle1wheelspeedright = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/2, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds1_frontaxle1wheelspeedright<0)
							rawWheelspeeds1_frontaxle1wheelspeedright=0;
						if(rawWheelspeeds1_frontaxle1wheelspeedright>255.99609375)
							rawWheelspeeds1_frontaxle1wheelspeedright=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds1_frontaxle1wheelspeedright=static_cast<double>((rawWheelspeeds1_frontaxle1wheelspeedright - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds1_frontaxle1wheelspeedright=static_cast<uint16_t>(round(transformedWheelspeeds1_frontaxle1wheelspeedright)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds1_frontaxle1wheelspeedright=0x0;
						tempPLWheelspeeds1_frontaxle1wheelspeedright=static_cast<uint64_t>(finalWheelspeeds1_frontaxle1wheelspeedright);
						tempPLWheelspeeds1_frontaxle1wheelspeedright=tempPLWheelspeeds1_frontaxle1wheelspeedright<<(16);
						GCMPayload_0x101=GCMPayload_0x101 | tempPLWheelspeeds1_frontaxle1wheelspeedright;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal wheelspeeds1.driveaxle1wheelspeedleft
					found=extracted=false;
					double rawWheelspeeds1_driveaxle1wheelspeedleft = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/3, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds1_driveaxle1wheelspeedleft<0)
							rawWheelspeeds1_driveaxle1wheelspeedleft=0;
						if(rawWheelspeeds1_driveaxle1wheelspeedleft>255.99609375)
							rawWheelspeeds1_driveaxle1wheelspeedleft=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds1_driveaxle1wheelspeedleft=static_cast<double>((rawWheelspeeds1_driveaxle1wheelspeedleft - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds1_driveaxle1wheelspeedleft=static_cast<uint16_t>(round(transformedWheelspeeds1_driveaxle1wheelspeedleft)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds1_driveaxle1wheelspeedleft=0x0;
						tempPLWheelspeeds1_driveaxle1wheelspeedleft=static_cast<uint64_t>(finalWheelspeeds1_driveaxle1wheelspeedleft);
						tempPLWheelspeeds1_driveaxle1wheelspeedleft=tempPLWheelspeeds1_driveaxle1wheelspeedleft<<(32);
						GCMPayload_0x101=GCMPayload_0x101 | tempPLWheelspeeds1_driveaxle1wheelspeedleft;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal wheelspeeds1.driveaxle1wheelspeedright
					found=extracted=false;
					double rawWheelspeeds1_driveaxle1wheelspeedright = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/4, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds1_driveaxle1wheelspeedright<0)
							rawWheelspeeds1_driveaxle1wheelspeedright=0;
						if(rawWheelspeeds1_driveaxle1wheelspeedright>255.99609375)
							rawWheelspeeds1_driveaxle1wheelspeedright=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds1_driveaxle1wheelspeedright=static_cast<double>((rawWheelspeeds1_driveaxle1wheelspeedright - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds1_driveaxle1wheelspeedright=static_cast<uint16_t>(round(transformedWheelspeeds1_driveaxle1wheelspeedright)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds1_driveaxle1wheelspeedright=0x0;
						tempPLWheelspeeds1_driveaxle1wheelspeedright=static_cast<uint64_t>(finalWheelspeeds1_driveaxle1wheelspeedright);
						tempPLWheelspeeds1_driveaxle1wheelspeedright=tempPLWheelspeeds1_driveaxle1wheelspeedright<<(48);
						GCMPayload_0x101=GCMPayload_0x101 | tempPLWheelspeeds1_driveaxle1wheelspeedright;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal wheelspeeds2.driveaxle2wheelspeedleft
					found=extracted=false;
					double rawWheelspeeds2_driveaxle2wheelspeedleft = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/5, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds2_driveaxle2wheelspeedleft<0)
							rawWheelspeeds2_driveaxle2wheelspeedleft=0;
						if(rawWheelspeeds2_driveaxle2wheelspeedleft>255.99609375)
							rawWheelspeeds2_driveaxle2wheelspeedleft=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds2_driveaxle2wheelspeedleft=static_cast<double>((rawWheelspeeds2_driveaxle2wheelspeedleft - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds2_driveaxle2wheelspeedleft=static_cast<uint16_t>(round(transformedWheelspeeds2_driveaxle2wheelspeedleft)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds2_driveaxle2wheelspeedleft=0x0;
						tempPLWheelspeeds2_driveaxle2wheelspeedleft=static_cast<uint64_t>(finalWheelspeeds2_driveaxle2wheelspeedleft);
						tempPLWheelspeeds2_driveaxle2wheelspeedleft=tempPLWheelspeeds2_driveaxle2wheelspeedleft<<(0);
						GCMPayload_0x102=GCMPayload_0x102 | tempPLWheelspeeds2_driveaxle2wheelspeedleft;
					}
					else {
						abort=true; // set to true and never reset to false to keep track of failures
					}
					///////// manipulating signal wheelspeeds2.driveaxle2wheelspeedright
					found=extracted=false;
					double rawWheelspeeds2_driveaxle2wheelspeedright = msg.getValueFromScalarField<double>(/*longid*/0,/*shortid*/6, found, extracted);
					
					if(found && extracted){
						if(rawWheelspeeds2_driveaxle2wheelspeedright<0)
							rawWheelspeeds2_driveaxle2wheelspeedright=0;
						if(rawWheelspeeds2_driveaxle2wheelspeedright>255.99609375)
							rawWheelspeeds2_driveaxle2wheelspeedright=255.99609375;
						
						const double SCALE = 0 ;
						const double OFFSET = 0.00390625 ;
						
						double transformedWheelspeeds2_driveaxle2wheelspeedright=static_cast<double>((rawWheelspeeds2_driveaxle2wheelspeedright - SCALE) / (double) OFFSET);
				
						
						// signal length is 16 bits long and the value will be casted to uint16_t
						uint16_t finalWheelspeeds2_driveaxle2wheelspeedright=static_cast<uint16_t>(round(transformedWheelspeeds2_driveaxle2wheelspeedright)); // avoid truncation errors
				
						// Endianness doesn't need fixing, skipping this step.
				
						uint64_t tempPLWheelspeeds2_driveaxle2wheelspeedright=0x0;
						tempPLWheelspeeds2_driveaxle2wheelspeedright=static_cast<uint64_t>(finalWheelspeeds2_driveaxle2wheelspeedright);
						tempPLWheelspeeds2_driveaxle2wheelspeedright=tempPLWheelspeeds2_driveaxle2wheelspeedright<<(16);
						GCMPayload_0x102=GCMPayload_0x102 | tempPLWheelspeeds2_driveaxle2wheelspeedright;
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
					for(uint8_t i=0;i<static_cast<uint8_t>(12.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x101 & 0xFF ;
						GCMPayload_0x101=GCMPayload_0x101>>8;
					}
				
					GCM_0x101.setData(rolledPayload);
					GCM_0x101.setLength(static_cast<uint8_t>(12.0));
				}
				return GCM_0x101;
				// set payload of GenericCANMessage and return
				// the rolling is due to Vector's dbc file numeration convention
				{
					uint64_t rolledPayload=0x0;
					for(uint8_t i=0;i<static_cast<uint8_t>(12.0); ++i)
					{
						rolledPayload=rolledPayload<<8;
						rolledPayload |= GCMPayload_0x102 & 0xFF ;
						GCMPayload_0x102=GCMPayload_0x102>>8;
					}
				
					GCM_0x102.setData(rolledPayload);
					GCM_0x102.setLength(static_cast<uint8_t>(12.0));
				}
				return GCM_0x102;
				}
				
				odcore::data::Container Wheels::decode(const ::automotive::GenericCANMessage &gcm) {
					odcore::data::Container c;
					switch(gcm.getIdentifier())
					{
				
				    	case 0x101 : 
				
				// since the order doesn't matter, store the payload in a map for future use replacing the current content held there
				m_payloads[gcm.getIdentifier()] = gcm.getData();
				    	break;
				
				
				    	case 0x102 : 
				
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
				
					// addressing signal wheelspeeds1.frontaxle1wheelspeedleft
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x101];
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
						uint64_t raw_wheelspeeds1_frontaxle1wheelspeedleft=0x0;
						raw_wheelspeeds1_frontaxle1wheelspeedleft=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds1_frontaxle1wheelspeedleft=raw_wheelspeeds1_frontaxle1wheelspeedleft >> (0);
				
						raw_wheelspeeds1_frontaxle1wheelspeedleft=raw_wheelspeeds1_frontaxle1wheelspeedleft & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds1_frontaxle1wheelspeedleft=static_cast<double>(raw_wheelspeeds1_frontaxle1wheelspeedleft);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds1_frontaxle1wheelspeedleft=(transformed_wheelspeeds1_frontaxle1wheelspeedleft*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds1_frontaxle1wheelspeedleft<0)
							transformed_wheelspeeds1_frontaxle1wheelspeedleft=0;
						else if(transformed_wheelspeeds1_frontaxle1wheelspeedleft>255.99609375)
							transformed_wheelspeeds1_frontaxle1wheelspeedleft=255.99609375;
						
						m_wheelspeeds1Frontaxle1wheelspeedleft=transformed_wheelspeeds1_frontaxle1wheelspeedleft;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds1Frontaxle1wheelspeedleft);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(1); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds1.frontaxle1wheelspeedleft");
						f->setShortFieldName("frontaxle1wheelspeedleft");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds1Frontaxle1wheelspeedleft));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal wheelspeeds1.frontaxle1wheelspeedright
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x101];
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
						uint64_t raw_wheelspeeds1_frontaxle1wheelspeedright=0x0;
						raw_wheelspeeds1_frontaxle1wheelspeedright=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds1_frontaxle1wheelspeedright=raw_wheelspeeds1_frontaxle1wheelspeedright >> (16);
				
						raw_wheelspeeds1_frontaxle1wheelspeedright=raw_wheelspeeds1_frontaxle1wheelspeedright & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds1_frontaxle1wheelspeedright=static_cast<double>(raw_wheelspeeds1_frontaxle1wheelspeedright);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds1_frontaxle1wheelspeedright=(transformed_wheelspeeds1_frontaxle1wheelspeedright*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds1_frontaxle1wheelspeedright<0)
							transformed_wheelspeeds1_frontaxle1wheelspeedright=0;
						else if(transformed_wheelspeeds1_frontaxle1wheelspeedright>255.99609375)
							transformed_wheelspeeds1_frontaxle1wheelspeedright=255.99609375;
						
						m_wheelspeeds1Frontaxle1wheelspeedright=transformed_wheelspeeds1_frontaxle1wheelspeedright;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds1Frontaxle1wheelspeedright);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(2); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds1.frontaxle1wheelspeedright");
						f->setShortFieldName("frontaxle1wheelspeedright");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds1Frontaxle1wheelspeedright));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal wheelspeeds1.driveaxle1wheelspeedleft
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x101];
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
						uint64_t raw_wheelspeeds1_driveaxle1wheelspeedleft=0x0;
						raw_wheelspeeds1_driveaxle1wheelspeedleft=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds1_driveaxle1wheelspeedleft=raw_wheelspeeds1_driveaxle1wheelspeedleft >> (32);
				
						raw_wheelspeeds1_driveaxle1wheelspeedleft=raw_wheelspeeds1_driveaxle1wheelspeedleft & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds1_driveaxle1wheelspeedleft=static_cast<double>(raw_wheelspeeds1_driveaxle1wheelspeedleft);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds1_driveaxle1wheelspeedleft=(transformed_wheelspeeds1_driveaxle1wheelspeedleft*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds1_driveaxle1wheelspeedleft<0)
							transformed_wheelspeeds1_driveaxle1wheelspeedleft=0;
						else if(transformed_wheelspeeds1_driveaxle1wheelspeedleft>255.99609375)
							transformed_wheelspeeds1_driveaxle1wheelspeedleft=255.99609375;
						
						m_wheelspeeds1Driveaxle1wheelspeedleft=transformed_wheelspeeds1_driveaxle1wheelspeedleft;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds1Driveaxle1wheelspeedleft);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(3); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds1.driveaxle1wheelspeedleft");
						f->setShortFieldName("driveaxle1wheelspeedleft");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds1Driveaxle1wheelspeedleft));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal wheelspeeds1.driveaxle1wheelspeedright
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x101];
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
						uint64_t raw_wheelspeeds1_driveaxle1wheelspeedright=0x0;
						raw_wheelspeeds1_driveaxle1wheelspeedright=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds1_driveaxle1wheelspeedright=raw_wheelspeeds1_driveaxle1wheelspeedright >> (48);
				
						raw_wheelspeeds1_driveaxle1wheelspeedright=raw_wheelspeeds1_driveaxle1wheelspeedright & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds1_driveaxle1wheelspeedright=static_cast<double>(raw_wheelspeeds1_driveaxle1wheelspeedright);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds1_driveaxle1wheelspeedright=(transformed_wheelspeeds1_driveaxle1wheelspeedright*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds1_driveaxle1wheelspeedright<0)
							transformed_wheelspeeds1_driveaxle1wheelspeedright=0;
						else if(transformed_wheelspeeds1_driveaxle1wheelspeedright>255.99609375)
							transformed_wheelspeeds1_driveaxle1wheelspeedright=255.99609375;
						
						m_wheelspeeds1Driveaxle1wheelspeedright=transformed_wheelspeeds1_driveaxle1wheelspeedright;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds1Driveaxle1wheelspeedright);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(4); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds1.driveaxle1wheelspeedright");
						f->setShortFieldName("driveaxle1wheelspeedright");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds1Driveaxle1wheelspeedright));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal wheelspeeds2.driveaxle2wheelspeedleft
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x102];
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
						uint64_t raw_wheelspeeds2_driveaxle2wheelspeedleft=0x0;
						raw_wheelspeeds2_driveaxle2wheelspeedleft=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds2_driveaxle2wheelspeedleft=raw_wheelspeeds2_driveaxle2wheelspeedleft >> (0);
				
						raw_wheelspeeds2_driveaxle2wheelspeedleft=raw_wheelspeeds2_driveaxle2wheelspeedleft & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds2_driveaxle2wheelspeedleft=static_cast<double>(raw_wheelspeeds2_driveaxle2wheelspeedleft);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds2_driveaxle2wheelspeedleft=(transformed_wheelspeeds2_driveaxle2wheelspeedleft*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds2_driveaxle2wheelspeedleft<0)
							transformed_wheelspeeds2_driveaxle2wheelspeedleft=0;
						else if(transformed_wheelspeeds2_driveaxle2wheelspeedleft>255.99609375)
							transformed_wheelspeeds2_driveaxle2wheelspeedleft=255.99609375;
						
						m_wheelspeeds2Driveaxle2wheelspeedleft=transformed_wheelspeeds2_driveaxle2wheelspeedleft;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds2Driveaxle2wheelspeedleft);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(5); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds2.driveaxle2wheelspeedleft");
						f->setShortFieldName("driveaxle2wheelspeedleft");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds2Driveaxle2wheelspeedleft));
				
						// Add created field to generic message.
						message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f));
					}
					// addressing signal wheelspeeds2.driveaxle2wheelspeedright
					{
						// Get the raw payload.
						uint64_t data = m_payloads[0x102];
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
						uint64_t raw_wheelspeeds2_driveaxle2wheelspeedright=0x0;
						raw_wheelspeeds2_driveaxle2wheelspeedright=data;
				
						// reset right-hand side of bit field
						raw_wheelspeeds2_driveaxle2wheelspeedright=raw_wheelspeeds2_driveaxle2wheelspeedright >> (16);
				
						raw_wheelspeeds2_driveaxle2wheelspeedright=raw_wheelspeeds2_driveaxle2wheelspeedright & 0b01111111111111111;
				
						// Endianness doesn't need fixing, skipping this step.
				
						// variable holding the transformed value
						double transformed_wheelspeeds2_driveaxle2wheelspeedright=static_cast<double>(raw_wheelspeeds2_driveaxle2wheelspeedright);
						
						// Apply value transformation (i.e. formula) to map raw value to (physically) meaningful value according to CAN message specification.
						const double SCALE = 0.00390625;
						const double OFFSET = 0;
						
						// scaling and adding
						transformed_wheelspeeds2_driveaxle2wheelspeedright=(transformed_wheelspeeds2_driveaxle2wheelspeedright*SCALE)+OFFSET;
						
						// clamping
						if(transformed_wheelspeeds2_driveaxle2wheelspeedright<0)
							transformed_wheelspeeds2_driveaxle2wheelspeedright=0;
						else if(transformed_wheelspeeds2_driveaxle2wheelspeedright>255.99609375)
							transformed_wheelspeeds2_driveaxle2wheelspeedright=255.99609375;
						
						m_wheelspeeds2Driveaxle2wheelspeedright=transformed_wheelspeeds2_driveaxle2wheelspeedright;
						
						// Create a field for a generic message.
						odcore::reflection::Field<double> *f = new odcore::reflection::Field<double>(m_wheelspeeds2Driveaxle2wheelspeedright);
						f->setLongFieldIdentifier(0); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setShortFieldIdentifier(6); // The identifiers specified here must match with the ones defined in the .odvd file!
						f->setLongFieldName("wheelspeeds2.driveaxle2wheelspeedright");
						f->setShortFieldName("driveaxle2wheelspeedright");
						f->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
						f->setSize(sizeof(m_wheelspeeds2Driveaxle2wheelspeedright));
				
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
				        	m_opendlv_proxy_reverefh16_Wheels.accept(mtvv);
						// Create the resulting container carrying a valid payload.
						c = odcore::data::Container(m_opendlv_proxy_reverefh16_Wheels);
					}
					return c;
				}
			} // end of namespace "reverefh16"
		} // end of namespace "proxy"
	} // end of namespace "opendlv"

} // canmapping
