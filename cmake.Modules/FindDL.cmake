# OpenDaVINCI - Portable middleware for distributed components.
# Copyright (C) 2008 - 2016  Christian Berger
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
# Find libdl.
FIND_PATH(DL_INCLUDE_DIR
          NAMES dlfcn.h
          PATHS /usr/local/include
                /usr/include)
MARK_AS_ADVANCED(DL_INCLUDE_DIR)
FIND_LIBRARY(DL_LIBRARY
             NAMES dl
             PATHS ${LIBDL}/lib/
                    /usr/lib/x86_64-linux-gnu/
                    /usr/local/lib64/
                    /usr/lib64/
                    /usr/lib/)
MARK_AS_ADVANCED(DL_LIBRARY)

###########################################################################
IF (DL_INCLUDE_DIR
    AND DL_LIBRARY)
    SET(DL_FOUND 1)
    SET(DL_LIBRARIES ${DL_LIBRARY})
    SET(DL_INCLUDE_DIRS ${DL_INCLUDE_DIR})
ENDIF()

MARK_AS_ADVANCED(DL_LIBRARIES)
MARK_AS_ADVANCED(DL_INCLUDE_DIRS)

IF (DL_FOUND)
    MESSAGE(STATUS "Found dl: ${DL_INCLUDE_DIRS}, ${DL_LIBRARIES}")
ELSE ()
    MESSAGE(STATUS "Could not find dl; disabling dynamic loading of shared objects.")
ENDIF()

