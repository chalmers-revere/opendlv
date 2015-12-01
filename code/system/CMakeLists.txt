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

project(opendlv-system)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules")

include(CompileFlags)

# OpenDaVINCI
find_package(OpenDaVINCI REQUIRED)

set(OPENDLVDATA_DIR "${CMAKE_INSTALL_PREFIX}")
find_package(OpenDLVData REQUIRED)

# CxxTest
include(CheckCxxTestEnvironment)

# Set common include directories
include_directories(SYSTEM ${OPENDAVINCI_INCLUDE_DIRS}/opendavinci)
include_directories(SYSTEM ${OPENDLVDATA_INCLUDE_DIRS}/opendlvdata)

# Set libraries
set(LIBRARIES ${OPENDAVINCI_LIBRARIES} ${OPENDLVDATA_LIBRARIES})


#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pedantic-errors")


add_subdirectory(core)
add_subdirectory(application)

install(PROGRAMS ${OPENDAVINCI_DIR}/bin/odsupercomponent DESTINATION bin COMPONENT system)
install(FILES ${OPENDAVINCI_DIR}/lib/libopendavinci.so DESTINATION lib COMPONENT system)
install(FILES ${OPENDAVINCI_DIR}/lib/libopendavinci-static.a DESTINATION lib COMPONENT system)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../config/configuration DESTINATION . COMPONENT system)

