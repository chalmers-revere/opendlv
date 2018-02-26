## OpenDLV - A modern microservice-based software ecosystem for self-driving vehicles.

OpenDLV is an open source software environment to support the development and
testing of self-driving vehicles driven by the following design principles

* Implemented using high quality and modern C++14 with a strong focus is on code clarity, portability, and performance.
* Based on [libcluon](https://github.com/chrberger/libcluon) - the world's first and only header-only, single-file middleware for distributed systems as found in robotic applications
* Based entirely on [microservices](https://martinfowler.com/articles/microservices.html)
* Strong focus on deployment and ease of use: All our microservices are automatically built on Docker hub
* CI-Status: [![Build Status](https://travis-ci.org/chalmers-revere/opendlv.svg?branch=new)](https://travis-ci.org/chalmers-revere/opendlv)
* License: [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.txt)
* Our team also provides *turn-key* solutions with Docker images for `amd64`, `armhf`, and `aarch64`


## Table of Contents
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build from sources on the example of Ubuntu 16.04 LTS](#build-from-sources-on-the-example-of-ubuntu-1604-lts)
* [License](#license)


## What is an OpenDLV session?

Applications based on OpenDLV are grouped in UDP multicast sessions
belonging to IPv4 address `225.0.0.X`, where X is from the within the range
`[1,254]`. All microservices belonging to the same UDP multicast group are
able to communicate with each other; thus, two applications running in different
UDP multicast sessions do not see each other and are completely separated.

The actual UDP multicast session is configured using the commandline parameter
`--cid=111`, where `111` would resolve to the UDP multicast address `225.0.0.111`.
Microservices exchange data using the message [`Envelope`](https://github.com/chrberger/libcluon/blob/master/libcluon/resources/cluonDataStructures.odvd#L23-L30)
that contains next to the actual message to be exchange further meta information
like sent and received timestamp and the point in time when the contained
message (the payload) was actually sampled. All messages are encoded in
Google's [Protobuf](https://developers.google.com/protocol-buffers/) data
format ([example](https://wandbox.org/permlink/rXayIZxXyVDt5Jgn)) that has
been adjusted to preserve forwards and backwards compatibility. As such,
an `Envelope` contains in its field `serializedData` the actually message
to be exchanged that is encoded in Protobuf. Furthermore, the `Envelope`
itself is also encoded in Protobuf but prepended with the byte sequence
`0x0D 0xA4` as magic number, followed by three bytes `0xXX 0xYY 0xZZ`
describing the length in bytes of the Protobuf-encoded `Envelope`. The
sequence `0xA4 0xXX 0xYY 0xZZ` is encoded in little endian and `0xZZ`
is usually 0 in practice.


When a microservice shall be used to interface with multiple hardware units of
the same type (e.g., multiple GPS units from the same vendor), simply add
`--id=Y`, where Y is a positive number to differentiate between messages of the
same type. At the receiving end, the value Y is made available in `Envelope`'s
field `senderStamp`.


## Dependencies
No dependencies! You just need a C++14-compliant compiler to compile this
project as it ships the following dependencies as part of the source distribution:

* [libcluon](https://github.com/chrberger/libcluon) - [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)
* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.1.2) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)


## Usage
We are providing the following microservices as multi-platform (amd64/x86_64, armhf, aarch64) Docker images:

* Complete ArchLinux-based [OpenDLV OS](https://github.com/chalmers-revere/opendlv.os) Operating System (start here if you want to initialize a blank computing unit - *WARNING!* All data will be erased!)
* Hardware/software interfaces:
  * GPS:
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom) [opendlv-device-gps-ncom](https://github.com/chalmers-revere/opendlv-device-gps-ncom) to interface with OxTS GPS/INSS units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.2 opendlv-device-gps-ncom --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111 --verbose`
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea) [opendlv-device-gps-nmea](https://github.com/chalmers-revere/opendlv-device-gps-nmea) to interface with Trimble GPS/INSS units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.3 opendlv-device-gps-nmea --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111 --verbose`
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos) [opendlv-device-gps-pos](https://github.com/chalmers-revere/opendlv-device-gps-pos) to interface with Applanix POS GPS/INSS units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-pos-multi:v0.0.1 opendlv-device-gps-pos --pos_ip=192.168.1.77 --pos_port=5602 --cid=111 --verbose`
  * LIDAR:
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e) [opendlv-device-lidar-hdl32e](https://github.com/chalmers-revere/opendlv-device-lidar-hdl32e) to interface with Velodyne HDL32e lidar units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.2 opendlv-device-lidar-hdl32e --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111 --verbose`
* Visualizations:
  * [opendlv-signal-viewer](https://github.com/chalmers-revere/opendlv-signal-viewer) to view any messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8080): `docker run --rm --net=host -p 8080:8080 chalmersrevere/opendlv-signal-viewer-amd64:v0.0.2 --cid=111`
![screenshot from signal viewer](https://raw.githubusercontent.com/chalmers-revere/opendlv-signal-viewer/master/signal-viewer.gif)
  * [opendlv-vehicle-view](https://github.com/chalmers-revere/opendlv-vehicle-view) to view vehicle messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8081): `docker run --rm --net=host -p 8081:8081 chalmersrevere/opendlv-vehicle-view-amd64:v0.0.1 --cid=111`

  ![screenshot from vehicle view](https://raw.githubusercontent.com/chalmers-revere/opendlv-vehicle-view/master/vehicle-view.gif)



## Build from sources on the example of Ubuntu 16.04 LTS
To build this software, you need cmake, C++14 or newer, and make. Having these
preconditions, just update the contained submodules.

```
git submodule update --init --recursive
git submodule update --remote --merge
git submodule status
```

Now, run `cmake` and `make` as follows:

```
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make && make test && make install
```


## License

* This project is released under the terms of the GNU GPLv3 License

