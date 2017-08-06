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
FILES_SENDING_CONTAINERS=$(grep -E "(getConference\(\)|onference)\.send\(" -r .|cut -f1 -d":"|sort|uniq)
for i in $FILES_SENDING_CONTAINERS; do
    :>`basename $i`.lst
    for j in $(grep -E "(getConference\(\)|onference)\.send\(" $i | sed -e 's/.send(/%/g'|cut -f2 -d"%"|cut -f1 -d")"); do
#        echo "File: $i, container: $j"
        CONTAINER_ARG1=$(cat $i | grep "Container $j" | grep -v "//" | cut -f1 -d"," | cut -f2 -d"(" | cut -f1 -d"[" | cut -f1 -d")")
        for k in $CONTAINER_ARG1; do
#            echo "Found type name $k"
            # Find type declaration as a variable previously defined in scope.
            cat $i | grep -G -B150 "Container $j($k" > out
            # Search for constructor instantiation.
            TYPE_PROCESSED=0
            if [ "$TYPE_PROCESSED" == "0" -a "${k:0:2}" == "m_" ]; then
                HEADER=$(echo `dirname $i`/../include/`basename $i|cut -f1 -d"."`.h)
#                echo "$k is probably defined in header file: $HEADER"
                if [ -f $HEADER ]; then
                    MESSAGE=$(cat $HEADER | grep ".*\s.*" | grep -G "$k" -m 1 | sed -e 's/^[[:space:]]*//')
                    if [ "$TYPE_PROCESSED" == "0" -a "$MESSAGE" != "" -a "$(echo $MESSAGE | wc -w)" != "1" ]; then
#                        echo "Message type = $MESSAGE"
                        T=$(echo $MESSAGE | cut -f1 -d" ")
                        echo "$i;sends;$T" >> `basename $i`.lst
                    fi
                else
                    echo "$k is probably defined in header file $HEADER but the file was not found."
                fi
                TYPE_PROCESSED=1
            fi
            MESSAGE=$(tac out | grep -v "Container" | grep -v "//" | grep -v "\[" | grep -v "push_back" | grep -v "return" | grep -v "<<" | grep ".*\s.*" | grep -G " $k(" -m 1 | sed -e 's/^[[:space:]]*//')
            if [ "$TYPE_PROCESSED" == "0" -a "$MESSAGE" != "" -a "$(echo $MESSAGE | wc -w)" != "1" ]; then
#                echo "Message type = $MESSAGE"
                T=$(echo $MESSAGE | cut -f1 -d" ")
                echo "$i;sends;$T" >> `basename $i`.lst
                TYPE_PROCESSED=1
            fi
            # Search for constructor-less instantiation.
            MESSAGE=$(tac out | grep -v "Container" | grep -v "//" | grep -v "\[" | grep -v "push_back" | grep -v "return" | grep -v "<<" | grep ".*\s.*" | grep -G " $k;" -m 1 | sed -e 's/^[[:space:]]*//')
            if [ "$TYPE_PROCESSED" == "0" -a "$MESSAGE" != "" -a "$(echo $MESSAGE | wc -w)" != "1" ]; then
#                echo "Message type = $MESSAGE"
                T=$(echo $MESSAGE | cut -f1 -d" ")
                echo "$i;sends;$T" >> `basename $i`.lst
                TYPE_PROCESSED=1
            fi
            # Search for assignment-based instantiation.
            MESSAGE=$(tac out | grep -v "Container" | grep -v "//" | grep -v "\[" | grep -v "push_back" | grep -v "return" | grep -v "<<" | grep ".*\s.*" | grep -G " $k = " -m 1 | sed -e 's/^[[:space:]]*//')
            if [ "$TYPE_PROCESSED" == "0" -a "$MESSAGE" != "" -a "$(echo $MESSAGE | wc -w)" != "1" ]; then
#                echo "Message type = $MESSAGE"
                T=$(echo $MESSAGE | cut -f1 -d" ")
                echo "$i;sends;$T" >> `basename $i`.lst
                TYPE_PROCESSED=1
            fi
            rm -f out
        done
    done
    cat `basename $i`.lst | sort | uniq >> containers.io && rm -f `basename $i`.lst
done

# Find lines where the content of a Container is queried. Such lines are usually
# found when a Container has been received.
grep ".*getDataType().*==.*::ID()" -r . | sed -e 's/\:.*\=\=//g' | sed -e 's/.{//g' > out
cat out | rev | cut -f3- -d":" | rev | sed -e 's/ /;receives;/' >> containers.io && rm -f out

cat containers.io | sort

