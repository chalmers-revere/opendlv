/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.Propulsion
/*
CANID       : 0x103
FQDN        : vehiclespeed.vehiclespeedpropshaft
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00390625
add         : 0
rangeStart  : 0
rangeEnd    : 255.99609375
*/

#ifndef CANMAPPINGTESTSUITE_PROPULSION_H_
#define CANMAPPINGTESTSUITE_PROPULSION_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/Propulsion.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::Propulsion opendlv_proxy_reverefh16_Propulsion_1(653.8084561333853);
        	canmapping::opendlv::proxy::reverefh16::Propulsion opendlv_proxy_reverefh16_Propulsion_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_Propulsion_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_Propulsion_2;
            ss2 << opendlv_proxy_reverefh16_Propulsion_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_Propulsion_1.toString();
        	//cout<<opendlv_proxy_reverefh16_Propulsion_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
    }

    void testEncode() {
    }
};

#endif /*CANMAPPINGTESTSUITE_PROPULSION_H_*/
