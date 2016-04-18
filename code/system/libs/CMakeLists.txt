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

project(opendlv-system-libs)

include (${CMAKE_SOURCE_DIR}/libs/macro_list_subdirectories.cmake)
# Enumerate all subdirs and process them:
list_subdirectories(LIST_LIBS_DIRS "${CMAKE_SOURCE_DIR}/libs")

foreach(dir ${LIST_LIBS_DIRS})
   SET(VAR_NAME "LIBS_BUILD_${dir}")
   SET(${VAR_NAME} ON CACHE BOOL "Build the lib ${dir}")
   MARK_AS_ADVANCED(${VAR_NAME})

   if (${VAR_NAME})
      add_subdirectory(${dir})
   endif (${VAR_NAME})
endforeach(dir)
