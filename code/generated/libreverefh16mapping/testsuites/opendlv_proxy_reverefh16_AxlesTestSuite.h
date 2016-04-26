/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.Axles
/*
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

#ifndef CANMAPPINGTESTSUITE_AXLES_H_
#define CANMAPPINGTESTSUITE_AXLES_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/Axles.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::Axles opendlv_proxy_reverefh16_Axles_1(564.3122459596707, -3257.426412591213, -4324.373526331336);
        	canmapping::opendlv::proxy::reverefh16::Axles opendlv_proxy_reverefh16_Axles_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_Axles_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_Axles_2;
            ss2 << opendlv_proxy_reverefh16_Axles_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_Axles_1.toString();
        	//cout<<opendlv_proxy_reverefh16_Axles_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
    }

    void testEncode() {
    }
};

#endif /*CANMAPPINGTESTSUITE_AXLES_H_*/
