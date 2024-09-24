#include "rclcpp/rclcpp.hpp"

#include <gnss_someip_lib/msg/gnss_data.hpp>

#include "std_msgs/msg/string.hpp"

using  GpsDataMsg = std_msgs::msg::String;

class GnssTopicSubsriber : public rclcpp::Node
{
  static constexpr auto node_name = "STR_Topic_Subscriber";

  static constexpr auto topic = "afterSOMEIP";
  static constexpr auto qos = 10;

public:
    GnssTopicSubsriber() : Node(node_name)
    {
      subscription = this->create_subscription<GpsDataMsg>(topic, qos, std::bind(&GnssTopicSubsriber::topic_callback, this, std::placeholders::_1));
    }

private:

     void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
  {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
  }
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

    rclcpp::Subscription<GpsDataMsg>::SharedPtr subscription;
};

auto main(int argc, char * argv[]) -> int
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GnssTopicSubsriber>());
  rclcpp::shutdown();
  return 0;
}
