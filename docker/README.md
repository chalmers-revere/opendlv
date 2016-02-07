# OpenDLV - Encapsulated Deployment with Docker

This example was realized with OpenDaVINCI, OpenDLV, and Docker to
demonstrate how to create a Docker image with the most recent OpenDLV
binaries.

This example assumes that you have the OpenDaVINCI binaries and libraries
at /opt/od.

First, change to docker and run make build to build OpenDLV from sources.
The binaries will be installed at opendlv.

    $ cd docker
    $ make

A Docker image containing the most recent binaries is created and tagged
as latest automatically.

Next, you can run a software distribution using docker-compose.

    $ make run-example1

Press Ctrl-C once you are finished testing your binaries.

    $ make docker-cleanup

The final call will remove recently exited containers.
