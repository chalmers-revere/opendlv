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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PERCEPTIONTESTSUITE_HPP_
#define PERCEPTIONTESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/perception.hpp"

/**
 * This class derives from Perception to allow access to protected methods.
 */
class PerceptionTestling : public opendlv::system::application::Perception {
  private:
    PerceptionTestling();
    
  public:
    PerceptionTestling(const int32_t &a_argc, char **a_argv) :
      Perception(a_argc, a_argv) {}

    // Here, you need to add all methods which are protected in Perception and
    // which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class PerceptionTest : public CxxTest::TestSuite {
  private:
    PerceptionTestling *m_pt;

  public:
    PerceptionTest() : m_pt(nullptr) {}
    PerceptionTest(PerceptionTest const &) = delete;
    PerceptionTest& operator=(PerceptionTest const &) = delete;

    /**
     * This method will be called before each testXYZ-method.
     */
    void setUp() {
      std::string argv0("perception");
      std::string argv1("--cid=100");
      int32_t argc = 2;
      char **argv;
      argv = new char*[2];
      argv[0] = const_cast<char*>(argv0.c_str());
      argv[1] = const_cast<char*>(argv1.c_str());

      // Create an instance of Perception through PerceptionTestling which 
      // will be deleted in tearDown().
      m_pt = new PerceptionTestling(argc, argv);
    }

    /**
     * This method will be called after each testXYZ-method.
     */
    void tearDown() {
      delete m_pt;
      m_pt = nullptr;
    }

    //////////////////////////////////////////////////////////////////////////
    // Below this line the actual testcases are defined.
    //////////////////////////////////////////////////////////////////////////
    void testPerceptionSuccessfullyCreated() {
      TS_ASSERT(m_pt != nullptr);
    }
};

#endif
