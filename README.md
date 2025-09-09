# ROS2\_AUTOSARAP\_Bridge\_Converter

## Overview

ROS2\_AUTOSARAP\_Bridge\_Converter is a bridge that performs conversion between DDS and SOME/IP, enabling communication between ROS 2 and the AUTOSAR Adaptive Platform (AUTOSAR AP).
With this bridge, ROS 2 and AUTOSAR AP can work together, and ROS 2 tools can be used within AUTOSAR AP.
This tool was released by Saitama University in collaboration with EMB IV and Hitachi Astemo(currently Astemo).

## Prerequisites

* ROS 2 Humble Hawksbill

## File Description

### Shell Scripts

* **colcon.sh**: Script for building the bridge converter
* **server\_exe.sh**: Script for launching the server side
* **client\_exe.sh**: Script for launching the client side
* **kill\_routingmanager.sh**: Script to force terminate server or client

### JSON Files (in `config/`)

* **vsomeip-server.json**: SOME/IP configuration file for the server
* **vsomeip-client.json**: SOME/IP configuration file for the client

## Setup

```bash
EXPORT COMMONAPI_CONFIG=<PATH>/src/install/gnss_someip_lib/etc/commonapi.ini
EXPORT COMMONAPI_DEFAULT_FOLDER=<PATH>/src/install/gnss_someip_lib/lib/

EXPORT INSTALL_PATH=/usr/bin

sudo apt-get update && apt install -y wget unzip git

# Installation of required tools
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata
sudo apt-get install -y openjdk-17-jdk openjdk-17-jre
sudo apt-get install -y g++ gcc cmake libboost-all-dev make doxygen asciidoc
sudo apt-get install -y net-tools iproute2
sudo apt-get install -y python3-pip

# Installation of required libraries
sudo apt-get install -y libboost-all-dev nlohmann-json3-dev graphviz source-highlight

sudo wget https://github.com/COVESA/capicxx-core-tools/releases/download/3.2.0.1/commonapi_core_generator.zip -P /opt 
    cd /opt 
    sudo unzip commonapi_core_generator.zip -d commonapi_core_generator 
    sudo ln -s /opt/commonapi_core_generator/commonapi-core-generator-linux-x86_64 $INSTALL_PATH/commonapi-core-generator

sudo wget https://github.com/COVESA/capicxx-someip-tools/releases/download/3.2.0.1/commonapi_someip_generator.zip -P /opt/ 
    cd /opt/ 
    sudo unzip commonapi_someip_generator.zip -d commonapi_someip_generator 
    sudo ln -s /opt/commonapi_someip_generator/commonapi-someip-generator-linux-x86_64 $INSTALL_PATH/commonapi-someip-generator

cd /opt && sudo git clone https://github.com/COVESA/dlt-daemon.git -b v2.18.8 && cd dlt-daemon 
    sudo mkdir build 
    cd build 
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr .. 
    sudo make -j$(nproc) 
    sudo make install
    
cd vsomeip_udp
    sudo mkdir build
    cd build
    sudo cmake 
    sudo make 
    sudo make install

cd /opt && sudo git clone https://github.com/COVESA/capicxx-core-runtime.git
    cd capicxx-core-runtime 
    sudo mkdir build 
    cd build 
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr .. 
    sudo make -j$(nproc) 
    sudo make install

cd /opt && sudo git clone https://github.com/COVESA/capicxx-someip-runtime.git  
cd capicxx-someip-runtime 
    sudo mkdir build 
    cd build 
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr .. 
    sudo make -j$(nproc) 
    sudo make install

cd /opt && sudo git clone https://github.com/cameron314/concurrentqueue.git
    cd concurrentqueue 
    sudo mkdir build 
    cd build 
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr .. 
    sudo make -j$(nproc) 
    sudo make install

sudo pip3 install git+https://github.com/miketsukerman/pyfranca.git

sudo pip3 install -e src/franca2ros
```

## Usage

1. Modify the configuration in `config/vsomeip-server.json` and `config/vsomeip-client.json` as needed
2. Configure the ServiceID and other parameters in `src/gnss-someip-lib/src/fidl/gnss.fdepl`
3. Run `colcon.sh` to build
4. Run `server_exe.sh` and `client_exe.sh` to launch the server and client

## Publications & Presentations

* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “DDS and SOME/IP Collaboration Framework.” Workshop on Embedded Technology and Network (ETNET), Nagasaki, 2024.
* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka and Takuya Azumi, “AUTOSAR AP and ROS 2 Collaboration Framework.” In Proceedings of the 27th Euromicro Conference Series on Digital System Design (DSD 2024), Paris, France, August 28-30, 2024.
* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “ROS 2 and AUTOSAR AP Integration.” ROSCon JP, Tokyo, Japan, 2024.
* Ryudai Iwakami, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “Collaboration between the Autoware Ecosystem and AUTOSAR AP via an Open-Source DDS-SOME/IP Bridge Converter.” AutowareCon, Nagoya, Japan, 2025.

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](./LICENSE) file for details.
