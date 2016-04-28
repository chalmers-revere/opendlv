#ifndef __INVERSEPERSPECTIVEMAPPING_HPP_
#define __INVERSEPERSPECTIVEMAPPING_HPP_

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

	InversePerspectiveMapping(const cv::Size &a_originalSize,const cv::Size &a_outputSize,
			const std::vector<cv::Point2f>& a_regionPoints, const std::vector<cv::Point2f>& a_outputPoints);
	void applyHomography(cv::Mat &a_inputImage,cv::Mat &a_outputImage, int borderMode = cv::BORDER_CONSTANT);
	cv::Point2d applyHomography(const cv::Point2d& inputPoint, const cv::Mat& a_H);
	void drawPoints( const std::vector<cv::Point2f>& _points, cv::Mat& _img );

private:
	void generateRemappingFunction();
	
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
