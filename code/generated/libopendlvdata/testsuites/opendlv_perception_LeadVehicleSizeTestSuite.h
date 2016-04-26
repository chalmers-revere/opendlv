/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PERCEPTION_LEADVEHICLESIZE_TESTSUITE_H
#define OPENDLV_PERCEPTION_LEADVEHICLESIZE_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/perception/LeadVehicleSize.h"


class opendlv_perception_LeadVehicleSize_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::perception;
	
			LeadVehicleSize obj1;
			TS_ASSERT_DELTA(obj1.getSize(), 0, 1e-5);
	
			obj1.setSize(2.5);
	
			TS_ASSERT_DELTA(obj1.getSize(), 2.5, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::perception;
	
			LeadVehicleSize obj1;
			TS_ASSERT_DELTA(obj1.getSize(), 0, 1e-5);
	
			LeadVehicleSize obj2(obj1);
			TS_ASSERT_DELTA(obj1.getSize(), obj2.getSize(), 1e-5);
	
			obj1.setSize(2.5);
	
			TS_ASSERT_DELTA(obj1.getSize(), 2.5, 1e-5);
	
			LeadVehicleSize obj3(obj1);
			TS_ASSERT_DELTA(obj1.getSize(), obj3.getSize(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getSize(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::perception;
	
			LeadVehicleSize obj1;
			TS_ASSERT_DELTA(obj1.getSize(), 0, 1e-5);
	
			LeadVehicleSize obj2;
			TS_ASSERT_DELTA(obj2.getSize(), 0, 1e-5);
	
			obj1.setSize(2.5);
	
			TS_ASSERT_DELTA(obj1.getSize(), 2.5, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getSize(), obj2.getSize(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getSize(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::perception;
	
			LeadVehicleSize obj1;
			TS_ASSERT_DELTA(obj1.getSize(), 0, 1e-5);
	
			LeadVehicleSize obj2;
			TS_ASSERT_DELTA(obj2.getSize(), 0, 1e-5);
	
			obj1.setSize(2.5);
	
			TS_ASSERT_DELTA(obj1.getSize(), 2.5, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getSize(), obj2.getSize(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getSize(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PERCEPTION_LEADVEHICLESIZE_TESTSUITE_H*/
