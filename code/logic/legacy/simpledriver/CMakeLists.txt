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

project(opendlv-logic-legacy-simpledriver)

include_directories(include)

set (CMAKE_MODULE_PATH "${ODVDOPENDLVSTANDARDMESSAGESET_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})
find_package (ODVDOpenDLVStandardMessageSet REQUIRED)
include_directories(SYSTEM ${ODVDOPENDLVSTANDARDMESSAGESET_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${ODVDOPENDLVSTANDARDMESSAGESET_LIBRARIES})

set (CMAKE_MODULE_PATH "${ODVDVEHICLE_DIR}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules" ${CMAKE_MODULE_PATH})
find_package (ODVDVehicle REQUIRED)
include_directories(SYSTEM ${ODVDVEHICLE_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${ODVDVEHICLE_LIBRARIES})

###############################################################################
# Build this project.
file(GLOB_RECURSE thisproject-sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME}-static STATIC ${thisproject-sources})
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/apps/${PROJECT_NAME}.cpp")
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static ${LIBRARIES}) 

include(RunTests)

###############################################################################
# Install this project.
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin COMPONENT ${CMAKE_PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}-static DESTINATION lib COMPONENT ${CMAKE_PROJECT_NAME})
install(FILES man/${PROJECT_NAME}.1 DESTINATION man/man1 COMPONENT ${CMAKE_PROJECT_NAME})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include/${CMAKE_PROJECT_NAME} COMPONENT ${CMAKE_PROJECT_NAME})

