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
PING_LOCALHOST=$(ping -W1 -c1 127.0.0.1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT1=$(ping -W1 -c1 10.42.42.1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_SCOTT2=$(ping -W1 -c1 10.42.42.2 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
MULTICAST_ROUTE=$(route -n | grep "225.0.0.0" 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# Devnode tests:
DEVNODE_PCAN=$(test -e /dev/pcan 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

echo "diagnostics-health.ping.localhost=$PING_LOCALHOST"
echo "diagnostics-health.ping.scott1=$PING_SCOTT1"
echo "diagnostics-health.ping.scott2=$PING_SCOTT2"
echo "diagnostics-health.multicast.route=$MULTICAST_ROUTE"
echo "diagnostics-health.devnode.pcan=$DEVNODE_PCAN"

