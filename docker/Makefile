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
# Parameters for this Makefile.
BASE_IMAGE=seresearch/opendlv-core-on-opendavinci-ubuntu-16.04-complete
BASE_IMAGE_VERSION=v0.8.10

###############################################################################
# Tools.
DOCKER=$(shell which docker)
CMAKE=$(shell which cmake)

###############################################################################
# Determine Git-related setting.
VERSION=v$(shell head -1 ../VERSION | cut -f1 -d" ")
GIT_CURRENT_BRANCH=$(shell git rev-parse --abbrev-ref HEAD)
GIT_HASH=$(shell git rev-parse --short HEAD)
GIT_COMMITS_IN_CURRENT_BRANCH=$(shell git rev-list --count `git rev-parse --abbrev-ref HEAD`)
GIT_HAS_UNCOMMITTED_CHANGES=$(shell if [ "`git ls-files -m ..`" != "" ]; then echo "-modified"; else echo ""; fi)
DOCKER_IMAGE_NAME_PREFIX=opendlv-on-
BUILD_PATH=$(BUILD_PATH_BASE)/$(DOCKER_IMAGE_NAME_PREFIX)$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")-$(GIT_CURRENT_BRANCH)

###############################################################################
# Directories.
SOURCE_TREE_PATH=$(shell dirname $(shell pwd))
BUILD_PATH_BASE=$(SOURCE_TREE_PATH)/docker/builds
CCACHE_PATH=$(HOME)/.ccache
MAPPED_VOLUME_SOURCES=opendlv.sources
MAPPED_VOLUME_BUILD=opendlv.build
MAPPED_VOLUME_INSTALL=/opt/opendlv
RUN_BUILD_AS=odv # OpenDaVINCI is built as odv.
RUN_BUILD_UID_AS=$(shell getent passwd `whoami` | cut -f3 -d":")
PACKAGES_DIR=$(BUILD_PATH)

###############################################################################
# Docker settings.
DOCKERFILE_BASEIMAGE=s%BASEIMAGE%$(BASE_IMAGE)%
DOCKER_REPOSITORY_NAME=seresearch
DOCKER_IMAGE_NAME=$(DOCKER_IMAGE_NAME_PREFIX)$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")
DOCKER_IMAGE_NAME_WITH_GIT_HASH=$(DOCKER_IMAGE_NAME)-$(GIT_HASH)
DOCKER_NETWORK_NAME=nw_$(DOCKER_IMAGE_NAME_WITH_GIT_HASH)
DOCKER_IMAGE_NAME_WITH_GIT_COMMITS_IN_CURRENT_BRANCH=$(DOCKER_IMAGE_NAME_PREFIX)$(shell echo $(BASE_IMAGE) | cut -f2 -d"/")-$(GIT_HASH):$(GIT_COMMITS_IN_CURRENT_BRANCH)
DOCKER_IMAGE_NAME_SHORT_NAME=$(DOCKER_REPOSITORY_NAME)/$(DOCKER_IMAGE_NAME)

###############################################################################

.PHONY: all buildComplete buildIncremental createDockerImage pushDockerImage clean cleanAll cleanDocker removeExitedContainers removeNoneDockerImages removeExistingDockerImages updateDockerBaseImage

###############################################################################

all: updateBuildComplete createDockerImage

###############################################################################

clean:
	$(CMAKE) -E remove_directory $(BUILD_PATH)

cleanAll: clean cleanDocker

cleanDocker: removeExitedContainers removeNoneDockerImages removeExistingDockerImages

removeExitedContainers:
	$(DOCKER) rm $$($(DOCKER) ps -a -q) || exit 0

removeNoneDockerImages:
	for i in $$($(DOCKER) images | grep none | tr -s " " ";" | cut -f3 -d";"); do $(DOCKER) rmi $$i; done

removeExistingDockerImages:
	$(DOCKER) rmi -f `$(DOCKER) images | grep $(DOCKER_IMAGE_NAME)- | tr -s " " ";" | cut -f3 -d";"` || exit 0

updateDockerBaseImage:
	$(DOCKER) pull $(BASE_IMAGE):$(BASE_IMAGE_VERSION)

