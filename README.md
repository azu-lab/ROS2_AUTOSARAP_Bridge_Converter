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

## Repository layout

Top-level directories and important files in this repository:

- `*/src/` — primary ROS 2 packages for the bridge (for example: `gnss_someip_lib`, `gnss_bridge`, `gnss_listener`, `gnss_provider`, and `franca2ros`).
- `string/` — workspace variants used for quick testing and experiments; each contains its own `src/`, `build/`, `install/`, and local configs.
- `pointcloud2/`, `tf2/` — additional workspace snapshots containing examples, Dockerfiles, and tf/pointcloud-related packages and artifacts.
- `ros/` — shared message definitions (if present) used across workspaces.
- `config/` — VSOMEIP configuration files (`vsomeip-client.json`, `vsomeip-server.json`) and init scripts.
- `install/`, `build/`, `log/` — build artifacts and logs (these are typically ignored by `.gitignore`).
- Top-level scripts: `colcon.sh`, `client_exe.sh`, `server_exe.sh`, `kill_routingmanager.sh`, `Makefile` — helpers for building and running the workspace.

Build examples (run from repository root):

```bash
# Full workspace build
colcon build

# Build individual packages only
colcon build --packages-select gnss_someip_lib
colcon build --packages-select gnss_bridge
```

Note: generated files such as `.msg` and other translator outputs are listed in `.gitignore`. If generated files were previously committed, remove them from the index with `git rm --cached <file>`.

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

* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka and Takuya Azumi, "AUTOSAR AP and ROS 2 Collaboration Framework for Development in Cloud Environment," in IEEE Open Journal of the Industrial Electronics Society, doi: 10.1109/OJIES.2025.3607248.
* Ryudai Iwakami, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “Collaboration between the Autoware Ecosystem and AUTOSAR AP via an Open-Source DDS-SOME/IP Bridge Converter.” AutowareCon, Nagoya, Japan, 2025.
* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “ROS 2 and AUTOSAR AP Integration.” ROSCon JP, Tokyo, Japan, 2024.
* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka and Takuya Azumi, “AUTOSAR AP and ROS 2 Collaboration Framework.” In Proceedings of the 27th Euromicro Conference Series on Digital System Design (DSD 2024), Paris, France, August 28-30, 2024.
* Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka, and Takuya Azumi, “DDS and SOME/IP Collaboration Framework.” Workshop on Embedded Technology and Network (ETNET), Nagasaki, 2024.


## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](./LICENSE) file for details.
## Repository layout

Top-level directories and important files in this repository:

- `src/` — primary ROS 2 packages for the bridge (for example: `gnss_someip_lib`, `gnss_bridge`, `gnss_listener`, `gnss_provider`, and `franca2ros`).
- `string/`, `int/` — workspace variants used for quick testing and experiments; each contains its own `src/`, `build/`, `install/`, and local configs.
- `pointcloud2/`, `tf2/` — additional workspace snapshots containing examples, Dockerfiles, and tf/pointcloud-related packages and artifacts.
- `ros/` — shared message definitions (if present) used across workspaces.
- `config/` — VSOMEIP configuration files (`vsomeip-client.json`, `vsomeip-server.json`) and init scripts.
- `install/`, `build/`, `log/` — build artifacts and logs (these are typically ignored by `.gitignore`).
- Top-level scripts: `colcon.sh`, `client_exe.sh`, `server_exe.sh`, `kill_routingmanager.sh`, `Makefile` — helpers for building and running the workspace.

Build examples (run from repository root):

```bash
# Full workspace build
colcon build

# Build individual packages only
colcon build --packages-select gnss_someip_lib
colcon build --packages-select gnss_bridge
```

Note: generated files such as `.msg` and other translator outputs are listed in `.gitignore`. If generated files were previously committed, remove them from the index with `git rm --cached <file>`.
