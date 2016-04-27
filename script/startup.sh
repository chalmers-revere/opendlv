#!/bin/bash

gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ../od3/bin/odsupercomponent --cid=111 --verbose=1"

sleep 2

# Proxy
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-core-proxy-can --cid=111 --freq=100"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-core-proxy-camera --cid=111 --freq=10"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-core-proxy-v2v --cid=111 --freq=1"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-core-proxy-gps --cid=111 --freq=1"

#sleep 2

# Sensation
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-sensation-audition --cid=111 --freq=1"

# Perception
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-perception-detectlane --cid=111 --freq=1"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-perception-detectvehicle --cid=111 --freq=1"

# Knowledge
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-knowledge-gcdc16-v2viclcm --cid=111 --freq=25"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-knowledge-linguistics-v2vcam --cid=111 --freq=25"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-knowledge-linguistics-v2vdenm --cid=111 --freq=25"

# Action
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-action-act --cid=111 --freq=100"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-action-communicate --cid=111"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-action-keepobjectsize --cid=111"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-action-keepobjectalignment --cid=111"
gnome-terminal -x bash -c "LD_LIBRARY_PATH=../od3/lib/:lib ./bin/opendlv-system-application-action-setopticalflow --cid=111"


