/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.AccelerationRequest
/*
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

#ifndef CANMAPPINGTESTSUITE_ACCELERATIONREQUEST_H_
#define CANMAPPINGTESTSUITE_ACCELERATIONREQUEST_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/AccelerationRequest.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::AccelerationRequest opendlv_proxy_reverefh16_AccelerationRequest_1(9593.103378423828, -4763.007965916244);
        	canmapping::opendlv::proxy::reverefh16::AccelerationRequest opendlv_proxy_reverefh16_AccelerationRequest_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_AccelerationRequest_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_AccelerationRequest_2;
            ss2 << opendlv_proxy_reverefh16_AccelerationRequest_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_AccelerationRequest_1.toString();
        	//cout<<opendlv_proxy_reverefh16_AccelerationRequest_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
		{
			// Mapping name opendlv.proxy.reverefh16.AccelerationRequest
			// id 0x203
			// payload 0x7FA501 : length 3 bytes

				::automotive::GenericCANMessage gcm_1;
				gcm_1.setIdentifier(0x203);
				gcm_1.setLength(3);
				gcm_1.setData(0x7FA501);

			canmapping::opendlv::proxy::reverefh16::AccelerationRequest test_0;
			
			test_0.decode(gcm_1);
			
			TS_ASSERT_DELTA(test_0.getAccelerationrequestEnable_accrequest() , 1, 1e-4);
			TS_ASSERT_DELTA(test_0.getAccelerationrequestAccelerationrequest() , 5, 1e-4);
		}
    }

    void testEncode() {
{
// Mapping name opendlv.proxy.reverefh16.AccelerationRequest


			// id 0x203
			// payload 0x7FA501 : length 3

			odcore::reflection::Message message;


		odcore::reflection::Field<double> *f_1 = new odcore::reflection::Field<double>(1);
		f_1->setLongFieldIdentifier(0);f_1->setShortFieldIdentifier(1);
		f_1->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_1));
		odcore::reflection::Field<double> *f_2 = new odcore::reflection::Field<double>(5);
		f_2->setLongFieldIdentifier(0);f_2->setShortFieldIdentifier(2);
		f_2->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_2));

			odcore::reflection::MessageToVisitableVisitor mtvv(message);
			::opendlv::proxy::reverefh16::AccelerationRequest HLClass;
			HLClass.accept(mtvv);
			odcore::data::Container c(HLClass);
			
			canmapping::opendlv::proxy::reverefh16::AccelerationRequest test_0;
			::automotive::GenericCANMessage GCM;
			GCM=test_0.encode(c);
			TS_ASSERT_EQUALS(GCM.getData(),static_cast<uint64_t>(0x7FA501));
		}
    }
};

#endif /*CANMAPPINGTESTSUITE_ACCELERATIONREQUEST_H_*/
