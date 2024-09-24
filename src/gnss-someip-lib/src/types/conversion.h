#pragma once 

#include <v1/gnss/common.hpp>
#include "std_msgs/msg/string.hpp"
#include <libgpsmm.h>

using namespace std::chrono;
using GnssDataMsg = std_msgs::msg::String;
using GnssData = v1::gnss::common::Str;

namespace Types::Conversion {

/**
 * @brief converts ROS2 MSG to CommonAPI generated data type 
 * 
 * @param gps_data 
 * @return GnssData 
 */
std::string to_capi_type(const GnssDataMsg & gps_data) {
    GnssData gnss_data;

    gnss_data.setStr(gps_data.data);

    std::string str = gnss_data.getStr();

    return str;
}

/**
 * @brief converts CommonAPI generated data type to ROS2 data type
 * 
 * @param gps_data 
 * @return GnssData 
 */
 GnssDataMsg from_capi_type(const GnssData & gnss_data) {
    
    GnssDataMsg gps_data_msg; 

    gps_data_msg.data = gnss_data.getStr();
    

    return gps_data_msg;
 }


} // namespace TypeConversion