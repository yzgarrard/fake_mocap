 #include <rclcpp/rclcpp.hpp>
 #include <geometry_msgs/msg/pose_stamped.hpp>

 #include <math.h>
 #include <sys/time.h>
 #include <chrono>

using namespace std::chrono;

class FakeMocapData : public rclcpp::Node
{
public: 
	explicit FakeMocapData() : Node("px4_vrpn_to_fcu_pose") 
  {
    publisher_ = 
      this->create_publisher<geometry_msgs::msg::PoseStamped>("vrpn_client_node/RPM_Drone/pose", 10);

    auto timer_callback = [this]() -> void {
      t += 0.0084;
      float x = sin(t);
      float y = sin(t + 3.14159/4.0);
      float z = sin(t + 3.14159/2.0);
      auto msg = geometry_msgs::msg::PoseStamped();
      msg.header.stamp.sec = t;
      msg.pose.position.x = x;
      msg.pose.position.y = y;
      msg.pose.position.z = z;
      publisher_->publish(msg);
		};
		timer_ = this->create_wall_timer(8.4ms, timer_callback);
	}
private:
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    double t = 0;
};


int main(int argc, char *argv[])
{
	std::cout << "Starting fake_mocap_publisher node..." << std::endl;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<FakeMocapData>());

	rclcpp::shutdown();
	return 0;
}
