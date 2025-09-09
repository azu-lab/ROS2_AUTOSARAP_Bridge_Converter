#pragma once 

#include <CommonAPI/CommonAPI.hpp>

#include <v0/gnss/GnssServerStubDefault.hpp>

#include <rclcpp/rclcpp.hpp>

#include "std_msgs/msg/string.hpp"

#include <types/conversion.h>

// #include "geometry_msgs/msg/pose.hpp"

// #include <tf2_ros/transform_listener.h>
// #include <geometry_msgs/msg/transform_stamped.hpp>

// #include "tf2_ros/static_transform_broadcaster_node.hpp"

// #include "tf2_ros/transform_broadcaster.h"
// #include <tf2_ros/buffer.h>
// #include <tf2_msgs/msg/tf_message.hpp>

#include <sensor_msgs/msg/point_cloud2.hpp>



#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>

#define DATA_SIZE 4096 //ファイル名用
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


class GnssSomeIpProvider : public v0::gnss::GnssServerStubDefault {

public:
    GnssSomeIpProvider() = default;
    ~GnssSomeIpProvider() = default;

    int time_count_SOMEIP_start = 0;
    int time_count_before_conversion = 0;
    int time_count_after_conversion = 0;
    

    void fireDataEvent(const GnssDataMsg & PC2_data) {
        
        RCLCPP_INFO(rclcpp::get_logger("PC2_SOMEIP_Provider"), "Sending PointCloud data over SOME/IP.");

        auto filename = std::to_string(PC2_data.width);

        std::ofstream startFile;    
        std::ofstream before_conversionFile;
        std::ofstream after_conversionFile;
        before_conversionFile.open("PointCloud2_evaluation_2pc/evaluation_" + filename + "/03_before_conversion_times_SOMEIP_" + filename + ".csv", std::ios::app);
        after_conversionFile.open("PointCloud2_evaluation_2pc/evaluation_" + filename + "/04_after_conversion_times_SOMEIP_" + filename + ".csv", std::ios::app);
        startFile.open("PointCloud2_evaluation_2pc/evaluation_" + filename + "/05_start1_times_SOMEIP_" + filename + ".csv", std::ios::app);
        
        auto before_conversion = std::chrono::high_resolution_clock::now();
        auto before_conversion_time = std::chrono::time_point_cast<std::chrono::microseconds>(before_conversion).time_since_epoch().count();
        
        

        auto data = Types::Conversion::to_capi_type(PC2_data);//convert to capi type

        auto after_conversion = std::chrono::high_resolution_clock::now();
        auto after_conversion_time = std::chrono::time_point_cast<std::chrono::microseconds>(after_conversion).time_since_epoch().count();
        
       

        // std::stringstream ss;
        // for (const auto& field : data.getFields()) {
        // ss << "Name: " << field.getName() << ", "
        // << "Offset: " << field.getOffset() << ", "
        // << "Datatype: " << static_cast<int>(field.getDatatype()) << ", "
        // << "Count: " << field.getCount() << std::endl;
        // }

        // RCLCPP_INFO(rclcpp::get_logger("logger_name"), "PointCloud2 Fields after convert PR:\n%s", ss.str().c_str());

        
        

        auto start = std::chrono::high_resolution_clock::now();
        auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();

        GnssServerStub::fireDataEvent(data);//send data

        before_conversionFile << "Run" << std::setw(6) << (time_count_before_conversion++) << "," << before_conversion_time << std::endl;

        after_conversionFile << "Run" << std::setw(6) << (time_count_after_conversion++) << "," << after_conversion_time << std::endl;

        startFile << "Run" << std::setw(6) << (time_count_SOMEIP_start++) << "," << start_time << std::endl;

        // auto start = std::chrono::high_resolution_clock::now();
        // auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
        // startFile << "Run" << std::setw(5) << (time_count_SOMEIP_start++) << ":" << start_time << std::endl;

        
    }

};

template <typename T>
class GnssSomeIpReporter : public rclcpp::Node
{
    static constexpr auto node_name = "PC2_SOMEIP_Reporter";

    static constexpr auto domain = "local";
    static constexpr auto instance = "GnssServer";
    static constexpr auto timer_duration = 500ms;

    static constexpr auto topic = "beforeSOMEIP";
    static constexpr auto qos = 10;


public:
    GnssSomeIpReporter()
        : Node(node_name)
        , someip_provider(std::make_shared<T>())
    {
  
          if(register_someip_service()) {
            RCLCPP_INFO(this->get_logger(), "SOME/IP Server has been registered");

            gpsd_data_subscription = this->create_subscription<sensor_msgs::msg::PointCloud2>(topic, qos, std::bind(&GnssSomeIpReporter::on_gpsd_data, this, std::placeholders::_1));
            
            publish_timer = this->create_wall_timer(timer_duration, [this]() {            
                            
                std::lock_guard<std::mutex> guard(mutex);
                
                someip_provider->fireDataEvent(PC2_data);

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

    void on_gpsd_data(const sensor_msgs::msg::PointCloud2 & msg) 
    {

        std::lock_guard<std::mutex> guard(mutex);


        PC2_data.header = msg.header;
        PC2_data.height = msg.height;
        PC2_data.width = msg.width;
        PC2_data.fields = msg.fields;
        PC2_data.is_bigendian = msg.is_bigendian;
        PC2_data.point_step = msg.point_step;
        PC2_data.row_step = msg.row_step;
        PC2_data.data = msg.data;
        PC2_data.is_dense = msg.is_dense;
        //データの受け渡し



    
        auto end = std::chrono::high_resolution_clock::now();
        auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
        std::ofstream endFile;
        auto filename = std::to_string(PC2_data.width);
        endFile.open("PointCloud2_evaluation_2pc/evaluation_" + filename + "/02_end_times_ROS2_" + filename + ".csv", std::ios::app);
        endFile << "Run" << std::setw(6) << (time_count_ROS2_end++) << "," << end_time << std::endl;

        RCLCPP_INFO(this->get_logger(), "Received PointCloud2 data from PC2_Client node");

        RCLCPP_INFO(rclcpp::get_logger("PointCloud2Logger_SOMEIP_Reporter"), 
        "PointCloud2 Info:\n"
        " - Height: %u\n"
        " - Width: %u\n"
        " - Point Step: %u\n"
        " - Row Step: %u\n"
        " - Is Dense: %s\n"
        " - Data Size: %lu",
        PC2_data.height,
        PC2_data.width,
        PC2_data.point_step,
        PC2_data.row_step,
        PC2_data.is_dense ? "True" : "False",
        PC2_data.data.size()
        );//データの中身を表示

        std::stringstream ss;
    for (const auto& field : PC2_data.fields) {
    ss << "Name: " << field.name << ", "
       << "Offset: " << field.offset << ", "
       << "Datatype: " << static_cast<int>(field.datatype) << ", "
       << "Count: " << field.count << std::endl;
}
    RCLCPP_INFO(rclcpp::get_logger("logger_name"), "PointCloud2 Fields:\n%s", ss.str().c_str());

        
        
    }

private:
    rclcpp::TimerBase::SharedPtr publish_timer;
    std::shared_ptr<T> someip_provider;

    std::mutex mutex;

    sensor_msgs::msg::PointCloud2 PC2_data;

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr gpsd_data_subscription;

    int time_count_ROS2_end = 0;
   


};