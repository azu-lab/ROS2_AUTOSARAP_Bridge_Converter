# ROS2_AUTOSARAP_Bridge_Converter

## 概要
ROS2_AUTOSARAP_Bridge_ConverterはDDSとSOME/IPの変換を行い，ROS 2とAUTOSAR Adaptive Platform (AUTOSAR AP)間をつなぐブリッジです．このブリッジによってROS 2とAUTOSAR APの連携が可能になり，ROS 2のツールをAUTOSAR APで利用することが可能になります．
このツールは埼玉大学とEMB IV及び日立Astemoによってリリースされました．

## 前提条件
- ROS 2 humble hawksbill

## ファイルの説明
### shファイル
- colcon.sh: ブリッジコンバータをbuildする際のファイル
- server_exe.sh: server側を起動するファイル
- client_exe.sh: client側を起動するファイル
- kill_routingmanager.sh: serverやclientを強制終了するファイル

### jsonファイル(config内)
- vsomeip-server.json: server側のsomeipの設定ファイル
- vsomeip-client.json: client側のsomeipの設定ファイル

## セットアップ
```

EXPORT COMMONAPI_CONFIG=<PATH>/src/install/gnss_someip_lib/etc/commonapi.ini
EXPORT COMMONAPI_DEFAULT_FOLDER=<PATH>/src/install/gnss_someip_lib/lib/

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
    sudo ln -s /opt/commonapi_core_generator/commonapi-core-generator-linux-x86_64 /usr/bin/commonapi-core-generator

sudo wget https://github.com/COVESA/capicxx-someip-tools/releases/download/3.2.0.1/commonapi_someip_generator.zip -P /opt/ 
    cd /opt/ 
    sudo unzip commonapi_someip_generator.zip -d commonapi_someip_generator 
    sudo ln -s /opt/commonapi_someip_generator/commonapi-someip-generator-linux-x86_64 /usr/bin/commonapi-someip-generator

cd /opt && sudo git clone https://github.com/COVESA/dlt-daemon.git -b v2.18.8 &&cd dlt-daemon 
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

 
 cd /opt && sudo git clone https://github.com/COVESA/vsomeip.git && cd vsomeip 
    sudo mkdir build 
    cd build 
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr .. 
    sudo make -j$(nproc) 
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

## 利用方法
1. config/vsomeip-server.json, config/vsomeip-client.json内の設定を適切に変更する
2. src/gnss-someip-lib/src/fidl/gnss.fdepl内のServiceID等を適切に設定する
3. colcon.shを実行しbuild
4. - server_exe.sh, client_exe.shを実行し，server, clientを起動


## Publications & presentations
- 岩上竜大,彭博,羽生浩幸,石郷岡祐,安積 卓也,“DDSとSOME/IPの協調フレームワーク.”  組込み技術とネットワークに関するワークショップ ETNET,長崎,2024.
- Ryudai Iwakami, Bo Peng, Hiroyuki Hanyu, Tasuku Ishigooka and Takuya Azumi, “AUTOSAR AP and ROS 2 Collaboration Framework” In Proceeding of the 27th Euromicro Conference Series on Digital System Design (DSD 2024), Paris, France, August 28-30, 2024