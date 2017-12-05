# Makefile - Makefile to build OpenDLV using Docker-encapsulated builders.
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

###############################################################################
# What Docker images shall be used?
# For example, seresearch/base is a generic name to the base image to be used
# that, for example, could point to seresearch/ubuntu
REPOSITORY=seresearch
IMAGE=opendlv-core-on-opendavinci-on-base
BASE_IMAGE=$(REPOSITORY)/$(IMAGE)
BASE_IMAGE_VERSION=v0.11.0
BASE_IMAGE_DEV_SUFFIX=-dev
BASE_IMAGE_DEV=$(REPOSITORY)/$(IMAGE)$(BASE_IMAGE_DEV_SUFFIX)
BASE_IMAGE_DEV_VERSION=$(BASE_IMAGE_VERSION)

###############################################################################
# Product names.
PROJECT_NAME=opendlv
PRODUCT=opendlv-on-
DEST_DIR=opendlv

###############################################################################
# Shall deb or rpm packages be created?
PACKAGING_ENABLED=NO

###############################################################################
# Tools.
DOCKER=$(shell which docker)
CMAKE=$(shell which cmake)

###############################################################################
# Directories.
SOURCE_TREE_PATH=$(shell dirname $(shell pwd))
BUILD_PATH_BASE=$(SOURCE_TREE_PATH)/docker/builds
BUILD_PATH=$(BUILD_PATH_BASE)/$(PRODUCT)$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")-$(GIT_CURRENT_BRANCH)
CCACHE_PATH=$(HOME)/.ccache
PACKAGES_DIR=$(BUILD_PATH)

###############################################################################
# Determine Git-related setting.
VERSION=v$(shell head -1 ../VERSION | cut -f1 -d" ")
GIT_CURRENT_BRANCH=$(shell git rev-parse --abbrev-ref HEAD)
GIT_HASH=$(shell git rev-parse --short HEAD)
GIT_COMMITS_IN_CURRENT_BRANCH=$(shell git rev-list --count `git rev-parse --abbrev-ref HEAD`)
GIT_HAS_UNCOMMITTED_CHANGES=$(shell if [ "`git ls-files -m ..`" != "" ]; then echo "-modified"; else echo ""; fi)

###############################################################################
# Set up user-related settings.
RUN_BUILD_AS=odv # OpenDaVINCI is built as odv.
RUN_BUILD_UID_AS=$(shell getent passwd `whoami` | cut -f3 -d":")

###############################################################################
# Resulting Docker image settings.
DOCKERFILE_BASEIMAGE=s%BASEIMAGE%$(BASE_IMAGE)%
DOCKER_IMAGE_NAME=$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")
DOCKER_IMAGE_NAME_WITH_GIT_HASH=$(DOCKER_IMAGE_NAME)-$(GIT_HASH)
DOCKER_NETWORK_NAME=nw_$(DOCKER_IMAGE_NAME_WITH_GIT_HASH)
DOCKER_IMAGE_NAME_WITH_GIT_COMMITS_IN_CURRENT_BRANCH=$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")-$(GIT_HASH):$(GIT_COMMITS_IN_CURRENT_BRANCH)
DOCKER_IMAGE_NAME_SHORT_NAME=$(REPOSITORY)/$(PRODUCT)$(IMAGE)

DOCKERFILE_BASEIMAGEDEV=s%BASEIMAGE%$(BASE_IMAGE_DEV)%
DOCKER_IMAGE_NAME_DEV=$(shell echo $(BASE_IMAGE_DEV) | cut -f2 -d"/")
DOCKER_IMAGE_NAME_DEV_WITH_GIT_HASH=$(DOCKER_IMAGE_DEV_NAME)-$(GIT_HASH)
DOCKER_IMAGE_NAME_DEV_WITH_GIT_COMMITS_IN_CURRENT_BRANCH=$(shell echo $(BASE_IMAGE_DEV) | cut -f2 -d"/")-$(GIT_HASH):$(GIT_COMMITS_IN_CURRENT_BRANCH)
DOCKER_IMAGE_NAME_DEV_SHORT_NAME=$(REPOSITORY)/$(PRODUCT)$(IMAGE)$(BASE_IMAGE_DEV_SUFFIX)

###############################################################################

.PHONY: all buildComplete createDockerImage clean cleanAll cleanDocker removeExitedContainers removeNoneImagesFromDocker removeExistingImagesForBaseImageFromDocker removeExistingImagesForBaseImageDevFromDocker updateDockerBaseImage updateDockerBaseImageDev updateDockerBaseImageNonDev updateBuildComplete

