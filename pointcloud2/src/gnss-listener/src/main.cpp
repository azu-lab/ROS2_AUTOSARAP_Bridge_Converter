#include "rclcpp/rclcpp.hpp"

// #include <gnss_someip_lib/msg/gnss_data.hpp>

#include "std_msgs/msg/string.hpp"

#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>

#include <sensor_msgs/msg/point_cloud2.hpp>

#define DATA_SIZE 128 //ファイル名用
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)



using  GpsDataMsg = sensor_msgs::msg::PointCloud2;
class GnssTopicSubsriber : public rclcpp::Node
{
  static constexpr auto node_name = "PC2_Topic_Subscriber";

  static constexpr auto topic = "returnSOMEIP";
  static constexpr auto qos = 10;

public:
    GnssTopicSubsriber() : Node(node_name)
    {
      subscription = this->create_subscription<sensor_msgs::msg::PointCloud2>(topic, qos, std::bind(&GnssTopicSubsriber::topic_callback, this, std::placeholders::_1));
    }

private:

     void topic_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) const
  {


    std::ofstream endFile;
    endFile.open("PointCloud2_evaluation_2pc/evaluation_" TOSTRING(DATA_SIZE) "/08_end_time_all_" TOSTRING(DATA_SIZE) ".csv", std::ios::app);
    auto end = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
    endFile << "Run" << std::setw(6) << (time_count_all_end++) << ":" << end_time << std::endl;

     RCLCPP_INFO(rclcpp::get_logger("PointCloud2Logger_topic"), 
        "PointCloud2 Info:\n"
        " - Height: %u\n"
        " - Width: %u\n"
        " - Point Step: %u\n"
        " - Row Step: %u\n"
        " - Is Dense: %s\n"
        " - Data Size: %lu",
        msg->height,
        msg->width,
        msg->point_step,
        msg->row_step,
        msg->is_dense ? "True" : "False",
        msg->data.size()
        
        );

         std::stringstream tt;
                    for (const auto& field : msg->fields) {
                    tt << "Name: " << field.name<< ", "
                    << "Offset: " << field.offset << ", "
                    << "Datatype: " << static_cast<int>(field.datatype) << ", "
                    << "Count: " << field.count << std::endl;
                }
                    RCLCPP_INFO(rclcpp::get_logger("logger_name"), "PointCloud2 Fields LI:\n%s", tt.str().c_str());


  }
  // rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription;

    mutable int time_count_all_end = 0;
};

auto main(int argc, char * argv[]) -> int
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GnssTopicSubsriber>());
  rclcpp::shutdown();
  return 0;
}
