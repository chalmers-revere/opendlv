/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */
// Test suite file for: opendlv.proxy.reverefh16.VehicleState
/*
CANID       : 0x104
FQDN        : vehicledynamics.acceleration_x
startBit    : 0
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00048828125
add         : -15.687
rangeStart  : -15.687
rangeEnd    : 16.31251171875
CANID       : 0x104
FQDN        : vehicledynamics.acceleration_y
startBit    : 16
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.00048828125
add         : -15.687
rangeStart  : -15.687
rangeEnd    : 16.31251171875
CANID       : 0x104
FQDN        : vehicledynamics.yawrate
startBit    : 32
length      : 16
lengthBytes : 2
endian      : little
multiplyBy  : 0.000122
add         : -3.92
rangeStart  : -3.92
rangeEnd    : 4.07527
*/

#ifndef CANMAPPINGTESTSUITE_VEHICLESTATE_H_
#define CANMAPPINGTESTSUITE_VEHICLESTATE_H_

#include "GeneratedHeaders_reverefh16mapping.h"
#include "generated/opendlv/proxy/reverefh16/VehicleState.h"

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
        	
        	
        	canmapping::opendlv::proxy::reverefh16::VehicleState opendlv_proxy_reverefh16_VehicleState_1(9245.031700045041, -4529.780565831447, -5263.773380876318);
        	canmapping::opendlv::proxy::reverefh16::VehicleState opendlv_proxy_reverefh16_VehicleState_2;
        	
        	stringstream ss1,ss2;
        	ss1 << opendlv_proxy_reverefh16_VehicleState_1;
        	
            TS_ASSERT(ss1.str().compare(ss2.str())!=0);
            
            ss1 >> opendlv_proxy_reverefh16_VehicleState_2;
            ss2 << opendlv_proxy_reverefh16_VehicleState_2;
            
        	//cout << endl;
        	//cout << ss1.str() << endl;
        	//cout << ss2.str() << endl;
            //cout << endl;
        	//cout<<opendlv_proxy_reverefh16_VehicleState_1.toString();
        	//cout<<opendlv_proxy_reverefh16_VehicleState_2.toString();
        	
            TS_ASSERT(ss1.str().compare(ss2.str())==0);
        }

	void testDecode() {
		{
			// Mapping name opendlv.proxy.reverefh16.VehicleState
			// id 0x104
			// payload 0x7F7DA77D877D : length 6 bytes

				::automotive::GenericCANMessage gcm_1;
				gcm_1.setIdentifier(0x104);
				gcm_1.setLength(6);
				gcm_1.setData(0x7F7DA77D877D);

			canmapping::opendlv::proxy::reverefh16::VehicleState test_0;
			
			test_0.decode(gcm_1);
			
			TS_ASSERT_DELTA(test_0.getVehicledynamicsAcceleration_x() , 0.000012, 1e-4);
			TS_ASSERT_DELTA(test_0.getVehicledynamicsAcceleration_y() , 0.019543, 1e-4);
			TS_ASSERT_DELTA(test_0.getVehicledynamicsYawrate() , 0.00047, 1e-4);
		}
    }

    void testEncode() {
{
// Mapping name opendlv.proxy.reverefh16.VehicleState


			// id 0x104
			// payload 0x7F7DA77D877D : length 6

			odcore::reflection::Message message;


		odcore::reflection::Field<double> *f_1 = new odcore::reflection::Field<double>(0.000012);
		f_1->setLongFieldIdentifier(0);f_1->setShortFieldIdentifier(1);
		f_1->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_1));
		odcore::reflection::Field<double> *f_2 = new odcore::reflection::Field<double>(0.019543);
		f_2->setLongFieldIdentifier(0);f_2->setShortFieldIdentifier(2);
		f_2->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_2));
		odcore::reflection::Field<double> *f_3 = new odcore::reflection::Field<double>(0.00047);
		f_3->setLongFieldIdentifier(0);f_3->setShortFieldIdentifier(3);
		f_3->setFieldDataType(odcore::data::reflection::AbstractField::DOUBLE_T);
		message.addField(std::shared_ptr<odcore::data::reflection::AbstractField>(f_3));

			odcore::reflection::MessageToVisitableVisitor mtvv(message);
			::opendlv::proxy::reverefh16::VehicleState HLClass;
			HLClass.accept(mtvv);
			odcore::data::Container c(HLClass);
			
			canmapping::opendlv::proxy::reverefh16::VehicleState test_0;
			::automotive::GenericCANMessage GCM;
			GCM=test_0.encode(c);
			TS_ASSERT_EQUALS(GCM.getData(),static_cast<uint64_t>(0x7F7DA77D877D));
		}
    }
};

#endif /*CANMAPPINGTESTSUITE_VEHICLESTATE_H_*/
