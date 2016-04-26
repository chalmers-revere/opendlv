/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_ACTUATION_TESTSUITE_H
#define OPENDLV_PROXY_ACTUATION_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/Actuation.h"


class opendlv_proxy_Actuation_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy;
	
			Actuation obj1;
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 0, 1e-5);
			TS_ASSERT(obj1.getIsValid() == false);
	
			obj1.setAcceleration(2.5);
			obj1.setSteering(2.5);
			obj1.setIsValid(true);
	
			TS_ASSERT_DELTA(obj1.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj1.getIsValid() == true);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy;
	
			Actuation obj1;
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 0, 1e-5);
			TS_ASSERT(obj1.getIsValid() == false);
	
			Actuation obj2(obj1);
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), obj2.getSteering(), 1e-5);
			TS_ASSERT(obj1.getIsValid() == obj2.getIsValid());
	
			obj1.setAcceleration(2.5);
			obj1.setSteering(2.5);
			obj1.setIsValid(true);
	
			TS_ASSERT_DELTA(obj1.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj1.getIsValid() == true);
	
			Actuation obj3(obj1);
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj3.getAcceleration(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), obj3.getSteering(), 1e-5);
			TS_ASSERT(obj1.getIsValid() == obj3.getIsValid());
	
			TS_ASSERT_DELTA(obj3.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj3.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj3.getIsValid() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy;
	
			Actuation obj1;
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 0, 1e-5);
			TS_ASSERT(obj1.getIsValid() == false);
	
			Actuation obj2;
			TS_ASSERT_DELTA(obj2.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSteering(), 0, 1e-5);
			TS_ASSERT(obj2.getIsValid() == false);
	
			obj1.setAcceleration(2.5);
			obj1.setSteering(2.5);
			obj1.setIsValid(true);
	
			TS_ASSERT_DELTA(obj1.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj1.getIsValid() == true);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), obj2.getSteering(), 1e-5);
			TS_ASSERT(obj1.getIsValid() == obj2.getIsValid());
	
			TS_ASSERT_DELTA(obj2.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj2.getIsValid() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy;
	
			Actuation obj1;
			TS_ASSERT_DELTA(obj1.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 0, 1e-5);
			TS_ASSERT(obj1.getIsValid() == false);
	
			Actuation obj2;
			TS_ASSERT_DELTA(obj2.getAcceleration(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSteering(), 0, 1e-5);
			TS_ASSERT(obj2.getIsValid() == false);
	
			obj1.setAcceleration(2.5);
			obj1.setSteering(2.5);
			obj1.setIsValid(true);
	
			TS_ASSERT_DELTA(obj1.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj1.getIsValid() == true);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getAcceleration(), obj2.getAcceleration(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSteering(), obj2.getSteering(), 1e-5);
			TS_ASSERT(obj1.getIsValid() == obj2.getIsValid());
	
			TS_ASSERT_DELTA(obj2.getAcceleration(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getSteering(), 2.5, 1e-5);
			TS_ASSERT(obj2.getIsValid() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_ACTUATION_TESTSUITE_H*/
