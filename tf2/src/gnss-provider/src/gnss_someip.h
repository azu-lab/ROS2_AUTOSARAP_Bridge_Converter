#pragma once 

#include <CommonAPI/CommonAPI.hpp>

#include <v0/gnss/GnssServerStubDefault.hpp>

#include <rclcpp/rclcpp.hpp>

#include "std_msgs/msg/string.hpp"

#include <types/conversion.h>

#include "geometry_msgs/msg/pose.hpp"

#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include "tf2_ros/static_transform_broadcaster_node.hpp"

#include "tf2_ros/transform_broadcaster.h"
#include <tf2_ros/buffer.h>
#include <tf2_msgs/msg/tf_message.hpp>



#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>



class GnssSomeIpProvider : public v0::gnss::GnssServerStubDefault {

public:
    GnssSomeIpProvider() = default;
    ~GnssSomeIpProvider() = default;

    int time_count_SOMEIP_start = 0;
    

    void fireDataEvent(const GnssDataMsg & gps_data) {
        
        RCLCPP_INFO(rclcpp::get_logger("TF2_SOMEIP_Provider"), "Sending string data over SOME/IP.");

        // std::ofstream startFile;
        // startFile.open("start_times_SOMEIP_1024byte.csv", std::ios::app);

        

        auto data = Types::Conversion::to_capi_type(gps_data);

        GnssServerStub::fireDataEvent(data);

        // auto start = std::chrono::high_resolution_clock::now();
        // auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
        // startFile << "Run" << std::setw(5) << (time_count_SOMEIP_start++) << ":" << start_time << std::endl;

        
    }

};

template <typename T>
class GnssSomeIpReporter : public rclcpp::Node
{
    static constexpr auto node_name = "TF2_SOMEIP_Reporter";

    static constexpr auto domain = "local";
    static constexpr auto instance = "GnssServer";
    static constexpr auto timer_duration = 2000ms;

    static constexpr auto topic = "tf";
    static constexpr auto qos = 10;


public:
    GnssSomeIpReporter()
        : Node(node_name)
        , someip_provider(std::make_shared<T>())
    {
            
         
          if(register_someip_service()) {
            RCLCPP_INFO(this->get_logger(), "SOME/IP Server has been registered");

            gpsd_data_subscription = this->create_subscription<tf2_msgs::msg::TFMessage>("tf", qos, std::bind(&GnssSomeIpReporter::on_gpsd_data, this, std::placeholders::_1));
            

            

            publish_timer = this->create_wall_timer(timer_duration, [this]() {            
                
               
        
                std::lock_guard<std::mutex> guard(mutex);
                // std::ofstream startFile;
                // startFile.open("start_times_SOMEIP.csv", std::ios::app);
                
                someip_provider->fireDataEvent(gps_data);
                
                // auto start = std::chrono::high_resolution_clock::now();
                // auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
                // startFile << "Run" << std::setw(5) << (time_count_SOMEIP_start++) << ":" << start_time << std::endl;
            });
         }  
    }

protected:

    

    bool register_someip_service() {
        if(!CommonAPI::Runtime::get()->registerService(domain,instance, someip_provider)) {
            //TODO: handle error case correctly
            RCLCPP_ERROR(this->get_logger(), "Failed to register SOME/IP Server");
            return false;
        }

        return true;
    }

    void on_gpsd_data(const tf2_msgs::msg::TFMessage & msg) 
    {

        std::lock_guard<std::mutex> guard(mutex);

        
        tf2_msgs::msg::TFMessage tf2_msg = msg;
        geometry_msgs::msg::TransformStamped tf2_data;
        tf2_data = tf2_msg.transforms[0];


    
        // auto end = std::chrono::high_resolution_clock::now();
        // auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
        // std::ofstream endFile;
        // endFile.open("end_times_ROS2_1024byte.csv", std::ios::app);
        // endFile << "Run" << std::setw(5) << (time_count_ROS2_end++) << ":" << end_time << std::endl;
        // endFile.close();

        RCLCPP_INFO(this->get_logger(), "Received String data from Tf2_Client node");

        gps_data = tf2_data;

        RCLCPP_INFO(this->get_logger(), 
    "Received transform:\n"
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
    gps_data.header.stamp.sec, 
    gps_data.header.stamp.nanosec, 
    gps_data.header.frame_id.c_str(), 
    gps_data.child_frame_id.c_str(), 
    gps_data.transform.translation.x, 
    gps_data.transform.translation.y, 
    gps_data.transform.translation.z, 
    gps_data.transform.rotation.x, 
    gps_data.transform.rotation.y, 
    gps_data.transform.rotation.z, 
    gps_data.transform.rotation.w);

        
    }

private:
    rclcpp::TimerBase::SharedPtr publish_timer;
    std::shared_ptr<T> someip_provider;

    std::mutex mutex;

    geometry_msgs::msg::TransformStamped gps_data;

    rclcpp::Subscription<tf2_msgs::msg::TFMessage>::SharedPtr gpsd_data_subscription;

    

    int time_count_ROS2_end = 0;
    int time_count_SOMEIP_start = 0;

    
    
};