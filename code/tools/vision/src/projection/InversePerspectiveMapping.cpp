#include "projection/InversePerspectiveMapping.hpp"


InversePerspectiveMapping::InversePerspectiveMapping()
	: m_originalSize(),
	  m_outputSize(),
		m_regionPoints(),
		m_outputPoints(),
		m_H(),
		m_H_inv(),
		m_gridX(),
		m_gridY(),
		m_inverseGridX(),
		m_inverseGridY()
{
}
InversePerspectiveMapping::InversePerspectiveMapping(const cv::Size& a_originalSize,const cv::Size& a_outputSize,
		 const std::vector<cv::Point2f>& a_regionPoints, const std::vector<cv::Point2f>& a_outputPoints)
	: m_originalSize(a_originalSize),
	  m_outputSize(a_outputSize),
		m_regionPoints(a_regionPoints),
		m_outputPoints(a_outputPoints),
		m_H(),
		m_H_inv(),
		m_gridX(),
		m_gridY(),
		m_inverseGridX(),
		m_inverseGridY()
{

	m_H = cv::getPerspectiveTransform( m_regionPoints, m_outputPoints );
	m_H_inv = m_H.inv();
	
	generateRemappingFunction();
	
}

void InversePerspectiveMapping::initialize(const cv::Size& a_originalSize,const cv::Size& a_outputSize,
		 const std::vector<cv::Point2f>& a_regionPoints, const std::vector<cv::Point2f>& a_outputPoints){
	m_H = cv::getPerspectiveTransform( a_regionPoints, a_outputPoints );
	m_H_inv = m_H.inv();

	m_originalSize = a_originalSize;
	m_outputSize = a_outputSize;

	m_regionPoints = a_regionPoints;
	m_outputPoints = a_outputPoints;
	
	generateRemappingFunction();

}


void InversePerspectiveMapping::applyHomography(cv::Mat &a_inputImage,cv::Mat &a_outputImage, int a_borderCondition)
{
	cv::remap(a_inputImage,a_outputImage,m_gridX,m_gridY,cv::INTER_LINEAR,a_borderCondition);
	
}

void InversePerspectiveMapping::drawPoints( const std::vector<cv::Point2f>& _points, cv::Mat& _img )
{
	cv::line(_img, cv::Point(static_cast<int>(_points[0].x), static_cast<int>(_points[0].y)), cv::Point(static_cast<int>(_points[3].x), static_cast<int>(_points[3].y)), CV_RGB( 205,205,0), 2);
	cv::line(_img, cv::Point(static_cast<int>(_points[2].x), static_cast<int>(_points[2].y)), cv::Point(static_cast<int>(_points[3].x), static_cast<int>(_points[3].y)), CV_RGB( 205,205,0), 2);
	cv::line(_img, cv::Point(static_cast<int>(_points[0].x), static_cast<int>(_points[0].y)), cv::Point(static_cast<int>(_points[1].x), static_cast<int>(_points[1].y)), CV_RGB( 205,205,0), 2);
	cv::line(_img, cv::Point(static_cast<int>(_points[2].x), static_cast<int>(_points[2].y)), cv::Point(static_cast<int>(_points[1].x), static_cast<int>(_points[1].y)), CV_RGB( 205,205,0), 2);
	
	for(size_t i=0; i<_points.size(); i++)
	{
		cv::circle(_img, cv::Point(static_cast<int>(_points[i].x), static_cast<int>(_points[i].y)), 2, CV_RGB(238,238,0), -1);
		cv::circle(_img, cv::Point(static_cast<int>(_points[i].x), static_cast<int>(_points[i].y)), 5, CV_RGB(255,255,255), 2);
	}
}



cv::Point2d InversePerspectiveMapping::applyHomography(const cv::Point2d& inputPoint, const cv::Mat& a_H)
{
	cv::Point2d returnPoint = cv::Point2d( -1, -1 );
	
	const double u = a_H.at<double>(0,0) * inputPoint.x + a_H.at<double>(0,1) * inputPoint.y + a_H.at<double>(0,2);
	const double v = a_H.at<double>(1,0) * inputPoint.x + a_H.at<double>(1,1) * inputPoint.y + a_H.at<double>(1,2);
	const double s = a_H.at<double>(2,0) * inputPoint.x + a_H.at<double>(2,1) * inputPoint.y + a_H.at<double>(2,2);
	if ( fabs(s)>0.0000001 )
	{
		returnPoint.x = ( u / s );
		returnPoint.y = ( v / s );
	}
	return returnPoint;	
}



void InversePerspectiveMapping::generateRemappingFunction()
{
	// Create remap images
	m_gridX.create(m_outputSize, CV_32F);
	m_gridY.create(m_outputSize, CV_32F);
	for( int j = 0; j < m_outputSize.height; ++j )
	{
		float* rowPointX = m_gridX.ptr<float>(j);
		float* rowPointY = m_gridY.ptr<float>(j);
		for( int i = 0; i < m_outputSize.width; ++i )
		{
			cv::Point2f pt = applyHomography( cv::Point2f( static_cast<float>(i), static_cast<float>(j) ), m_H_inv );
			rowPointX[i] = pt.x;
			rowPointY[i] = pt.y;
		}
	}

	m_inverseGridX.create(m_originalSize, CV_32F);
	m_inverseGridY.create(m_originalSize, CV_32F);

	for( int j = 0; j < m_originalSize.height; ++j )
	{
		float* rowPointX = m_inverseGridX.ptr<float>(j);
		float* rowPointY = m_inverseGridY.ptr<float>(j);
		for( int i = 0; i < m_originalSize.width; ++i )
		{
			cv::Point2f pt = applyHomography( cv::Point2f( static_cast<float>(i), static_cast<float>(j) ), m_H );
			rowPointX[i] = pt.x;
			rowPointY[i] = pt.y;			
		}
	}	
}