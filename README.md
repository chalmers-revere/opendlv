## OpenDLV

OpenDLV is an open source software environment to support the development and
testing of self-driving vehicles. Its design principle is based on microservices
and the provided software handles communication, safety and override functions,
sensor fusion, decision making, and visualisation.

It is written entirely in high-quality, standard C++14 with a strong focus is on
code clarity, portability, and performance.

[![Build Status](https://travis-ci.org/chalmers-revere/opendlv.svg?branch=new)](https://travis-ci.org/chalmers-revere/opendlv) [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.txt)


## Table of Contents
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build from sources on the example of Ubuntu 16.04 LTS](#build-from-sources-on-the-example-of-ubuntu-1604-lts)
* [License](#license)


## Dependencies
No dependencies! You just need a C++14-compliant compiler to compile this
project as it ships the following dependencies as part of the source distribution:

* [libcluon](https://github.com/chrberger/libcluon) - [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)
* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.1.2) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)


## Usage
The following microservices are provided:

* Hardware/software interfaces:
  * GPS:
    * [opendlv-device-gps-ncom](https://github.com/chalmers-revere/opendlv-device-gps-ncom) to interface with OXTS GPS/INSS units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-ncom-multi:v0.0.2 opendlv-device-gps-ncom --ncom_ip=0.0.0.0 --ncom_port=3000 --cid=111 --verbose`
    * [opendlv-device-gps-nmea](https://github.com/chalmers-revere/opendlv-device-gps-nmea) to interface with Trimble GPS/INSS units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-gps-nmea-multi:v0.0.3 opendlv-device-gps-nmea --nmea_ip=10.42.42.112 --nmea_port=9999 --cid=111 --verbose`
  * LIDAR:
    * [opendlv-device-lidar-hdl32e](https://github.com/chalmers-revere/opendlv-device-lidar-hdl32e) to interface with Velodyne HDL32e lidar units: `docker run --init --rm --net=host chalmersrevere/opendlv-device-lidar-hdl32e-multi:v0.0.2 opendlv-device-lidar-hdl32e --hdl32e_ip=0.0.0.0 --hdl32e_port=2368 --cid=111 --verbose`


## Build from sources on the example of Ubuntu 16.04 LTS
To build this software, you need cmake, C++14 or newer, and make. Having these
preconditions, just run `cmake` and `make` as follows:

```
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make && make test && make install
```


## License

* This project is released under the terms of the GNU GPLv3 License

