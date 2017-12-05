#!/bin/sh
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

CONTAINERSIO=$1
MESSAGE=$2

if [ "x$CONTAINERSIO" != "x" -a "y$MESSAGE" != "y" ]; then
    cat $CONTAINERSIO | grep "$MESSAGE" | grep "sends" > senders
    cat $CONTAINERSIO | grep "$MESSAGE" | grep "receives" > receivers

    echo "digraph {" > $MESSAGE.dot
    echo "  rankdir=LR;" >> $MESSAGE.dot
    for i in $(cat senders); do
        SENDER=$(echo $i|cut -f1 -d";"|rev|cut -f1 -d"/"|rev|cut -f1 -d".");
        for j in $(cat receivers); do
            RECEIVER=$(echo $j|cut -f1 -d";"|rev|cut -f1 -d"/"|rev|cut -f1 -d".");
            echo "  $SENDER -> $RECEIVER [label=\"$MESSAGE\"];" >> $MESSAGE.dot
        done
    done
    echo "}" >> $MESSAGE.dot
    dot -Tpdf -o $MESSAGE.pdf $MESSAGE.dot
    rm senders receivers
else
    echo "Run as $0 containers.io message"
fi

