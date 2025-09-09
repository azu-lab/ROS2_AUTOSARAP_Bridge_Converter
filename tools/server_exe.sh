#!/bin/bash
curpath=`pwd`

# 環境変数の設定
export COMMONAPI_DEFAULT_FOLDER=/${curpath}/install/gnss_someip_lib/lib/
export COMMONAPI_CONFIG=${curpath}/install/gnss_someip_lib/etc/commonapi.ini


# ROS2の環境設定
source /opt/ros/humble/setup.bash
source ${curpath}/install/setup.bash

# VSOMEIPの設定
export VSOMEIP_CONFIGURATION=/${curpath}/config/vsomeip-server.json

# dlt-daemonが実行中でないことを確認してから起動
if ! pgrep -x "dlt-daemon" > /dev/null
then
    dlt-daemon -&
fi

# routingmanagerdが実行中でないことを確認してから起動
if ! pgrep -x "routingmanagerd" > /dev/null
then
    routingmanagerd &
fi

# gnss-serverの起動
VSOMEIP_APPLICATION_NAME=gnss-server ros2 run gnss_provider gnss-server 4096


