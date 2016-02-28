# Makefile - Makefile to create a Docker image.
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

PRODUCT=opendlv
GITVERSION=$(shell git describe --abbrev=0 --tags | head -n1)
GITHASH=$(shell git rev-parse --short HEAD)
GITUNCOMMITTEDCHANGES=$(shell if [ "`git ls-files -m ..`" != "" ]; then echo "-modified"; else echo ""; fi)
VERSION="$(GITVERSION)-$(GITHASH)$(GITUNCOMMITTEDCHANGES)"
REPOSITORY=seresearch

BUILDLOG=build.log

all: build-incremental docker-tag-latest

build/Makefile:
	rm -fr build opendlv && mkdir opendlv && mkdir build && cd build && PATH=/opt/od/bin:$$PATH cmake -D OPENDAVINCI_DIR=/opt/od -D CMAKE_INSTALL_PREFIX=../opendlv ../.. && make

build-incremental: build/Makefile
	cd build && find . -name "system*-build" -type f -exec rm -f {} \; && find . -name "tools*-build" -type f -exec rm -f {} \; && make && cd ..

clean:
	rm -f $(BUILDLOG)
	rm -fr build opendlv

docker: build-incremental docker-update-image
	docker build -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1

docker-update-image:
	docker pull $(shell awk '/^FROM/ { print $$2; exit }' Dockerfile)

docker-tag-latest: docker
	docker tag -f $(shell tail -1 $(BUILDLOG) | cut -f3 -d" ") $(REPOSITORY)/$(PRODUCT):latest

docker-cleanup:
	docker rm $$(docker ps -a -q)

run-example1: docker-tag-latest
	cd example1 && docker-compose up
