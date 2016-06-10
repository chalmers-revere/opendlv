# OpenDLV - Encapsulated Deployment with Docker

This example was realized with OpenDaVINCI, OpenDLV, and Docker to demonstrate how to create a Docker image with the most recent OpenDLV binaries.

By using Docker, reproducible builds as well as easily shareable and traceable binary distributions of microservices from OpenDLV and OpenDaVINCI are enabled.

First, make sure that you have a recent Docker environment installed.

Next, change to the docker sub-folder and get the most recent OpenDaVINCI binary distribution:

    $ cd docker
    $ make docker-update-opdavinci

The OpenDaVINCI binary distribution bundles are required libraries and dependency to (a) run software based on OpenDaVINCI (such as OpenDLV) and (b) to build sources based on OpenDaVINCI in a reproducible way.

Next, we create a Docker image for OpenDLV based on the binary distribution of OpenDaVINCI; this image contains further libraries that are needed for OpenDLV:

    $ make create-builder

Once the Docker image is ready, we instantiate a container therefrom and build the OpenDLV sources. Therefor, the source tree is mapped inside the instantiated Docker container and built; the resulting binaries are made available outside the Docker container in the folder build.from.docker:

    $ make build-fresh

The command ```make build-fresh``` will build the entire source tree from scratch. Afterwards, any new (minor) changes can be built using:

    $ make build-incremental

After the binaries for OpenDLV have been built, the Docker images for the microservices in use can be prepared. The Makefile contains as an example a ps3controller-based environment that allows to control the FH16 truck using a ps3 controller. To build this set of microservices, simply run:

    $ make fh16-ps3controller

This call will automatically include ```make build-incremental```; so, whenever a change to the OpenDLV source tree is made, the set of microservices can be built conveniently.

After the microservices have been built, they can be started by using ```docker-compose```:

    $ cd fh16-ps3controller
    $ docker-compose up

Now, this set of microservices is waiting for an ```odsupercomponent``` to become available for the CID session 71. Thus, simply start one in a new terminal in the same folder:

    $ odsupercomponent --cid=71

Depending on where you have installed OpenDaVINCI, you might need to adjust the LD_LIBRARY_PATH:

    $ LD_LIBRARY_PATH=/opt/od3/lib /opt/od3/bin/odsupercomponent --cid=71

As soon as the microservices notice the availability of ```odsupercomponent``` for CID 71, the are activated and start to operate.

