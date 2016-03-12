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

project(opendlv-system-application-sensation)

# Set include directories
include_directories(include)
include_directories(${EKF_lib_INCLUDE_DIR})


# Set libraries
set(LIBRARIES ${LIBRARIES})

# Build
file(GLOB_RECURSE thisproject-sources
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sensation.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/audition/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/echolocation/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/geolocation/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/proprioception/orientation/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/proprioception/wheel/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/color/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/depth/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/directionofmovement/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/edge/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/looming/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/monochrome/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/opticalflow/*.cpp"
  )

add_library(${PROJECT_NAME}lib-static STATIC ${thisproject-sources})

#add_dependencies(${PROJECT_NAME}lib-static EKF_lib)# --- all the libs should be built before the application
#add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
#target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}lib-static ${LIBRARIES} ) 

# Mauro's code:
add_executable(${PROJECT_NAME}-sensation-mauro "${CMAKE_CURRENT_SOURCE_DIR}/src/sensation-mauro.cpp")
target_link_libraries(${PROJECT_NAME}-sensation-mauro ${PROJECT_NAME}lib-static ${LIBRARIES}) 


add_executable(${PROJECT_NAME}-audition "${CMAKE_CURRENT_SOURCE_DIR}/src/audition/main.cpp")
target_link_libraries(${PROJECT_NAME}-audition ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-echolocation "${CMAKE_CURRENT_SOURCE_DIR}/src/echolocation/main.cpp")
target_link_libraries(${PROJECT_NAME}-echolocation ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-geolocation "${CMAKE_CURRENT_SOURCE_DIR}/src/geolocation/main.cpp")
target_link_libraries(${PROJECT_NAME}-geolocation ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-proprioception-orientation "${CMAKE_CURRENT_SOURCE_DIR}/src/proprioception/orientation/main.cpp")
target_link_libraries(${PROJECT_NAME}-proprioception-orientation ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-proprioception-wheel "${CMAKE_CURRENT_SOURCE_DIR}/src/proprioception/wheel/main.cpp")
target_link_libraries(${PROJECT_NAME}-proprioception-wheel ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-color "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/color/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-color ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-depth "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/depth/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-depth ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-directionofmovement "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/directionofmovement/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-directionofmovement ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-edge "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/edge/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-edge ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-looming "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/looming/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-looming ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-monochrome "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/monochrome/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-monochrome ${PROJECT_NAME}lib-static ${LIBRARIES}) 

add_executable(${PROJECT_NAME}-vision-opticalflow "${CMAKE_CURRENT_SOURCE_DIR}/src/vision/opticalflow/main.cpp")
target_link_libraries(${PROJECT_NAME}-vision-opticalflow ${PROJECT_NAME}lib-static ${LIBRARIES}) 


# Test
include(RunTests)

# Install
install(TARGETS ${PROJECT_NAME}-sensation-mauro RUNTIME DESTINATION bin COMPONENT system-application)

install(TARGETS ${PROJECT_NAME}-audition RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-echolocation RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-geolocation RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-proprioception-orientation RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-proprioception-wheel RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-color RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-depth RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-directionofmovement RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-edge RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-looming RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-monochrome RUNTIME DESTINATION bin COMPONENT system-application)
install(TARGETS ${PROJECT_NAME}-vision-opticalflow RUNTIME DESTINATION bin COMPONENT system-application)
