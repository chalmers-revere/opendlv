/**
 * Copyright (C) 2017 Chalmers Revere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_TESTSUITE_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_TESTSUITE_HPP

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/orbmap.hpp"


class OrbMapTest : public CxxTest::TestSuite {
   public:
    void setUp() {}

    void tearDown() {}

    void testConstructor() {
        opendlv::logic::sensation::OrbMap orbMap();
    }
    void testAddOrbKeyFrame() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbFrame> keyFrame;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbKeyFrame(keyFrame));
    }
    void testAddOrbMapPoint() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbMapPoint> mapPoint;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbMapPoint(mapPoint));
    }
    void testSetReferenceMapPoints() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbMapPoint> mapPoint;
        std::vector<std::shared_ptr<opendlv::logic::sensation::OrbMapPoint>> referenceMapPoints;
        referenceMapPoints.push_back(mapPoint);
        TS_ASSERT_THROWS_NOTHING(orbMap.SetReferenceMapPoints(referenceMapPoints));
    }
    void testOrbKeyFramesCount() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbFrame> keyFrame;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbKeyFrame(keyFrame));
        TS_ASSERT_EQUALS(orbMap.OrbKeyFramesCount(), 1);
    }
    void testOrbMapPointsCount() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbMapPoint> mapPoint;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbMapPoint(mapPoint));
        TS_ASSERT_EQUALS(orbMap.OrbMapPointsCount(), 1);
    }
    void testDeleteOrbKeyFrame() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbFrame> keyFrame;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbKeyFrame(keyFrame));
        TS_ASSERT_EQUALS(orbMap.OrbKeyFramesCount(), 1);
        TS_ASSERT_THROWS_NOTHING(orbMap.DeleteOrbKeyFrame(keyFrame));
        TS_ASSERT_EQUALS(orbMap.OrbKeyFramesCount(), 0);
    }
    void testDeleteOrbMapPoint() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbMapPoint> mapPoint;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbMapPoint(mapPoint));
        TS_ASSERT_EQUALS(orbMap.OrbMapPointsCount(), 1);
        TS_ASSERT_THROWS_NOTHING(orbMap.DeleteOrbMapPoint(mapPoint));
        TS_ASSERT_EQUALS(orbMap.OrbMapPointsCount(), 0);
    }
    void testReset() {
        opendlv::logic::sensation::OrbMap orbMap;
        std::shared_ptr<opendlv::logic::sensation::OrbMapPoint> mapPoint;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbMapPoint(mapPoint));
        TS_ASSERT_EQUALS(orbMap.OrbMapPointsCount(), 1);

        std::shared_ptr<opendlv::logic::sensation::OrbFrame> keyFrame;
        TS_ASSERT_THROWS_NOTHING(orbMap.PushOrbKeyFrame(keyFrame));
        TS_ASSERT_EQUALS(orbMap.OrbKeyFramesCount(), 1);
        
        std::vector<std::shared_ptr<opendlv::logic::sensation::OrbMapPoint>> referenceMapPoints;
        referenceMapPoints.push_back(mapPoint);
        TS_ASSERT_THROWS_NOTHING(orbMap.SetReferenceMapPoints(referenceMapPoints));

        orbMap.Reset();
        TS_ASSERT_EQUALS(orbMap.OrbMapPointsCount(), 0);
        TS_ASSERT_EQUALS(orbMap.OrbKeyFramesCount(), 0);

    }
};

#endif