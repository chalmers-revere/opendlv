#ifndef PROJECTION_INVERSEPERSPECTIVEMAPPING_HPP_
#define PROJECTION_INVERSEPERSPECTIVEMAPPING_HPP_

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <Eigen/Dense>
#include <iostream>

class InversePerspectiveMapping
{

public:
  InversePerspectiveMapping();
  InversePerspectiveMapping(cv::Size const &a_originalSize,
      cv::Size const &a_outputSize,
      std::vector<cv::Point2f> const &a_regionPoints,
      std::vector<cv::Point2f> const &a_outputPoints);
  void ApplyHomography(cv::Mat &a_inputImage,cv::Mat &a_outputImage,
      int borderMode = cv::BORDER_CONSTANT);
  cv::Point2d ApplyHomography(cv::Point2d const & inputPoint,
      cv::Mat const &a_H);
  void DrawPoints(std::vector<cv::Point2f> const & _points, cv::Mat& _img );
  void Initialize(cv::Size const & a_originalSize,cv::Size const & a_outputSize,
      std::vector<cv::Point2f> const & a_regionPoints,
      std::vector<cv::Point2f> const & a_outputPoints);

private:
  void GenerateRemappingFunction();
  
  cv::Size m_originalSize;
  cv::Size m_outputSize;
  
  std::vector<cv::Point2f> m_regionPoints;
  std::vector<cv::Point2f> m_outputPoints;
  
  cv::Mat m_H;
  cv::Mat m_H_inv;
  
  cv::Mat m_gridX;
  cv::Mat m_gridY;
  
  cv::Mat m_inverseGridX;
  cv::Mat m_inverseGridY;
  

};





#endif
