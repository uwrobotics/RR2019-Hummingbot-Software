/** @file lane_detection_processor.hpp
 *  @author Waleed Ahmed (w29ahmed)
 *  @author Carson Chen
 *  @author Martin Ethier
 *  @author Matthew Post
 *  @author Toni Ogunmade(oluwatoni)
 *  @competition IARRC 2019
 */
#ifndef __LANE_DETECTION_HPP
#define __LANE_DETECTION_HPP

// Standard includes
#include <string>

// ROS includes
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <nav_msgs/OccupancyGrid.h>
#include <std_msgs/Bool.h>

// OpenCV includes
#include <opencv2/opencv.hpp>

class LaneDetection
{
  public:
    LaneDetection(ros::NodeHandle nh);
    ~LaneDetection();
    cv::Mat GetContours(const cv::Mat &image);
    
  private:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber img_subscriber_;
    ros::Publisher grid_pub_;
    ros::Publisher horiz_lane_monitor_pub_;

    // Publishers for debugging
    // ros::Publisher test_thres_img_pub_;
    // ros::Publisher test_warp_img_pub_;
    // ros::Publisher test_contour_filter_img_pub_;
    // ros::Publisher test_horiz_lane_removal_img_pub_;

    // Parameters
    cv::Mat warp_src_coords_;
    int min_contour_size_;
    double resolution_;
    double camera_width_offset_;
    double camera_height_offset_;
    std::string camera_feed_;
    bool monitor_horizontal_lanes_;
    nav_msgs::MapMetaData meta_data_;

    void ImgCallback(const sensor_msgs::ImageConstPtr& msg);
    void InitializeSubscribers();
    void InitializePublishers();

    void Threshold(const cv::Mat &input_img, cv::Mat &output_img);
    void Warp(const cv::Mat &input_img, cv::Mat &output_img, const cv::Mat src);
    cv::Mat ContourFilter(const cv::Mat &img, const int min_contour_size);
    bool RemoveHorizontalLanes(const cv::Mat &input_img, cv::Mat &output_img);
    void ConvertToOccupancyGrid(const cv::Mat &img, nav_msgs::OccupancyGrid &grid_msg);
};

#endif //__LANE_DETECTION_HPP