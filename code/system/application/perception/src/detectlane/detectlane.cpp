/**
 * Copyright (C) 2015 Chalmers REVERE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "detectlane/Drawing.h"
#include "detectlane/DecisionMaking.h"
#include "detectlane/ExtractingRegions.h"
#include "detectlane/ProcessImage.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"


#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "detectlane/detectlane.hpp"

namespace opendlv {
namespace perception {
namespace detectlane {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DetectLane::DetectLane(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-detectlane"),
    m_setup(false),
    m_width(640),
    m_height(480),
    m_outputWidth(),
    m_outputHeight(),
    m_maxRow(450),
    m_minRow(200),
    m_midRegion(),
    m_threshold(180),
    m_standardLaneWidth(3.75),
    m_initialized(false),
    m_regions(),
    m_leftCameraRegions(),
    m_rightCameraRegions(),
    m_lines(),
    m_leftLines(),
    m_rightLines(),
    m_nPoints(),
    m_nRegions(),
    m_recoveredPoints(),
    m_k(),
    m_m(),
    m_K(),
    m_M(),
    m_pointsPerRegion(),
    m_regionIndex(),
    m_laneLocation2(),
    m_leftIpm(),
    m_rightIpm(),
    m_transformationMatrix(),
    m_leftTransformationMatrix(),
    m_rightTransformationMatrix(),
    m_scale()

{
}

DetectLane::~DetectLane()
{
}

/**
 * Receives .
 * Sends .
 */
