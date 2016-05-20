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

# COMMAND FOR BUILDING INSIDE IMAGE:
# docker run --name build_container \
# 			-v $(OPENDLV_HOST_PATH):/opt/data \
# 			-w /opt/data \
# 			-e "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:"$(OPENDAVINCI_PATH)"/bin" \
# 			-e "LD_LIBRARY_PATH="$(OPENDAVINCI_PATH)"/lib" \
# 			-it $(REPOSITORY)/$(PRODUCT):latest \
# 			sh -c "mkdir -p build && cd build && cmake -D CMAKE_INSTALL_PREFIX="$(OPENDLV_PATH)" -D OPENDAVINCI_DIR="$(OPENDAVINCI_PATH)" .. && make clean && make"

PRODUCT=opendlv
GITVERSION=$(shell git describe --abbrev=0 --tags | head -n1)
GITHASH=$(shell git rev-parse --short HEAD)
GITUNCOMMITTEDCHANGES=$(shell if [ "`git ls-files -m ..`" != "" ]; then echo "-modified"; else echo ""; fi)
VERSION="$(GITVERSION)-$(GITHASH)$(GITUNCOMMITTEDCHANGES)"
REPOSITORY=chalmersrevere
DOCKER_BUILD_NAME=build_container
BUILDLOG=build.log
OPENDLV_HOST_PATH=$(shell dirname $(shell pwd))

all: docker-update-image build/Makefile

build/Makefile:
	cd .. && docker build -f docker/Dockerfile -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee docker/$(BUILDLOG) || exit 1

clean:
	rm -f $(BUILDLOG)
	rm -fr ../build-docker

docker-push: docker-tag-latest
	docker push $(REPOSITORY)/$(PRODUCT):latest

docker-update-image:
	docker pull $(shell awk '/^FROM/ { print $$2; exit }' Dockerfile)

docker-tag-latest:
	docker tag $(shell tail -1 $(BUILDLOG) | cut -f3 -d" ") $(REPOSITORY)/$(PRODUCT):latest

docker-cleanup:
	docker rm $$(docker ps -a -q)

run-example1: docker-tag-latest
	cd example1 && docker-compose up

run-all: docker-tag-latest
	cd run && docker-compose up -d
