/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_REVEREFH16_AXLES_TESTSUITE_H
#define OPENDLV_PROXY_REVEREFH16_AXLES_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/reverefh16/Axles.h"


class opendlv_proxy_reverefh16_Axles_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Axles obj1;
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 0.0, 1e-5);
	
			obj1.setLoadAxle11(1.0);
			obj1.setLoadAxle12(1.0);
			obj1.setLoadAxle13(1.0);
	
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 1.0, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Axles obj1;
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 0.0, 1e-5);
	
			Axles obj2(obj1);
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), obj2.getLoadAxle11(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), obj2.getLoadAxle12(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), obj2.getLoadAxle13(), 1e-5);
	
			obj1.setLoadAxle11(1.0);
			obj1.setLoadAxle12(1.0);
			obj1.setLoadAxle13(1.0);
	
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 1.0, 1e-5);
	
			Axles obj3(obj1);
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), obj3.getLoadAxle11(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), obj3.getLoadAxle12(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), obj3.getLoadAxle13(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getLoadAxle13(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Axles obj1;
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 0.0, 1e-5);
	
			Axles obj2;
			TS_ASSERT_DELTA(obj2.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle13(), 0.0, 1e-5);
	
			obj1.setLoadAxle11(1.0);
			obj1.setLoadAxle12(1.0);
			obj1.setLoadAxle13(1.0);
	
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 1.0, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), obj2.getLoadAxle11(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), obj2.getLoadAxle12(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), obj2.getLoadAxle13(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle13(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy::reverefh16;
	
			Axles obj1;
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 0.0, 1e-5);
	
			Axles obj2;
			TS_ASSERT_DELTA(obj2.getLoadAxle11(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle12(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle13(), 0.0, 1e-5);
	
			obj1.setLoadAxle11(1.0);
			obj1.setLoadAxle12(1.0);
			obj1.setLoadAxle13(1.0);
	
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), 1.0, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getLoadAxle11(), obj2.getLoadAxle11(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle12(), obj2.getLoadAxle12(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLoadAxle13(), obj2.getLoadAxle13(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getLoadAxle11(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle12(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLoadAxle13(), 1.0, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_REVEREFH16_AXLES_TESTSUITE_H*/
