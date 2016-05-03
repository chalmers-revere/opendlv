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
    m_maxRow(450),
    m_minRow(200),
    m_midRegion(),
    m_threshold(180),
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
    m_rightIpm()

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

    std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(mySharedImg.getName()));
    
    const uint32_t nrChannels = 3;//mySharedImg.getBytesPerPixel();;
    int imgWidth = mySharedImg.getWidth();
    int imgHeight = mySharedImg.getHeight();
    cv::Mat image;
   
    IplImage* myIplImage;
    myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels);
    cv::Mat src(myIplImage);
    cv::Mat test;

    if (!sharedMem->isValid()) {
      return;
    }
    
    sharedMem->lock();
    {
      memcpy(src.data, sharedMem->getSharedMemory(), imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
    
    //-----------------------------
    // Re-size the source image
    //-----------------------------
    resize(src, src, Size(m_width,m_height),0,0,INTER_CUBIC);
    

    
    // From this point implement lane detection algorithm
    //-----------------------------
    // Definitions for the video choice
    //-----------------------------
    m_initialized = false;
    if(mySharedImg.getName() == "front-left"){
      m_regions = m_leftCameraRegions;
      m_leftIpm->applyHomography(src,src);
      m_lines = m_leftLines;
      m_initialized = true;
    }
    if(mySharedImg.getName() == "front-right"){
      m_regions = m_rightCameraRegions;
      m_rightIpm->applyHomography(src,src);
      m_lines = m_rightLines;
      m_initialized = true;
    }
    if(!m_initialized){
      return;
    }
    
    // Holds the k and m values for the region lines
    m_k = m_lines.col(0);
    m_m = m_lines.col(1);
    
    //-----------------------------
    // Choose processing type
    //-----------------------------
    cv::inRange(src, cv::Scalar(m_threshold, m_threshold, m_threshold), cv::Scalar(255, 255, 255), image);
    medianBlur(image, image, 3);

     
    //-----------------------------
    // Local line search
    //-----------------------------
    ScanImage(&image,&m_lines,&m_recoveredPoints,m_nPoints,m_minRow,m_maxRow);
    
    
    //-----------------------------
    // Extract lines from points
    //-----------------------------
    ExtractLines(&m_recoveredPoints,&m_K,&m_M,m_nRegions,m_nPoints,&m_pointsPerRegion);

    m_laneLocation2 = Eigen::VectorXd::Zero(m_nRegions, 1);
    
    //-----------------------------
    // Make decision based on lines
    //-----------------------------
    SelectLanesV2(m_pointsPerRegion, m_laneLocation2);
    
    if (fabs(m_pointsPerRegion.sum()) > 0.0001){
      
      SelectLaneOrientation(m_regionIndex,m_laneLocation2,(int)m_recoveredPoints.cols());
      int p1 = m_regionIndex(0,0);
      int p2 = m_regionIndex(1,0);
      
      //-----------------------------
      // Draw boarders/lines - VISUALIZATION
      //-----------------------------
      DrawBorders(&src,m_minRow,m_maxRow,m_K(p1,0),m_K(p2,0),m_M(p1,0),m_M(p2,0));
      //DrawTracks(&src, &K, &M,MINROW,MAXROW,Scalar(0,0,255));
      DrawTracks(&src, &m_k,&m_m,m_minRow,Scalar(255,255,255));
      
      //-----------------------------
      // Calculate lane offset
      //-----------------------------
      double laneOffset = GetLateralPosition(m_K(p1,0),m_M(p1,0),
                                      m_K(p2,0),m_M(p2,0),
                                      m_lines.col(0)(m_midRegion),m_lines.col(1)(m_midRegion),
                                      (m_maxRow));

      //-----------------------------
      // Approximate heading angle
      //-----------------------------
      double d1 = GetLateralPosition(m_K(p1,0),m_M(p1,0),
                                  m_K(p2,0),m_M(p2,0),
                                m_lines.col(0)(m_midRegion),m_lines.col(1)(m_midRegion),
                                  (m_maxRow));
      double d2 = GetLateralPosition(m_K(p1,0),m_M(p1,0),
                                  m_K(p2,0),m_M(p2,0),
                                  m_lines.col(0)(m_midRegion),m_lines.col(1)(m_midRegion),
                                  (m_minRow));
      // Factor 15 is assumed length between minRow and Maxrow in real life
      double theta = atan((d2-d1) / (double)4);
      

      std::cout<<"Offset: "<<laneOffset<<" Heading: "<<theta<<std::endl;
      // Send the message
      opendlv::perception::LanePosition lanePosition(laneOffset,theta);
      odcore::data::Container msg(lanePosition);  
      getConference().send(msg);
  		
    }
    
    //-----------------------------
    // Show image
    //-----------------------------
    imshow("1", src);
    imshow("2", image);
    waitKey(1);
  	
    cvReleaseImage(&myIplImage);
  }
}

