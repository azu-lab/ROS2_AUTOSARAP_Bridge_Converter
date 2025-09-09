#!/bin/bash

dlt-daemon -p 50002 &
routingmanagerd &

export VSOMEIP_CONFIGURATION=/home/ros2-humble/vsomeip-ros-bridge/config/vsomeip-client.json

VSOMEIP_APPLICATION_NAME=gnss-client ros2 run gnss_bridge gnss-bridge &

ros2 run gnss_listener gnss-listener
