/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_WHEELS_TESTSUITE_H
#define OPENDLV_PROXY_REVEREFH16_WHEELS_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/reverefh16/Wheels.h"


class opendlv_proxy_reverefh16_Wheels_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Wheels obj1;
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 0.0, 1e-5);
	
			obj1.setSpeedWheel111(1.0);
			obj1.setSpeedWheel112(1.0);
			obj1.setSpeedWheel121(1.0);
			obj1.setSpeedWheel122(1.0);
			obj1.setSpeedWheel131(1.0);
			obj1.setSpeedWheel132(1.0);
	
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 1.0, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Wheels obj1;
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 0.0, 1e-5);
	
			Wheels obj2(obj1);
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), obj2.getSpeedWheel111(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), obj2.getSpeedWheel112(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), obj2.getSpeedWheel121(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), obj2.getSpeedWheel122(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), obj2.getSpeedWheel131(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), obj2.getSpeedWheel132(), 1e-5);
	
			obj1.setSpeedWheel111(1.0);
			obj1.setSpeedWheel112(1.0);
			obj1.setSpeedWheel121(1.0);
			obj1.setSpeedWheel122(1.0);
			obj1.setSpeedWheel131(1.0);
			obj1.setSpeedWheel132(1.0);
	
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 1.0, 1e-5);
	
			Wheels obj3(obj1);
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), obj3.getSpeedWheel111(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), obj3.getSpeedWheel112(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), obj3.getSpeedWheel121(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), obj3.getSpeedWheel122(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), obj3.getSpeedWheel131(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), obj3.getSpeedWheel132(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeedWheel132(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Wheels obj1;
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 0.0, 1e-5);
	
			Wheels obj2;
			TS_ASSERT_DELTA(obj2.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel132(), 0.0, 1e-5);
	
			obj1.setSpeedWheel111(1.0);
			obj1.setSpeedWheel112(1.0);
			obj1.setSpeedWheel121(1.0);
			obj1.setSpeedWheel122(1.0);
			obj1.setSpeedWheel131(1.0);
			obj1.setSpeedWheel132(1.0);
	
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 1.0, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), obj2.getSpeedWheel111(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), obj2.getSpeedWheel112(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), obj2.getSpeedWheel121(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), obj2.getSpeedWheel122(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), obj2.getSpeedWheel131(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), obj2.getSpeedWheel132(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel132(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Wheels obj1;
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 0.0, 1e-5);
	
			Wheels obj2;
			TS_ASSERT_DELTA(obj2.getSpeedWheel111(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel112(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel121(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel122(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel131(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel132(), 0.0, 1e-5);
	
			obj1.setSpeedWheel111(1.0);
			obj1.setSpeedWheel112(1.0);
			obj1.setSpeedWheel121(1.0);
			obj1.setSpeedWheel122(1.0);
			obj1.setSpeedWheel131(1.0);
			obj1.setSpeedWheel132(1.0);
	
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), 1.0, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getSpeedWheel111(), obj2.getSpeedWheel111(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel112(), obj2.getSpeedWheel112(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel121(), obj2.getSpeedWheel121(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel122(), obj2.getSpeedWheel122(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel131(), obj2.getSpeedWheel131(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeedWheel132(), obj2.getSpeedWheel132(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getSpeedWheel111(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel112(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel121(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel122(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel131(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeedWheel132(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_REVEREFH16_WHEELS_TESTSUITE_H*/
