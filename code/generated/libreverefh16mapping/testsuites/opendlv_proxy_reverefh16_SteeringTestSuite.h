/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.Steering
/*
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

#ifndef CANMAPPINGTESTSUITE_STEERING_H_
#define CANMAPPINGTESTSUITE_STEERING_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/Steering.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::Steering opendlv_proxy_reverefh16_Steering_1(-4209.799655327777, 3300.5256038930784);
        	canmapping::opendlv::proxy::reverefh16::Steering opendlv_proxy_reverefh16_Steering_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_Steering_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_Steering_2;
            ss2 << opendlv_proxy_reverefh16_Steering_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_Steering_1.toString();
        	//cout<<opendlv_proxy_reverefh16_Steering_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
    }

    void testEncode() {
    }
};

#endif /*CANMAPPINGTESTSUITE_STEERING_H_*/
