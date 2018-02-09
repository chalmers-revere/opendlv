# OpenDLV - Software for driverless vehicles realized with OpenDaVINCI
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

project(opendlv-logic)

###########################################################################
# Set the search path for .cmake files.
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake.Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${OPENDAVINCI_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})
SET (CMAKE_MODULE_PATH "${ODVDOPENDLVSTANDARDMESSAGESET_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})

###########################################################################
# Global compile flags.
include(CompileFlags)

###########################################################################
# Find CxxTest.
include(CheckCxxTestEnvironment)

###########################################################################
# Find OpenDaVINCI.
find_package(OpenDaVINCI REQUIRED)
set(OPENDLV_DIR ${OPENDAVINCI_DIR})
find_package(OpenDLV REQUIRED)
set(AUTOMOTIVEDATA_DIR ${OPENDAVINCI_DIR})
find_package(AutomotiveData REQUIRED)

###########################################################################
# Find OpenCV.
IF( (EXISTS "/usr/pkg/include/opencv") OR (EXISTS "/usr/pkg/include/opencv2") )
    SET(OPENCV_ROOT_DIR "/usr/pkg")
ENDIF()
FIND_PACKAGE(OpenCV REQUIRED)

###############################################################################
# Set header files from OpenCV.
INCLUDE_DIRECTORIES (SYSTEM ${OpenCV_INCLUDE_DIRS})

###########################################################################
# Find ODVDOpenDLVData.
find_package(ODVDOpenDLVData REQUIRED)

###########################################################################
# Find ODVDopendlv.
find_package(ODVDOpenDLVStandardMessageSet REQUIRED)

###########################################################################
# Set common include directories.
include_directories(SYSTEM ${OPENDAVINCI_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENDLV_INCLUDE_DIRS})
include_directories(SYSTEM ${AUTOMOTIVEDATA_INCLUDE_DIRS})
include_directories(SYSTEM ${ODVDOPENDLVDATA_INCLUDE_DIRS})
include_directories(SYSTEM ${ODVDOPENDLVSTANDARDMESSAGESET_INCLUDE_DIRS})
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

###########################################################################
# Set libraries to link against.
set(LIBRARIES ${OPENDAVINCI_LIBRARIES}
  ${OPENDLV_LIBRARIES}
  ${AUTOMOTIVEDATA_LIBRARIES}
  ${ODVDOPENDLVSTANDARDMESSAGESET_LIBRARIES}
  ${ODVDOPENDLVDATA_LIBRARIES})

###########################################################################
# Add subfolders with sources.
add_subdirectory(action/act)
add_subdirectory(action/communicate)
add_subdirectory(action/keepobjectalignment)
add_subdirectory(action/keepobjectsize)
add_subdirectory(action/setgroundspeed)
add_subdirectory(action/setopticalrotation)
add_subdirectory(knowledge/identity)
add_subdirectory(knowledge/scene)
add_subdirectory(knowledge/v2vcam)
add_subdirectory(knowledge/v2vdenm)
add_subdirectory(legacy/simpledriver)
add_subdirectory(perception/detectlane)
add_subdirectory(perception/detectvehicle)
add_subdirectory(sensation/audition)
add_subdirectory(sensation/directionofmovement)
add_subdirectory(sensation/echolocation)
add_subdirectory(sensation/geolocator)
add_subdirectory(sensation/opticalflow)

###########################################################################
# Create packages for selected Linux distributions.
set(CHANGELOG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../ChangeLog")
include(CreatePackages)
