#pragma once 

#include <v0/gnss/common.hpp>
#include "std_msgs/msg/string.hpp"
#include <libgpsmm.h>
#include <builtin_interfaces/msg/time.hpp>

// #include "geometry_msgs/msg/pose.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

#include <iostream>

// #include "tf2_ros/static_transform_broadcaster_node.hpp"

using namespace std::chrono;
using GnssDataMsg = sensor_msgs::msg::PointCloud2;
using GnssData = v0::gnss::common::PointCloud2;

namespace Types::Conversion {

/**
 * @brief converts ROS2 MSG to CommonAPI generated data type 
 * 
 * @param gps_data 
 * @return GnssData 
 */
GnssData to_capi_type(const GnssDataMsg & gps_data) {
    GnssData gnss_data;


    v0::gnss::common::Header header;
    v0::gnss::common::Sec_nanosec time_stamp_2;
    v0::gnss::common::PointField point_field[gps_data.fields.size()];
    v0::gnss::common::PointCloud2 point_cloud_2;
    std::vector<v0::gnss::common::PointField> point_fields;


    time_stamp_2.setSec(gps_data.header.stamp.sec);
    time_stamp_2.setNanosec(gps_data.header.stamp.nanosec);

    header.setTime_stamp(time_stamp_2);

    header.setFrame_id(gps_data.header.frame_id);

    
    if (!gps_data.fields.empty()) {
        for (size_t i = 0; i < gps_data.fields.size(); ++i) {
            point_field[i].setName(gps_data.fields[i].name);
            point_field[i].setOffset(gps_data.fields[i].offset);
            point_field[i].setDatatype(gps_data.fields[i].datatype);
            point_field[i].setCount(gps_data.fields[i].count);
            point_fields.push_back(point_field[i]);
            gnss_data.setFields(point_fields);

        }
        // point_field[i].setName(gps_data.fields[i].name);
        // // point_field.setOffset(gps_data.fields[0].offset);
        // // point_field.setDatatype(gps_data.fields[0].datatype);
        // // point_field.setCount(gps_data.fields[0].count);

        
        
    }
 

    gnss_data.setHeader(header);
    gnss_data.setHeight(gps_data.height);
    gnss_data.setWidth(gps_data.width);
    gnss_data.setIs_bigendian(gps_data.is_bigendian);
    gnss_data.setPoint_step(gps_data.point_step);
    gnss_data.setRow_step(gps_data.row_step);
    gnss_data.setData(gps_data.data);
    gnss_data.setIs_dense(gps_data.is_dense);

    
    
    return gnss_data;
}

/**
 * @brief converts CommonAPI generated data type to ROS2 data type
 * 
 * @param gps_data 
 * @return GnssData 
 */
 GnssDataMsg from_capi_type(const GnssData & PC2_data) {

    GnssDataMsg PC2_data_msg;

    PC2_data_msg.header.stamp.sec = PC2_data.getHeader().getTime_stamp().getSec();
    PC2_data_msg.header.stamp.nanosec = PC2_data.getHeader().getTime_stamp().getNanosec();
    PC2_data_msg.header.frame_id = PC2_data.getHeader().getFrame_id();

    if(PC2_data.getFields().size() > 0) {
        PC2_data_msg.fields.resize(PC2_data.getFields().size());

        for (size_t i = 0; i < PC2_data.getFields().size(); ++i) {
    PC2_data_msg.fields[i].name = PC2_data.getFields()[i].getName();
    PC2_data_msg.fields[i].offset = PC2_data.getFields()[i].getOffset();
    PC2_data_msg.fields[i].datatype = PC2_data.getFields()[i].getDatatype();
    PC2_data_msg.fields[i].count = PC2_data.getFields()[i].getCount();

    // std::cout << "PC2_data_msg.fields[i].name: " << PC2_data_msg.fields[i].name << std::endl;
}
    }

        

    // PC2_data_msg.fields[0].name = PC2_data.getFields()[0].getName();
    // PC2_data_msg.fields[0].offset = PC2_data.getFields()[0].getOffset();
    // PC2_data_msg.fields[0].datatype = PC2_data.getFields()[0].getDatatype();
    // PC2_data_msg.fields[0].count = PC2_data.getFields()[0].getCount();

    PC2_data_msg.height = PC2_data.getHeight();
    PC2_data_msg.width = PC2_data.getWidth();
    PC2_data_msg.is_bigendian = PC2_data.getIs_bigendian();
    PC2_data_msg.point_step = PC2_data.getPoint_step();
    PC2_data_msg.row_step = PC2_data.getRow_step();
    PC2_data_msg.data = PC2_data.getData();
    PC2_data_msg.is_dense = PC2_data.getIs_dense();

    

    return PC2_data_msg;



 }


} // namespace TypeConversion