/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_CONTROLSTATE_TESTSUITE_H
#define OPENDLV_PROXY_CONTROLSTATE_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/ControlState.h"


class opendlv_proxy_ControlState_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy;
	
			ControlState obj1;
			TS_ASSERT(obj1.getAllowAutonomous() == false);
	
			obj1.setAllowAutonomous(true);
	
			TS_ASSERT(obj1.getAllowAutonomous() == true);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy;
	
			ControlState obj1;
			TS_ASSERT(obj1.getAllowAutonomous() == false);
	
			ControlState obj2(obj1);
			TS_ASSERT(obj1.getAllowAutonomous() == obj2.getAllowAutonomous());
	
			obj1.setAllowAutonomous(true);
	
			TS_ASSERT(obj1.getAllowAutonomous() == true);
	
			ControlState obj3(obj1);
			TS_ASSERT(obj1.getAllowAutonomous() == obj3.getAllowAutonomous());
	
			TS_ASSERT(obj3.getAllowAutonomous() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy;
	
			ControlState obj1;
			TS_ASSERT(obj1.getAllowAutonomous() == false);
	
			ControlState obj2;
			TS_ASSERT(obj2.getAllowAutonomous() == false);
	
			obj1.setAllowAutonomous(true);
	
			TS_ASSERT(obj1.getAllowAutonomous() == true);
	
			obj2 = obj1;
			TS_ASSERT(obj1.getAllowAutonomous() == obj2.getAllowAutonomous());
	
			TS_ASSERT(obj2.getAllowAutonomous() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy;
	
			ControlState obj1;
			TS_ASSERT(obj1.getAllowAutonomous() == false);
	
			ControlState obj2;
			TS_ASSERT(obj2.getAllowAutonomous() == false);
	
			obj1.setAllowAutonomous(true);
	
			TS_ASSERT(obj1.getAllowAutonomous() == true);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT(obj1.getAllowAutonomous() == obj2.getAllowAutonomous());
	
			TS_ASSERT(obj2.getAllowAutonomous() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_CONTROLSTATE_TESTSUITE_H*/
