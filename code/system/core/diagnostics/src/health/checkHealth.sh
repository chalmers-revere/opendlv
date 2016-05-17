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

# Reachability tests:
HAS_MULTICAST_ROUTE=$(route -n | grep "225.0.0.0" 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_LOCALHOST=$(ping -W1 -c1 127.0.0.1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT1=$(ping -W1 -c1 10.42.42.1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT2=$(ping -W1 -c1 10.42.42.60 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_TRIMBLE=$(ping -W1 -c1 10.42.42.112 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_COMBOX=$(ping -W1 -c1 10.42.42.61 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# Devnode tests:
HAS_DEV_PCAN=$(test -e /dev/pcan 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

echo "has.multicast_route=$HAS_MULTICAST_ROUTE"
echo "ping.localhost=$PING_LOCALHOST"
echo "ping.scott1=$PING_SCOTT1"
echo "ping.scott2=$PING_SCOTT2"
echo "ping.trimble=$PING_TRIMBLE"
echo "ping.combox=$PING_COMBOX"
echo "has.dev.pcan=$HAS_DEV_PCAN"

