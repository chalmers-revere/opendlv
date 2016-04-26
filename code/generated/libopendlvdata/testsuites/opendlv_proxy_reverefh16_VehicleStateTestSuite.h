/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_TESTSUITE_H
#define OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/reverefh16/VehicleState.h"


class opendlv_proxy_reverefh16_VehicleState_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy::reverefh16;
	
			VehicleState obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 0.0, 1e-5);
	
			obj1.setAccelerationX(1.0);
			obj1.setAccelerationY(1.0);
			obj1.setYawRate(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 1.0, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy::reverefh16;
	
			VehicleState obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 0.0, 1e-5);
	
			VehicleState obj2(obj1);
			TS_ASSERT_DELTA(obj1.getAccelerationX(), obj2.getAccelerationX(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), obj2.getAccelerationY(), 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), obj2.getYawRate(), 1e-5);
	
			obj1.setAccelerationX(1.0);
			obj1.setAccelerationY(1.0);
			obj1.setYawRate(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 1.0, 1e-5);
	
			VehicleState obj3(obj1);
			TS_ASSERT_DELTA(obj1.getAccelerationX(), obj3.getAccelerationX(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), obj3.getAccelerationY(), 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), obj3.getYawRate(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getYawRate(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy::reverefh16;
	
			VehicleState obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 0.0, 1e-5);
	
			VehicleState obj2;
			TS_ASSERT_DELTA(obj2.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getYawRate(), 0.0, 1e-5);
	
			obj1.setAccelerationX(1.0);
			obj1.setAccelerationY(1.0);
			obj1.setYawRate(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 1.0, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationX(), obj2.getAccelerationX(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), obj2.getAccelerationY(), 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), obj2.getYawRate(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getYawRate(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy::reverefh16;
	
			VehicleState obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 0.0, 1e-5);
	
			VehicleState obj2;
			TS_ASSERT_DELTA(obj2.getAccelerationX(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAccelerationY(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getYawRate(), 0.0, 1e-5);
	
			obj1.setAccelerationX(1.0);
			obj1.setAccelerationY(1.0);
			obj1.setYawRate(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), 1.0, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getAccelerationX(), obj2.getAccelerationX(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAccelerationY(), obj2.getAccelerationY(), 1e-5);
			TS_ASSERT_DELTA(obj1.getYawRate(), obj2.getYawRate(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getAccelerationX(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAccelerationY(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getYawRate(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_REVEREFH16_VEHICLESTATE_TESTSUITE_H*/
