/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_PEDALS_TESTSUITE_H
#define OPENDLV_PROXY_REVEREFH16_PEDALS_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/reverefh16/Pedals.h"


class opendlv_proxy_reverefh16_Pedals_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Pedals obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 0.0, 1e-5);
	
			obj1.setAccelerationPedalPosition(1.0);
			obj1.setBrakePedalPosition(1.0);
			obj1.setTorsionBarTorque(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 1.0, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Pedals obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 0.0, 1e-5);
	
			Pedals obj2(obj1);
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), obj2.getAccelerationPedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), obj2.getBrakePedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), obj2.getTorsionBarTorque(), 1e-5);
	
			obj1.setAccelerationPedalPosition(1.0);
			obj1.setBrakePedalPosition(1.0);
			obj1.setTorsionBarTorque(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 1.0, 1e-5);
	
			Pedals obj3(obj1);
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), obj3.getAccelerationPedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), obj3.getBrakePedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), obj3.getTorsionBarTorque(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getTorsionBarTorque(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Pedals obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 0.0, 1e-5);
	
			Pedals obj2;
			TS_ASSERT_DELTA(obj2.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getTorsionBarTorque(), 0.0, 1e-5);
	
			obj1.setAccelerationPedalPosition(1.0);
			obj1.setBrakePedalPosition(1.0);
			obj1.setTorsionBarTorque(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 1.0, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), obj2.getAccelerationPedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), obj2.getBrakePedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), obj2.getTorsionBarTorque(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getTorsionBarTorque(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Pedals obj1;
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 0.0, 1e-5);
	
			Pedals obj2;
			TS_ASSERT_DELTA(obj2.getAccelerationPedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getBrakePedalPosition(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getTorsionBarTorque(), 0.0, 1e-5);
	
			obj1.setAccelerationPedalPosition(1.0);
			obj1.setBrakePedalPosition(1.0);
			obj1.setTorsionBarTorque(1.0);
	
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), 1.0, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getAccelerationPedalPosition(), obj2.getAccelerationPedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getBrakePedalPosition(), obj2.getBrakePedalPosition(), 1e-5);
			TS_ASSERT_DELTA(obj1.getTorsionBarTorque(), obj2.getTorsionBarTorque(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getAccelerationPedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getBrakePedalPosition(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getTorsionBarTorque(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_REVEREFH16_PEDALS_TESTSUITE_H*/