void DetectLane::setUp()
{
  m_width = 640;
  m_height = 480;

  // regionPoints.push_back(cv::Point2f(-260, 417) );
  // regionPoints.push_back(cv::Point2f(1039, 335) );
  // regionPoints.push_back(cv::Point2f( 440, 142) );
  // regionPoints.push_back(cv::Point2f(170, 142) );


  std::vector<cv::Point2f> leftRegionPoints;
 leftRegionPoints.push_back(cv::Point2f(-170,480));
leftRegionPoints.push_back(cv::Point2f(900,480));
leftRegionPoints.push_back(cv::Point2f(610,270));
leftRegionPoints.push_back(cv::Point2f(0,270));

  std::vector<cv::Point2f> rightRegionPoints;
  rightRegionPoints.push_back(cv::Point2f(-310,383));
  rightRegionPoints.push_back(cv::Point2f(865,383));
  rightRegionPoints.push_back(cv::Point2f(497,274));
  rightRegionPoints.push_back(cv::Point2f(264,274));

  std::vector<cv::Point2f> outputPoints;
  cv::Size outputSize(m_width / 2, m_height / 2);
  outputPoints.push_back(cv::Point2f(0, m_height/2) );
  outputPoints.push_back(cv::Point2f(m_width/2, m_height/2) );
  outputPoints.push_back(cv::Point2f(m_width/2, 0) );
  outputPoints.push_back(cv::Point2f(0, 0) ); 


  m_leftIpm.reset(new InversePerspectiveMapping(cv::Size(m_width,m_height),cv::Size(m_width/2,m_height/2),leftRegionPoints,outputPoints));
  m_rightIpm.reset(new InversePerspectiveMapping(cv::Size(m_width,m_height),cv::Size(m_width/2,m_height/2),rightRegionPoints,outputPoints));
    

  m_regions = Eigen::MatrixXd(7,4);

  //-----------------------------
  // Scaling: Calibrations were made for 640x480 resolution
  //-----------------------------
  m_minRow = 0;//m_minRow * (m_height / 480.0);
  m_maxRow = m_height/2;//m_maxRow * (m_height / 480.0);

  m_leftCameraRegions = Eigen::MatrixXd(7,4);
  m_leftCameraRegions << 
      50, 0, 50,100,
      150,0,150,100,
      270,0,270,100,
      325,0,325,100,
      380,0,380,100,
      490,0,490,100,
      590,0,590,100;  // m_leftCameraRegions <<     
  //   125, 161, 16, 215,
  //   207, 183, 110, 433,
  //   269, 177, 260, 448,
  //   295, 183, 308, 451,
  //   307, 184, 391, 445,
  //   323, 195, 456, 450,
  //   361, 197, 531, 453,
  //   406, 191, 624, 392,
  //   453, 145, 609, 202;

  m_leftCameraRegions.col(0) *= (m_width / 640.0) / 2;
  m_leftCameraRegions.col(2) *= (m_width / 640.0) / 2;
  m_leftCameraRegions.col(1) *= (m_height / 480.0) / 2;
  m_leftCameraRegions.col(3) *= (m_height / 480.0) / 2;

  m_leftLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_leftCameraRegions,m_leftLines);

  m_rightCameraRegions = Eigen::MatrixXd(7,4);
  m_rightCameraRegions <<
      50, 0, 50,100,
      150,0,150,100,
      270,0,270,100,
      325,0,325,100,
      380,0,380,100,
      490,0,490,100,
      590,0,590,100;
  m_rightCameraRegions.col(0) *= (m_width / 640.0);
  m_rightCameraRegions.col(2) *= (m_width / 640.0);
  m_rightCameraRegions.col(1) *= (m_height / 480.0);
  m_rightCameraRegions.col(3) *= (m_height / 480.0);

  m_rightLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_rightCameraRegions,m_rightLines);
  //-----------------------------
  // Image processing parameters
  //-----------------------------
  m_nPoints = m_maxRow-m_minRow;
  
  //-----------------------------
  // Initializations
  //-----------------------------
  m_lines = Eigen::MatrixXd(m_regions.rows(),2);

  // Middle region line index
  m_midRegion = (int)(m_lines.rows()-1)/2;
  

  m_recoveredPoints = Eigen::MatrixXd(m_nPoints,m_lines.rows()+2);
  
  m_nRegions = m_recoveredPoints.cols()-1;
   
  // Counts the number of points per each region
  m_pointsPerRegion = Eigen::VectorXd(m_nRegions,1);
  
  // Holds the index that decides the left and right road track
  m_regionIndex = Eigen::MatrixXd(2,1);

  m_k = Eigen::MatrixXd(m_regions.rows(),1);
  m_m = Eigen::MatrixXd(m_regions.rows(),1);

  m_K = Eigen::MatrixXd(m_nRegions,1);
  m_M = Eigen::MatrixXd(m_nRegions,1);

  m_setup = true;
 }

void DetectLane::tearDown()
{
}

} // detectlane
} // perception
} // opendlv
