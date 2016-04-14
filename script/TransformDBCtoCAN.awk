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

# Run this script as: cat <MyDBCFile.dbc> | gawk -f TransformDBCtoCAN.awk

# Extract identifier from the message encoded in BO_.
/^BO_/ {
    HEX=$2
    NAME=gensub(/\:/, "", "g", $3)
}

# Match " SG_" signals and transform them to odCANDataStructureGenerator format.
/^.SG_ / {
    INPUT=gensub(/\:/, " ", "g", $0)
    INPUT=gensub(/\|/, " ", "g", INPUT)
    INPUT=gensub(/\ /, " ", "g", INPUT)
    INPUT=gensub(/@/, " ", "g", INPUT)
    INPUT=gensub(/\+/, " ", "g", INPUT)
    INPUT=gensub(/\(/, " ", "g", INPUT)
    INPUT=gensub(/\)/, " ", "g", INPUT)
    INPUT=gensub(/\[/, " ", "g", INPUT)
    INPUT=gensub(/\]/, " ", "g", INPUT)
    INPUT=gensub(/\,/, " ", "g", INPUT)

    split(INPUT, SIGNAL, " ")
    if (SIGNAL[5] == "0") {
        ENDIAN="big"
    }
    else {
        ENDIAN="little"
    }

    printf("%s.%s in 0x%x at bit %s for %s bit is %s endian multiply by %s add %s with range [%s, %s];\n",tolower(NAME),tolower(SIGNAL[2]),HEX,SIGNAL[3],SIGNAL[4],ENDIAN,SIGNAL[6],SIGNAL[7],SIGNAL[8],SIGNAL[9])
}
