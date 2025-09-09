#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// #include <gnss_someip_lib/msg/gnss_data.hpp> 

#include <types/conversion.h>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/buffer.h>
#include <tf2_msgs/msg/tf_message.hpp>

#include <tf2_ros/static_transform_broadcaster_node.hpp>

#include <geometry_msgs/msg/transform_stamped.hpp>


using GpsDataMsg = geometry_msgs::msg::TransformStamped;
using GnssData = v0::gnss::common::TransformStamped;
template <typename SomeIpClient> 
class SomeIpPublisher : public rclcpp::Node
{
    static constexpr auto Topic = "afterSOMEIP";
    static constexpr auto QoS = 10;

public:
    SomeIpPublisher(std::string node_name) 
        : Node(node_name)
    {
        publisher = this->create_publisher<GpsDataMsg>(Topic, QoS);

        someip_client.setMessageCallback(std::bind(&SomeIpPublisher::publish, this, std::placeholders::_1));
    }

private:

    void publish(const GpsDataMsg & message) {        

        RCLCPP_INFO(this->get_logger(), "Publishing SOME/IP message on topic %s", Topic);

        publisher->publish(message);
    }

private:
    SomeIpClient someip_client;
    
    rclcpp::Publisher<GpsDataMsg>::SharedPtr publisher;
};