void DetectLane::nextContainer(odcore::data::Container &c)
{
	// Code to read data
	// TODO: Implement a way to see if shared image is from left or right camera.
	// Algorithm is needed to set the search regions depending on the 
	if (c.getDataType() != odcore::data::image::SharedImage::ID()) {
    return;
  }
//  std::cout << "Received container of type " << c.getDataType() << 
//      " sent at " <<   c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      std::endl;
  
  if(m_setup == true)
  {
    odcore::data::image::SharedImage mySharedImg = 
        c.getData<odcore::data::image::SharedImage>();

    std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
          odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
              mySharedImg.getName()));
    
    const uint32_t nrChannels = 3;//mySharedImg.getBytesPerPixel();;
    int imgWidth = mySharedImg.getWidth();
    int imgHeight = mySharedImg.getHeight();
    cv::Mat image;
   
    IplImage* myIplImage;
    myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight),
        IPL_DEPTH_8U, nrChannels);
    cv::Mat src(myIplImage);
    cv::Mat test;

    if (!sharedMem->isValid()) {
      return;
    }
    
    sharedMem->lock();
    {
      memcpy(src.data, sharedMem->getSharedMemory(),
          imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
        
    // From this point implement lane detection algorithm
    //-----------------------------
    // Definitions for the video choice
    //-----------------------------
    m_initialized = false;
    cv::resize(src,src,cv::Size(m_outputWidth,m_outputHeight));
    if(mySharedImg.getName() == "front-left"){
      /* OLD CODE 
      m_leftIpm->ApplyHomography(src,src);

      m_regions = m_leftCameraRegions;
      m_lines = m_leftLines;
      m_transformationMatrix = m_leftTransformationMatrix;
      m_initialized = true;
      */

      // New code
      m_regions = m_leftCameraRegions;
      m_lines = m_leftLines;
      m_transformationMatrix = m_leftTransformationMatrix;
      m_initialized = true;
    }
    if(mySharedImg.getName() == "front-right"){
      /* OLD CODE
      m_rightIpm->ApplyHomography(src,src);

      m_regions = m_rightCameraRegions;
      m_lines = m_rightLines;
      m_transformationMatrix = m_rightTransformationMatrix;
      m_initialized = true;
      */

      // New code
      m_regions = m_rightCameraRegions;
      m_lines = m_rightLines;
      m_transformationMatrix = m_rightTransformationMatrix;
      m_initialized = true;
    }
    
    
    cvReleaseImage(&myIplImage);


    if(!m_initialized){
      return;
    }
    
    // Holds the k and m values for the region lines
    m_k = m_lines.col(0);
    m_m = m_lines.col(1);
    
    //-----------------------------
    // Choose processing type
    //-----------------------------
    cv::inRange(src, cv::Scalar(m_threshold, m_threshold, m_threshold),
        cv::Scalar(255, 255, 255), image);
    medianBlur(image, image, 3);

     
    //-----------------------------
    // Local line search
    //-----------------------------

    ScanImage(&image, &m_lines, &m_recoveredPoints, m_nPoints, m_minRow, 
        m_maxRow);


    
    //-----------------------------
    // Extract lines from points
    //-----------------------------
    ExtractLines(&m_recoveredPoints, &m_K, &m_M, m_nRegions, m_nPoints,
        &m_pointsPerRegion);

    m_laneLocation2 = Eigen::VectorXd::Zero(m_nRegions, 1);
    
    //-----------------------------
    // Make decision based on lines
    //-----------------------------
    SelectLanesV2(m_pointsPerRegion, m_laneLocation2);
    
    if (fabs(m_pointsPerRegion.sum()) > 0.0001){
      
      SelectLaneOrientation(m_regionIndex,m_laneLocation2,
          (int)m_recoveredPoints.cols());
      int p1 = m_regionIndex(0,0);
      int p2 = m_regionIndex(1,0);
      
      //-----------------------------
      // Draw boarders/lines - VISUALIZATION
      //-----------------------------
      DrawBorders(&src,m_minRow, m_maxRow, m_K(p1,0), m_K(p2,0), m_M(p1,0),
          m_M(p2,0));
      //DrawTracks(&src, &K, &M,MINROW,MAXROW,Scalar(0,0,255));
      DrawTracks(&src, &m_k, &m_m, m_minRow, Scalar(255,255,255));
      
      //-----------------------------
      // Calculate lane offset
      //-----------------------------

      double laneOffset = GetLateralPosition(m_K(p1,0), m_M(p1,0),
          m_K(p2,0), m_M(p2,0),
          m_lines.col(0)(m_midRegion),
          m_lines.col(1)(m_midRegion),
          (m_maxRow));


      //-----------------------------
      // Approximate heading angle
      //-----------------------------
      double d1 = GetLateralPosition(m_K(p1,0) ,m_M(p1,0),
          m_K(p2,0), m_M(p2,0),
          m_lines.col(0)(m_midRegion),
          m_lines.col(1)(m_midRegion),
          (m_maxRow));
      double d2 = GetLateralPosition(m_K(p1,0) ,m_M(p1,0),
          m_K(p2,0),m_M(p2,0),
          m_lines.col(0)(m_midRegion),m_lines.col(1)(m_midRegion),
          (m_minRow));
      // Factor 4 is assumed length between minRow and Maxrow in real life
      double theta = atan((d2-d1) / (double)4);


      double newLaneOffset = GetLaneOffset(m_K(p1,0),
          m_M(p1,0),m_K(p2,0), m_M(p2,0),m_maxRow,p1,p2);
      double newHeadingAngle = GetHeadingAngle(m_K(p1,0),
          m_M(p1,0),m_K(p2,0), m_M(p2,0),m_minRow,m_maxRow, p1,p2);

      std::cout << "Offset: " << laneOffset << " Heading: " << theta 
          << std::endl;

      std::cout << "New offset: " << newLaneOffset << " New heading: "
          << newHeadingAngle << std::endl << std::endl;

      
      if(std::isfinite(theta) && std::isfinite(laneOffset)){
        // Send the message

      opendlv::perception::LanePosition lanePosition(laneOffset,theta);
      odcore::data::Container msg(lanePosition);  
      getConference().send(msg);



  		}
    }
    char key = waitKey(1);
    if(key == 'w')
      m_threshold +=10;
    else if(key == 's')
      m_threshold -=10;
    
    //-----------------------------
    // Show image
    //-----------------------------
    imshow("1", src);
    imshow("2", image);
    waitKey(1);
  }
}

void DetectLane::ReadWarpPointsMatrix(std::vector<cv::Point2f> & a_points,std::string a_fileName)
{
  std::ifstream file(a_fileName);

  if(file.is_open()){

    for(uint i = 0; i < a_points.size(); i++){
      file >> a_points.at(i).x;
      file >> a_points.at(i).y;
    }

  }

  file.close();

}

