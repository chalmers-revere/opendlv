# OpenDLV Action - Software component for decision-making and autonomy
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

project(opendlv-system-application-action)

# Set include directories
include_directories(include)

# Set libraries
set(LIBRARIES ${LIBRARIES})

# Build
file(GLOB_RECURSE thisproject-sources 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/act/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/communicate/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/increaseobjectmargin/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/keepobjectalignment/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/keepobjectsize/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/removeloomingfront/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/removeloomingrear/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/setgroundspeed/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/setopticalrotation/*.cpp"
  )

add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

add_executable(${PROJECT_NAME}-act "${CMAKE_CURRENT_SOURCE_DIR}/src/act/main.cpp")
target_link_libraries(${PROJECT_NAME}-act ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-communicate "${CMAKE_CURRENT_SOURCE_DIR}/src/communicate/main.cpp")
target_link_libraries(${PROJECT_NAME}-communicate ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-increaseobjectmargin "${CMAKE_CURRENT_SOURCE_DIR}/src/increaseobjectmargin/main.cpp")
target_link_libraries(${PROJECT_NAME}-increaseobjectmargin ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-keepobjectalignment "${CMAKE_CURRENT_SOURCE_DIR}/src/keepobjectalignment/main.cpp")
target_link_libraries(${PROJECT_NAME}-keepobjectalignment ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-keepobjectsize "${CMAKE_CURRENT_SOURCE_DIR}/src/keepobjectsize/main.cpp")
target_link_libraries(${PROJECT_NAME}-keepobjectsize ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-removeloomingfront "${CMAKE_CURRENT_SOURCE_DIR}/src/removeloomingfront/main.cpp")
target_link_libraries(${PROJECT_NAME}-removeloomingfront ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-removeloomingrear "${CMAKE_CURRENT_SOURCE_DIR}/src/removeloomingrear/main.cpp")
target_link_libraries(${PROJECT_NAME}-removeloomingrear ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-setgroundspeed "${CMAKE_CURRENT_SOURCE_DIR}/src/setgroundspeed/main.cpp")
target_link_libraries(${PROJECT_NAME}-setgroundspeed ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-setopticalrotation "${CMAKE_CURRENT_SOURCE_DIR}/src/setopticalrotation/main.cpp")
target_link_libraries(${PROJECT_NAME}-setopticalrotation ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME}-act RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-communicate RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-increaseobjectmargin RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-keepobjectalignment RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-keepobjectsize RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-removeloomingfront RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-removeloomingrear RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-setgroundspeed RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-setopticalrotation RUNTIME DESTINATION bin COMPONENT system-application)
