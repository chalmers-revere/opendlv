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
file(GLOB_RECURSE thisproject-sources 
#  "${CMAKE_CURRENT_SOURCE_DIR}/src/android/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/can/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ledstrip/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/relay/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/gps/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/imu/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lidar/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sonararray/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/v2v/*.cpp"
  )


add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

#add_executable(${PROJECT_NAME}-android "${CMAKE_CURRENT_SOURCE_DIR}/src/android/main.cpp")
#target_link_libraries(${PROJECT_NAME}-android ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-can "${CMAKE_CURRENT_SOURCE_DIR}/src/can/main.cpp")
target_link_libraries(${PROJECT_NAME}-can ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-ledstrip "${CMAKE_CURRENT_SOURCE_DIR}/src/ledstrip/main.cpp")
target_link_libraries(${PROJECT_NAME}-ledstrip ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-relay "${CMAKE_CURRENT_SOURCE_DIR}/src/relay/main.cpp")
target_link_libraries(${PROJECT_NAME}-relay ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-camera "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/main.cpp")
target_link_libraries(${PROJECT_NAME}-camera ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-gps "${CMAKE_CURRENT_SOURCE_DIR}/src/gps/main.cpp")
target_link_libraries(${PROJECT_NAME}-gps ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-imu "${CMAKE_CURRENT_SOURCE_DIR}/src/imu/main.cpp")
target_link_libraries(${PROJECT_NAME}-imu ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-lidar "${CMAKE_CURRENT_SOURCE_DIR}/src/lidar/main.cpp")
target_link_libraries(${PROJECT_NAME}-lidar ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sonararray "${CMAKE_CURRENT_SOURCE_DIR}/src/sonararray/main.cpp")
target_link_libraries(${PROJECT_NAME}-sonararray ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-v2v "${CMAKE_CURRENT_SOURCE_DIR}/src/v2v/main.cpp")
target_link_libraries(${PROJECT_NAME}-v2v ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
#install(TARGETS ${PROJECT_NAME}-android RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-can RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-ledstrip RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-relay RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-camera RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-gps RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-imu RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-lidar RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sonararray RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-v2v RUNTIME DESTINATION bin COMPONENT system-core)
