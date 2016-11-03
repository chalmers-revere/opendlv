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
# Find libstdc++.
FIND_LIBRARY(CPPEXPERIMENTAL_LIBRARY
             NAMES stdc++fs
             PATHS ${LIBCPPEXPERIMENTAL}/lib/
                    /usr/lib/gcc/x86_64-linux-gnu/5
                    /usr/lib/gcc/x86_64-linux-gnu/6
                    /usr/lib/x86_64-linux-gnu/
                    /usr/local/lib64/
                    /usr/lib64/
                    /usr/lib/)
MARK_AS_ADVANCED(CPPEXPERIMENTAL_LIBRARY)

###########################################################################
IF (CPPEXPERIMENTAL_LIBRARY)
    SET(CPPEXPERIMENTAL_FOUND 1)
    SET(CPPEXPERIMENTAL_LIBRARIES ${CPPEXPERIMENTAL_LIBRARY})
ENDIF()

MARK_AS_ADVANCED(CPPEXPERIMENTAL_LIBRARIES)

IF (CPPEXPERIMENTAL_FOUND)
    MESSAGE(STATUS "Found libstdc++fs: ${CPPEXPERIMENTAL_LIBRARIES}")
ELSE ()
    MESSAGE(STATUS "Could not find libstdc++fs.")
ENDIF()