###############################################################################

all: updateBuildComplete createDockerImage

###############################################################################

clean:
	$(CMAKE) -E remove_directory $(BUILD_PATH)

cleanAll: clean cleanDocker

cleanDocker: removeExitedContainers removeNoneImagesFromDocker removeExistingImagesForBaseImageDevFromDocker removeExistingImagesForBaseImageFromDocker

removeExitedContainers:
	$(DOCKER) rm $$($(DOCKER) ps -a -q) || exit 0

removeNoneImagesFromDocker:
	for i in $$($(DOCKER) images | grep none | tr -s " " ";" | cut -f3 -d";"); do $(DOCKER) rmi $$i; done

removeExistingImagesForBaseImageFromDocker:
	$(DOCKER) rmi -f `$(DOCKER) images | grep $(PRODUCT)$(IMAGE) | tr -s " " ";" | cut -f3 -d";"` || exit 0

removeExistingImagesForBaseImageDevFromDocker:
	$(DOCKER) rmi -f `$(DOCKER) images | grep $(PRODUCT)$(IMAGE)$(BASE_IMAGE_DEV_SUFFIX) | tr -s " " ";" | cut -f3 -d";"` || exit 0

updateDockerImages: updateDockerBaseImage updateDockerBaseImageDev

updateDockerBaseImage: updateDockerBaseImageNonDev updateDockerBaseImageDev

updateDockerBaseImageNonDev:
	$(DOCKER) pull $(BASE_IMAGE):$(BASE_IMAGE_VERSION)

updateDockerBaseImageDev:
	$(DOCKER) pull $(BASE_IMAGE_DEV):$(BASE_IMAGE_DEV_VERSION)

###############################################################################
define compile
	$(CMAKE) -E make_directory $(BUILD_PATH)
	$(CMAKE) -E make_directory $(BUILD_PATH)/opt/$(DEST_DIR)
	$(CMAKE) -E make_directory $(BUILD_PATH)/$(PROJECT_NAME).build
	$(CMAKE) -E make_directory $(BUILD_PATH)/$(PROJECT_NAME).deb
	$(DOCKER) network inspect $(DOCKER_NETWORK_NAME) || $(DOCKER) network create $(DOCKER_NETWORK_NAME)
	$(DOCKER) network ls
	$(DOCKER) run -ti --rm -v $(SOURCE_TREE_PATH):/opt/$(PROJECT_NAME).sources -v $(BUILD_PATH)/$(PROJECT_NAME).build:/opt/$(PROJECT_NAME).build -v $(CCACHE_PATH):/opt/ccache -v $(BUILD_PATH)/opt/$(DEST_DIR):/opt/$(DEST_DIR) -v $(SOURCE_TREE_PATH)/docker/completeBuild.sh:/opt/completeBuild.sh -w /opt $(BASE_IMAGE_DEV):$(BASE_IMAGE_DEV_VERSION) /opt/completeBuild.sh $(RUN_BUILD_AS) $(RUN_BUILD_UID_AS) $(PACKAGING_ENABLED)
	$(DOCKER) network rm $(DOCKER_NETWORK_NAME)
	mkdir -p $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME) && for i in $$(find $(BUILD_PATH)/$(PROJECT_NAME).build -type f -name "*deb" | grep -v CPack); do cp $$i $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME); done
	touch $(BUILD_PATH)/$(PROJECT_NAME).build/build.incremental
endef

###############################################################################
# This target uses a Docker container to build the $(PROJECT_NAME) source tree using the selected Docker image.
$(BUILD_PATH)/$(PROJECT_NAME).build/build.incremental:
	$(call compile)

buildComplete:
	$(call compile)

updateBuildComplete: updateDockerBaseImage
	$(call compile)

# Dependency to allow incremental builds.
buildIncremental: $(BUILD_PATH)/$(PROJECT_NAME).build/build.incremental
	$(DOCKER) run -ti --rm -v $(SOURCE_TREE_PATH):/opt/$(PROJECT_NAME).sources -v $(BUILD_PATH)/$(PROJECT_NAME).build:/opt/$(PROJECT_NAME).build -v $(CCACHE_PATH):/opt/ccache -v $(BUILD_PATH)/opt/$(DEST_DIR):/opt/$(DEST_DIR) -v $(SOURCE_TREE_PATH)/docker/incrementalBuild.sh:/opt/incrementalBuild.sh -w /opt $(BASE_IMAGE_DEV):$(BASE_IMAGE_DEV_VERSION) /opt/incrementalBuild.sh $(RUN_BUILD_AS) $(RUN_BUILD_UID_AS)
	mkdir -p $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME) && for i in $$(find $(BUILD_PATH)/$(PROJECT_NAME).build -type f -name "*deb" | grep -v CPack); do cp $$i $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME); done

