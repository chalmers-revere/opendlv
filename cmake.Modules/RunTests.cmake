# OpenDLV - OpenDLV based on OpenDaVINCI
# Copyright (C) 2017 Chalmers Revere
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

if(CXXTEST_FOUND)
  file(GLOB TESTSUITES "${CMAKE_CURRENT_SOURCE_DIR}/testsuites/*.h")
    
  foreach(TESTSUITE ${TESTSUITES})
    string(REPLACE "/" ";" TESTSUITE_LIST ${TESTSUITE})

    list(LENGTH TESTSUITE_LIST len)
    math(EXPR lastItem "${len}-1")
    list(GET TESTSUITE_LIST "${lastItem}" TESTSUITE-FILENAME)

    set(CXXTEST_TESTGEN_ARGS ${CXXTEST_TESTGEN_ARGS} --world=${PROJECT_NAME}-${TESTSUITE-FILENAME})
    CXXTEST_ADD_TEST(${TESTSUITE-FILENAME}-TestSuite ${TESTSUITE-FILENAME}-TestSuite.cpp ${TESTSUITE})
    if(UNIX)
      if((("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
          OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
          OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "DragonFly"))
          AND (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        set_source_files_properties(${TESTSUITE-FILENAME}-TestSuite.cpp PROPERTIES COMPILE_FLAGS "-Wno-effc++ -Wno-float-equal -Wno-error=switch-default -Wno-error=suggest-attribute=noreturn")
      else()
        set_source_files_properties(${TESTSUITE-FILENAME}-TestSuite.cpp PROPERTIES COMPILE_FLAGS "-Wno-effc++ -Wno-float-equal")
      endif()
    endif()
    if(WIN32)
      set_source_files_properties(${TESTSUITE-FILENAME}-TestSuite.cpp PROPERTIES COMPILE_FLAGS "")
    endif()
    set_tests_properties(${TESTSUITE-FILENAME}-TestSuite PROPERTIES TIMEOUT 3000)
    target_link_libraries(${TESTSUITE-FILENAME}-TestSuite ${PROJECT_NAME}-static ${LIBRARIES})
  endforeach()
endif(CXXTEST_FOUND)
