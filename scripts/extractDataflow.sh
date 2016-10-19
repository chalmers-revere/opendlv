#!/bin/bash
# OpenDaVINCI - Portable middleware for distributed components.
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

cat <<EOF >../extractSending.awk
BEGIN { RS = "--" ; FS = "\n" }

{
    # Skip first $1
    split(\$2, FIRSTLINE, " ")
    SOURCE=gensub(/\-/, "", "g", FIRSTLINE[1])
    TYPE=FIRSTLINE[2]
    split(FIRSTLINE[3], NAME, "(")

    split(\$3, SECONDLINE, " ")
    CONTAINER1=SECONDLINE[3]
    CONTAINER2=SECONDLINE[4]
    CONTAINER3=SECONDLINE[5]
    CONTAINER4=SECONDLINE[6]

    if ( (SOURCE != "" ) && (TYPE != "") && (NAME[1] != "") && (CONTAINER1 != "") ) {
        print SOURCE, TYPE, NAME[1], CONTAINER1, CONTAINER2, CONTAINER3, CONTAINER4
    }
}
EOF

# Get data about sending Containers.
grep "getConference().send(" -B2 -r . | gawk -f ../extractSending.awk > containers.sent && rm -f ../extractSending.awk

grep "==.*::ID()" -r . | sed -e 's/\:.*\=\=//g' | sed -e 's/.{//g' > containers.received


