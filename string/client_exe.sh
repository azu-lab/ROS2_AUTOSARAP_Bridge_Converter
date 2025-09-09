#!/bin/bash

curpath=`pwd`

# 環境変数の設定
export COMMONAPI_DEFAULT_FOLDER=/${curpath}/install/gnss_someip_lib/lib/
export COMMONAPI_CONFIG=${curpath}/install/gnss_someip_lib/etc/commonapi.ini


# ROS2の環境設定
source /opt/ros/humble/setup.bash

source ${curpath}/install/setup.bash
# VSOMEIPの設定
export VSOMEIP_CONFIGURATION=${curpath}/config/vsomeip-client.json


# dlt-daemonが実行中でないことを確認してから起動
if ! pgrep -x "dlt-daemon" > /dev/null
then
    echo "dlt-daemon is not running."
    dlt-daemon -&
fi

# routingmanagerdが実行中でないことを確認してから起動
if ! pgrep -x "routingmanagerd" > /dev/null
then
    echo "routingmanagerd is not running."
    routingmanagerd &
fi


# gnss-clientの起動
VSOMEIP_APPLICATION_NAME=gnss-client ros2 run gnss_bridge gnss-bridge &

ros2 run gnss_listener gnss-listener
