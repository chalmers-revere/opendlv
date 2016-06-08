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
REPOSITORY=chalmersrevere
BUILDLOG=build.log
OPENDLV_HOST_PATH=$(shell dirname $(shell pwd))

# Workflow:
#
# Once or after change to OpenDaVINCI: make docker-update-opendavinci
#
# Initial build: make build-fresh
#
# Incremental building: make build-incremental
#
# Run example2: make run-example2
#
# Stop example2: make stop-example2
#
# Watch example2: make watch-example2

# This target will pull the latest OpenDaVINCI binary distribution.
# It should be called every now and then or on request.
docker-update-opendavinci:
	docker pull $(shell awk '/^FROM/ { print $$2; exit }' builder/Dockerfile)

###############################################################################
###############################################################################

# This target removes exited containers.
docker-cleanup-exited-containers:
	docker rm $$(docker ps -a -q)

# This target cleans up the environment to start fresh.
clean:
	cd builder && rm -f $(BUILDLOG) && cd ..
	sudo rm -fr ../build.from.docker ../code/generated/lib*

###############################################################################
###############################################################################

# This target creates a Docker image to compile the OpenDLV source tree.
create-builder:
	cd builder && docker build -f Dockerfile -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1
	docker images | grep $$(tail -1 builder/$(BUILDLOG) | cut -f3 -d" ") | grep "latest" && exit 0 || docker tag $$(tail -1 builder/$(BUILDLOG) | cut -f3 -d" ") $(REPOSITORY)/$(PRODUCT):latest

###############################################################################
###############################################################################

# This target compiles the OpenDLV source tree from scratch.
build-fresh: create-builder
	docker run --rm=true -ti -v $(OPENDLV_HOST_PATH):/opt/opendlv-sources $(REPOSITORY)/$(PRODUCT):$(VERSION) /build.sh FRESH

# This tartget compiles the OpenDLV source tree in case of minor changes to code/system.
build-incremental: create-builder
	docker run --rm=true -ti -v $(OPENDLV_HOST_PATH):/opt/opendlv-sources $(REPOSITORY)/$(PRODUCT):$(VERSION) /build.sh INCREMENTAL

###############################################################################
###############################################################################
# Template for FH16 microservices.
fh16-copy-opendlv:
	cp -r ../build.from.docker/install fh16/opendlv

# Build base image for FH16 truck.
fh16: build-incremental fh16-copy-opendlv
	cd fh16 && docker-compose build

###############################################################################
###############################################################################
# FH16 ps3controller.
fh16-ps3controller-copy-opendlv:
	cp -r ../build.from.docker/install fh16-ps3controller/opendlv

# Build base image for FH16 truck.
fh16-ps3controller: build-incremental fh16-ps3controller-copy-opendlv
	cd fh16-ps3controller && docker-compose build

#docker-push: docker-tag-latest
#	docker push $(REPOSITORY)/$(PRODUCT):latest

