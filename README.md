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

OpenDLV's microservices conform to the [OpenDLV Standard Message Set](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd) that enables exchangability across
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
**No dependencies!** You just need a C++14-compliant compiler to compile this
project as it ships the following dependencies as part of the source distribution:

* [libcluon](https://github.com/chrberger/libcluon) - [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)
* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.1.2) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)


## Usage
We are providing the following microservices as multi-platform (amd64/x86_64, armhf, aarch64) Docker images:

* Complete ArchLinux-based [OpenDLV OS](https://github.com/chalmers-revere/opendlv.os) Operating System (start here if you want to initialize a blank computing unit - *WARNING!* All data will be erased!)
* Hardware/software interfaces:
  * GPS:
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-ncom) [opendlv-device-gps-ncom](https://github.com/chalmers-revere/opendlv-device-gps-ncom) to interface with **OxTS GPS/INSS** units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.2 opendlv-device-gps-ncom --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111 --verbose`
      * Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L137-L140)
      * Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L133-L135)
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-nmea) [opendlv-device-gps-nmea](https://github.com/chalmers-revere/opendlv-device-gps-nmea) to interface with **Trimble GPS/INSS** units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.3 opendlv-device-gps-nmea --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111 --verbose`
      * Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L137-L140)
      * Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L133-L135)
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-gps-pos) [opendlv-device-gps-pos](https://github.com/chalmers-revere/opendlv-device-gps-pos) to interface with **Applanix POS GPS/INSS** units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-pos-multi:v0.0.1 opendlv-device-gps-pos --pos_ip=192.168.1.77 --pos_port=5602 --cid=111 --verbose`
      * Provides: [Latitude/Longitude](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L137-L140)
      * Provides: [Heading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L133-L135)
      * Provides: [device-specific messages](https://github.com/chalmers-revere/opendlv-device-gps-pos/blob/master/src/pos-message-set.odvd)
  * LIDAR:
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-vlp16) [opendlv-device-lidar-vlp16](https://github.com/chalmers-revere/opendlv-device-lidar-vlp16) to interface with **Velodyne VLP16** lidar units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-vlp16-multi:v0.0.1 opendlv-device-lidar-vlp16 --vlp16_ip=0.0.0.0 --vlp16_port=2368 --cid=111 --verbose`
      * Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L152-L158)
    * [![Build Status](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e.svg?branch=master)](https://travis-ci.org/chalmers-revere/opendlv-device-lidar-hdl32e) [opendlv-device-lidar-hdl32e](https://github.com/chalmers-revere/opendlv-device-lidar-hdl32e) to interface with **Velodyne HDL32e** lidar units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.2 opendlv-device-lidar-hdl32e --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111 --verbose`
      * Provides: [PointCloudReading](https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L152-L158)
* Visualizations:
  * [opendlv-signal-viewer](https://github.com/chalmers-revere/opendlv-signal-viewer) to view any messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8080): `docker run --rm --net=host -p 8080:8080 chalmersrevere/opendlv-signal-viewer-multi:v0.0.4 --cid=111`
![screenshot from signal viewer](https://raw.githubusercontent.com/chalmers-revere/opendlv-signal-viewer/master/signal-viewer.gif)
  * [opendlv-vehicle-view](https://github.com/chalmers-revere/opendlv-vehicle-view) to view vehicle messages from the OpenDLV Standard Message Set exchanged in the communication session 111 (after starting this microservice, point your web-browser to the computer's IP address, port 8081): `docker run --rm --net=host -p 8081:8081 chalmersrevere/opendlv-vehicle-view-multi:v0.0.8 --cid=111`

  ![screenshot from vehicle view](https://raw.githubusercontent.com/chalmers-revere/opendlv-vehicle-view/master/vehicle-view-animated.gif)



## Build from sources on the example of Ubuntu 16.04 LTS
To build this software, you need cmake, C++14 or newer, and make. Having these
preconditions, update the contained submodules first.

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
