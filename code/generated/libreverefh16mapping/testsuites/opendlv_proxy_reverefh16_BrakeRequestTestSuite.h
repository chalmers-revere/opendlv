/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.BrakeRequest
/*
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

#ifndef CANMAPPINGTESTSUITE_BRAKEREQUEST_H_
#define CANMAPPINGTESTSUITE_BRAKEREQUEST_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/BrakeRequest.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::BrakeRequest opendlv_proxy_reverefh16_BrakeRequest_1(-5070.424833165103, -7653.893559781402);
        	canmapping::opendlv::proxy::reverefh16::BrakeRequest opendlv_proxy_reverefh16_BrakeRequest_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_BrakeRequest_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_BrakeRequest_2;
            ss2 << opendlv_proxy_reverefh16_BrakeRequest_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_BrakeRequest_1.toString();
        	//cout<<opendlv_proxy_reverefh16_BrakeRequest_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
		{
			// Mapping name opendlv.proxy.reverefh16.BrakeRequest
			// id 0x202
			// payload 0x7F2D01 : length 3 bytes

				::automotive::GenericCANMessage gcm_1;
				gcm_1.setIdentifier(0x202);
				gcm_1.setLength(3);
				gcm_1.setData(0x7F2D01);

			canmapping::opendlv::proxy::reverefh16::BrakeRequest test_0;
			
			test_0.decode(gcm_1);
			
			TS_ASSERT_DELTA(test_0.getBrakerequestEnable_brakerequest() , 1, 1e-4);
			TS_ASSERT_DELTA(test_0.getBrakerequestBrakerequest() , -10, 1e-4);
		}
    }

    void testEncode() {
{
// Mapping name opendlv.proxy.reverefh16.BrakeRequest


			// id 0x202
			// payload 0x7F2D01 : length 3

			odcore::reflection::Message message;


		odcore::reflection::Field<double> *f_1 = new odcore::reflection::Field<double>(1);
		f_1->setLongFieldIdentifier(0);f_1->setShortFieldIdentifier(1);
		f_1->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_1));
		odcore::reflection::Field<double> *f_2 = new odcore::reflection::Field<double>(-10);
		f_2->setLongFieldIdentifier(0);f_2->setShortFieldIdentifier(2);
		f_2->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_2));

			odcore::reflection::MessageToVisitableVisitor mtvv(message);
			::opendlv::proxy::reverefh16::BrakeRequest HLClass;
			HLClass.accept(mtvv);
			odcore::data::Container c(HLClass);
			
			canmapping::opendlv::proxy::reverefh16::BrakeRequest test_0;
			::automotive::GenericCANMessage GCM;
			GCM=test_0.encode(c);
			TS_ASSERT_EQUALS(GCM.getData(),static_cast<uint64_t>(0x7F2D01));
		}
    }
};

#endif /*CANMAPPINGTESTSUITE_BRAKEREQUEST_H_*/
