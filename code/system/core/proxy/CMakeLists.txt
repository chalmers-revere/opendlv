# OpenDLV Proxy - Software component for interfacing with the
#         low level vehicle interface to access sensors
#         and actuators
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

project(opendlv-system-core-proxy)

find_package(OpenCV REQUIRED)

# Set include directories
include_directories(include)
include_directories(${OpenCV_INCLUDE_DIR})

# Set libraries
set(LIBRARIES ${LIBRARIES} ${OpenCV_LIBS})

# Build
file(GLOB_RECURSE thisproject-sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin COMPONENT system-core)
