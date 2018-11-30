## OpenDLV - A modern microservice-based software ecosystem for self-driving vehicles.

OpenDLV is a modern open source software environment to support the development and
testing of self-driving vehicles driven by the following design principles:

* Implemented using high quality and modern C++14 with a strong focus is on code clarity, portability, and performance.
* Based entirely on [microservices](https://martinfowler.com/articles/microservices.html).
* Strong focus on deployment and ease of use: All our microservices are automatically built on Docker hub: We provide *turn-key* solutions with Docker images for `amd64`, `armhf`, and `aarch64`
* Realized with [libcluon](https://github.com/chrberger/libcluon) - the world's first and only single-file, header-only middleware for distributed systems for robotic applications.
* CI-Status: [![Build Status](https://travis-ci.org/chalmers-revere/opendlv.svg?branch=new)](https://travis-ci.org/chalmers-revere/opendlv)
* License: [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.txt)

## Table of Contents
* [What is an OpenDLV session?](#what-is-an-opendlv-session)
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build from sources on the example of Ubuntu 16.04 LTS](#build-from-sources-on-the-example-of-ubuntu-1604-lts)
* [License](#license)
* [Publications](#publications)

## What is an OpenDLV session?

Applications based on OpenDLV are grouped in UDP multicast sessions
belonging to IPv4 address `225.0.0.X`, where X is from the within the range
`[1,254]`. All microservices belonging to the same UDP multicast group are
able to communicate with each other; thus, two applications running in different
UDP multicast sessions do not see each other and are completely separated.

The actual UDP multicast session is selected using the commandline parameter
`--cid=111`, where `111` would define the UDP multicast address `225.0.0.111`.
Microservices exchange data using the message [`Envelope`](https://github.com/chrberger/libcluon/blob/master/libcluon/resources/cluonDataStructures.odvd#L23-L30)
that contains besides the actual message to send further meta information
like sent and received timestamp and the point in time when the contained
message was actually sampled. All messages are encoded in
Google's [Protobuf](https://developers.google.com/protocol-buffers/) data
format ([example](https://wandbox.org/permlink/rXayIZxXyVDt5Jgn)) that has
been adjusted to preserve forwards and backwards compatibility using
[libcluon](https://wandbox.org/permlink/rXayIZxXyVDt5Jgn)'s native implementation
of Protobuf.

An `Envelope` contains in its field `serializedData` the actually message
to be exchanged that is encoded in Protobuf. Furthermore, the `Envelope`
itself is also encoded in Protobuf but prepended with the byte sequence
`0x0D 0xA4` as magic number, followed by three bytes `0xXX 0xYY 0xZZ`
describing the length in bytes of the Protobuf-encoded `Envelope`. The
sequence `0xA4 0xXX 0xYY 0xZZ` is encoded in little endian and `0xZZ`
is usually 0 in practice.

As participants in a UDP multicast session automatically receive any exchanged
`Envelope`s, a receiver can differentiate what message to expect by checking
`Envelope`'s field [dataType](https://github.com/chrberger/libcluon/blob/master/libcluon/resources/cluonDataStructures.odvd#L24),
which is referring to a message identifier (for instance, `Envelope`'s
[message identifier](https://github.com/chrberger/libcluon/blob/master/libcluon/resources/cluonDataStructures.odvd#L23)
is 1).

OpenDLV's microservices conform to the [OpenDLV Standard Message Set](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd) that enables exchangeability across
hardware/software interfaces to decouple high-level application logic from
low-level device drivers. For instance, OpenDLV's hardware/software interface to
access an *Applanix GPS unit* is called [opendlv-device-gps-pos](https://github.com/chalmers-revere/opendlv-device-gps-pos)
according to Applanix' internal data format POS that is used across several
units in their product portfolio. The microservice [opendlv-device-gps-pos](https://github.com/chalmers-revere/opendlv-device-gps-pos)
provides GPS information in messages [`GeodeticWgs84Reading`](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L137-L140)
and [`GeodeticHeadingReading`](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L133-L135).

As the microservices for the various GPS units (like, for instance Trimble and
Applanix) all broadcast the aforementioned messages, the hardware units can be
exchanged transparently for the high-level application logic. However, when
several GPS units shall be operated in parallel, it is necessary to distinguish
between them. Therefore, the commandline parameter `--id=Y` can be provided,
where Y is a positive number to differentiate between messages of the
same type. At the receiving end, the value Y is made available in `Envelope`'s
field `senderStamp`. As an example, when using an Applanix unit next to a
Trimble unit, the respective microservices could be supplied with the suffixes
`--id=1` and `--id=2`.

## Dependencies
You need a C++14-compliant compiler to compile this project as it ships the following dependencies as part of the source distribution:

* [libcluon](https://github.com/chrberger/libcluon) - [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)
* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.1.2) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)

For building [cluon-rec2fuse](https://github.com/chrberger/cluon-rec2fuse), [libfuse](https://github.com/libfuse/libfuse) is required.

## Usage
We are providing the following microservices as multi-platform (amd64/x86_64, armhf, aarch64) Docker images:

### OpenDLV.OS

Complete ArchLinux-based [OpenDLV OS](https://github.com/chalmers-revere/opendlv.os) Operating System (start here if you want to initialize a blank computing unit - *WARNING!* All data will be erased!)

### Hardware/Software Interfaces:
---
#### **Gamepad** (such as PS3 or PS4 controllers): [![opendlv-device-gamepad](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gamepad")](https://github.com/chalmers-revere/opendlv-device-gamepad) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gamepad-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gamepad-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gamepad-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gamepad-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gamepad.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gamepad)
* Provides: [device-specific messages](https://github.com/chalmers-revere/opendlv-device-gamepad/blob/master/src/actuationrequestmessage.odvd)
* Command to run with Docker for PS3 controllers: `docker run --rm -ti --init --net=host --device /dev/input/js0 chalmersrevere/opendlv-device-gamepad-multi:v0.0.10 --device=/dev/input/js0 --axis_leftright=0 --axis_updown=3 --freq=100 --acc_min=0 --acc_max=50 --dec_min=0 --dec_max=-10 --steering_min=-10 --steering_max=10 --steering_max_rate=5.0 --cid=111 --verbose`
* Command to run with Docker for PS4 controllers: `docker run --rm -ti --init --net=host --device /dev/input/js0 chalmersrevere/opendlv-device-gamepad-multi:v0.0.10 --device=/dev/input/js0 --axis_leftright=0 --axis_updown=4 --freq=100 --acc_min=0 --acc_max=50 --dec_min=0 --dec_max=-10 --steering_min=-10 --steering_max=10 --steering_max_rate=5.0 --cid=111 --verbose`
* Section for `docker-compose.yml` for PS3 controllers:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-ps3controller:
        container_name: dev-ps3controller
        image: chalmersrevere/opendlv-device-gamepad-multi:v0.0.10
        restart: on-failure
        network_mode: "host"
        devices:
        - "/dev/input/js0:/dev/input/js0"
        command: "--device=/dev/input/js0 --axis_leftright=0 --axis_updown=3 --freq=100 --acc_min=0 --acc_max=50 --dec_min=0 --dec_max=-10 --steering_min=-10 --steering_max=10 --steering_max_rate=5.0 --cid=111"
```
* Section for `docker-compose.yml` for PS4 controllers:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-ps4controller:
        container_name: dev-ps4controller
        image: chalmersrevere/opendlv-device-gamepad-multi:v0.0.10
        restart: on-failure
        network_mode: "host"
        devices:
        - "/dev/input/js0:/dev/input/js0"
        command: "--device=/dev/input/js0 --axis_leftright=0 --axis_updown=4 --freq=100 --acc_min=0 --acc_max=50 --dec_min=0 --dec_max=-10 --steering_min=-10 --steering_max=10 --steering_max_rate=5.0 --cid=111"
```
---
### GPS devices

#### **Applanix POS GPS/INSS** units: [![opendlv-device-gps-pos](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-pos")](https://github.com/chalmers-revere/opendlv-device-gps-pos) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos)
* Provides: [Latitude/Longitude (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L137-L140)
* Provides: [Heading (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L133-L135)
* Provides: [device-specific messages](https://github.com/chalmers-revere/opendlv-device-gps-pos/blob/master/src/pos-message-set.odvd)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-pos-multi:v0.0.7 --pos_ip=192.168.1.77 --pos_port=5602 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-pos:
        container_name: dev-gps-pos
        image: chalmersrevere/opendlv-device-gps-pos-multi:v0.0.7
        restart: on-failure
        network_mode: "host"
        command: "--pos_ip=10.42.42.40 --pos_port=5602 --cid=111"
```
---
#### **OxTS GPS/INSS** units: [![opendlv-device-gps-ncom](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-ncom")](https://github.com/chalmers-revere/opendlv-device-gps-ncom) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom)
* Provides: [Latitude/Longitude (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L145-L148)
* Provides: [Altitude (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L89-L91)
* Provides: [Heading (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L141-L143)
* Provides: [Latitude/Longitude/Altitude/Heading (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L267-L272)
* Provides: [GroundSpeed (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L129-L131)
* Provides: [Acceleration X/Y/Z (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L71-L75)
* Provides: [Angular Velocity X/Y/Z (OpenDLV Standard Message Set v0.9.5)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/fb11778810a37d76d45e83e52ea054dac2e2a350/opendlv.odvd#L77-L81)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.15 --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-ncom:
        container_name: dev-gps-ncom
        image: chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.15
        restart: on-failure
        network_mode: "host"
        command: "--ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111"
```
---
#### **Trimble GPS/INSS** units: [![opendlv-device-gps-nmea](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-nmea")](https://github.com/chalmers-revere/opendlv-device-gps-nmea) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea)
* Provides: [Latitude/Longitude (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L137-L140)
* Provides: [Heading (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L133-L135)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.13 --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-nmea:
        container_name: dev-gps-nmea
        image: chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.13
        restart: on-failure
        network_mode: "host"
        command: "--nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111"
```
---
### LIDAR devices

#### **Velodyne HDL32e** lidar units: [![opendlv-device-lidar-hdl32e](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-hdl32e")](https://github.com/chalmers-revere/opendlv-device-lidar-hdl32e) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e)
* Provides: [PointCloudReading (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L152-L158)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.10 --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-hdl32e:
        container_name: dev-lidar-hdl32e
        image: chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.10
        restart: on-failure
        network_mode: "host"
        command: "--hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111"
```
---
#### **Velodyne VLP32c** lidar units: [![opendlv-device-lidar-vlp32c](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-vlp32c")](https://github.com/chalmers-revere/opendlv-device-lidar-vlp32c) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp32c-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp32c-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp32c-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp32c-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp32c.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp32c)
* Provides: [PointCloudReading (OpenDLV Standard Message Set v0.9.7)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/bd5007e7723654563c388129a96a70b559f7fef6/opendlv.odvd#L165-L172)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-vlp32c-multi:v0.0.2 --vlp32c_ip=0.0.0.0 --vlp32c_port=2368 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-vlp32c:
        container_name: dev-lidar-vlp32c
        image: chalmersrevere/opendlv-device-lidar-vlp32c-multi:v0.0.2
        restart: on-failure
        network_mode: "host"
        command: "--vlp32c_ip=0.0.0.0 --vlp32c_port=2368 --cid=111"
```
---
#### **Velodyne VLP16** lidar units: [![opendlv-device-lidar-vlp16](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-vlp16")](https://github.com/chalmers-revere/opendlv-device-lidar-vlp16) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16)
* Provides: [PointCloudReading (OpenDLV Standard Message Set v0.9.1)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/0b85a1c4b151258c21b2368295a3c203232675e9/opendlv.odvd#L152-L158)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-vlp16-multi:v0.0.9 --vlp16_ip=0.0.0.0 --vlp16_port=2368 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-vlp16c:
        container_name: dev-lidar-vlp16c
        image: chalmersrevere/opendlv-device-lidar-vlp16-multi:v0.0.9
        restart: on-failure
        network_mode: "host"
        command: "--vlp16_ip=0.0.0.0 --vlp16_port=2368 --cid=111"
```
#### **RPLidar** lidar units: [![opendlv-device-lidar-rplidar](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-rplidar")](https://github.com/chalmers-revere/opendlv-device-lidar-rplidar) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-rplidar-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-rplidar-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-rplidar-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-rplidar-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-rplidar.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-rplidar)
* Provides: [PointCloudReading (OpenDLV Standard Message Set v0.9.9)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/6c6ac1f893ab181bbcd7c8a913ad117067fd6f4c/opendlv.odvd#L165-L178)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-rplidar-multi:v0.0.4 --device=/dev/ttyUSB0 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-rplidar:
        container_name: dev-lidar-rplidar
        image: chalmersrevere/opendlv-device-lidar-rplidar-multi:v0.0.4
        restart: on-failure
        network_mode: "host"
        devices:
        - "/dev/ttyUSB0:/dev/ttyUSB0"
        command: "--device=/dev/ttyUSB0 --cid=111"
```
---
### Ultrasound devices

#### SRF08 devices connected via I2C bus: [![opendlv-device-ultrasonic-srf08](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-ultrasonic-srf08")](https://github.com/chalmers-revere/opendlv-device-ultrasonic-srf08) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-ultrasonic-srf08-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-ultrasonic-srf08-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-ultrasonic-srf08-armhf/tags/)
* Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L165-L171)
* Command to run with Docker: `docker run --rm -ti --net=host --privileged --device=/dev/i2c-1 chalmersrevere/opendlv-device-ultrasonic-srf08-multi:v0.0.10 opendlv-device-ultrasonic-srf08 --dev=/dev/i2c-1 --bus-address=112 --cid=111 --freq=5 --id=0`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-ultrasonic-srf08:
        container_name: dev-ultrasonic-srf08
        image: chalmersrevere/opendlv-device-ultrasonic-srf08-multi:v0.0.10
        restart: on-failure
        network_mode: "host"
        privileged: true
        devices:
        - "/dev/i2c-1:/dev/i2c-1"
        command: "opendlv-device-ultrasonic-srf08 --dev=/dev/i2c-1 --bus-address=112 --cid=111 --freq=5 --range=100 --gain=1 --id=0 --cid=111"
```
---
### Camera devices and video processing

OpenDLV contains a highly modular and easy-to-use framework to grab video frames from various cameras, share them via shared memory, and encode/decode them into h264 frames to broadcast into an OD4Session for OpenDLV. The microservices are divided into video sources (e.g., [opendlv-device-camera-v4l](https://github.com/chalmers-revere/opendlv-device-camera-v4l), [opendlv-device-camera-opencv](https://github.com/chalmers-revere/opendlv-device-camera-opencv), [opendlv-device-camera-ueye](https://github.com/chalmers-revere/opendlv-device-camera-ueye)), or [opendlv-device-camera-rpi](https://github.com/chalmers-revere/opendlv-device-camera-rpi)) and video sinks (e.g., [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder)) to process incoming video frames. Video sinks provide frames in two image formats: [I420-encoded image](https://wiki.videolan.org/YUV/#I420) and ARGB. The former format can be directly used for video compression (e.g., h264 encoding), while the latter can be directly used for image detection algorithms ([opendlv-examples](https://github.com/chalmers-revere/opendlv-examples)).

#### **Video4Linux** cameras (e.g., /dev/video0): [![opendlv-device-camera-v4l](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-camera-v4l")](https://github.com/chalmers-revere/opendlv-device-camera-v4l) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-v4l-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-v4l-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-v4l-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-v4l-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-camera-v4l.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-camera-v4l)
* This microservice interfaces with a Video4Linux-supported camera and provides both, an [I420-encoded image](https://wiki.videolan.org/YUV/#I420) and an ARGB-encoded image residing in two separate shared memory areas. Other OpenDLV microservices can attach to this shared memory area for further processing (for instance [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder)).
* The following image formats are supported:
  * MJPEG
  * YUYV422
* Command to run with Docker: `docker run --rm -ti --init --ipc=host -v /tmp:/tmp -e DISPLAY=$DISPLAY --device /dev/video0 chalmersrevere/opendlv-device-camera-v4l-multi:v0.0.6 --camera=/dev/video0 --width=640 --height=480 --freq=20 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-camera-v4l:
        container_name: dev-camera-v4l
        image: chalmersrevere/opendlv-device-camera-v4l-multi:v0.0.6
        restart: on-failure
        ipc: "host"
        volumes:
        - /tmp:/tmp
        devices:
        - "/dev/video0:/dev/video0"
        command: "--camera=/dev/video0 --width=640 --height=480 --freq=20"
```
#### **OpenCV-supported** cameras: [![opendlv-device-camera-opencv](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-camera-opencv")](https://github.com/chalmers-revere/opendlv-device-camera-opencv) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-opencv-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-opencv-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-opencv-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-opencv-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-camera-opencv.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-camera-opencv)
* This microservice interfaces with an OpenCV-supported camera and provides both, an [I420-encoded image](https://wiki.videolan.org/YUV/#I420) and an ARGB-encoded image residing in two separate shared memory areas. Other OpenDLV microservices can attach to this shared memory area for further processing (for instance [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder)). This microservice also allows to interface with network-attached cameras (i.e., those providing an MJPEG stream for example).
* Command to run with Docker: `docker run --rm -ti --init --ipc=host -e DISPLAY=$DISPLAY --device /dev/video0 -v /tmp:/tmp chalmersrevere/opendlv-device-camera-opencv-multi:v0.0.10 --camera=/dev/video0 --width=640 --height=480 --freq=20`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-camera-opencv:
        container_name: dev-camera-opencv
        image: chalmersrevere/opendlv-device-camera-opencv-multi:v0.0.10
        restart: on-failure
        ipc: "host"
        volumes:
        - /tmp:/tmp
        devices:
        - "/dev/video0:/dev/video0"
        command: "--camera=/dev/video0 --width=640 --height=480 --freq=20"
```
#### **RaspberryPi** camera: [![opendlv-device-camera-rpi](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-camera-rpi")](https://github.com/chalmers-revere/opendlv-device-camera-rpi) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-rpi-armhf/tags/)
* This microservice interfaces with an RPi camera and provides both, an [I420-encoded image](https://wiki.videolan.org/YUV/#I420) and an ARGB-encoded image residing in two separate shared memory areas. Other OpenDLV microservices can attach to this shared memory area for further processing (for instance [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder)).
* Command to run with Docker: `docker run --rm -ti --init --ipc=host -e DISPLAY=$DISPLAY --device /dev/video0 -v /tmp:/tmp chalmersrevere/opendlv-device-camera-rpi-armhf:v0.0.5 --width=640 --height=480 --freq=20`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-camera-rpi:
        container_name: dev-camera-rpi
        image: chalmersrevere/opendlv-device-camera-rpi-armhf:v0.0.5
        restart: on-failure
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--width=640 --height=480 --freq=20"
```
#### **IDS uEye** cameras: [![opendlv-device-camera-ueye](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-camera-ueye")](https://github.com/chalmers-revere/opendlv-device-camera-ueye) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-ueye-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-ueye-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-ueye-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-camera-ueye-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-camera-ueye.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-camera-ueye)
* This microservice interfaces with an IDS uEye camera and provides both, an [I420-encoded image](https://wiki.videolan.org/YUV/#I420) and an ARGB-encoded image residing in two separate shared memory areas. Other OpenDLV microservices can attach to this shared memory area for further processing (for instance [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder)).
* Command to run with Docker: `docker run --rm -ti --init --ipc=host -v /tmp:/tmp --pid=host -v /var/run:/var/run -e DISPLAY=$DISPLAY chalmersrevere/opendlv-device-camera-ueye-multi:v0.0.4 --width=752 --height=480 --pixel_clock=10 --freq=20`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-camera-ueye:
        container_name: dev-camera-ueye
        image: chalmersrevere/opendlv-device-camera-ueye-multi:v0.0.4
        restart: on-failure
        ipc: "host"
        pid: "host"
        volumes:
        - /tmp:/tmp
        - /var/run:/var/run
        command: "--width=752 --height=480 --pixel_clock=10 --freq=20"
```
---
#### [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder) to encode video frames from a shared memory into h264 frames (OpenH264 Video Codec provided by Cisco Systems, Inc.) as [ImageReading (OpenDLV Standard Message Set v0.9.6)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155):
* This microservice attaches to an I420-encoded image residing in a shared memory area to encode it into an h264 frame to be broadcasted to other OpenDLV microservices.
* During the Docker-ized build process for this microservice, Cisco's binary library is downloaded from Cisco's webserver and installed on the user's computer due to legal implications arising from the patents around the [AVC/h264 format](http://www.mpegla.com/main/programs/avc/pages/intro.aspx).
* End user's notice according to [AVC/H.264 Patent Portfolio License Conditions](https://www.openh264.org/BINARY_LICENSE.txt):
**When you are using this software and build scripts from this repository, you are agreeing to and obeying the terms under which Cisco is making the binary library available.**
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-h264-encoder-amd64:
        container_name: video-h264-encoder-amd64
        build:
            context: https://github.com/chalmers-revere/opendlv-video-h264-encoder.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=111 --name=video0.i420 --width=640 --height=480"
```

#### [opendlv-video-h264-decoder](https://github.com/chalmers-revere/opendlv-video-h264-decoder.git) to decode h264 video frames from an [ImageReading (OpenDLV Standard Message Set v0.9.6)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155) into a shared memory (OpenH264 Video Codec provided by Cisco Systems, Inc.):
* During the Docker-ized build process for this microservice, Cisco's binary library is downloaded from Cisco's webserver and installed on the user's computer due to legal implications arising from the patents around the [AVC/h264 format](http://www.mpegla.com/main/programs/avc/pages/intro.aspx).
* End user's notice according to [AVC/H.264 Patent Portfolio License Conditions](https://www.openh264.org/BINARY_LICENSE.txt):
**When you are using this software and build scripts from this repository, you are agreeing to and obeying the terms under which Cisco is making the binary library available.**
* Section for `docker-compose.yml`:
 ```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-h264-decoder-amd64:
        container_name: video-h264-decoder-amd64
        build:
            context: https://github.com/chalmers-revere/opendlv-video-h264-decoder.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        environment:
        - DISPLAY=${DISPLAY}
        command: "--cid=111 --name=imageData"
```

#### [opendlv-video-x264-encoder](https://github.com/chalmers-revere/opendlv-video-x264-encoder) to encode video frames from a shared memory into h264 frames as [ImageReading (OpenDLV Standard Message Set v0.9.6)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155):
* This microservice attaches to an I420-encoded image residing in a shared memory area to encode it into an h264 frame to be broadcasted to other OpenDLV microservices.
* Due to legal implications arising from the patents around the [AVC/h264 format](http://www.mpegla.com/main/programs/avc/pages/intro.aspx), we are not distributing binaries or Docker images but only provide build instructions that can be easily integrated with a `docker-compose.yml` file.
* Section for `docker-compose.yml` to build for `amd64`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-x264-encoder-amd64:
        container_name: video-x264-encoder-amd64
        build:
            context: https://github.com/chalmers-revere/opendlv-video-x264-encoder.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=111 --name=video0.i420 --width=640 --height=480"
```
* Section for `docker-compose.yml` to build for `armhf`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-x264-encoder-armhf:
        container_name: video-x264-encoder-armhf
        build:
            context: https://github.com/chalmers-revere/opendlv-video-x264-encoder.git
            dockerfile: Dockerfile.armhf
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=111 --name=video0.i420 --width=640 --height=480"
```
* Section for `docker-compose.yml` to build for `aarch64`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-x264-encoder-aarch64:
        container_name: video-x264-encoder-aarch64
        build:
            context: https://github.com/chalmers-revere/opendlv-video-x264-encoder.git
            dockerfile: Dockerfile.aarch64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=111 --name=video0.i420 --width=640 --height=480"
```

#### [opendlv-video-vpx-encoder](https://github.com/chalmers-revere/opendlv-video-vpx-encoder.git) to encode video frames from a shared memory into VP8 or VP9 frames as [ImageReading (OpenDLV Standard Message Set v0.9.6)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155): [![opendlv-video-vpx-encoder](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-video-vpx-encoder")](https://github.com/chalmers-revere/opendlv-video-vpx-encoder) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-encoder-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-encoder-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-encoder-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-encoder-aarch64/tags/)
* Command to run with Docker: `docker run --rm -ti --init --net=host --ipc=host -v /tmp:/tmp chalmersrevere/opendlv-video-vpx-encoder-multi:v0.0.5 --cid=111 --name=video0.i420 --width=640 --height=480 --vp8`
* Section for `docker-compose.yml`:
 ```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-vpx-encoder:
        container_name: video-vpx-encoder
        image: chalmersrevere/opendlv-video-vpx-encoder-multi:v0.0.5
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        environment:
        - DISPLAY=${DISPLAY}
        command: "--cid=111 --name=video0.i420 --width=640 --height=480 --vp8"
```

#### [opendlv-video-vpx-decoder](https://github.com/chalmers-revere/opendlv-video-vpx-decoder.git) to decode h264 video frames from an [ImageReading (OpenDLV Standard Message Set v0.9.6)](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/40f0cdb83632c3d122d2f35e028331494313330f/opendlv.odvd#L150-L155) into a shared memory: [![opendlv-video-vpx-decoder](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-video-vpx-decoder")](https://github.com/chalmers-revere/opendlv-video-vpx-decoder) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-decoder-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-decoder-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-decoder-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-video-vpx-decoder-aarch64/tags/)
* Command to run with Docker: `docker run --rm -ti --init --net=host --ipc=host -v /tmp:/tmp -e DISPLAY=$DISPLAY chalmersrevere/opendlv-video-vpx-decoder-multi:v0.0.5 --cid=253 --name=video0.arg0 --verbose`
* Section for `docker-compose.yml`:
 ```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-vpx-decoder:
        container_name: video-vpx-decoder
        image: chalmersrevere/opendlv-video-vpx-decoder-multi:v0.0.5
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        environment:
        - DISPLAY=${DISPLAY}
        command: "--cid=111 --name=imageData"
```
---
### Data Post Processing:
#### [rec2csv-png](https://github.com/chalmers-revere/rec2csv-png) to extract messages as .csv and h264 frames as separate .png files from a .rec file from a recorded OpenDLV session (OpenH264 Video Codec provided by Cisco Systems, Inc.):
* Example for a `docker-compose.yml`:
 ```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
     rec2csv_png:
        container_name: rec2csv_png
        build:
            context: https://github.com/chalmers-revere/rec2csv-png.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        volumes:
        - .:/opt/data
        working_dir: /opt/data
        command: "--rec=YourRecording.rec --odvd=YourMessageSpec.odvd"
```
        
#### [cluon-rec2fuse](https://github.com/chrberger/cluon-rec2fuse) to *mount* a recording file to a folder and dynamically map its content as .csv files: 
```
  docker run --rm -ti -v $PWD/myrecording.rec:/opt/input.rec \
                    -v $PWD/opendlv.odvd:/opt/odvd \
                    -v $PWD/mnt:/opt/output:shared \
                    --cap-add SYS_ADMIN \
                    --cap-add MKNOD \
                    --security-opt apparmor:unconfined \
                    --device=/dev/fuse \
                    -v /etc/passwd:/etc/passwd:ro \
                    -v /etc/group:/etc/group \
                    chrberger/cluon-rec2fuse-amd64:v0.0.104 \
                    /bin/sh -c "chown $UID:$UID /opt/output && \
                    su -s /bin/sh $USER -c 'cluon-rec2fuse --rec=/opt/input.rec --odvd=/opt/odvd -f /opt/output' \
                    && tail -f /dev/null"
```
[![asciicast](https://asciinema.org/a/tMLc9lvmnTKlcwSHSIuepF4It.png)](https://asciinema.org/a/tMLc9lvmnTKlcwSHSIuepF4It?autoplay=1)
---
### Visualizations:
#### [cluon-livefeed](https://github.com/chrberger/cluon-livefeed) to display any messages exchanged in the communication session 111 on console: `docker run --rm -ti --init --net=host chrberger/cluon-livefeed-multi:v0.0.104 --cid=111` [![asciicast](https://asciinema.org/a/zT1Mr5aKUGx3k43ax8a9eapBb.png)](https://asciinema.org/a/zT1Mr5aKUGx3k43ax8a9eapBb?autoplay=1)

#### [opendlv-signal-viewer](https://github.com/chalmers-revere/opendlv-signal-viewer) to view any messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8080): `docker run --rm --net=host -p 8080:8080 chalmersrevere/opendlv-signal-viewer-multi:v0.0.8 --cid=111`
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    signal-viewer:
        container_name: signal-viewer
        image: chalmersrevere/opendlv-signal-viewer-multi:v0.0.8
        restart: on-failure
        network_mode: "host"
        ports:
        - "8080:8080"
        command: "--cid=111"
```
![screenshot from signal viewer](https://raw.githubusercontent.com/chalmers-revere/opendlv-signal-viewer/master/signal-viewer.gif)

#### [opendlv-vehicle-view](https://github.com/chalmers-revere/opendlv-vehicle-view) to view vehicle messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8081): `docker run --rm --net=host --name=opendlv-vehicle-view -v ~/recordings:/opt/vehicle-view/recordings -v /var/run/docker.sock:/var/run/docker.sock -p 8081:8081 chalmersrevere/opendlv-vehicle-view-multi:v0.0.51`
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    vehicle-view:
        container_name: opendlv-vehicle-view
        image: chalmersrevere/opendlv-vehicle-view-multi:v0.0.51
        restart: on-failure
        network_mode: "host"
        volumes:
        - ~/recordings:/opt/vehicle-view/recordings
        - /var/run/docker.sock:/var/run/docker.sock
        ports:
        - "8081:8081"
```
![screenshot from vehicle view](https://raw.githubusercontent.com/chalmers-revere/opendlv-vehicle-view/master/vehicle-view-animated.gif)


## Build from sources on the example of Ubuntu 16.04 LTS
To build this software, you need cmake, C++14 or newer, and make. Having these
preconditions, update the contained submodules first.

```
git submodule update --init --recursive
git submodule update --remote --merge
git submodule status
```

Now, you can change into the various sub-folders and run `cmake` and `make` as follows:

```
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make && make test && make install
```


## License

* This project is released under the terms of the GNU GPLv3 License.


## Publications

The following list contains publications related to the OpenDLV Software Ecosystem:

* O. Benderius, C. Berger, V. Malmsten Lundgren: ["The Best Rated Human-Machine Interface Design for Autonomous Vehicles in the 2016 Grand Cooperative Driving Challenge"](http://ieeexplore.ieee.org/abstract/document/8057581/).
* H. Yin, C. Berger: ["When to use what data set for your self-driving car algorithm: An overview of publicly available driving datasets"](https://www.researchgate.net/publication/320475411_When_to_use_what_data_set_for_your_self-driving_car_algorithm_An_overview_of_publicly_available_driving_datasets)
* F. Giaimo, C. Berger, C. Kirchner: ["Considerations About Continuous Experimentation for Resource-Constrained Platforms in Self-driving Vehicles"](https://link.springer.com/chapter/10.1007/978-3-319-65831-5_6)
* H. Yin, C. Berger: ["Mastering data complexity for autonomous driving with adaptive point clouds for urban environments"](http://ieeexplore.ieee.org/document/7995901/)
* C. Berger, B. Nguyen, O. Benderius: ["Containerized Development and Microservices for Self-Driving Vehicles: Experiences & Best Practices"](http://ieeexplore.ieee.org/abstract/document/7958428/)
* F. Giaimo, C. Berger: ["Design Criteria to Architect Continuous Experimentation for Self-Driving Vehicles"](http://ieeexplore.ieee.org/document/7930218/)
* P. Masek, M. Thulin, H. Andrade, C. Berger, O. Benderius: ["Systematic Evaluation of Sandboxed Software Deployment for Real-time Software on the Example of a Self-Driving Heavy Vehicle"](https://arxiv.org/abs/1608.06759)
* C. Berger: ["An Open Continuous Deployment Infrastructure for a Self-driving Vehicle Ecosystem"](https://link.springer.com/chapter/10.1007/978-3-319-39225-7_14)
* F. Giaimo, C. Berger, I. Crnkovic: ["
Continuous Experimentation on Cyber-Physical Systems: Challenges and Opportunities"](https://dl.acm.org/citation.cfm?id=2962709)
* H. Yin, F. Giaimo, H. Andrade, C. Berger, I. Crnkovic: ["Adaptive Message Restructuring Using Model-Driven Engineering"](https://link.springer.com/chapter/10.1007/978-3-319-32467-8_67)
* H. Andrade, F. Giaimo, C. Berger, I. Crnkovic: ["Systematic evaluation of three data marshalling approaches for distributed software systems"](https://dl.acm.org/citation.cfm?id=2846705)
* F. Giaimo, H. Andrade, C. Berger, I. Crnkovic: ["Improving Bandwidth Efficiency with Self-Adaptation for Data Marshalling on the Example of a Self-Driving Miniature Car"](https://dl.acm.org/citation.cfm?id=2797454)
* C. Berger: ["Accelerating Regression Testing for Scaled Self-Driving Cars with Lightweight Virtualization - A Case Study"](http://ieeexplore.ieee.org/document/7173936/)
