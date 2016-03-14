/**
 * Copyright (C) 2015 Chalmers REVERE
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

#ifndef SENSORCAMERATESTSUITE_HPP_
#define SENSORCAMERATESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/sensor/camera/camera.hpp"

/**
 * This class derives from sensor::camera::Camera to allow access to protected
 * methods.
 */
class SensorCameraTestling : public opendlv::proxy::sensor::camera::Camera {
 private:
  SensorCameraTestling();

 public:
  SensorCameraTestling(const int32_t &a_argc, char **a_argv)
      : Camera(a_argc, a_argv)
  {
  }

  // Here, you need to add all methods which are protected in
  // sensor::camera::Camera and which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class SensorCameraTest : public CxxTest::TestSuite {
 private:
  SensorCameraTestling *m_pt;

 public:
  SensorCameraTest()
      : m_pt(nullptr)
  {
  }
  SensorCameraTest(SensorCameraTest const &) = delete;
  SensorCameraTest &operator=(SensorCameraTest const &) = delete;

  /**
   * This method will be called before each testXYZ-method.
   */
  void setUp()
  {
    std::string argv0("proxy-sensor-camera");
    std::string argv1("--cid=100");
    int32_t argc = 2;
    char **argv;
    argv = new char *[2];
    argv[0] = const_cast<char *>(argv0.c_str());
    argv[1] = const_cast<char *>(argv1.c_str());

    // Create an instance of Camera through SensorCameraTestling which
    // will be deleted in tearDown().
    m_pt = new SensorCameraTestling(argc, argv);
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
  void testSensorCameraSuccessfullyCreated()
  {
    TS_ASSERT(m_pt != nullptr);
  }
};

#endif
