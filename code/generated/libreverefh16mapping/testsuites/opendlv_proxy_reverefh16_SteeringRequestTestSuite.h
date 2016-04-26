/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.SteeringRequest
/*
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

#ifndef CANMAPPINGTESTSUITE_STEERINGREQUEST_H_
#define CANMAPPINGTESTSUITE_STEERINGREQUEST_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/SteeringRequest.h"

#include <automotivedata/GeneratedHeaders_AutomotiveData.h>
#include <opendlvdata/GeneratedHeaders_opendlvdata.h>
#include "cxxtest/TestSuite.h"
#include <sstream>
#include <iostream>
#include "opendavinci/odcore/data/Container.h"
#include <opendavinci/odcore/reflection/MessageToVisitableVisitor.h>
#include <opendavinci/odcore/reflection/Message.h>
#include <opendavinci/odcore/reflection/Field.h>

using namespace std;

/**
 * The actual testsuite starts here.
 */
class CANBridgeTest : public CxxTest::TestSuite {
    public:
        void testSer_Deser() {
        	
        	
        	canmapping::opendlv::proxy::reverefh16::SteeringRequest opendlv_proxy_reverefh16_SteeringRequest_1(-2169.3330479179585, 6755.650639602969, 4424.940860478744);
        	canmapping::opendlv::proxy::reverefh16::SteeringRequest opendlv_proxy_reverefh16_SteeringRequest_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_SteeringRequest_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_SteeringRequest_2;
            ss2 << opendlv_proxy_reverefh16_SteeringRequest_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_SteeringRequest_1.toString();
        	//cout<<opendlv_proxy_reverefh16_SteeringRequest_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
		{
			// Mapping name opendlv.proxy.reverefh16.SteeringRequest
			// id 0x201
			// payload 0xFFFF358201 : length 5 bytes

				::automotive::GenericCANMessage gcm_1;
				gcm_1.setIdentifier(0x201);
				gcm_1.setLength(5);
				gcm_1.setData(0xFFFF358201);

			canmapping::opendlv::proxy::reverefh16::SteeringRequest test_0;
			
			test_0.decode(gcm_1);
			
			TS_ASSERT_DELTA(test_0.getSteerrequestEnable_steerreq() , 1, 1e-4);
			TS_ASSERT_DELTA(test_0.getSteerrequestSteerreq_rwa() , 3.2, 1e-4);
			TS_ASSERT_DELTA(test_0.getSteerrequestSteerreq_deltatrq() , 33.535, 1e-4);
		}
    }

    void testEncode() {
{
// Mapping name opendlv.proxy.reverefh16.SteeringRequest


			// id 0x201
			// payload 0xFFFF358201 : length 5

			odcore::reflection::Message message;


		odcore::reflection::Field<double> *f_1 = new odcore::reflection::Field<double>(1);
		f_1->setLongFieldIdentifier(0);f_1->setShortFieldIdentifier(1);
		f_1->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_1));
		odcore::reflection::Field<double> *f_2 = new odcore::reflection::Field<double>(3.2);
		f_2->setLongFieldIdentifier(0);f_2->setShortFieldIdentifier(2);
		f_2->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_2));
		odcore::reflection::Field<double> *f_3 = new odcore::reflection::Field<double>(33.535);
		f_3->setLongFieldIdentifier(0);f_3->setShortFieldIdentifier(3);
		f_3->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_3));

			odcore::reflection::MessageToVisitableVisitor mtvv(message);
			::opendlv::proxy::reverefh16::SteeringRequest HLClass;
			HLClass.accept(mtvv);
			odcore::data::Container c(HLClass);
			
			canmapping::opendlv::proxy::reverefh16::SteeringRequest test_0;
			::automotive::GenericCANMessage GCM;
			GCM=test_0.encode(c);
			TS_ASSERT_EQUALS(GCM.getData(),static_cast<uint64_t>(0xFFFF358201));
		}
    }
};

#endif /*CANMAPPINGTESTSUITE_STEERINGREQUEST_H_*/
