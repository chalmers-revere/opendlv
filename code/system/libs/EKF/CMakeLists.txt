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

# Note: This CMake file is not standalone, it defines the library EKF

project(EKF_lib)   #define the project 

set (lib_SOURCE_DIR ${PROJECT_SOURCE_DIR} )
set (EKF_lib_INCLUDE_DIR ${lib_SOURCE_DIR}/include CACHE PATH "Path to the headers of EKF_lib")


#select all headers in the include dir
FILE(GLOB LIB_HEADERS "${lib_SOURCE_DIR}/include/*.hpp")


# add library 
#add_library(${PROJECT_NAME}  STATIC ${LIB_SOURCES} ${LIB_HEADERS} )  #static for now, think about shared
# not necessary for header only

#add_definitions(-DEKF_LIB) 
#target_link_libraries(${PROJECT_NAME}  ${----} )

# set the postfix _d for debug libraries 
#set_target_properties(${PROJECT_NAME}  PROPERTIES DEBUG_POSTFIX "_d")

# Test
include(RunTests)

# Install
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin COMPONENT system-libs)    #this does not work !


# allows folders for MSVC
#if (MSVC AND ENABLE_SOLUTION_FOLDERS) 
#	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "libraries")
#endif (MSVC AND ENABLE_SOLUTION_FOLDERS)



