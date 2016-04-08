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

#ifndef SAFETYTESTSUITE_HPP_
#define SAFETYTESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/safety.hpp"

/**
 * This class derives from Safety to allow access to protected methods.
 */
class SafetyTestling : public opendlv::system::Safety {
 private:
  SafetyTestling();

 public:
  SafetyTestling(const int32_t &a_argc, char **a_argv)
      : Safety(a_argc, a_argv)
  {
  }

  // Here, you need to add all methods which are protected in Safety and
  // which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class SafetyTest : public CxxTest::TestSuite {
 private:
  SafetyTestling *m_st;

 public:
  SafetyTest()
      : m_st(nullptr)
  {
  }
  SafetyTest(SafetyTest const &) = delete;
  SafetyTest &operator=(SafetyTest const &) = delete;

  /**
   * This method will be called before each testXYZ-method.
   */
  void setUp()
  {
    std::string argv0("safety");
    std::string argv1("--cid=100");
    int32_t argc = 2;
    char **argv;
    argv = new char *[2];
    argv[0] = const_cast<char *>(argv0.c_str());
    argv[1] = const_cast<char *>(argv1.c_str());

    // Create an instance of Safety through SafetyTestling which
    // will be deleted in tearDown().
    m_st = new SafetyTestling(argc, argv);
  }

  /**
   * This method will be called after each testXYZ-method.
   */
  void tearDown()
  {
    delete m_st;
    m_st = nullptr;
  }

  //////////////////////////////////////////////////////////////////////////
  // Below this line the actual testcases are defined.
  //////////////////////////////////////////////////////////////////////////
  void testSafetySuccessfullyCreated()
  {
    TS_ASSERT(m_st != nullptr);
  }
};

#endif
