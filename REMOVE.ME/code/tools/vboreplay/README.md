# vboreplay

vboreplay is a tool to replay VBO recordings from VBOX equipment to be used with OpenDaVINCI-based application.

## Getting Started Guide

To use this software, you need to have a .VBO recording file.

First, you need to build this software. This will be done automatically by following the instructions given here: https://github.com/opendavinci/ReVeRe

Next, open a terminal to start odsupercomponent to provide the configuration data. This component needs to be started where the file 'configuration' is located in the source tree:

    $ cd ReVeRe
    $ odsupercomponent --cid=111

Next, open another terminal and start the visualization environment odcockpit in the same directory where the configuration file is located:

    $ cd ReVeRe
    $ odcockpit --cid=111

Once the visualization environment was opened, you can start the visualization plugin 'BirdsEyeMap'.

Finally, start replaying a recording file. Thus, open a terminal and point your current working directory to where the binaries of ReVeRe have been installed to:

    $ cd /opt/ReVeRe/bin
    $ LD_LIBRARY_PATH=/opt/ReVeRe/lib ./vboreplay --cid=111 --freq=50 < /PATH/TO/A/VBO/RECORDING.VBO

<img src="https://github.com/opendavinci/ReVeRe/blob/master/software/vboreplay/Visualization.png" alt="Visualization from a given VBO data trace" width=606 height=389>

