#!/bin/bash

# incrementalBuild.sh - Script to build OpenDLV.
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

BUILD_AS=$1
UID_AS=$2

# Adding user for building.
groupadd $BUILD_AS
useradd $BUILD_AS -g $BUILD_AS

cat <<EOF > /opt/opendlv.build/build.sh
#!/bin/bash
export PATH=/usr/lib/ccache:$PATH
export CCACHE_DIR=/opt/ccache

cd /opt/opendlv.build

echo "[opendlv Docker builder] Incremental build."

mkdir -p build.system && cd build.system
PATH=/opt/od4/bin:$PATH cmake -D CXXTEST_INCLUDE_DIR=/opt/opendlv.sources/thirdparty/cxxtest -D OPENDAVINCI_DIR=/opt/od4 -D CMAKE_INSTALL_PREFIX=/opt/opendlv /opt/opendlv.sources/code/core/system

make -j4 && make test && make install
EOF

chmod 755 /opt/opendlv.build/build.sh
chown $UID_AS:$UID_AS /opt/opendlv.build/build.sh
chown -R $UID_AS:$UID_AS /opt

su -m `getent passwd $UID_AS|cut -f1 -d":"` -c /opt/opendlv.build/build.sh

