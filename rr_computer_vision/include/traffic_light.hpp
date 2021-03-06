/**
 * @file rr_traffic_light.hpp
 * @brief Traffic Light Header File
 * @author Yuchi(Allan) Zhao
 * @author Waleed Ahmed (w29ahmed)
 * @competition IARRC 2019
 */

#ifndef TRAFFIC_LIGHT
#define TRAFFIC_LIGHT

// ROS includes
#include <ros/ros.h>
#include <image_transport/image_transport.h>

// OpenCV includes
#include <opencv2/opencv.hpp>

class TrafficLightDetection
{
  public:
    TrafficLightDetection(ros::NodeHandle nh);
    ~TrafficLightDetection();
    bool race_started;

  private:
    ros::NodeHandle nh_;
    ros::ServiceClient client_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber img_subscriber_;
    // image_transport::Publisher test_publisher_;
    // image_transport::Publisher test_blob_publisher_;

    void SetBlobDetectorParams();
    void ImgCallback(const sensor_msgs::ImageConstPtr& msg);
    void CropImage(const cv::Mat& input_img, cv::Mat& threshold_img);
    void RedColorThreshold(const cv::Mat& input_img, cv::Mat& threshold_img);
    void RedLightDetection(const cv::Mat& threshold_img);
    void FindBoundRect(const cv::Mat& threshold_img, cv::Mat& crop_img, std::vector<cv::KeyPoint>& keypoints, const int max_area_index);

    // Blob detector
    cv::Rect boundRect_;
    cv::SimpleBlobDetector::Params params_;
    cv::Ptr<cv::SimpleBlobDetector> detector_;

    // Internal states/counters
    bool red_light_detected_;
    double default_pixel_ratio_;
    int red_light_counter_;
    int green_light_counter_;

    cv::Scalar hsv_lower_bounds_;
    cv::Scalar hsv_upper_bounds_;

    // Constants
    const double pixel_ratio_range_ = 0.40;
    const int red_frame_counter_max_ = 10;
    const int green_frame_counter_max_ = 5;
};

#endif
