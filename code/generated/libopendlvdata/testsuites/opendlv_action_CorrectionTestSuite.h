/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_ACTION_CORRECTION_TESTSUITE_H
#define OPENDLV_ACTION_CORRECTION_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/action/Correction.h"

#include "opendavinci/odcore/data/TimeStamp.h"

class opendlv_action_Correction_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::action;
	
			Correction obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 0, 1e-5);
	
			obj1.setType("Hello World!");
			obj1.setIsInhibitory(true);
			obj1.setAmplitude(2.5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 2.5, 1e-5);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::action;
	
			Correction obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 0, 1e-5);
	
			Correction obj2(obj1);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getIsInhibitory() == obj2.getIsInhibitory());
			TS_ASSERT_DELTA(obj1.getAmplitude(), obj2.getAmplitude(), 1e-5);
	
			obj1.setType("Hello World!");
			obj1.setIsInhibitory(true);
			obj1.setAmplitude(2.5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 2.5, 1e-5);
	
			Correction obj3(obj1);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj3.getType()));
			TS_ASSERT(obj1.getIsInhibitory() == obj3.getIsInhibitory());
			TS_ASSERT_DELTA(obj1.getAmplitude(), obj3.getAmplitude(), 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj3.getType(), "Hello World!"));
			TS_ASSERT(obj3.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj3.getAmplitude(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::action;
	
			Correction obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 0, 1e-5);
	
			Correction obj2;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), ""));
			TS_ASSERT(obj2.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj2.getAmplitude(), 0, 1e-5);
	
			obj1.setType("Hello World!");
			obj1.setIsInhibitory(true);
			obj1.setAmplitude(2.5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 2.5, 1e-5);
	
			obj2 = obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getIsInhibitory() == obj2.getIsInhibitory());
			TS_ASSERT_DELTA(obj1.getAmplitude(), obj2.getAmplitude(), 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), "Hello World!"));
			TS_ASSERT(obj2.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj2.getAmplitude(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::action;
	
			Correction obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 0, 1e-5);
	
			Correction obj2;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), ""));
			TS_ASSERT(obj2.getIsInhibitory() == false);
			TS_ASSERT_DELTA(obj2.getAmplitude(), 0, 1e-5);
	
			obj1.setType("Hello World!");
			obj1.setIsInhibitory(true);
			obj1.setAmplitude(2.5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj1.getAmplitude(), 2.5, 1e-5);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getIsInhibitory() == obj2.getIsInhibitory());
			TS_ASSERT_DELTA(obj1.getAmplitude(), obj2.getAmplitude(), 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), "Hello World!"));
			TS_ASSERT(obj2.getIsInhibitory() == true);
			TS_ASSERT_DELTA(obj2.getAmplitude(), 2.5, 1e-5);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_ACTION_CORRECTION_TESTSUITE_H*/
