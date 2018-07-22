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
#### GPS: Interface to **OxTS GPS/INSS** units  [![opendlv-device-gps-ncom](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-ncom")](https://github.com/chalmers-revere/opendlv-device-gps-ncom) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-ncom-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom):
* Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L145-L148)
* Provides: [Altitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L89-L91)
* Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L141-L143)
* Provides: [Latitude/Longitude/Altitude/Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L267-L272)
* Provides: [GroundSpeed](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L129-L131)
* Provides: [Acceleration X/Y/Z](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L71-L75)
* Provides: [Angular Velocity X/Y/Z](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L77-L81)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.12 opendlv-device-gps-ncom --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-ncom:
        image: chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.12
        restart: on-failure
        network_mode: "host"
        command: "opendlv-device-gps-ncom --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111"
```
---
#### GPS: Interface to **Trimble GPS/INSS** units [![opendlv-device-gps-nmea](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-nmea")](https://github.com/chalmers-revere/opendlv-device-gps-nmea) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-nmea-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea):
* Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L145-L148)
* Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L141-L143)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.7 opendlv-device-gps-nmea --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-nmea:
        image: chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.7
        restart: on-failure
        network_mode: "host"
        command: "opendlv-device-gps-nmea --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111"
```
---
#### GPS: Interface to **Applanix POS GPS/INSS** units [![opendlv-device-gps-pos](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-gps-pos")](https://github.com/chalmers-revere/opendlv-device-gps-pos) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-gps-pos-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos):
* Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L145-L148)
* Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L141-L143)
* Provides: [device-specific messages](https://github.com/chalmers-revere/opendlv-device-gps-pos/blob/master/src/pos-message-set.odvd)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-pos-multi:v0.0.5 opendlv-device-gps-pos --pos_ip=192.168.1.77 --pos_port=5602 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-gps-pos:
        image: chalmersrevere/opendlv-device-gps-pos-multi:v0.0.5
        restart: on-failure
        network_mode: "host"
        command: "opendlv-device-gps-pos --pos_ip=10.42.42.40 --pos_port=5602 --cid=111"
```
---
#### LIDAR: Interface to **Velodyne VLP16** lidar units [![opendlv-device-lidar-vlp16](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-vlp16")](https://github.com/chalmers-revere/opendlv-device-lidar-vlp16) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-vlp16-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16):
* Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L160-L166)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-vlp16-multi:v0.0.8 opendlv-device-lidar-vlp16 --vlp16_ip=0.0.0.0 --vlp16_port=2368 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-vlp16c:
        image: chalmersrevere/opendlv-device-lidar-vlp16-multi:v0.0.8
        restart: on-failure
        network_mode: "host"
        command: "opendlv-device-lidar-vlp16 --vlp16_ip=0.0.0.0 --vlp16_port=2368 --cid=111"
```
---
#### LIDAR: Interface to **Velodyne HDL32e** lidar units [![opendlv-device-lidar-hdl32e](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-lidar-hdl32e")](https://github.com/chalmers-revere/opendlv-device-lidar-hdl32e) [![Docker (multi)](https://img.shields.io/badge/Docker-multi-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-multi/tags/) [![Docker (amd64)](https://img.shields.io/badge/Docker-amd64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-amd64/tags/) [![Docker (armhf)](https://img.shields.io/badge/Docker-armhf-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-armhf/tags/) [![Docker (aarch64)](https://img.shields.io/badge/Docker-aarch64-blue.svg)](https://hub.docker.com/r/chalmersrevere/opendlv-device-lidar-hdl32e-aarch64/tags/) [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e):
* Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L160-L166)
* Command to run with Docker: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.9 opendlv-device-lidar-hdl32e --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111 --verbose`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-lidar-hdl32e:
        image: chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.9
        restart: on-failure
        network_mode: "host"
        command: "opendlv-device-lidar-hdl32e --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111"
```
---
#### Ultrasonic: Interface to SRF08 ultrasound devices connected via I2C bus [![opendlv-device-ultrasonic-srf08](https://raw.githubusercontent.com/encharm/Font-Awesome-SVG-PNG/master/black/png/24/github.png "opendlv-device-ultrasonic-srf08")](https://github.com/chalmers-revere/opendlv-device-ultrasonic-srf08)
* Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L165-L171)
* Command to run with Docker: `docker run --rm -ti --net=host --privileged --device=/dev/i2c-1 chalmersrevere/opendlv-device-ultrasonic-srf08-multi:v0.0.10 opendlv-device-ultrasonic-srf08 --dev=/dev/i2c-1 --bus-address=112 --cid=111 --freq=5 --id=0`
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    dev-ultrasonic-srf08:
        image: chalmersrevere/opendlv-device-ultrasonic-srf08-multi:v0.0.10
        restart: on-failure
        network_mode: "host"
        privileged: true
        devices:
        - "/dev/i2c-1:/dev/i2c-1"
        command: "opendlv-device-ultrasonic-srf08 --dev=/dev/i2c-1 --bus-address=112 --cid=111 --freq=5 --range=100 --gain=1 --id=0 --cid=111"
```
---
#### Video: OpenDLV contains an easy-to-use framework to grab video frames from various cameras, share them via shared memory, and encode/decode them into h264 frames to broadcast into an OD4Session.

##### [opendlv-video-h264-encoder](https://github.com/chalmers-revere/opendlv-video-h264-encoder.git) to encode video frames from a shared memory into h264 frames (OpenH264 Video Codec provided by Cisco Systems, Inc.) as [ImageReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L150-L155):
* Section for `docker-compose.yml`:
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
 video-h264-encoder-amd64:
        build:
            context: https://github.com/chalmers-revere/opendlv-video-h264-encoder.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
        command: "--cid=111 --name=camera02 --width=640 --height=480 --yuyv422"
```
  
##### [opendlv-video-h264-decoder](https://github.com/chalmers-revere/opendlv-video-h264-decoder.git) to decode h264 video frames from an [ImageReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L150-L155) into a shared memory (OpenH264 Video Codec provided by Cisco Systems, Inc.):
* Section for `docker-compose.yml`:
 ```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
    video-h264-decoder-amd64:
        build:
            context: https://github.com/chalmers-revere/opendlv-video-h264-decoder.git
            dockerfile: Dockerfile.amd64
        restart: on-failure
        network_mode: "host"
        ipc: "host"
        volumes:
        - /tmp:/tmp
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
        image: chalmersrevere/opendlv-signal-viewer-multi:v0.0.8
        restart: on-failure
        network_mode: "host"
        ports:
        - "8080:8080"
        command: "--cid=111"
```
![screenshot from signal viewer](https://raw.githubusercontent.com/chalmers-revere/opendlv-signal-viewer/master/signal-viewer.gif)

#### [opendlv-vehicle-view](https://github.com/chalmers-revere/opendlv-vehicle-view) to view vehicle messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8081): `docker run --rm --net=host -p 8081:8081 chalmersrevere/opendlv-vehicle-view-multi:v0.0.19 --cid=111`
```yml
version: '2' # Must be present exactly once at the beginning of the docker-compose.yml file
services:    # Must be present exactly once at the beginning of the docker-compose.yml file
   vehicle-view:
        image: chalmersrevere/opendlv-vehicle-view-multi:v0.0.19
        restart: on-failure
        network_mode: "host"
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
