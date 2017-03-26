# OpenDLV - OpenDLV based on OpenDaVINCI
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


# Enable CPack to create .deb and .rpm.
# Read version from first line of ChangeLog
file(STRINGS "${CHANGELOG_PATH}" BUILD_NUMBER)
macro(setup_package_version_variables PACKAGENAME)
  string(REGEX MATCHALL "[0-9]+" VERSION_COMPONENTS "${PACKAGENAME}")
  list(LENGTH VERSION_COMPONENTS LEN)
  if(${LEN} GREATER 0)
    list(GET VERSION_COMPONENTS 0 MAJOR)
  endif()
  if(${LEN} GREATER 1)
    list(GET VERSION_COMPONENTS 1 MINOR)
  endif()
  if(${LEN} GREATER 2)
    list(GET VERSION_COMPONENTS 2 PATCH)
  endif()
endmacro()
setup_package_version_variables(${BUILD_NUMBER})

if((UNIX)
    AND (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "DragonFly")
    AND (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "OpenBSD")
    AND (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "NetBSD") )
  set(CPACK_GENERATOR "DEB;RPM")

  set(CPACK_PACKAGE_CONTACT "Christian Berger")
  set(CPACK_PACKAGE_VENDOR "${CPACK_PACKAGE_CONTACT}")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenDLV drives autonomous vehicles.")
  set(CPACK_PACKAGE_NAME "opendlv")
  set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PATCH}")
  set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  set(CPACK_COMPONENTS_ALL opendlv)

  # Debian packages:
  set(CPACK_DEBIAN_PACKAGE_SECTION "main")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  if("${ARMHF}" STREQUAL "YES")
    set(ARCH "armhf")
  else()
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
      set(ARCH "amd64")
    else()
      set(ARCH "i386")
    endif()
  endif()
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${ARCH}")
  set(CPACK_DEB_COMPONENT_INSTALL ON)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "opendavinci-lib")

  # RPM packages:
  if("${ARMHF}" STREQUAL "YES")
    set(ARCH "armhf")
  else()
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
      set(ARCH "x86_64")
    else()
      set(ARCH "i686")
    endif()
  endif()
  set(CPACK_RPM_PACKAGE_BUILDARCH "Buildarch: ${CPACK_RPM_PACKAGE_ARCHITECTURE}")
  set(CPACK_RPM_COMPONENT_INSTALL ON)
  set(CPACK_RPM_PACKAGE_LICENSE "GPL")
  set(CPACK_RPM_PACKAGE_REQUIRES "opendavinci-lib")

  # Resulting package name:
  set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${ARCH})
endif()

include(CPack)