void DetectLane::ReadInputWindowSize(std::string a_inputSizeString)
{
  std::stringstream stringStream(a_inputSizeString);
  std::vector<std::string> result;

  while( stringStream.good() )
  {
    std::string subString;
    getline(stringStream, subString, ',' );
    result.push_back( subString );
  }
  m_width = atoi(result.at(0).c_str());
  m_height = atoi(result.at(1).c_str());

}

void DetectLane::ReadOutputWindowSize(std::string a_outputSizeString)
{
  std::stringstream stringStream(a_outputSizeString);
  std::vector<std::string> result;

  while( stringStream.good() )
  {
    std::string subString;
    getline(stringStream, subString, ',' );
    result.push_back( subString );
  }
  m_outputWidth = atoi(result.at(0).c_str());
  m_outputHeight = atoi(result.at(1).c_str());

}


void DetectLane::setUp()
{

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string inputSizeString = kv.getValue<std::string>("perception-detectlane.inputSize");
  ReadInputWindowSize(inputSizeString);

  m_width = 1280;
  m_height = 720;
  std::string outputSizeString = kv.getValue<std::string>("perception-detectlane.outputSize");
  ReadOutputWindowSize(outputSizeString);

  std::vector<cv::Point2f> leftRegionPoints(4);
  ReadWarpPointsMatrix(leftRegionPoints,"./share/opendlv/tools/vision/projection/leftCameraWarpPoints.csv");

  std::vector<cv::Point2f> rightRegionPoints(4);
  ReadWarpPointsMatrix(rightRegionPoints, "./share/opendlv/tools/vision/projection/rightCameraWarpPoints.csv");

  std::vector<cv::Point2f> outputPoints;
  cv::Size inputSize(m_width,m_height);
  cv::Size outputSize(m_outputWidth, m_outputHeight);
  outputPoints.push_back(cv::Point2f(0, m_outputHeight) );
  outputPoints.push_back(cv::Point2f(m_outputWidth, m_outputHeight) );
  outputPoints.push_back(cv::Point2f(m_outputWidth, 0) );
  outputPoints.push_back(cv::Point2f(0, 0) ); 


  m_leftIpm.reset(new InversePerspectiveMapping(
          inputSize,outputSize,
          leftRegionPoints,outputPoints));
  m_rightIpm.reset(new InversePerspectiveMapping(
          inputSize, outputSize,
          rightRegionPoints, outputPoints));
    
  m_regions = Eigen::MatrixXd(7,4);

  //-----------------------------
  // Scaling: Calibrations were made for 640x480 resolution
  //-----------------------------
  m_minRow = 150;//m_minRow * (m_height / 480.0);
  m_maxRow = 450;//m_maxRow * (m_height / 480.0);

  /* OLD CODE 
  std::string leftCameraRegionsFile = 
    "./share/opendlv/system/application/perception/detectlane/leftCameraRegions.csv";
  m_leftCameraRegions = ReadMatrix(leftCameraRegionsFile,
      m_regions.rows(),m_regions.cols());

  std::string rightCameraRegionsFile = 
    "./share/opendlv/system/application/perception/detectlane/rightCameraRegions.csv";
  m_rightCameraRegions = ReadMatrix(rightCameraRegionsFile,
      m_regions.rows(),m_regions.cols());
  */

  m_leftCameraRegions = Eigen::MatrixXd(7,4);
  m_leftCameraRegions <<
    221, 122, 13, 233,
    267, 114, 33, 461,
    293, 112, 327, 462,
    299, 101, 444, 474,
    303, 100, 528, 469,
    319, 105, 609, 332,
    336, 87, 630, 201;



  m_rightCameraRegions = Eigen::MatrixXd(7,4);
  m_rightCameraRegions <<
    0 , 40 , 0 ,40 ,
    0 , 30 , 0, 10,
    0 , 30 , 0 ,10,
    0 , 30 , 0 ,10,
    0, 30 , 0 , 10,
    0 , 0 ,0 ,0,
    0 ,0 ,0 ,0;

  m_leftCameraRegions.col(0) *= (m_outputWidth / 640.0);
  m_leftCameraRegions.col(2) *= (m_outputWidth / 640.0);
  m_leftCameraRegions.col(1) *= (m_outputHeight / 480.0);
  m_leftCameraRegions.col(3) *= (m_outputHeight / 480.0);

  m_rightCameraRegions.col(0) *= (m_outputWidth / 640.0);
  m_rightCameraRegions.col(2) *= (m_outputWidth / 640.0);
  m_rightCameraRegions.col(1) *= (m_outputHeight / 480.0);
  m_rightCameraRegions.col(3) *= (m_outputHeight / 480.0);

  m_leftLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_leftCameraRegions,m_leftLines);

  m_rightLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_rightCameraRegions, m_rightLines);

  //-----------------------------
  // Image processing parameters
  //-----------------------------
  m_nPoints = m_maxRow - m_minRow;
  
  //-----------------------------
  // Initializations
  //-----------------------------
  m_lines = Eigen::MatrixXd(m_regions.rows(), 2);

  // Middle region line index
  m_midRegion = (int)(m_lines.rows()-1)/2;

  m_recoveredPoints = Eigen::MatrixXd(m_nPoints, m_lines.rows()+2);
  
  m_nRegions = m_recoveredPoints.cols()-1;
   
  // Counts the number of points per each region
  m_pointsPerRegion = Eigen::VectorXd(m_nRegions,1);
  
  // Holds the index that decides the left and right road track
  m_regionIndex = Eigen::MatrixXd(2,1);

  m_k = Eigen::MatrixXd(m_regions.rows(),1);
  m_m = Eigen::MatrixXd(m_regions.rows(),1);

  m_K = Eigen::MatrixXd(m_nRegions,1);
  m_M = Eigen::MatrixXd(m_nRegions,1);

  /* OLD CODE 
  m_leftTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/leftCameraTransformationMatrixWarped.csv",3,3);
  std::cout<<m_leftTransformationMatrix;
  std::cout<<"\n------\n----\n---\n";
  m_rightTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/rightCameraTransformationMatrixWarped.csv",3,3);
  */

  // NEW CODE
  m_leftTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/leftCameraTransformationMatrix.csv",3,3);
  std::cout<<m_leftTransformationMatrix;
  std::cout<<"\n------\n----\n---\n";
  m_rightTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/rightCameraTransformationMatrix.csv",3,3);
  // --------------------------------

  m_scale << m_width / (double)m_outputWidth, m_height / (double)m_outputHeight, 1;
 
  if(false)
  {
    std::cout<<"Input size\n";
    std::cout<<m_width<< " " << m_height << std::endl;
    std::cout<<"Output size\n",
    std::cout << m_outputWidth << " " << m_outputHeight<<std::endl;
    std::cout<<"Left camera regions:\n";
    std::cout<<m_leftCameraRegions;
    std::cout<<std::endl;
    std::cout<<"Right camear regions:\n";
    std::cout<<m_rightCameraRegions<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Left camera transformation matrix\n";
    std::cout<<m_leftTransformationMatrix<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Right camera transformation matrix\n";
    std::cout<<m_rightTransformationMatrix<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Left camera warp points :\n";
    for(int i = 0; i < 4; i++)
      std::cout<<leftRegionPoints.at(i).x << " " << leftRegionPoints.at(i).y<<std::endl;
    std::cout<<"Right camera warp points :\n";
    for(int i = 0; i < 4; i++)
      std::cout<<rightRegionPoints.at(i).x << " " << rightRegionPoints.at(i).y<<std::endl;


  }
  m_setup = true;
 }

