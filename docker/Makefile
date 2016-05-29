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

build-and-run-gcdc-rule0: build-incremental run-gcdc-rule0 watch-gcdc-rule0

docker-update-opendavinci:
	docker pull $(shell awk '/^FROM/ { print $$2; exit }' Dockerfile)

docker-cleanup-exited-containers:
	docker rm $$(docker ps -a -q)

clean: docker-cleanup-exited-containers
	cd builder && rm -f $(BUILDLOG) && cd ..

create-builder:
	cd builder && docker build -f Dockerfile -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1
	docker images | grep $$(tail -1 builder/$(BUILDLOG) | cut -f3 -d" ") | grep "latest" && exit 0 || docker tag $(shell tail -1 builder/$(BUILDLOG) | cut -f3 -d" ") $(REPOSITORY)/$(PRODUCT):latest

build-fresh: create-builder
	docker run --rm=true -ti -v $(OPENDLV_HOST_PATH):/opt/opendlv-sources $(REPOSITORY)/$(PRODUCT):$(VERSION) /build.sh FRESH

build-incremental: create-builder
	docker run --rm=true -ti -v $(OPENDLV_HOST_PATH):/opt/opendlv-sources $(REPOSITORY)/$(PRODUCT):$(VERSION) /build.sh INCREMENTAL


# Example2:
run-example2:
	cd example2 && docker-compose up -d && cd ..

watch-example2:
	cd example2 && watch -n1 docker-compose ps && cd ..

stop-example2:
	cd example2 && docker-compose stop && cd ..


# GCDC modules:
run-gcdc-rule0:
	cd gcdc-rule0 && docker-compose up -d && cd ..

watch-gcdc-rule0:
	cd gcdc-rule0 && watch -n1 docker-compose ps && cd ..

stop-gcdc-rule0:
	cd gcdc-rule0 && docker-compose stop && cd ..

remove-containers-gcdc-rule0:
	cd gcdc-rule0 && docker-compose rm -f && cd ..



docker-push: docker-tag-latest
	docker push $(REPOSITORY)/$(PRODUCT):latest

