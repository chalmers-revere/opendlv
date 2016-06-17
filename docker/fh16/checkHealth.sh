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
LOCALHOST=127.0.0.1
SCOTT1=10.42.42.1
SCOTT2=10.42.42.60
COMBOX=10.42.42.61
TRIMBLE=10.42.42.112
MULTICAST_ROUTE=225.0.0.0
DOCKER_VOLUME_PREFIX=/from.host
CAN_PORT=2
PROC_PCAN=${DOCKER_VOLUME_PREFIX}/proc/pcan
DEV_PCAN=${DOCKER_VOLUME_PREFIX}/dev/pcan${CAN_PORT}
DEV_PCAN_SPEED=0x011c

# Reachability tests:
HAS_MULTICAST_ROUTE=$(ip route | grep "$MULTICAST_ROUTE" 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_LOCALHOST=$(ping -W1 -c1 $LOCALHOST 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT1=$(ping -W1 -c1 $SCOTT1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT2=$(ping -W1 -c1 $SCOTT1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_TRIMBLE=$(ping -W1 -c1 $TRIMBLE 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_COMBOX=$(ping -W1 -c1 $COMBOX 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# Devnode tests:
HAS_DEV_PCAN=$(test -e $DEV_PCAN 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PCAN_NETDEV_DISABLED=$(cat $PROC_PCAN 2>/dev/null | grep -v "^*" | tr -s " " " " | cut -f4 -d" "| grep -v "^$"| sed "${CAN_PORT}q;d")
PCAN_NETDEV_DISABLED=$(test "$PCAN_NETDEV_DISABLED" == "-NA-" && echo "PASSED" || echo "FAILED")
PCAN_CORRECT_SPEED=$(cat $PROC_PCAN 2>/dev/null | grep -v "^*" | tr -s " " " " | cut -f7 -d" "| grep -v "^$"| sed "${CAN_PORT}q;d")
PCAN_CORRECT_SPEED=$(test "$PCAN_CORRECT_SPEED" == "$DEV_PCAN_SPEED" && echo "PASSED" || echo "FAILED")

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
echo "$(hostname).has.${DEV_PCAN}=$HAS_DEV_PCAN"
echo "$(hostname).has.dev.pcan.no_netdev=$PCAN_NETDEV_DISABLED"
echo "$(hostname).has.dev.pcan.correct_speed=$PCAN_CORRECT_SPEED"
echo "$(hostname).has.multicast_route=$HAS_MULTICAST_ROUTE"
echo "$(hostname).ping.localhost=$PING_LOCALHOST"
echo "$(hostname).ping.scott1=$PING_SCOTT1"
echo "$(hostname).ping.scott2=$PING_SCOTT2"
echo "$(hostname).ping.trimble=$PING_TRIMBLE"
echo "$(hostname).ping.combox=$PING_COMBOX"
echo "$(hostname).diskspace.used=$DISKSPACE_USED"
echo "$(hostname).diskspace.available=$DISKSPACE_LEFT"

