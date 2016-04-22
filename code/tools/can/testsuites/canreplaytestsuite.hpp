/**
 * Copyright (C) 2016 Chalmers REVERE
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef CANREPLAYTESTSUITE_HPP_
#define CANREPLAYTESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <opendavinci/odcore/data/Container.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "reverefh16mapping/generated/opendlv/proxy/reverefh16/VehicleState.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

// Include local header files.
#include "../include/replay/ascreplay.hpp"

/**
 * This class derives from opendlv::proxy::can::replay::CANASCReplay
 * to allow access to protected methods.
 */
class CANASCReplayTestling : public opendlv::tools::can::replay::AscReplay {
 private:
  CANASCReplayTestling();

 public:
  CANASCReplayTestling(const int32_t &a_argc, char **a_argv)
      : opendlv::tools::can::replay::AscReplay(a_argc, a_argv)
  {
  }

  // Here, you need to add all methods which are protected in
  // sensor::camera::Camera and which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class CANASCReplayTest : public CxxTest::TestSuite {
 private:
  CANASCReplayTestling *m_pt;

 public:
  CANASCReplayTest()
      : m_pt(nullptr)
  {
  }
  CANASCReplayTest(CANASCReplayTest const &) = delete;
  CANASCReplayTest &operator=(CANASCReplayTest const &) = delete;

  /**
   * This method will be called before each testXYZ-method.
   */
  void setUp()
  {
    std::string argv0("proxy-can-ascreplay");
    std::string argv1("--cid=100");
    int32_t argc = 2;
    char **argv;
    argv = new char *[2];
    argv[0] = const_cast<char *>(argv0.c_str());
    argv[1] = const_cast<char *>(argv1.c_str());

    m_pt = new CANASCReplayTestling(argc, argv);
  }

  /**
   * This method will be called after each testXYZ-method.
   */
  void tearDown()
  {
    delete m_pt;
    m_pt = nullptr;
  }

  //////////////////////////////////////////////////////////////////////////
  // Below this line the actual testcases are defined.
  //////////////////////////////////////////////////////////////////////////
  void testCANASCReplayTestlingSuccessfullyCreated()
  {
    TS_ASSERT(m_pt != nullptr);
  }

  void testDecodeInvalidPayload()
  {
    std::vector<odcore::data::Container> result =
    m_pt->getMessages("Hello World");
    TS_ASSERT(result.size() == 0);
  }

  void testDecodeValidPayload()
  {
    std::string payload(" 279.733972 1  104             Rx   d 6 7F 7D A7 7D "
                        "87 7D  Length = 388000 BitCount = 101");
    std::vector<odcore::data::Container> result = m_pt->getMessages(payload);
    TS_ASSERT(result.size() == 1);
    TS_ASSERT(
    result.at(0).getDataType() == opendlv::proxy::reverefh16::VehicleState::ID());

    opendlv::proxy::reverefh16::VehicleState vd =
    result.at(0).getData<opendlv::proxy::reverefh16::VehicleState>();
    TS_ASSERT_DELTA(vd.getAccelerationX(), 1.17188e-05, 1e-5);
    TS_ASSERT_DELTA(vd.getAccelerationY(), 0.019543, 1e-5);
    TS_ASSERT_DELTA(vd.getYawRate(), 0.00047, 1e-5);
  }

  void testEncodeHighLevelMessage()
  {
    // Create a high-level message.
    opendlv::proxy::reverefh16::VehicleState vd;
    vd.setAccelerationX(1.17188e-05);
    vd.setAccelerationY(0.019543);
    vd.setYawRate(0.00047);

    // Check values therein.
    TS_ASSERT_DELTA(vd.getAccelerationX(), 1.17188e-05, 1e-5);
    TS_ASSERT_DELTA(vd.getAccelerationY(), 0.019543, 1e-5);
    TS_ASSERT_DELTA(vd.getYawRate(), 0.00047, 1e-5);

    // Create the message mapping.
    canmapping::opendlv::proxy::reverefh16::VehicleState vd_mapping;
    // The high-level message needs to be put into a Container.
    odcore::data::Container c(vd);
    automotive::GenericCANMessage gcm = vd_mapping.encode(c);
    TS_ASSERT(gcm.getData() == 0x7F7DA77D877D);

    // Test how the CAN device driver would write the data:
    const uint8_t LENGTH = gcm.getLength();
    uint64_t data = gcm.getData();
    char *da = new char[LENGTH+1];
    for (uint8_t i = 0; i < LENGTH; i++) {
        da[LENGTH-1-i] = (data & 0xFF);
        data = data >> 8;
    }
    da[LENGTH]='\0';
    std::stringstream sstr;
    for(uint8_t i = 0; i < LENGTH; i++) {
      sstr << std::hex << (int)(uint8_t)da[i];
    }
    TS_ASSERT(sstr.str() == "7f7da77d877d");
    delete [] da;
  }
};

#endif
