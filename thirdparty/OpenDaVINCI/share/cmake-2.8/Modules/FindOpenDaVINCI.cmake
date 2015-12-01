# OpenDaVINCI - Portable middleware for distributed components.
# Copyright (C) 2008 - 2015  Christian Berger
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

###########################################################################
# Try to find OpenDaVINCI library.
#
# The user can specify an additional search path using the CMake variable
# OPENDAVINCI_DIR

# First, search at the specific user path setting.
IF(NOT ("${OPENDAVINCI_DIR}" STREQUAL ""))
    FIND_PATH(OPENDAVINCI_INCLUDE_DIR core/platform.h
                 NAMES opendavinci
                 PATHS ${OPENDAVINCI_DIR}/include
                 NO_DEFAULT_PATH)

    FIND_LIBRARY(OPENDAVINCI_LIBRARY
                 NAMES opendavinci opendavinci-static
                 PATHS ${OPENDAVINCI_DIR}/lib
                 NO_DEFAULT_PATH)
ENDIF()
IF(   ("${OPENDAVINCI_INCLUDE_DIR}" STREQUAL "OPENDAVINCI_INCLUDE_DIR-NOTFOUND")
   OR ("${OPENDAVINCI_DIR}" STREQUAL "") )
    MESSAGE(STATUS "Trying to find OpenDAVINCI in default paths.")
    # If not found, use the system's search paths.
    FIND_PATH(OPENDAVINCI_INCLUDE_DIR core/platform.h
                 NAMES opendavinci
                 PATHS /usr/include
                       /usr/local/include)

    FIND_LIBRARY(OPENDAVINCI_LIBRARY
                 NAMES opendavinci opendavinci-static
                 PATHS /usr/lib
                       /usr/lib64
                       /usr/local/lib
                       /usr/local/lib64)
ENDIF()

IF("${OPENDAVINCI_INCLUDE_DIR}" STREQUAL "")
    MESSAGE(FATAL_ERROR "Could not find OpenDaVINCI library.")
ELSE()
    SET (FOUND_OPENDAVINCI 1)
ENDIF()

###########################################################################
# Artifacts based on OpenDaVINCI need to link against a threading library.
FIND_PACKAGE (Threads REQUIRED)
FIND_PACKAGE (LibRT)
IF(    (LIBRT_FOUND)
   AND ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") )
    MESSAGE(STATUS "Using RT on Linux for libopendavinci.")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LINUX_RT")
ENDIF()

###########################################################################
# Set linking libraries.
SET(OPENDAVINCI_LIBRARIES ${OPENDAVINCI_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
IF(LIBRT_FOUND)
    SET(OPENDAVINCI_LIBRARIES ${OPENDAVINCI_LIBRARIES} ${LIBRT_LIBRARIES})
ENDIF()

# On Mac OS X, we need to add CoreFoundation and IOKit.
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    FIND_LIBRARY(COREFOUNDATION_FRAMEWORK CoreFoundation)
    FIND_LIBRARY(IOKIT_FRAMEWORK IOKit)
    SET (OPENDAVINCI_LIBRARIES ${OPENDAVINCI_LIBRARIES} ${COREFOUNDATION_FRAMEWORK} ${IOKIT_FRAMEWORK})
ENDIF()

# On Windows, we need to link against setupapi.
IF(WIN32)
    SET(OPENDAVINCI_LIBRARIES ${OPENDAVINCI_LIBRARIES} setupapi)
ENDIF()
SET(OPENDAVINCI_INCLUDE_DIRS ${OPENDAVINCI_INCLUDE_DIR})

###########################################################################
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibOpenDaVINCI DEFAULT_MSG
                                  OPENDAVINCI_LIBRARY OPENDAVINCI_INCLUDE_DIR)
MARK_AS_ADVANCED(OPENDAVINCI_INCLUDE_DIR OPENDAVINCI_LIBRARY)

