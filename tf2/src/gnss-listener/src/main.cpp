#include "rclcpp/rclcpp.hpp"

// #include <gnss_someip_lib/msg/gnss_data.hpp>

#include "std_msgs/msg/string.hpp"


#include <geometry_msgs/msg/transform_stamped.hpp>



using  GpsDataMsg = geometry_msgs::msg::TransformStamped;
class GnssTopicSubsriber : public rclcpp::Node
{
  static constexpr auto node_name = "TF2_Topic_Subscriber";

  static constexpr auto topic = "afterSOMEIP";
  static constexpr auto qos = 10;

public:
    GnssTopicSubsriber() : Node(node_name)
    {
      subscription = this->create_subscription<GpsDataMsg>(topic, qos, std::bind(&GnssTopicSubsriber::topic_callback, this, std::placeholders::_1));
    }

private:
    // void gnss_topic_callback(const GpsDataMsg & msg) const
    // {
    //   RCLCPP_INFO(this->get_logger(), "GNSS position latitude %f, longitude %f", 
    //       msg.position.fix.latitude,
    //       msg.position.fix.longitude
    //     );
    // }

     void topic_callback(const geometry_msgs::msg::TransformStamped::SharedPtr msg) const
  {
    RCLCPP_INFO(this->get_logger(), "Listening to topic %s\n"
    "transforms:\n"
    "- header:\n"
    "    stamp:\n"
    "      sec: %d\n"
    "      nanosec: %u\n"
    "    frame_id: %s\n"
    "  child_frame_id: %s\n"
    "  transform:\n"
    "    translation:\n"
    "      x: %f\n"
    "      y: %f\n"
    "      z: %f\n"
    "    rotation:\n"
    "      x: %f\n"
    "      y: %f\n"
    "      z: %f\n"
    "      w: %f",
    topic,
    msg->header.stamp.sec, 
    msg->header.stamp.nanosec, 
    msg->header.frame_id.c_str(), 
    msg->child_frame_id.c_str(), 
    msg->transform.translation.x, 
    msg->transform.translation.y, 
    msg->transform.translation.z, 
    msg->transform.rotation.x, 
    msg->transform.rotation.y, 
    msg->transform.rotation.z, 
    msg->transform.rotation.w);

  }
  rclcpp::Subscription<geometry_msgs::msg::TransformStamped>::SharedPtr subscription_;

    rclcpp::Subscription<GpsDataMsg>::SharedPtr subscription;
};

auto main(int argc, char * argv[]) -> int
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GnssTopicSubsriber>());
  rclcpp::shutdown();
  return 0;
}
