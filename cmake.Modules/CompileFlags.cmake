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

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(COMPILER_FLAGS "-fstack-protector-all -fPIC -ggdb -Werror -pedantic-errors -Wall -Wextra")
  set(C_COMPILER_FLAGS "-std=c99")
  set(CXX_COMPILER_FLAGS "-std=c++14 -Weffc++")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(CXX_PICKY_ERRORS "-Warray-bounds -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wdouble-promotion -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wformat=2 -Winline -Winvalid-pch -Wlogical-op -Wlong-long -Wmissing-braces -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wno-deprecated -Wno-maybe-uninitialized -Wnormalized=nfc -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-aliasing=2 -Wsuggest-attribute=const -Wswitch-default -Wswitch-enum -Wsync-nand -Wtrampolines -Wunreachable-code -Wunsafe-loop-optimizations -Wunused -Wvarargs -Wvariadic-macros -Wvector-operation-performance -Wvla -Wwrite-strings")

  set(COMPILER_FLAGS "-fstack-protector-all -fPIC -ggdb -Werror -pedantic-errors -Wall -Wextra")
  set(C_COMPILER_FLAGS "-std=c99")
  set(CXX_COMPILER_FLAGS "-std=c++14 -Weffc++ ${CXX_PICKY_ERRORS}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(COMPILER_FLAGS "/MP /Gy /GR- /Zi /W4 /WX /wd\"4100\" /wd\"4127\" /wd\"4244\" /wd\"4512\" /wd\"4701\" /wd\"4702\" /wd\"4996\"")
endif()

set(CMAKE_C_FLAGS "${COMPILER_FLAGS} ${C_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${CXX_COMPILER_FLAGS}")
