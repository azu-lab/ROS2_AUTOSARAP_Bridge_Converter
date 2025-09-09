#!/bin/bash

dlt-daemon -p 50002 &
routingmanagerd &

export VSOMEIP_CONFIGURATION=/home/ros2-humble/vsomeip-ros-bridge/config/vsomeip-server.json

VSOMEIP_APPLICATION_NAME=gnss-server ros2 run gnss_provider gnss-server