###############################################################################
define compile
	$(CMAKE) -E make_directory $(BUILD_PATH)
	$(CMAKE) -E make_directory $(BUILD_PATH)$(MAPPED_VOLUME_INSTALL)
	$(CMAKE) -E make_directory $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD)
	$(CMAKE) -E make_directory $(BUILD_PATH)/opendlv.deb
	$(DOCKER) network inspect $(DOCKER_NETWORK_NAME) || $(DOCKER) network create $(DOCKER_NETWORK_NAME)
	$(DOCKER) network ls
	$(DOCKER) run -ti --rm -v $(SOURCE_TREE_PATH):/opt/$(MAPPED_VOLUME_SOURCES) -v $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD):/opt/$(MAPPED_VOLUME_BUILD) -v $(BUILD_PATH)$(MAPPED_VOLUME_INSTALL):$(MAPPED_VOLUME_INSTALL) -v $(CCACHE_PATH):/opt/ccache -v $(SOURCE_TREE_PATH)/docker/completeBuild.sh:/opt/completeBuild.sh -w /opt $(BASE_IMAGE):$(BASE_IMAGE_VERSION) /opt/completeBuild.sh $(RUN_BUILD_AS) $(RUN_BUILD_UID_AS)
	$(DOCKER) network rm $(DOCKER_NETWORK_NAME)
	mkdir -p $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME) && for i in $$(find $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD) -type f -name "*deb" | grep -v CPack); do cp $$i $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME); done
	touch $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD)/build.incremental
endef

###############################################################################
# Build targets using the selected Docker image.
buildComplete:
	$(call compile)

updateBuildComplete: updateDockerBaseImage
	$(call compile)

buildIncremental:
	$(DOCKER) run -ti --rm -v $(SOURCE_TREE_PATH):/opt/$(MAPPED_VOLUME_SOURCES) -v $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD):/opt/$(MAPPED_VOLUME_BUILD) -v $(BUILD_PATH)$(MAPPED_VOLUME_INSTALL):$(MAPPED_VOLUME_INSTALL) -v $(CCACHE_PATH):/opt/ccache -v $(SOURCE_TREE_PATH)/docker/incrementalBuild.sh:/opt/incrementalBuild.sh -w /opt $(BASE_IMAGE):$(BASE_IMAGE_VERSION) /opt/incrementalBuild.sh $(RUN_BUILD_AS) $(RUN_BUILD_UID_AS)
	mkdir -p $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME) && for i in $$(find $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD) -type f -name "*deb" | grep -v CPack); do cp $$i $(PACKAGES_DIR)/$(DOCKER_IMAGE_NAME); done

###############################################################################
# This target creates a new Docker image based on the selected Docker image using the freshly built binaries.
createDockerImage:
	cat $(SOURCE_TREE_PATH)/docker/Dockerfile.template | sed -e $(DOCKERFILE_BASEIMAGE) > $(BUILD_PATH)/Dockerfile
	echo $(MAPPED_VOLUME_BUILD) > $(BUILD_PATH)/.dockerignore
	cd $(BUILD_PATH) && $(DOCKER) build -t $(DOCKER_REPOSITORY_NAME)/$(DOCKER_IMAGE_NAME_WITH_GIT_COMMITS_IN_CURRENT_BRANCH) . | tee $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD)/build.log || exit 1
	$(DOCKER) images | grep $$(tail -1 $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD)/build.log | cut -f3 -d" ") | grep "latest" && exit 0 || $(DOCKER) tag $$(tail -1 $(BUILD_PATH)/$(MAPPED_VOLUME_BUILD)/build.log | cut -f3 -d" ") $(DOCKER_IMAGE_NAME_SHORT_NAME):latest
	$(DOCKER) tag $(DOCKER_IMAGE_NAME_SHORT_NAME):latest $(DOCKER_IMAGE_NAME_SHORT_NAME):$(VERSION)

###############################################################################

pushDockerImage: createDockerImage
	$(DOCKER) push $(DOCKER_IMAGE_NAME_SHORT_NAME):latest
	$(DOCKER) push $(DOCKER_IMAGE_NAME_SHORT_NAME):$(VERSION)

