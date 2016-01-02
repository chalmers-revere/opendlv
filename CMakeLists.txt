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

# PATH=/opt/od/bin:$PATH cmake -D CMAKE_INSTALL_PREFIX=/opt/ReVeRe -D OPENDAVINCI_DIR=/opt/od -D CXXTEST_INCLUDE_DIR=/home/msv/GITHUB/OpenDaVINCI/cxxtest/ ..

project(opendlv)

###########################################################################
# Enable the configuration of external projects, as used below.
include(ExternalProject)

# Java is needed to run the odDataStructureGenerator tool.
include(FindJava REQUIRED)

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

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

###########################################################################
# Set the search path for .cmake files.
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake.Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${OPENDAVINCI_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})


###########################################################################
# OpenDaVINCI
include(FindodDataStructureGenerator REQUIRED)
find_package(OpenDaVINCI REQUIRED)

# CxxTest
if("${CXXTEST_INCLUDE_DIR}" STREQUAL "")
  message(STATUS "No CXXTEST_INCLUDE_DIR was supplied, using the default (./thirdparty/cxxtest) directory.")
  set(CXXTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/cxxtest) 
endif()

include(CheckCxxTestEnvironment REQUIRED)

# Install output
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
  message(STATUS "No CMAKE_INSTALL_PREFIX was supplied, using the default (./build/out) directory.")
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/out)
endif()

set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})



# This "external project" deletes the existing libopendlvdata.
ExternalProject_Add(libopendlvdata-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata
    PATCH_COMMAND ""
    SOURCE_DIR ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND "")

# This "external project" generates the data structures.
ExternalProject_Add(libopendlvdata
    DEPENDS libopendlvdata-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR}/code/generated && ${Java_JAVA_EXECUTABLE} -jar ${ODDATASTRUCTUREGENERATOR_EXECUTABLE} --withCMake ${CMAKE_SOURCE_DIR}/resources/OpenDLVData.odvd
    PATCH_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target ${MAKE_PACKAGE})


# Build and install the system software stack.
ExternalProject_Add(system
    DEPENDS libopendlvdata
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/system"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
               -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
               -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target ${MAKE_PACKAGE})

