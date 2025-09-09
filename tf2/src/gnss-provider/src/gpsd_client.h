#pragma once
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/serialization.hpp>

// #include "geometry_msgs/msg/pose.hpp"

#include <string>
#include <vector>


#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>

#include <random>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "std_msgs/msg/string.hpp"

#include "tf2_ros/static_transform_broadcaster_node.hpp"

#include "tf2_ros/transform_broadcaster.h"


class GpsdClient : public rclcpp::Node {
    static constexpr auto node_name = "TF2_Client_node";
    static constexpr auto waiting_time = 1000000;
    static constexpr auto topic = "tf";
    static constexpr auto qos = 10;
    static constexpr auto gpsd_read_timer_delay = 2000ms;

    // std::chrono::system_clock::time_point  start;
    // std::time_t time_stamp;

public:

    

    GpsdClient() : Node(node_name)
    {
        // publisher = this->create_publisher<GnssDataMsg>(topic, qos);
        // if(!init()) {
        //     RCLCPP_WARN(this->get_logger(), "No connection to gpsd");
        // }

        timer = this->create_wall_timer(gpsd_read_timer_delay, std::bind(&GpsdClient::timer_callback, this));
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  }

    
    ~GpsdClient() = default;
    
    void timer_callback()
    {

        std::random_device rd;  // 乱数生成器のシードを初期化
        std::mt19937 gen(rd()); // 標準のメルセンヌ・ツイスター乱数生成器

        // 位置と回転に使用するための乱数分布を定義（ここでは例として-10.0から10.0まで）
        std::uniform_real_distribution<> dis(-10.0, 10.0);


        geometry_msgs::msg::TransformStamped t;
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "parent_frame";
        t.child_frame_id = "child_frame";

        // 位置（translation）のランダムな値を設定
        t.transform.translation.x = dis(gen);
        t.transform.translation.y = dis(gen);
        t.transform.translation.z = dis(gen);

        // 回転（rotation）のランダムな値を設定
        t.transform.rotation.x = dis(gen);
        t.transform.rotation.y = dis(gen);
        t.transform.rotation.z = dis(gen);
        t.transform.rotation.w = dis(gen); // 正規化が必要な場合はここで行う

        tf_broadcaster_->sendTransform(t);

RCLCPP_INFO(this->get_logger(), 
    "Sent transform:\n"
    "  transforms:\n"
    "  - header:\n"
    "      stamp:\n"
    "        sec: %d\n"
    "        nanosec: %u\n"
    "      frame_id: %s\n"
    "    child_frame_id: %s\n"
    "    transform:\n"
    "      translation:\n"
    "        x: %f\n"
    "        y: %f\n"
    "        z: %f\n"
    "      rotation:\n"
    "        x: %f\n"
    "        y: %f\n"
    "        z: %f\n"
    "        w: %f",
    t.header.stamp.sec, 
    t.header.stamp.nanosec, 
    t.header.frame_id.c_str(), 
    t.child_frame_id.c_str(), 
    t.transform.translation.x, 
    t.transform.translation.y, 
    t.transform.translation.z, 
    t.transform.rotation.x, 
    t.transform.rotation.y, 
    t.transform.rotation.z, 
    t.transform.rotation.w);

        //   publisher->publish(message);
        //   auto start = std::chrono::high_resolution_clock::now();
        //   auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
        //   startFile << "Run" << std::setw(5) << (time_count_ROS2_start++) << ":" << start_time << std::endl;
    }

private:

    std::mutex mutex;
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Publisher<GnssDataMsg>::SharedPtr publisher;
    size_t count_;
    int time_count_ROS2_start = 0;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    
};