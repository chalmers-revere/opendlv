/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_GPSREADING_TESTSUITE_H
#define OPENDLV_PROXY_GPSREADING_TESTSUITE_H

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "opendavinci/odcore/opendavinci.h"
#include "opendavinci/odcore/strings/StringToolbox.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"
#include "opendlvdata/generated/opendlv/proxy/GpsReading.h"


class opendlv_proxy_GpsReading_TestSuite : public CxxTest::TestSuite {

	public:
		void testCreateObject() {
			using namespace opendlv::proxy;
	
			GpsReading obj1;
			TS_ASSERT_DELTA(obj1.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == false);
			TS_ASSERT(obj1.getHasRtk() == false);
	
			obj1.setTimestamp(1.0);
			obj1.setLatitude(1.0);
			obj1.setLongitude(1.0);
			obj1.setAltitude(2.5);
			obj1.setNorthHeading(2.5);
			obj1.setSpeed(2.5);
			obj1.setLatitudeDirection(4);
			obj1.setLongitudeDirection(4);
			obj1.setSatelliteCount(4);
			obj1.setHasHeading(true);
			obj1.setHasRtk(true);
	
			TS_ASSERT_DELTA(obj1.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == true);
			TS_ASSERT(obj1.getHasRtk() == true);
		}
	
		void testCreateAndCopyObject() {
			using namespace opendlv::proxy;
	
			GpsReading obj1;
			TS_ASSERT_DELTA(obj1.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == false);
			TS_ASSERT(obj1.getHasRtk() == false);
	
			GpsReading obj2(obj1);
			TS_ASSERT_DELTA(obj1.getTimestamp(), obj2.getTimestamp(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), obj2.getLatitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), obj2.getLongitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), obj2.getAltitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), obj2.getNorthHeading(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), obj2.getSpeed(), 1e-5);
			TS_ASSERT(obj1.getHasHeading() == obj2.getHasHeading());
			TS_ASSERT(obj1.getHasRtk() == obj2.getHasRtk());
	
			obj1.setTimestamp(1.0);
			obj1.setLatitude(1.0);
			obj1.setLongitude(1.0);
			obj1.setAltitude(2.5);
			obj1.setNorthHeading(2.5);
			obj1.setSpeed(2.5);
			obj1.setLatitudeDirection(4);
			obj1.setLongitudeDirection(4);
			obj1.setSatelliteCount(4);
			obj1.setHasHeading(true);
			obj1.setHasRtk(true);
	
			TS_ASSERT_DELTA(obj1.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == true);
			TS_ASSERT(obj1.getHasRtk() == true);
	
			GpsReading obj3(obj1);
			TS_ASSERT_DELTA(obj1.getTimestamp(), obj3.getTimestamp(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), obj3.getLatitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), obj3.getLongitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), obj3.getAltitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), obj3.getNorthHeading(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), obj3.getSpeed(), 1e-5);
			TS_ASSERT(obj1.getHasHeading() == obj3.getHasHeading());
			TS_ASSERT(obj1.getHasRtk() == obj3.getHasRtk());
	
			TS_ASSERT_DELTA(obj3.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj3.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj3.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj3.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj3.getHasHeading() == true);
			TS_ASSERT(obj3.getHasRtk() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj3.toString()));
		}
	
		void testCreateAndAssignObject() {
			using namespace opendlv::proxy;
	
			GpsReading obj1;
			TS_ASSERT_DELTA(obj1.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == false);
			TS_ASSERT(obj1.getHasRtk() == false);
	
			GpsReading obj2;
			TS_ASSERT_DELTA(obj2.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj2.getHasHeading() == false);
			TS_ASSERT(obj2.getHasRtk() == false);
	
			obj1.setTimestamp(1.0);
			obj1.setLatitude(1.0);
			obj1.setLongitude(1.0);
			obj1.setAltitude(2.5);
			obj1.setNorthHeading(2.5);
			obj1.setSpeed(2.5);
			obj1.setLatitudeDirection(4);
			obj1.setLongitudeDirection(4);
			obj1.setSatelliteCount(4);
			obj1.setHasHeading(true);
			obj1.setHasRtk(true);
	
			TS_ASSERT_DELTA(obj1.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == true);
			TS_ASSERT(obj1.getHasRtk() == true);
	
			obj2 = obj1;
			TS_ASSERT_DELTA(obj1.getTimestamp(), obj2.getTimestamp(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), obj2.getLatitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), obj2.getLongitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), obj2.getAltitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), obj2.getNorthHeading(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), obj2.getSpeed(), 1e-5);
			TS_ASSERT(obj1.getHasHeading() == obj2.getHasHeading());
			TS_ASSERT(obj1.getHasRtk() == obj2.getHasRtk());
	
			TS_ASSERT_DELTA(obj2.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj2.getHasHeading() == true);
			TS_ASSERT(obj2.getHasRtk() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}
	
		void testCreateAndSerializeObject() {
			using namespace opendlv::proxy;
	
			GpsReading obj1;
			TS_ASSERT_DELTA(obj1.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == false);
			TS_ASSERT(obj1.getHasRtk() == false);
	
			GpsReading obj2;
			TS_ASSERT_DELTA(obj2.getTimestamp(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLatitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLongitude(), 0.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAltitude(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getNorthHeading(), 0, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeed(), 0, 1e-5);
			TS_ASSERT(obj2.getHasHeading() == false);
			TS_ASSERT(obj2.getHasRtk() == false);
	
			obj1.setTimestamp(1.0);
			obj1.setLatitude(1.0);
			obj1.setLongitude(1.0);
			obj1.setAltitude(2.5);
			obj1.setNorthHeading(2.5);
			obj1.setSpeed(2.5);
			obj1.setLatitudeDirection(4);
			obj1.setLongitudeDirection(4);
			obj1.setSatelliteCount(4);
			obj1.setHasHeading(true);
			obj1.setHasRtk(true);
	
			TS_ASSERT_DELTA(obj1.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj1.getHasHeading() == true);
			TS_ASSERT(obj1.getHasRtk() == true);
	
			stringstream sstr;
			sstr << obj1;
			sstr >> obj2;
	
			TS_ASSERT_DELTA(obj1.getTimestamp(), obj2.getTimestamp(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLatitude(), obj2.getLatitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getLongitude(), obj2.getLongitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getAltitude(), obj2.getAltitude(), 1e-5);
			TS_ASSERT_DELTA(obj1.getNorthHeading(), obj2.getNorthHeading(), 1e-5);
			TS_ASSERT_DELTA(obj1.getSpeed(), obj2.getSpeed(), 1e-5);
			TS_ASSERT(obj1.getHasHeading() == obj2.getHasHeading());
			TS_ASSERT(obj1.getHasRtk() == obj2.getHasRtk());
	
			TS_ASSERT_DELTA(obj2.getTimestamp(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLatitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getLongitude(), 1.0, 1e-5);
			TS_ASSERT_DELTA(obj2.getAltitude(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getNorthHeading(), 2.5, 1e-5);
			TS_ASSERT_DELTA(obj2.getSpeed(), 2.5, 1e-5);
			TS_ASSERT(obj2.getHasHeading() == true);
			TS_ASSERT(obj2.getHasRtk() == true);
	
			TS_ASSERT(odcore::strings::StringToolbox::equalsIgnoreCase(obj1.toString(), obj2.toString()));
		}

};

#endif /*OPENDLV_PROXY_GPSREADING_TESTSUITE_H*/
