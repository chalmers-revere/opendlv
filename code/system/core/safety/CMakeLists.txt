# OpenDLV Safety - Software component for system safety.
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

project(opendlv-system-core-safety)

# Set include directories
include_directories(include)

# Set libraries
set(LIBRARIES ${LIBRARIES})

# Build
file(GLOB_RECURSE thisproject-sources 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/checkactuation/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/manualoverride/*.cpp"
  )


add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

add_executable(${PROJECT_NAME}-checkactuation "${CMAKE_CURRENT_SOURCE_DIR}/src/checkactuation/main.cpp")
target_link_libraries(${PROJECT_NAME}-checkactuation ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-manualoverride "${CMAKE_CURRENT_SOURCE_DIR}/src/manualoverride/main.cpp")
target_link_libraries(${PROJECT_NAME}-manualoverride ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME}-checkactuation RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-manualoverride RUNTIME DESTINATION bin COMPONENT system-core)
