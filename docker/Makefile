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

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16:
	for i in $$(cat fh16/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16_$$i seresearch/fh16_$$i:$(VERSION); docker tag fh16_$$i seresearch/fh16_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16: docker-tag-fh16
	for i in $$(cat fh16/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16_$$i:$(VERSION); docker push seresearch/fh16_$$i:latest; done

fh16-copy-opendlv:
	rm -fr fh16/opendlv
	cp -r ../build.from.docker/install fh16/opendlv
	cp fh16/checkHealth.sh fh16/opendlv/bin

# Build base image for FH16 truck.
fh16: build-incremental fh16-copy-opendlv
	cd fh16 && docker-compose build


###############################################################################
###############################################################################
# FH16 ps3controller.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-ps3controller:
	for i in $$(cat fh16-ps3controller/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16ps3controller_$$i seresearch/fh16ps3controller_$$i:$(VERSION); docker tag fh16ps3controller_$$i seresearch/fh16ps3controller_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-ps3controller: docker-tag-fh16-ps3controller
	for i in $$(cat fh16-ps3controller/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16ps3controller_$$i:$(VERSION); docker push seresearch/fh16ps3controller_$$i:latest; done

fh16-ps3controller-copy-opendlv:
	rm -fr fh16-ps3controller/opendlv
	cp -r ../build.from.docker/install fh16-ps3controller/opendlv
	cp fh16-ps3controller/checkHealth.sh fh16-ps3controller/opendlv/bin

# Build image for FH16 truck for PS3 controller.
fh16-ps3controller: build-incremental fh16-ps3controller-copy-opendlv
	cd fh16-ps3controller && docker-compose build


###############################################################################
###############################################################################
# FH16 CAN recording.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-canrecording:
	for i in $$(cat fh16-canrecording/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16canrecording_$$i seresearch/fh16canrecording_$$i:$(VERSION); docker tag fh16canrecording_$$i seresearch/fh16canrecording_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-canrecording: docker-tag-fh16-canrecording
	for i in $$(cat fh16-canrecording/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16canrecording_$$i:$(VERSION); docker push seresearch/fh16canrecording_$$i:latest; done

fh16-canrecording-copy-opendlv:
	rm -fr fh16-canrecording/opendlv
	cp -r ../build.from.docker/install fh16-canrecording/opendlv
	cp fh16-canrecording/checkHealth.sh fh16-canrecording/opendlv/bin

# Build image for FH16 truck for CAN data recording.
fh16-canrecording: build-incremental fh16-canrecording-copy-opendlv
	cd fh16-canrecording && docker-compose build


###############################################################################
###############################################################################
# FH16 sensor recording.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-sensorrecording:
	for i in $$(cat fh16-sensorrecording/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag sensorrecording_$$i seresearch/fh16sensorrecording_$$i:$(VERSION); docker tag fh16sensorrecording_$$i seresearch/fh16sensorrecording_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-sensorrecording: docker-tag-fh16-sensorrecording
	for i in $$(cat fh16-sensorrecording/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/sensorrecording_$$i:$(VERSION); docker push seresearch/sensorrecording_$$i:latest; done

fh16-sensorrecording-copy-opendlv:
	rm -fr fh16-sensorrecording/opendlv
	cp -r ../build.from.docker/install fh16-sensorrecording/opendlv
	cp fh16-sensorrecording/checkHealth.sh fh16-sensorrecording/opendlv/bin

# Build image for FH16 truck for sensor data recording.
fh16-sensorrecording: build-incremental fh16-sensorrecording-copy-opendlv
	cd fh16-sensorrecording && docker-compose build


###############################################################################
###############################################################################
# FH16 lidar-echolocation-scene.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-lidar-echolocation-scene:
	for i in $$(cat fh16-lidar-echolocation-scene/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16lidarecholocationscene_$$i seresearch/fh16lidarecholocationscene_$$i:$(VERSION); docker tag fh16lidarecholocationscene_$$i seresearch/fh16lidarecholocationscene_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-lidar-echolocation-scene: docker-tag-fh16-lidar-echolocation-scene
	for i in $$(cat fh16-lidar-echolocation-scene/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16lidarecholocationscene_$$i:$(VERSION); docker push seresearch/fh16lidarecholocationscene_$$i:latest; done

fh16-lidar-echolocation-scene-copy-opendlv:
	rm -fr fh16-lidar-echolocation-scene/opendlv
	cp -r ../build.from.docker/install fh16-lidar-echolocation-scene/opendlv
	cp fh16-lidar-echolocation-scene/checkHealth.sh fh16-lidar-echolocation-scene/opendlv/bin

# Build image for FH16 truck for lidar-echolocation-scene.
fh16-lidar-echolocation-scene: build-incremental fh16-lidar-echolocation-scene-copy-opendlv
	cd fh16-lidar-echolocation-scene && docker-compose build


###############################################################################
###############################################################################
# FH16 lidar-echolocation-scene-ivrule.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-lidar-echolocation-scene-ivrule:
	for i in $$(cat fh16-lidar-echolocation-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16lidarecholocationsceneivrule_$$i seresearch/fh16lidarecholocationsceneivrule_$$i:$(VERSION); docker tag fh16lidarecholocationsceneivrule_$$i seresearch/fh16lidarecholocationsceneivrule_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-lidar-echolocation-scene-ivrule: docker-tag-fh16-lidar-echolocation-scene-ivrule
	for i in $$(cat fh16-lidar-echolocation-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16lidarecholocationsceneivrule_$$i:$(VERSION); docker push seresearch/fh16lidarecholocationsceneivrule_$$i:latest; done

fh16-lidar-echolocation-scene-ivrule-copy-opendlv:
	rm -fr fh16-lidar-echolocation-scene-ivrule/opendlv
	cp -r ../build.from.docker/install fh16-lidar-echolocation-scene-ivrule/opendlv
	cp fh16-lidar-echolocation-scene-ivrule/checkHealth.sh fh16-lidar-echolocation-scene-ivrule/opendlv/bin

# Build image for FH16 truck for lidar-echolocation-scene-ivrule.
fh16-lidar-echolocation-scene-ivrule: build-incremental fh16-lidar-echolocation-scene-ivrule-copy-opendlv
	cd fh16-lidar-echolocation-scene-ivrule && docker-compose build


###############################################################################
###############################################################################
# FH16 camera-detectvehicle-scene-ivrule.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-camera-detectvehicle-scene-ivrule:
	for i in $$(cat fh16-camera-detectvehicle-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16cameradetectvehiclesceneivrule_$$i seresearch/fh16cameradetectvehiclesceneivrule_$$i:$(VERSION); docker tag fh16cameradetectvehiclesceneivrule_$$i seresearch/fh16cameradetectvehiclesceneivrule_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-camera-detectvehicle-scene-ivrule: docker-tag-fh16-camera-detectvehicle-scene-ivrule
	for i in $$(cat fh16-detectvehicle-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16cameradetectvehiclesceneivrule_$$i:$(VERSION); docker push seresearch/fh16cameradetectvehiclesceneivrule_$$i:latest; done

fh16-camera-detectvehicle-scene-ivrule-copy-opendlv:
	rm -fr fh16-camera-detectvehicle-scene-ivrule/opendlv
	cp -r ../build.from.docker/install fh16-camera-detectvehicle-scene-ivrule/opendlv
	cp fh16-camera-detectvehicle-scene-ivrule/checkHealth.sh fh16-camera-detectvehicle-scene-ivrule/opendlv/bin

# Build image for FH16 truck for camera-detectvehicle-scene-ivrule.
fh16-camera-detectvehicle-scene-ivrule: build-incremental fh16-camera-detectvehicle-scene-ivrule-copy-opendlv
	cd fh16-camera-detectvehicle-scene-ivrule && docker-compose build


###############################################################################
###############################################################################
# FH16 lidar-echolocation-camera-detectvehicle-scene-ivrule.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule:
	for i in $$(cat fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16lidarecholocationcameradetectvehiclesceneivrule_$$i seresearch/fh16lidarecholocationcameradetectvehiclesceneivrule_$$i:$(VERSION); docker tag fh16lidarecholocationcameradetectvehiclesceneivrule_$$i seresearch/fh16lidarecholocationcameradetectvehiclesceneivrule_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule: docker-tag-fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule
	for i in $$(cat fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16lidarecholocationcameradetectvehiclesceneivrule_$$i:$(VERSION); docker push seresearch/fh16lidarecholocationcameradetectvehiclesceneivrule_$$i:latest; done

fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule-copy-opendlv:
	rm -fr fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/opendlv
	cp -r ../build.from.docker/install fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/opendlv
	cp fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/checkHealth.sh fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule/opendlv/bin

# Build image for FH16 truck for lidar-echolocation-camera-detectvehicle-scene-ivrule.
fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule: build-incremental fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule-copy-opendlv
	cd fh16-lidar-echolocation-camera-detectvehicle-scene-ivrule && docker-compose build


###############################################################################
###############################################################################
# FH16 IV2016.

# Tag Docker images using Git hashes for traceability.
docker-tag-fh16-iv2016:
	for i in $$(cat fh16-iv2016/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker tag fh16iv2016_$$i seresearch/fh16iv2016_$$i:$(VERSION); docker tag fh16iv2016_$$i seresearch/fh16iv2016_$$i:latest; done

# Push Docker latest images for backup.
docker-push-fh16-iv2016: docker-tag-fh16-iv2016
	for i in $$(cat fh16-iv2016/docker-compose.yml | grep -v "^ "| grep -v "#" | grep -v "^$$" | cut -f1 -d":"); do docker push seresearch/fh16iv2016_$$i:$(VERSION); docker push seresearch/fh16iv2016_$$i:latest; done

fh16-iv2016-copy-opendlv:
	rm -fr fh16-iv2016/opendlv
	cp -r ../build.from.docker/install fh16-iv2016/opendlv
	cp fh16-iv2016/checkHealth.sh fh16-iv2016/opendlv/bin

# Build image for FH16 truck for IV demo.
fh16-iv2016: build-incremental fh16-iv2016-copy-opendlv
	cd fh16-iv2016 && docker-compose build

