# OpenDLV Sensation - Software component for vehicle sensing.
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

project(opendlv-system-application-knowledge)

# Set include directories
include_directories(include)

# Set libraries
set(LIBRARIES ${LIBRARIES})

# Build
file(GLOB_RECURSE thisproject-sources 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/gcdc16/rule/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/gcdc16/v2viclcm/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/identity/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/linguistics/v2vcam/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/linguistics/v2vdenm/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/scene/*.cpp"
  )

add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

add_executable(${PROJECT_NAME}-gcdc16-rule "${CMAKE_CURRENT_SOURCE_DIR}/src/gcdc16/rule/main.cpp")
target_link_libraries(${PROJECT_NAME}-gcdc16-rule ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-gcdc16-v2viclcm "${CMAKE_CURRENT_SOURCE_DIR}/src/gcdc16/v2viclcm/main.cpp")
target_link_libraries(${PROJECT_NAME}-gcdc16-v2viclcm ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-identity "${CMAKE_CURRENT_SOURCE_DIR}/src/identity/main.cpp")
target_link_libraries(${PROJECT_NAME}-identity ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-linguistics-v2vcam "${CMAKE_CURRENT_SOURCE_DIR}/src/linguistics/v2vcam/main.cpp")
target_link_libraries(${PROJECT_NAME}-linguistics-v2vcam ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-linguistics-v2vdenm "${CMAKE_CURRENT_SOURCE_DIR}/src/linguistics/v2vdenm/main.cpp")
target_link_libraries(${PROJECT_NAME}-linguistics-v2vdenm ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-scene "${CMAKE_CURRENT_SOURCE_DIR}/src/scene/main.cpp")
target_link_libraries(${PROJECT_NAME}-scene ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME}-gcdc16-rule RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-gcdc16-v2viclcm RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-identity RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-linguistics-v2vcam RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-linguistics-v2vdenm RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-scene RUNTIME DESTINATION bin COMPONENT system-application)
