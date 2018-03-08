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

cmake_minimum_required(VERSION 2.8)

project(opendlv)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake.Modules" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${OPENDAVINCI_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})

include(ExternalProject)

include(FindJava REQUIRED)
include(FindodDataStructureGenerator REQUIRED)

set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

if(NOT("${DPKG_EXECUTABLE}" STREQUAL "DPKG_EXECUTABLE-NOTFOUND")
    AND NOT("${RPMBUILD_EXECUTABLE}" STREQUAL "RPMBUILD_EXECUTABLE-NOTFOUND")
    AND ("${PACKAGING_ENABLED}" STREQUAL "YES") )
  message(STATUS "Enabling building .deb and .rpm packages.")
  set(MAKE_PACKAGE package)
else()
  # If the platform does not provide dpkg and rpmbuild, make package is
  # replaced with make help to simply have a no-op.
  set(MAKE_PACKAGE help)
endif()

ExternalProject_Add (libodvdopendlvdata-cleanup
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/libodvdopendlvdata
  PATCH_COMMAND ""
  SOURCE_DIR ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  TEST_COMMAND ""
  INSTALL_COMMAND "")

ExternalProject_Add (libodvdopendlvdata
  DEPENDS libodvdopendlvdata-cleanup
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && ${Java_JAVA_EXECUTABLE} -jar ${ODDATASTRUCTUREGENERATOR_EXECUTABLE} --withCMake ${CMAKE_CURRENT_SOURCE_DIR}/resources/odvd/ODVDOpenDLVData.odvd
  PATCH_COMMAND ""
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/libodvdopendlvdata"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
  CMAKE_ARGS "-DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}"
  CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}"
  CMAKE_ARGS "-DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}"
  TEST_BEFORE_INSTALL 1
  TEST_COMMAND ${CMAKE_CTEST_COMMAND} ${CTEST_PARAMETERS}
  INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libodvdopendlvdata-prefix/src/libodvdopendlvdata-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libodvdopendlvdata-prefix/src/libodvdopendlvdata-build --target ${MAKE_PACKAGE})

ExternalProject_Add (gui
  DEPENDS libodvdopendlvdata
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/gui"
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
    CMAKE_ARGS "-DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}"
    CMAKE_ARGS "-DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}"
  TEST_BEFORE_INSTALL 1
  INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/gui-prefix/src/gui-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/gui-prefix/src/gui-build --target ${MAKE_PACKAGE})

ExternalProject_Add (logic
  DEPENDS libodvdopendlvdata
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/logic"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
  CMAKE_ARGS "-DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}"
  CMAKE_ARGS "-DODVDOPENDLVSTANDARDMESSAGESET_DIR=${ODVDOPENDLVSTANDARDMESSAGESET_DIR}"
  CMAKE_ARGS "-DODVDV2V_DIR=${ODVDV2V_DIR}"
  CMAKE_ARGS "-DODVDVEHICLE_DIR=${ODVDVEHICLE_DIR}"
  CMAKE_ARGS "-DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}"
  CMAKE_ARGS "-DEIGEN3_INCLUDE_DIR=${OPENDAVINCI_DIR}/include/opendavinci"
  CMAKE_ARGS "-DKALMAN_INCLUDE_DIR=${KALMAN_INCLUDE_DIR}"
  CMAKE_ARGS "-DTINYDNN_INCLUDE_DIR=${TINYDNN_INCLUDE_DIR}"
  TEST_BEFORE_INSTALL 1
  INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/logic-prefix/src/logic-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/logic-prefix/src/logic-build --target ${MAKE_PACKAGE})

ExternalProject_Add (tool
  DEPENDS libodvdopendlvdata
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/tool"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
  CMAKE_ARGS "-DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}"
  CMAKE_ARGS "-DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}"
  TEST_BEFORE_INSTALL 1
  INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/tool-prefix/src/tool-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/tool-prefix/src/tool-build --target ${MAKE_PACKAGE})
