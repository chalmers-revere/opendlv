#!/bin/bash
# OpenDaVINCI - Portable middleware for distributed components.
# Copyright (C) 2017  Christian Berger
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

:>containers.io
FILES_SENDING_CONTAINERS=$(grep "getConference().send(" -r .|cut -f1 -d":"|sort|uniq)
for i in $FILES_SENDING_CONTAINERS; do
    :>`basename $i`.lst
    for j in $(grep "getConference().send(" $i | sed -e 's/.send(/%/g'|cut -f2 -d"%"|cut -f1 -d")"); do
        #echo "File: $i, container: $j"
        CONTAINER_ARG1=$(cat $i | grep "Container $j" | grep -v "//" | cut -f1 -d"," | cut -f2 -d"(" | cut -f1 -d"[" | cut -f1 -d")")
        #echo "Type name $CONTAINER_ARG1"
        cat $i | grep -G -B50 "Container $j($CONTAINER_ARG1" > out
        MESSAGE=$(tac out | grep -v "Container" | grep -v "//" | grep -v "\[" | grep -v "push_back" | grep -v "<<" | grep ".*\s.*" | grep -G " $CONTAINER_ARG1(" -m 1 | sed -e 's/^[[:space:]]*//')
        if [ "$MESSAGE" != "" -a "$(echo $MESSAGE | wc -w)" != "1" ]; then
            #echo "Message type = $MESSAGE"
            T=$(echo $MESSAGE | cut -f1 -d" ")
            echo "$i;sends;$T" >> `basename $i`.lst
        fi
        rm -f out
    done
    cat `basename $i`.lst | sort | uniq >> containers.io && rm -f `basename $i`.lst
done

# Find lines where the content of a Container is queried. Such lines are usually
# found when a Container has been received.
#grep ".*getDataType().*==.*::ID()" -r . | sed -e 's/\:.*\=\=//g' | sed -e 's/.{//g' > containers.received
grep ".*getDataType().*==.*::ID()" -r . | sed -e 's/\:.*\=\=//g' | sed -e 's/.{//g' > out
cat out | rev | cut -f3- -d":" | rev | sed -e 's/ /;receives;/' >> containers.io && rm -f out

cat containers.io | sort

