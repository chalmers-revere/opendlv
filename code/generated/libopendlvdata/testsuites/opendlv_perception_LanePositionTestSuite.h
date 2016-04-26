/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PERCEPTION_LANEPOSITION_TESTSUITE_H
#define OPENDLV_PERCEPTION_LANEPOSITION_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/perception/LanePosition.h"


class opendlv_perception_LanePosition_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::perception;
	
			LanePosition obj1;
			TS_ASSERT_DELTA(obj1.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 0, 1e-5);
	
			obj1.setOffset(2.5);
			obj1.setHeading(2.5);
	
			TS_ASSERT_DELTA(obj1.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 2.5, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::perception;
	
			LanePosition obj1;
			TS_ASSERT_DELTA(obj1.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 0, 1e-5);
	
			LanePosition obj2(obj1);
			TS_ASSERT_DELTA(obj1.getOffset(), obj2.getOffset(), 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), obj2.getHeading(), 1e-5);
	
			obj1.setOffset(2.5);
			obj1.setHeading(2.5);
	
			TS_ASSERT_DELTA(obj1.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 2.5, 1e-5);
	
			LanePosition obj3(obj1);
			TS_ASSERT_DELTA(obj1.getOffset(), obj3.getOffset(), 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), obj3.getHeading(), 1e-5);
	
			TS_ASSERT_DELTA(obj3.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj3.getHeading(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::perception;
	
			LanePosition obj1;
			TS_ASSERT_DELTA(obj1.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 0, 1e-5);
	
			LanePosition obj2;
			TS_ASSERT_DELTA(obj2.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getHeading(), 0, 1e-5);
	
			obj1.setOffset(2.5);
			obj1.setHeading(2.5);
	
			TS_ASSERT_DELTA(obj1.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 2.5, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getOffset(), obj2.getOffset(), 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), obj2.getHeading(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getHeading(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::perception;
	
			LanePosition obj1;
			TS_ASSERT_DELTA(obj1.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 0, 1e-5);
	
			LanePosition obj2;
			TS_ASSERT_DELTA(obj2.getOffset(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getHeading(), 0, 1e-5);
	
			obj1.setOffset(2.5);
			obj1.setHeading(2.5);
	
			TS_ASSERT_DELTA(obj1.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), 2.5, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getOffset(), obj2.getOffset(), 1e-5);
			TS_ASSERT_DELTA(obj1.getHeading(), obj2.getHeading(), 1e-5);
	
			TS_ASSERT_DELTA(obj2.getOffset(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getHeading(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PERCEPTION_LANEPOSITION_TESTSUITE_H*/
