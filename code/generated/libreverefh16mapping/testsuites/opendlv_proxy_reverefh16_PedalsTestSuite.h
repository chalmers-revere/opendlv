/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.Pedals
/*
CANID       : 0x106
FQDN        : manualdriver.accelerationpedalposition
startBit    : 0
length      : 8
lengthBytes : 1
endian      : little
multiplyBy  : 0.4
add         : 0
rangeStart  : 0
rangeEnd    : 102
CANID       : 0x106
FQDN        : manualdriver.brakepedalposition
startBit    : 8
length      : 8
lengthBytes : 1
endian      : little
multiplyBy  : 0.4
add         : 0
rangeStart  : 0
rangeEnd    : 102
CANID       : 0x106
FQDN        : manualdriver.torsionbartorque
startBit    : 23
length      : 16
lengthBytes : 2
endian      : big
multiplyBy  : 0.0009765625
add         : -32
rangeStart  : -32
rangeEnd    : 31.9990234375
*/

#ifndef CANMAPPINGTESTSUITE_PEDALS_H_
#define CANMAPPINGTESTSUITE_PEDALS_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/Pedals.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::Pedals opendlv_proxy_reverefh16_Pedals_1(3025.0715573997095, -6647.209476998662, 1566.818472262261);
        	canmapping::opendlv::proxy::reverefh16::Pedals opendlv_proxy_reverefh16_Pedals_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_Pedals_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_Pedals_2;
            ss2 << opendlv_proxy_reverefh16_Pedals_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_Pedals_1.toString();
        	//cout<<opendlv_proxy_reverefh16_Pedals_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
    }

    void testEncode() {
    }
};

#endif /*CANMAPPINGTESTSUITE_PEDALS_H_*/
