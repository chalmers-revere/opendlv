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

# To compile OpenDLV using OpenDaVINCI installed at /opt/od, call CMake like:
#
# PATH=/opt/od/bin:$PATH cmake -D CMAKE_INSTALL_PREFIX=/opt/od ..

project(opendlv)

# Enable the configuration of external projects, as used below.
include(ExternalProject)

# Java is needed to run the odDataStructureGenerator tool.
include(FindJava REQUIRED)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

# Set the search path for .cmake files.
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake.Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake-2.8/Modules" ${CMAKE_MODULE_PATH})
#SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata/cmake.Modules" ${CMAKE_MODULE_PATH})

SET (SEARCH_PATH_FOR_CMAKE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/cmake.Modules:${CMAKE_INSTALL_PREFIX}/share/cmake-2.8/Modules)
if(NOT ("${OPENDAVINCI_DIR}" STREQUAL ""))
    SET (SEARCH_PATH_FOR_CMAKE_MODULES ${SEARCH_PATH_FOR_CMAKE_MODULES}:${OPENDAVINCI_DIR}/share/cmake-2.8/Modules)
endif()


# OpenDaVINCI
find_package(OpenDaVINCI REQUIRED)
include(FindodDataStructureGenerator REQUIRED)

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

# TODO: Improvement to generated CMakeLists.txt is possible, remove line 37 and instead add as CMAKE_ARGS
# Workaround: Hardcoded path to OpenDaVINCI in FindOpenDaVINCI

# TODO: Improvement to generated CMakeLists.txt is possible, it assumes relative path to cxxtest
# Workaround: Install cxxtest on system(?), seems to work.

# This "external project" generates the data structures.
ExternalProject_Add(libopendlvdata
    DEPENDS libopendlvdata-cleanup
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR}/code/generated && ${Java_JAVA_EXECUTABLE} -jar ${ODDATASTRUCTUREGENERATOR_EXECUTABLE} --withCMake ${CMAKE_SOURCE_DIR}/resources/OpenDLVData.odvd
    PATCH_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/generated/libopendlvdata"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
    CMAKE_ARGS -DCMAKE_MODULE_PATH=${PROJECT_ROOT}/cmake.Modules:${SEARCH_PATH_FOR_CMAKE_MODULES}:${CMAKE_MODULE_PATH}
    CMAKE_ARGS -DPROJECT_ROOT=${PROJECT_ROOT}
    CMAKE_ARGS -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
    CMAKE_ARGS -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/libopendlvdata-prefix/src/libopendlvdata-build --target ${MAKE_PACKAGE})


# Build and install the system software stack.
ExternalProject_Add(system
    DEPENDS libopendlvdata
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/code/system"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
    CMAKE_ARGS -DCMAKE_MODULE_PATH=${PROJECT_ROOT}/cmake.Modules:${SEARCH_PATH_FOR_CMAKE_MODULES}:${CMAKE_MODULE_PATH}
    CMAKE_ARGS -DPROJECT_ROOT=${PROJECT_ROOT}
    CMAKE_ARGS -DOPENDAVINCI_DIR=${OPENDAVINCI_DIR}
    CMAKE_ARGS -DCXXTEST_INCLUDE_DIR=${CXXTEST_INCLUDE_DIR}
    TEST_BEFORE_INSTALL 1
    INSTALL_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target install COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/system-prefix/src/system-build --target ${MAKE_PACKAGE})

