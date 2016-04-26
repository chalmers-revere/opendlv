/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_SENSATION_VOICE_TESTSUITE_H
#define OPENDLV_SENSATION_VOICE_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/sensation/Voice.h"


class opendlv_sensation_Voice_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::sensation;
	
			Voice obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), ""));
	
			obj1.setType("Hello World!");
			obj1.setSize(4000);
			obj1.setData("Hello World!");
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), "Hello World!"));
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::sensation;
	
			Voice obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), ""));
	
			Voice obj2(obj1);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getSize() == obj2.getSize());
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), obj2.getData()));
	
			obj1.setType("Hello World!");
			obj1.setSize(4000);
			obj1.setData("Hello World!");
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), "Hello World!"));
	
			Voice obj3(obj1);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj3.getType()));
			TS_ASSERT(obj1.getSize() == obj3.getSize());
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), obj3.getData()));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj3.getType(), "Hello World!"));
			TS_ASSERT(obj3.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj3.getData(), "Hello World!"));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::sensation;
	
			Voice obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), ""));
	
			Voice obj2;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), ""));
			TS_ASSERT(obj2.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getData(), ""));
	
			obj1.setType("Hello World!");
			obj1.setSize(4000);
			obj1.setData("Hello World!");
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), "Hello World!"));
	
			obj2 = obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getSize() == obj2.getSize());
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), obj2.getData()));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), "Hello World!"));
			TS_ASSERT(obj2.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getData(), "Hello World!"));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::sensation;
	
			Voice obj1;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), ""));
			TS_ASSERT(obj1.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), ""));
	
			Voice obj2;
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), ""));
			TS_ASSERT(obj2.getSize() == 0);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getData(), ""));
	
			obj1.setType("Hello World!");
			obj1.setSize(4000);
			obj1.setData("Hello World!");
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), "Hello World!"));
			TS_ASSERT(obj1.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), "Hello World!"));
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getType(), obj2.getType()));
			TS_ASSERT(obj1.getSize() == obj2.getSize());
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.getData(), obj2.getData()));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getType(), "Hello World!"));
			TS_ASSERT(obj2.getSize() == 4000);
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj2.getData(), "Hello World!"));
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_SENSATION_VOICE_TESTSUITE_H*/
