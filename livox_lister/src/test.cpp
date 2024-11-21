#include <rclcpp/rclcpp.hpp>
#include <livox_ros_driver2/msg/custom_msg.hpp>

class LivoxListener : public rclcpp::Node
{
public:
    LivoxListener() : Node("livox_listener")
    {
        // 订阅Livox点云话题
        subscription_ = this->create_subscription<livox_ros_driver2::msg::CustomMsg>(
            "/livox/lidar", 10, std::bind(&LivoxListener::pointCloudCallback, this, std::placeholders::_1));
    }

private:
    void pointCloudCallback(const livox_ros_driver2::msg::CustomMsg::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received point cloud with %d points", msg->point_num);
        RCLCPP_INFO(this->get_logger(), "The framer is %s", msg->header.frame_id.c_str());
        RCLCPP_INFO(this->get_logger(), "time: %d", msg->header.stamp.sec);
        
        // 打印前5个点的信息
        for (int i = 0; i < 5 && i < msg->point_num; ++i)
        {
            const auto& point = msg->points[i];
            RCLCPP_INFO(this->get_logger(), "Point %d: x = %f, y = %f, z = %f, reflectivity = %d, line= %f, offset_time= %f", 
                        i, point.x, point.y, point.z, point.reflectivity, point.line, point.offset_time);
        }
    }

    rclcpp::Subscription<livox_ros_driver2::msg::CustomMsg>::SharedPtr subscription_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);  // 初始化 ROS 2 系统

    // 创建节点并运行
    rclcpp::spin(std::make_shared<LivoxListener>());

    rclcpp::shutdown();  // 关闭 ROS 2 系统
    return 0;
}
