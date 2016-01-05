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

#ifndef PROXYTESTSUITE_HPP_
#define PROXYTESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

// Include local header files.
#include "../include/proxy.hpp"

/**
 * This class derives from Proxy to allow access to protected methods.
 */
class ProxyTestling : public opendlv::system::Proxy {
  private:
    ProxyTestling();
    
  public:
    ProxyTestling(const int32_t &a_argc, char **a_argv) :
      Proxy(a_argc, a_argv) {}

    // Here, you need to add all methods which are protected in Proxy and
    // which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class ProxyTest : public CxxTest::TestSuite {
  private:
    ProxyTestling *m_pt;

  public:
    ProxyTest() : m_pt(nullptr) {}
    ProxyTest(ProxyTest const &) = delete;
    ProxyTest& operator=(ProxyTest const &) = delete;

    /**
     * This method will be called before each testXYZ-method.
     */
    void setUp() {
      std::string argv0("proxy");
      std::string argv1("--cid=100");
      int32_t argc = 2;
      char **argv;
      argv = new char*[2];
      argv[0] = const_cast<char*>(argv0.c_str());
      argv[1] = const_cast<char*>(argv1.c_str());

      // Create an instance of Proxy through ProxyTestling which 
      // will be deleted in tearDown().
      m_pt = new ProxyTestling(argc, argv);
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
    void testProxySuccessfullyCreated() {
      TS_ASSERT(m_pt != nullptr);
    }
};

#endif
