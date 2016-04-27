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
      a_argc, a_argv, "perception-detectlane")
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
//    std::cout << "--- Received unimportant container of type " << 
//        c.getDataType() << std::endl;
    return;
  }
//  std::cout << "Received container of type " << c.getDataType() << 
//      " sent at " <<   c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      std::endl;
  
  odcore::data::image::SharedImage mySharedImg = 
      c.getData<odcore::data::image::SharedImage>();

//  cout << "Received a SharedImage of size: (" << mySharedImg.getWidth() << 
//      ", " << mySharedImg.getHeight() << ")" << endl;

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(mySharedImg.getName()));
  
  const uint32_t nrChannels = 3;//mySharedImg.getBytesPerPixel();;
  int imgWidth = mySharedImg.getWidth();
  int imgHeight = mySharedImg.getHeight();
  cv::Mat image;
 
  IplImage* myIplImage;
  myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels);
  cv::Mat src(myIplImage);

  if (!sharedMem->isValid()) {
    return;
  }
  
  sharedMem->lock();
  {
    memcpy(src.data, sharedMem->getSharedMemory(), imgWidth*imgHeight*nrChannels);
  }
  sharedMem->unlock();
  
  
  // From this point implement lane detection algorithm

  // TODO: CHECK IF IT IS LEFT OR RIGTH CAMERA

  //-----------------------------
  // Definitions for the video choice
  //-----------------------------
  int width = 640, height = 480;
  int MAXROW, MINROW;

  MINROW = 200;
  MAXROW = 450;

  Eigen::MatrixXd regions(5,4);
  bool initialized = false;
  if(mySharedImg.getName() == "front-left"){
    // Eigen::MatrixXd regions(5,4);
    //region = col1,row1, col2,row2
    regions << 
    156, 229, 91, 441,
    281, 217, 272, 456,
    328, 228, 382, 452,
    381, 218, 495, 436,
    448, 197, 622, 416;
    initialized = true;
  }
  if(mySharedImg.getName() == "front-right"){
    // Eigen::MatrixXd regions(5,4);
    //region = col1,row1,col2,row2
    regions<<
    215, 230, 75, 392,
    306, 245, 195, 431,
    379, 214, 316, 439,
    425, 237, 423, 445,
    507, 221, 574, 414;
    initialized = true;
  }
  if(!initialized){
    return;
  }

  
  //-----------------------------
  // Scaling: Calibrations were made for 640x480 resolution
  //-----------------------------
  MINROW = MINROW * ( height / 480.0); MAXROW = MAXROW * (height / 480.0);
  regions.col(0) = regions.col(0) * ( width / 640.0);
  regions.col(2) = regions.col(2) * ( width / 640.0);
  regions.col(1) = regions.col(1) * ( height / 480.0);
  regions.col(3) = regions.col(3) * ( height / 480.0);
  
  //-----------------------------
  // Image processing parameters
  //-----------------------------
  int nPoints = MAXROW-MINROW;
  
  //-----------------------------
  // Initializations
  //-----------------------------
  
  // Matrix holding the lines col = row * k + m for the region lines
  Eigen::MatrixXd lines(regions.rows(),2);
  
  // Matrix holding the mean column for each region on each row
  Eigen::MatrixXd recoveredPoints(nPoints,lines.rows()+2);
  
  // Number of search regions
  long nRegions = recoveredPoints.cols()-1;
  
  // Holds the K and M parameters for each region
  Eigen::MatrixXd K(nRegions,1), M(nRegions,1);
  
  // Counts the number of points per each region
  Eigen::VectorXd pointsPerRegion(nRegions,1);
  
  // Holds the index that decides the left and right road track
  Eigen::MatrixXd regionIndex(2,1);
  
  // Holds the location of found lanes
  Eigen::VectorXd laneLocation2;
  
  // Holds the k and m values from the previous frame
  Eigen::MatrixXd kPrev = Eigen::MatrixXd::Zero(regions.rows()+1, 1);
  Eigen::MatrixXd mPrev = Eigen::MatrixXd::Zero(regions.rows()+1, 1);

	// Momentum parameter
	// double alpha = 0.5;

	// Lane offset variable
	double laneOffset;

	// Holds the index of the left and right road track
	int p1,p2;
	//int prevP1,prevP2;

	// Middle region line index
	int midRegion = (int)(lines.rows()-1)/2;

	//-----------------------------
  // Extracts the equation for the region lines
  //-----------------------------
  GetRegionLinesV2(regions, lines);
  
  // Holds the k and m values for the region lines
  Eigen::MatrixXd k = lines.col(0);
  Eigen::MatrixXd m = lines.col(1);
  
  //-----------------------------
  // Initialize windows
  //-----------------------------
  //namedWindow("1", 1);
  //namedWindow("Canny",1);
  
  //moveWindow("1", 0, 0);
  
  //-----------------------------
  // Re-size the source image
  //-----------------------------
  resize(src, src, Size(width,height),0,0,INTER_CUBIC);

  //-----------------------------
  // Choose processing type
  //-----------------------------
  // OLA CHANGE HERE IF NEEDED
  int T1 = 180;
  cv::inRange(src, cv::Scalar(T1, T1, T1), cv::Scalar(255, 255, 255), image);
  medianBlur(image, image, 3);

    
  //-----------------------------
  // Local line search
  //-----------------------------
  ScanImage(&image,&lines,&recoveredPoints,nPoints,MINROW,MAXROW);
  
  
  //-----------------------------
  // Extract lines from points
  //-----------------------------
  ExtractLines(&recoveredPoints,&K,&M,nRegions,nPoints,&pointsPerRegion);

  //-----------------------------
  // Make decision based on lines
  //-----------------------------
  laneLocation2 = Eigen::VectorXd::Zero(nRegions, 1);
  SelectLanesV2(pointsPerRegion, laneLocation2);
  
  if (fabs(pointsPerRegion.sum()) > 0.0001){
    SelectLaneOrientation(regionIndex,laneLocation2,(int)recoveredPoints.cols());
    p1 = regionIndex(0,0), p2 = regionIndex(1,0);
    
    
    
    //-----------------------------
    //Add momentum & update previous lines
    //-----------------------------
    
    // AddMomentum(K,kPrev,M,mPrev,alpha,regionIndex);
    // kPrev = K;
    // mPrev = M;
    // prevP1 = p1;
    // prevP2 = p2;
    
    
    //-----------------------------
    // Draw boarders/lines - VISUALIZATION
    //-----------------------------
    
    DrawBorders(&src,MINROW,MAXROW,K(p1,0),K(p2,0),M(p1,0),M(p2,0));
 //   DrawTracks(&src, &K, &M,MINROW,MAXROW,Scalar(0,0,255));
   DrawTracks(&src, &k,&m,MINROW,Scalar(255,255,255));
    

    //-----------------------------
    // Calculate lane offset
    //-----------------------------
    laneOffset = GetLateralPosition(K(p1,0),M(p1,0),
                                    K(p2,0),M(p2,0),
                                    lines.col(0)(midRegion),lines.col(1)(midRegion),
                                    (MAXROW));
    //-----------------------------
    // Approximate heading angle
    //-----------------------------
    double d1 = GetLateralPosition(K(p1,0),M(p1,0),
                                K(p2,0),M(p2,0),
                              lines.col(0)(midRegion),lines.col(1)(midRegion),
                                (MAXROW));
    double d2 = GetLateralPosition(K(p1,0),M(p1,0),
                                K(p2,0),M(p2,0),
                                lines.col(0)(midRegion),lines.col(1)(midRegion),
                                (MINROW));
    
    double theta = atan((d2-d1) / (double)15);
    
    // DEBUG PRINT
    //std::cout<<"Heading angle "<<theta<<std::endl;
    //std::cout<<"Offset "<<laneOffset<<std::endl;
		
    
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
  waitKey(10);
	
  cvReleaseImage(&myIplImage);
  }

void DetectLane::setUp()
{
}

void DetectLane::tearDown()
{
}

} // detectlane
} // perception
} // opendlv