###############################################################################
# This target creates a new Docker image based on the selected Docker image using the freshly built binaries.
createDockerImage: createDockerImageNonDev createDockerImageDev

createDockerImageNonDev:
	cat $(SOURCE_TREE_PATH)/docker/Dockerfile.template | sed -e $(DOCKERFILE_BASEIMAGE) > $(BUILD_PATH)/Dockerfile
	cat $(SOURCE_TREE_PATH)/docker/Dockerfile.template.nonDev | sed -e $(DOCKERFILE_BASEIMAGE) > $(BUILD_PATH)/Dockerfile && $(CMAKE) -E remove_directory $(BUILD_PATH)/opt.nonDev && $(CMAKE) -E copy_directory $(BUILD_PATH)/opt $(BUILD_PATH)/opt.nonDev && $(CMAKE) -E remove_directory $(BUILD_PATH)/opt.nonDev/${DEST_DIR}/man && $(CMAKE) -E remove_directory $(BUILD_PATH)/opt.nonDev/${DEST_DIR}/include
	echo "${PROJECT_NAME}.build" > $(BUILD_PATH)/.dockerignore
	echo "${PROJECT_NAME}.deb" >> $(BUILD_PATH)/.dockerignore
	echo "$(DOCKER_IMAGE_NAME)" >> $(BUILD_PATH)/.dockerignore
	echo "opt" >> $(BUILD_PATH)/.dockerignore
	cd $(BUILD_PATH) && $(DOCKER) build -t seresearch/$(DOCKER_IMAGE_NAME_WITH_GIT_COMMITS_IN_CURRENT_BRANCH) . | tee $(BUILD_PATH)/$(PROJECT_NAME).build/build.log || exit 1
	$(DOCKER) images | grep $$(tail -1 $(BUILD_PATH)/$(PROJECT_NAME).build/build.log | cut -f3 -d" ") | grep "latest" && exit 0 || $(DOCKER) tag $$(tail -1 $(BUILD_PATH)/$(PROJECT_NAME).build/build.log | cut -f3 -d" ") $(DOCKER_IMAGE_NAME_SHORT_NAME):latest
	$(DOCKER) tag $(DOCKER_IMAGE_NAME_SHORT_NAME):latest $(DOCKER_IMAGE_NAME_SHORT_NAME):$(VERSION)

createDockerImageDev:
	cat $(SOURCE_TREE_PATH)/docker/Dockerfile.template | sed -e $(DOCKERFILE_BASEIMAGEDEV) > $(BUILD_PATH)/Dockerfile
	echo "${PROJECT_NAME}.build" > $(BUILD_PATH)/.dockerignore
	echo "${PROJECT_NAME}.deb" >> $(BUILD_PATH)/.dockerignore
	echo "$(DOCKER_IMAGE_NAME)" >> $(BUILD_PATH)/.dockerignore
	echo "opt.nonDev" >> $(BUILD_PATH)/.dockerignore
	cd $(BUILD_PATH) && $(DOCKER) build -t seresearch/$(DOCKER_IMAGE_NAME_DEV_WITH_GIT_COMMITS_IN_CURRENT_BRANCH) . | tee $(BUILD_PATH)/$(PROJECT_NAME).build/build.log || exit 1
	$(DOCKER) images | grep $$(tail -1 $(BUILD_PATH)/$(PROJECT_NAME).build/build.log | cut -f3 -d" ") | grep "latest" && exit 0 || $(DOCKER) tag $$(tail -1 $(BUILD_PATH)/$(PROJECT_NAME).build/build.log | cut -f3 -d" ") $(DOCKER_IMAGE_NAME_DEV_SHORT_NAME):latest
	$(DOCKER) tag $(DOCKER_IMAGE_NAME_DEV_SHORT_NAME):latest $(DOCKER_IMAGE_NAME_DEV_SHORT_NAME):$(VERSION)

###############################################################################

pushDockerImage: createDockerImage
	$(DOCKER) push $(DOCKER_IMAGE_NAME_SHORT_NAME):latest
	$(DOCKER) push $(DOCKER_IMAGE_NAME_SHORT_NAME):$(VERSION)
	$(DOCKER) push $(DOCKER_IMAGE_NAME_DEV_SHORT_NAME):latest
	$(DOCKER) push $(DOCKER_IMAGE_NAME_DEV_SHORT_NAME):$(VERSION)

