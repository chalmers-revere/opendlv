/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_ACCELERATIONREQUEST_TESTSUITE_H
#define OPENDLV_PROXY_REVEREFH16_ACCELERATIONREQUEST_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/reverefh16/AccelerationRequest.h"


class opendlv_proxy_reverefh16_AccelerationRequest_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy::reverefh16;
	
			AccelerationRequest obj1;
			TS_ASSERT(obj1.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0.0, 1e-5);
	
			obj1.setEnableRequest(true);
			obj1.setAcceleration(1.0);
	
			TS_ASSERT(obj1.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 1.0, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy::reverefh16;
	
			AccelerationRequest obj1;
			TS_ASSERT(obj1.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0.0, 1e-5);
	
			AccelerationRequest obj2(obj1);
			TS_ASSERT(obj1.getEnableRequest() == obj2.getEnableRequest());
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
	
			obj1.setEnableRequest(true);
			obj1.setAcceleration(1.0);
	
			TS_ASSERT(obj1.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 1.0, 1e-5);
	
			AccelerationRequest obj3(obj1);
			TS_ASSERT(obj1.getEnableRequest() == obj3.getEnableRequest());
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj3.getAcceleration(), 1e-5);
	
			TS_ASSERT(obj3.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj3.getAcceleration(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy::reverefh16;
	
			AccelerationRequest obj1;
			TS_ASSERT(obj1.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0.0, 1e-5);
	
			AccelerationRequest obj2;
			TS_ASSERT(obj2.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj2.getAcceleration(), 0.0, 1e-5);
	
			obj1.setEnableRequest(true);
			obj1.setAcceleration(1.0);
	
			TS_ASSERT(obj1.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 1.0, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT(obj1.getEnableRequest() == obj2.getEnableRequest());
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
	
			TS_ASSERT(obj2.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj2.getAcceleration(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy::reverefh16;
	
			AccelerationRequest obj1;
			TS_ASSERT(obj1.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0.0, 1e-5);
	
			AccelerationRequest obj2;
			TS_ASSERT(obj2.getEnableRequest() == false);
			TS_ASSERT_DELTA(obj2.getAcceleration(), 0.0, 1e-5);
	
			obj1.setEnableRequest(true);
			obj1.setAcceleration(1.0);
	
			TS_ASSERT(obj1.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj1.getAcceleration(), 1.0, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT(obj1.getEnableRequest() == obj2.getEnableRequest());
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
	
			TS_ASSERT(obj2.getEnableRequest() == true);
			TS_ASSERT_DELTA(obj2.getAcceleration(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_REVEREFH16_ACCELERATIONREQUEST_TESTSUITE_H*/
