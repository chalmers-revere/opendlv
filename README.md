# OpenDLV

OpenDLV is an open software environment intended to run in autonomous self-driving vehicles. It runs on the vehicle itself and should handle hardware communication, safety and override functions, sensor fusion, decision making, and visualisation. It is written entirely in standard C++, and is formally tested (unit tests and code coverage).

The software is based on the compact middle-ware OpenDaVINCI (http://www.opendavinci.org).

## Clone and Build Source Repository

Clone this repository:

    $ git clone https://github.com/chalmers-revere/opendlv.git

You can build the entire source tree:

    $ cd opendlv && mkdir build && cd build
    $ cmake ..
    $ make

Alternatively you can use locally compiled version of OpenDaVINCI by instead running:

    $ cmake -DOPENDAVINCI_DIR=/path/to/opendavinci ..

## Test the Software

Next, you can test the software by starting several terminals.

Terminal 1 (odsupercomponent will load and provide the configuration data):

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/odsupercomponent --cid=111 --verbose=1

Terminal 2:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/autonomy --cid=111

Terminal 3:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/perception --cid=111

Terminal 4:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/safety --cid=111

Terminal 5:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/diagnostics --cid=111

Now, the different modules should start printing data to the console.
