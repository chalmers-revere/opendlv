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
include_directories(${OPENCV_INCLUDE_DIRS})

# Set libraries
set(LIBRARIES ${LIBRARIES} ${OPENCV_LIBRARIES})

# Build
file(GLOB_RECURSE thisproject-sources 
  #  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/propulsion/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/brake/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/steering/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/ledstrip/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/toggeler/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/v2v/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/camera/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/gps/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/imu/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/lidar/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/sonararray/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/v2v/*.cpp"
  )


add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

add_executable(${PROJECT_NAME}-actuator-propulsion "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/propulsion/main.cpp")
target_link_libraries(${PROJECT_NAME}-actuator-propulsion ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-actuator-brake "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/brake/main.cpp")
target_link_libraries(${PROJECT_NAME}-actuator-brake ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-actuator-steering "${CMAKE_CURRENT_SOURCE_DIR}/src/actuator/steering/main.cpp")
target_link_libraries(${PROJECT_NAME}-actuator-steering ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-communicator-ledstrip "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/ledstrip/main.cpp")
target_link_libraries(${PROJECT_NAME}-communicator-ledstrip ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-communicator-toggeler "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/toggeler/main.cpp")
target_link_libraries(${PROJECT_NAME}-communicator-toggeler ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-communicator-v2v "${CMAKE_CURRENT_SOURCE_DIR}/src/communicator/v2v/main.cpp")
target_link_libraries(${PROJECT_NAME}-communicator-v2v ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-camera "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/camera/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-camera ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-gps "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/gps/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-gps ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-imu "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/imu/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-imu ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-lidar "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/lidar/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-lidar ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-sonararray "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/sonararray/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-sonararray ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-sensor-v2v "${CMAKE_CURRENT_SOURCE_DIR}/src/sensor/v2v/main.cpp")
target_link_libraries(${PROJECT_NAME}-sensor-v2v ${PROJECT_NAME}lib-static ${LIBRARIES}) 

# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME}-actuator-propulsion RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-actuator-brake RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-actuator-steering RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-communicator-ledstrip RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-communicator-toggeler RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-communicator-v2v RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-camera RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-gps RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-imu RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-lidar RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-sonararray RUNTIME DESTINATION bin COMPONENT system-core)
install(TARGETS ${PROJECT_NAME}-sensor-v2v RUNTIME DESTINATION bin COMPONENT system-core)
