#!/bin/bash
#
# Copyright (C) 2016  Christian Berger
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

DIFF=`which diff`
CLANG_FORMAT=`which clang-format-3.6`
CPPCHECK=`which cppcheck`
PMCCABE=`which pmccabe`

if [ "$1" == "" ]; then
    echo "Run as '$0 file'."
    exit 1
fi

# Check formatting style.
if [ "$CLANG_FORMAT" != "" ]; then
    $CLANG_FORMAT $1 > $1.tmp
    RESULT=$($DIFF -q $1 $1.tmp >/dev/null)
    if [ "$RESULT" != "0" ]; then
        echo "[$1:1]: (style) Preferred formatting stlye NOT matching."
    fi
    rm $1.tmp
fi

# Run cppcheck.
if [ "$CPPCHECK" != "" ]; then
    $CPPCHECK --enable=warning,style,performance,portability --language=c++ --std=c++11 --std=posix -q $1
fi

# Run pmccabe.
if [ "$PMCCABE" != "" ]; then
    echo "[$1:1]: (complexity) McCabe complexity, #statements, #lines"
    $PMCCABE -b $1 | tr -s "\t" "," | sed -e 's/\"/\[/' | sed -e 's/\"\,\ line\ /\:/' | sed -e 's/\:\ /\]\: \(complexity\)\ /' | sort -n -t"," -k6 -r | cut -f1,2,4,6 -d"," | sed -e 's/\%\,/\:\ /' | sed -e 's/\,/\,\ /g'
fi