void DetectLane::tearDown()
{
}

Eigen::MatrixXd DetectLane::ReadMatrix(std::string fileName, int nRows,
    int nCols)
{
  std::ifstream file(fileName);

  Eigen::MatrixXd matrix(nRows,nCols);


  if (file.is_open()) {
    for(int i = 0; i < nRows; i++){
      for(int j = 0; j < nCols; j++){
        double item = 0.0;
        file >> item;

   
        matrix(i,j) = item;

      }
    }
  }
  
  file.close();

  return matrix;

}

void DetectLane::TransformPointToGlobalFrame(Eigen::Vector3d &point)
{
  // std::cout<<"point before \n";
  // std::cout<<point<<std::endl;
  point = point.cwiseProduct(m_scale);
  // std::cout<<"point after 1\n";
  // std::cout<<point<<std::endl;
  point = m_transformationMatrix * point;

  // std::cout<<"point final before \n";
  // std::cout<<point<<std::endl;

  point = point / point(2);
  // std::cout<<"point final \n";
  // std::cout<<point<<std::endl;

}

double DetectLane::GetHeadingAngle(double kLeft,double mLeft, double kRight,
    double mRight, double row1, double row2, int leftPointIndex, 
    int rightPointIndex )

{
  if( leftPointIndex == rightPointIndex )
  {
    double col1 = kLeft * row1 + mLeft;
    Eigen::Vector3d point1(col1, row1, 1);
    TransformPointToGlobalFrame(point1);

    double dxPoint1 = point1(0);
    double dyPoint1 = point1(1);

    double col2 = kLeft * row2 + mLeft;
    Eigen::Vector3d point2(col2,row2,1);
    TransformPointToGlobalFrame(point2);

    double dxPoint2 = point2(0);
    double dyPoint2 = point2(1);

    return std::atan2(dyPoint1-dyPoint2,dxPoint1-dxPoint2);

  }
  else 
  {
    //Find x,y point of middle of lane for row1
    double colLeft1 = kLeft * row1 + mLeft;
    double colRight1 = kRight * row1 + mRight;

    Eigen::Vector3d leftPoint1(colLeft1, row1, 1);
    TransformPointToGlobalFrame(leftPoint1);
    
    Eigen::Vector3d rightPoint1(colRight1, row1, 1);  
    TransformPointToGlobalFrame(rightPoint1);

    double dxPoint1 = (leftPoint1(0) + rightPoint1(0) ) / 2.0;
    double dyPoint1 = (leftPoint1(1) + rightPoint1(1) ) / 2.0;

    //Find x,y point of middle of lane for row2
    double colLeft2 = kLeft * row2 + mLeft;
    double colRight2 = kRight * row2 + mRight;

    Eigen::Vector3d leftPoint2(colLeft2, row2, 1);
    TransformPointToGlobalFrame(leftPoint2);


    Eigen::Vector3d rightPoint2(colRight2, row2, 1);  
    TransformPointToGlobalFrame(rightPoint2);

    double dxPoint2 = (leftPoint2(0) + rightPoint2(0) ) / 2.0;
    double dyPoint2 = (leftPoint2(1) + rightPoint2(1) ) / 2.0;

    return std::atan2(dyPoint1-dyPoint2,dxPoint1-dxPoint2);
  }
}



double DetectLane::GetLaneOffset(double kLeft,double mLeft, double kRight,
    double mRight,double row, int leftPointIndex,int rightPointIndex)
{
  if(leftPointIndex == rightPointIndex)
  {
    std::cout<<"Using only one lane marking\n";
    double col = kLeft * row + mLeft;
    Eigen::Vector3d point(col, row, 1);
    TransformPointToGlobalFrame(point);

    std::cout<<"Lane is at position : " << point(1) <<std::endl;
    if(leftPointIndex <= m_nRegions / 2 - 1)
      return ( point(1) - m_standardLaneWidth / 2 );
    else
      return ( point(1) + m_standardLaneWidth / 2 ); 
  
  }
  else 
  {
    double colLeft = kLeft * row + mLeft;
    double colRight = kRight * row + mRight;

    Eigen::Vector3d leftPoint(colLeft, row, 1);
    TransformPointToGlobalFrame(leftPoint);
   
    Eigen::Vector3d rightPoint(colRight, row, 1);  
    TransformPointToGlobalFrame(rightPoint);
    return (leftPoint(1) + rightPoint(1)) / 2.0;
  }
}



} // detectlane
} // perception
} // opendlv
