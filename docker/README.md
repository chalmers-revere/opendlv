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





# Run all OpenDLV components with docker
[Example output]:http://i.imgur.com/eWsbz4m.jpg


Make sure to be located in the opendlv/docker folder before running.

**Start everything (-d flag sets detached mode):**
````
docker-compose up -d
````



**Stop everything:**
````
docker-compose down
````



**Show all (exited and current) docker containers (processes):**
````
docker ps -a
````
Example output from running `docker ps -a`:
![Example output][Example output]

As you can see everything is running except for two containers. Far right the name suggests it is the *camera* and *v2v*. To see the log for these failed containers you can run: 
````
docker logs [CONTAINER ID]
````

**Get log from running container. The name of service is found in the *docker-compose.yaml* file:**
````
docker-compose logs [NAME OF SERVICE]
````
*Example:*
````
docker-compose logs opendlv-system-application-perception-detectvehicle
````


### *docker-compose.yaml* file
```YAML
# Please note that docker-compose does not prescribe a startup order


version: "2"
services:
  odsupercomponent:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib
    - PATH=/opt/od/bin
    command: "odsupercomponent --cid=111"



  # Proxy

  opendlv-system-core-proxy-can:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-core-proxy-can --cid=111 --freq=100"


  opendlv-system-core-proxy-camera-0:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-core-proxy-camera --cid=111 --freq=10 --id=0"

  opendlv-system-core-proxy-camera-1:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-core-proxy-camera --cid=111 --freq=10 --id=1"


  opendlv-system-core-proxy-v2v:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-core-proxy-camera --cid=111 --freq=1"


  opendlv-system-core-proxy-gps:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-core-proxy-gps --cid=111 --freq=1"



  # Sensation

  opendlv-system-application-sensation-audition:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-sensation-audition --cid=111 --freq=1"
    


  # Perception

  opendlv-system-application-perception-detectlane:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-perception-detectlane --cid=111 --freq=1"
    
    
  opendlv-system-application-perception-detectvehicle:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-perception-detectvehicle --cid=111 --freq=1"



  # Knowledge

  opendlv-system-application-knowledge-gcdc16-v2viclcm:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-knowledge-gcdc16-v2viclcm --cid=111 --freq=25"
    
    
  opendlv-system-application-knowledge-linguistics-v2vcam:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-knowledge-linguistics-v2vcam --cid=111 --freq=25"
    
    
  opendlv-system-application-knowledge-linguistics-v2vdenm:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-knowledge-linguistics-v2vdenm --cid=111 --freq=25"
    
    
    
  # Action

  opendlv-system-application-action-act:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-act --cid=111 --freq=100"


  opendlv-system-application-action-communicate:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-communicate --cid=111"
    
    
  opendlv-system-application-action-keepobjectsize:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-keepobjectsize --cid=111"


  opendlv-system-application-action-keepobjectalignment:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-keepobjectalignment --cid=111"
    

  opendlv-system-application-action-setopticalflow:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-setopticalflow --cid=111"



  opendlv-new-component:
    image: pletron/opendlv:latest
    volumes:
    - .:/opt/data
    working_dir: "/opt/data"
    depends_on:
      - odsupercomponent
    environment:
    - LD_LIBRARY_PATH=/opt/od/lib:/opt/odlv/lib
    - PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/odlv/bin
    command: "opendlv-system-application-action-setopticalflow --cid=111"

```
