#!/bin/bash
#
# Copyright (C) 2016 Christian Berger
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

# System configuration:
SCOTT1=10.42.42.1
LEFTCAMERA=10.42.42.90
MULTICAST_ROUTE=225.0.0.0
DOCKER_VOLUME_PREFIX=/from.host
DEV_SICK=${DOCKER_VOLUME_PREFIX}/dev/ttyS0

# Reachability tests:
HAS_MULTICAST_ROUTE=$(ip route | grep "$MULTICAST_ROUTE" 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT1=$(ping -W1 -c1 $SCOTT1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_LEFTCAMERA=$(ping -W1 -c1 $LEFTCAMERA 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# Devnode tests:
HAS_DEV_SICK=$(test -e $DEV_SICK 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# System:
KERNEL=$(cat /proc/version | cut -f3 -d" ")
LOADAVERAGE=$(cat /proc/loadavg | cut -f1-3 -d" "|tr -s " " "/")
UPTIME=$(echo "`cat /proc/uptime |cut -f1 -d" "`/60"|bc)

# Disk space:
DISKSPACE_USED=$(df -h | grep "/$"|tr -s " " " "|cut -f3 -d" ")
DISKSPACE_LEFT=$(df -h | grep "/$"|tr -s " " " "|cut -f4 -d" ")

echo "health.host=$(hostname)"
echo "$(hostname).system.kernel=$KERNEL"
echo "$(hostname).system.loadaverage=$LOADAVERAGE"
echo "$(hostname).system.uptime=${UPTIME}min"
echo "$(hostname).has.${DEV_SICK}=$HAS_DEV_SICK"
echo "$(hostname).has.multicast_route=$HAS_MULTICAST_ROUTE"
echo "$(hostname).ping.scott1=$PING_SCOTT1"
echo "$(hostname).ping.leftCamera=$PING_LEFTCAMERA"
echo "$(hostname).diskspace.used=$DISKSPACE_USED"
echo "$(hostname).diskspace.available=$DISKSPACE_LEFT"

