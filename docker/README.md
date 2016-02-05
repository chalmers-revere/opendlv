# OpenDLV - Encapsulated Deployment with Docker

This example was realized with OpenDaVINCI, OpenDLV, and Docker to
demonstrate how to create a Docker image with the most recent OpenDLV
binaries.

This example assumes that you have the OpenDaVINCI binaries and libraries
at /opt/od.

First, change to docker and run make build to build OpenDLV from sources.
The binaries will be installed at opendlv.

    $ cd docker
    $ make build

Next, create a Docker image containing the most recent binaries.

    $ make docker
    $ make docker-tag-latest

Finally, you can run a software distribution using docker-compose.

    $ cd example1
    $ docker-compose up
