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
VERSION=$(shell cat VERSION)
REPOSITORY=seresearch

BUILDLOG=build.log

build:
	rm -fr build opendlv && mkdir opendlv && mkdir build && cd build && PATH=/opt/od/bin:$$PATH cmake -D OPENDAVINCI_DIR=/opt/od -D CMAKE_INSTALL_PREFIX=../opendlv ../.. && make

build-incremental:
	cd build && find . -name "*-build" -type f -exec rm -f {} \; && make && cd ..

clean:
	rm -f $(BUILDLOG)
	rm -fr build opendlv

docker:
	docker build -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1

docker-tag-latest:
	docker tag -f $(shell tail -1 $(BUILDLOG) | cut -f3 -d" ") $(REPOSITORY)/$(PRODUCT):latest

#push: docker-tag-latest
#	docker push $(REPOSITORY)/$(PRODUCT):$(VERSION) && docker push $(REPOSITORY)/$(PRODUCT):latest && docker rmi $(REPOSITORY)/$(PRODUCT):$(VERSION) && docker rmi $(REPOSITORY)/$(PRODUCT):latest

