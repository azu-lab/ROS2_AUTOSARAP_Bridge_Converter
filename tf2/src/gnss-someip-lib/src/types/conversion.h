#pragma once 

#include <v0/gnss/common.hpp>
#include "std_msgs/msg/string.hpp"
#include <libgpsmm.h>
#include <builtin_interfaces/msg/time.hpp>

// #include "geometry_msgs/msg/pose.hpp"


#include "tf2_ros/static_transform_broadcaster_node.hpp"

using namespace std::chrono;
using GnssDataMsg = geometry_msgs::msg::TransformStamped;
using GnssData = v0::gnss::common::TransformStamped;

namespace Types::Conversion {

/**
 * @brief converts ROS2 MSG to CommonAPI generated data type 
 * 
 * @param gps_data 
 * @return GnssData 
 */
GnssData to_capi_type(const GnssDataMsg & gps_data) {
    GnssData gnss_data;

    v0::gnss::common::TransformStamped transformStamped;
    v0::gnss::common::Header header;
    v0::gnss::common::Sec_nanosec time_stamp_2;
    v0::gnss::common::Tf2_transform transform;
    v0::gnss::common::Vector3 vector;
    v0::gnss::common::Quaternion orientation;

    // const builtin_interfaces::msg::Time_& stamp = gps_data.header.stamp;

    // // 秒数をdoubleに変換
    // double time_stamp = stamp.sec + static_cast<double>(stamp.nanosec) / 1e9;
    // const v0::gnss::common::Sec_nanosec& gnss_time_stamp = gnss_data.header.getTime_stamp();

    // builtin_interfaces::msg::Time_ ros_time_stamp;



    orientation.setX(gps_data.transform.rotation.x);
    orientation.setY(gps_data.transform.rotation.y);
    orientation.setZ(gps_data.transform.rotation.z);
    orientation.setW(gps_data.transform.rotation.w);

    vector.setX(gps_data.transform.translation.x);
    vector.setY(gps_data.transform.translation.y);
    vector.setZ(gps_data.transform.translation.z);

    transform.setOrientation(orientation);
    transform.setVector(vector);

    gnss_data.setTransform(transform);

    time_stamp_2.setSec(gps_data.header.stamp.sec);
    time_stamp_2.setNanosec(gps_data.header.stamp.nanosec);

    header.setTime_stamp(time_stamp_2);

   
    // ros_time_stamp.sec = gnss_time_stamp.getSec();
    // ros_time_stamp.nanosec = gnss_time_stamp.getNanosec();

    // gnss_data.header.time_stamp = ros_time_stamp;

    header.setFrame_id(gps_data.header.frame_id);

    gnss_data.setHeader(header);

    gnss_data.setChild_frame_id(gps_data.child_frame_id);




    return gnss_data;
}

/**
 * @brief converts CommonAPI generated data type to ROS2 data type
 * 
 * @param gps_data 
 * @return GnssData 
 */
 GnssDataMsg from_capi_type(const GnssData & gnss_data) {

    GnssDataMsg gps_data_msg;

    gps_data_msg.header.stamp.sec = gnss_data.getHeader().getTime_stamp().getSec();
    gps_data_msg.header.stamp.nanosec = gnss_data.getHeader().getTime_stamp().getNanosec();
    gps_data_msg.header.frame_id = gnss_data.getHeader().getFrame_id();

    gps_data_msg.child_frame_id = gnss_data.getChild_frame_id();

    gps_data_msg.transform.rotation.x = gnss_data.getTransform().getOrientation().getX();
    gps_data_msg.transform.rotation.y = gnss_data.getTransform().getOrientation().getY();
    gps_data_msg.transform.rotation.z = gnss_data.getTransform().getOrientation().getZ();
    gps_data_msg.transform.rotation.w = gnss_data.getTransform().getOrientation().getW();

    gps_data_msg.transform.translation.x = gnss_data.getTransform().getVector().getX();
    gps_data_msg.transform.translation.y = gnss_data.getTransform().getVector().getY();
    gps_data_msg.transform.translation.z = gnss_data.getTransform().getVector().getZ();

    

    // gps_data_msg.header.stamp = gnss_data.getHeader().getTime_stamp();
    // gps_data_msg.header.frame_id = gnss_data.getHeader().getFrame_id();
    // gps_data_msg.child_frame_id = gnss_data.getChild_frame_id();


    // gps_data_msg.transform.rotation.x = gnss_data.getTransform().getOrientation().getX();
    // gps_data_msg.transform.rotation.y = gnss_data.getTransform().getOrientation().getY();
    // gps_data_msg.transform.rotation.z = gnss_data.getTransform().getOrientation().getZ();
    // gps_data_msg.transform.rotation.w = gnss_data.getTransform().getOrientation().getW();

    // gps_data_msg.transform.translation.x = gnss_data.getTransform().getVector().getX();
    // gps_data_msg.transform.translation.y = gnss_data.getTransform().getVector().getY();
    // gps_data_msg.transform.translation.z = gnss_data.getTransform().getVector().getZ();

  


    
    
    return gps_data_msg;
 }


} // namespace TypeConversion