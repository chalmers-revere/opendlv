/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.Wheels
/*
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

#ifndef CANMAPPINGTESTSUITE_WHEELS_H_
#define CANMAPPINGTESTSUITE_WHEELS_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/Wheels.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::Wheels opendlv_proxy_reverefh16_Wheels_1(6520.117574983407, -6835.851611411312, 7879.018539108696, -176.72274593654038, -8502.808493223984, -4331.992268586682);
        	canmapping::opendlv::proxy::reverefh16::Wheels opendlv_proxy_reverefh16_Wheels_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_Wheels_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_Wheels_2;
            ss2 << opendlv_proxy_reverefh16_Wheels_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_Wheels_1.toString();
        	//cout<<opendlv_proxy_reverefh16_Wheels_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
    }

    void testEncode() {
    }
};

#endif /*CANMAPPINGTESTSUITE_WHEELS_H_*/
