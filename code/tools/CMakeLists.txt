# OpenDLV - Software for driverless vehicles realized with OpenDaVINCI
# Copyright (C) 2015 Chalmers ReVeRe
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

project(opendlv-tools)

###########################################################################
# Set the search path for .cmake files.
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake.Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${OPENDAVINCI_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})

###########################################################################
# Global compile flags.
include(CompileFlags)

###########################################################################
# Find CxxTest.
include(CheckCxxTestEnvironment)

###########################################################################
# Find OpenDaVINCI.
find_package(OpenDaVINCI REQUIRED)

###########################################################################
# Find odcantools.
set(AUTOMOTIVEDATA_DIR "${OPENDAVINCI_DIR}")
set(ODCANTOOLS_DIR "${OPENDAVINCI_DIR}")
find_package(odcantools REQUIRED)

###########################################################################
# Find OpenDLV.
set(OPENDLV_DIR "${OPENDAVINCI_DIR}")
find_package(OpenDLV REQUIRED)

###########################################################################
# Find reverefh16mapping.
set(REVEREFH16MAPPING_DIR "${CMAKE_INSTALL_PREFIX}")
find_package(reverefh16mapping REQUIRED)

###########################################################################
# Find OpenDLV data structures.
set(OPENDLVDATA_DIR "${CMAKE_INSTALL_PREFIX}")
find_package(opendlvdata REQUIRED)

###########################################################################
# Set common include directories.
include_directories(SYSTEM ${OPENDAVINCI_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENDLV_INCLUDE_DIRS})
include_directories(SYSTEM ${ODCANTOOLS_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENDLVDATA_INCLUDE_DIRS})
include_directories(SYSTEM ${REVEREFH16MAPPING_INCLUDE_DIRS})
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

###########################################################################
# Set libraries to link against.
set(LIBRARIES ${OPENDAVINCI_LIBRARIES}
              ${OPENDLV_LIBRARIES} 
              ${ODCANTOOLS_LIBRARIES}
              ${OPENDLVDATA_LIBRARIES}
              ${REVEREFH16MAPPING_LIBRARIES})

add_subdirectory(can)
add_subdirectory(vboreplay)
add_subdirectory(vision)
add_subdirectory(simulation)
add_subdirectory(truckreplay)
add_subdirectory(dataspy)
