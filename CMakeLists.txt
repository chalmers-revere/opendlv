# OpenDLV - Software for driverless vehicles realized with OpenDaVINCI
# Copyright (C) 2015 Chalmers REVERE
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

# To create a proper build environment, you are supposed to call CMake as follows:
#
# PATH=/opt/od/bin:$PATH cmake -D CMAKE_INSTALL_PREFIX=<Where shall OpenDLV be installed?> 
#                              -D OPENDAVINCI_DIR=<Where is OpenDaVINCI installed?> 
#                              -D CXXTEST_INCLUDE_DIR=<Where is CxxTest installed?> ..
#
# The adjusted PATH variable is required to find odDataStructureGenerator-latest.jar.
# CMAKE_INSTALL_PREFIX specifies where to install OpenDLV.
# OPENDAVINCI_DIR specifies where OpenDaVINCI is installed (if different from /usr/include and /usr/lib).
# CXXTEST_INCLUDE_DIR specifies where the UnitTest environment CxxTest can be found.

project(opendlv)

###########################################################################
# If dpkg and rpmbuild are available, .deb and .rpm packages will be
# created. Otherwise, the software is simply installed.
IF(     NOT("${DPKG_EXECUTABLE}" STREQUAL "DPKG_EXECUTABLE-NOTFOUND")
    AND NOT("${RPMBUILD_EXECUTABLE}" STREQUAL "RPMBUILD_EXECUTABLE-NOTFOUND") )
    MESSAGE(STATUS "Enabling building .deb and .rpm packages.")
#    SET(MAKE_PACKAGE package) # TODO: Enable packaging for code/system.
    SET(MAKE_PACKAGE install)
ELSE()
    # If the platform does not provide dpkg and rpmbuild, make package is
    # replaced with make install.
    SET(MAKE_PACKAGE install)
ENDIF()

###########################################################################
# Set the search path for .cmake files.
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake.Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${OPENDAVINCI_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})

###########################################################################
# Enable the configuration of external projects, as used below.
include(ExternalProject)

###########################################################################
# Java is needed to run the odDataStructureGenerator tool
# for generating the data structures.
include(FindJava REQUIRED)

###########################################################################
# Find odDataStructureGenerator.
include(FindodDataStructureGenerator REQUIRED)

###########################################################################
# Find odCANDataStructureGenerator.
include(FindodCANDataStructureGenerator REQUIRED)

###########################################################################
# Find OpenDaVINCI.
find_package(OpenDaVINCI REQUIRED)

###########################################################################
# Find CxxTest.
if("${CXXTEST_INCLUDE_DIR}" STREQUAL "")
  message(STATUS "No CXXTEST_INCLUDE_DIR was supplied, using the default (./thirdparty/cxxtest) directory.")
  set(CXXTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/cxxtest) 
endif()
include(CheckCxxTestEnvironment REQUIRED)

###########################################################################
# Find Eigen3.
if("${EIGEN3_INCLUDE_DIR}" STREQUAL "")
  message(STATUS "No EIGEN3_INCLUDE_DIR was supplied, using the default (./thirdparty/eigen3) directory.")
  set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/eigen3) 
endif()

###########################################################################
# Find Kalman.
if("${KALMAN_INCLUDE_DIR}" STREQUAL "")
  message(STATUS "No KALMAN_INCLUDE_DIR was supplied, using the default (./thirdparty/kalman/include) directory.")
  set(KALMAN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/kalman/include) 
endif()

###########################################################################
# Find tiny_cnn.
if("${TINYCNN_INCLUDE_DIR}" STREQUAL "")
  message(STATUS "No TINYCNN_INCLUDE_DIR was supplied, using the default (./thirdparty/tiny-cnn) directory.")
  set(TINYCNN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/tiny-cnn) 
endif()

###########################################################################
# Global project variables.
set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
message(STATUS "Installing OpenDLV to ${INSTALL_DIR}.")
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

###########################################################################
###########################################################################
# This "external project" deletes any previously generated data structures libopendlvdata.
ExternalProject_Add(libopendlvdata-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata
    PATCH_COMMAND ""
    SOURCE_DIR ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND "")

###########################################################################
# This "external project" generates the data structures in libopendlvdata.
ExternalProject_Add(libopendlvdata
    DEPENDS libopendlvdata-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR}/code/generated && ${Java_JAVA_EXECUTABLE} -jar ${ODDATASTRUCTUREGENERATOR_EXECUTABLE} --withCMake ${CMAKE_SOURCE_DIR}/resource/odvd/opendlvdata.odvd
    PATCH_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target ${MAKE_PACKAGE})

###########################################################################
# This "external project" deletes any previously generated data structures libopendlvdata.
ExternalProject_Add(libreverefh16mapping-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/code/generated/libreverefh16mapping
    PATCH_COMMAND ""
    SOURCE_DIR ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND "")

###########################################################################
# This "external project" generates the data structures in libopendlvdata.
ExternalProject_Add(libreverefh16mapping
    DEPENDS libreverefh16mapping-cleanup libopendlvdata
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR}/code/generated && ${Java_JAVA_EXECUTABLE} -jar ${ODCANDATASTRUCTUREGENERATOR_EXECUTABLE} --withCMake ${CMAKE_SOURCE_DIR}/resource/can/reverefh16/reverefh16mapping.can
    PATCH_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/generated/libreverefh16mapping"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libreverefh16mapping-prefix/src/libreverefh16mapping-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libreverefh16mapping-prefix/src/libreverefh16mapping-build --target ${MAKE_PACKAGE})

###########################################################################
# This "external project" builds and installs the system software stack.
ExternalProject_Add(system
    DEPENDS libopendlvdata libreverefh16mapping
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/system"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
               -DEIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}
               -DKALMAN_INCLUDE_DIR=${KALMAN_INCLUDE_DIR}
               -DTINYCNN_INCLUDE_DIR=${TINYCNN_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target ${MAKE_PACKAGE})

###########################################################################
# This "external project" builds and installs the tools software stack.
ExternalProject_Add(tools
    DEPENDS libopendlvdata libreverefh16mapping
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/tools"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
               -DEIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/tools-prefix/src/tools-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/tools-prefix/src/tools-build --target ${MAKE_PACKAGE})

