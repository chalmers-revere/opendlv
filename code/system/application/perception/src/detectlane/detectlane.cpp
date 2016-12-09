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

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include <opendavinci/odcore/base/Lock.h>


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
    : TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-detectlane")
    , m_initialized(false)
    , m_image()
    , m_visualMemory()
    , m_intensityThreshold()
    , m_cannyThreshold()
    , m_houghThreshold()
    , m_lineDiff()
    , m_blur()
    , m_memThreshold()
    , m_upperLaneLimit()
    , m_lowerLaneLimit()
    , m_screenSize()
    , m_roi()
    , m_mtx()
    , m_debug()
    , m_camera()
    , m_transformationMatrix()
    , m_counter(0)

{
}

DetectLane::~DetectLane()
{
  m_image.release();
}
void DetectLane::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_intensityThreshold = kv.getValue<uint16_t>("perception-detectlane.intensityThreshold");
  m_cannyThreshold = kv.getValue<uint16_t>("perception-detectlane.cannyThreshold");
  m_houghThreshold = kv.getValue<uint16_t>("perception-detectlane.houghThreshold");
  m_lineDiff = kv.getValue<float>("perception-detectlane.lineDiff");
  m_blur = kv.getValue<uint16_t>("perception-detectlane.blurStrength");
  m_memThreshold = kv.getValue<double>("perception-detectlane.memThreshold");
  m_upperLaneLimit = kv.getValue<uint16_t>("perception-detectlane.upperLaneLimit");
  m_lowerLaneLimit = kv.getValue<uint16_t>("perception-detectlane.lowerLaneLimit");
  m_screenSize[0] = kv.getValue<uint16_t>("perception-detectlane.screenWidth");
  m_screenSize[1] = kv.getValue<uint16_t>("perception-detectlane.screenHeight");
  m_roi[0] = kv.getValue<uint16_t>("perception-detectlane.roiX");
  m_roi[1] = kv.getValue<uint16_t>("perception-detectlane.roiY");
  m_roi[2] = kv.getValue<uint16_t>("perception-detectlane.roiWidth");
  m_roi[3] = kv.getValue<uint16_t>("perception-detectlane.roiHeight");
  m_debug = (kv.getValue<int32_t>("perception-detectlane.debug") == 1);
  m_camera = kv.getValue<std::string>("perception-detectlane.camera");
  // Insert correct transformation matrix depending on which camera 
  if(m_camera == "frontLeft"){
  m_transformationMatrix = ReadMatrix(
          "/opt/opendlv.data/front-left-pixel2world-matrix.csv",3,3);
    std::cout << "chose correct matrix" << std::endl;
  }
  m_initialized = true;
}

void DetectLane::tearDown()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DetectLane::body()
{

  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING)
  {
    
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

/**
 * Receives images from vision source and detect lanes.
 * Sends drivalble lanes objects.
 */
void DetectLane::nextContainer(odcore::data::Container &a_c)
{

  if (!m_initialized || a_c.getDataType() != odcore::data::image::SharedImage::ID()){
    return;
  }

  // Increase the counter for each frame
  m_counter++;

  odcore::data::TimeStamp now;

  odcore::data::image::SharedImage mySharedImg =
        a_c.getData<odcore::data::image::SharedImage>();
  // std::cout<<mySharedImg.getName()<<std::endl;

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
      odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
          mySharedImg.getName()));
  const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
  const uint32_t imgWidth = mySharedImg.getWidth();
  const uint32_t imgHeight = mySharedImg.getHeight();

  IplImage* myIplImage = cvCreateImage(cvSize(imgWidth,imgHeight), IPL_DEPTH_8U,
      nrChannels);
  cv::Mat tmpImage = cv::Mat(myIplImage);

  if(!sharedMem->isValid()){
    std::cout << "Return from line 148" << std::endl;
    return;
  }

  sharedMem->lock();
  {
    memcpy(tmpImage.data, sharedMem->getSharedMemory(),
        imgWidth*imgHeight*nrChannels);
  }
  sharedMem->unlock();
  m_image.release();
  m_image = tmpImage.clone();
  tmpImage.release();

  cvReleaseImage(&myIplImage);


  cv::Rect rectROI(m_roi[0], m_roi[1], m_roi[2], m_roi[3]);
  // cv::Rect rectROI(0, 450, 1280, 270);
  cv::Mat croppedImg;
  try {
    odcore::base::Lock l(m_mtx);
    croppedImg = m_image(rectROI);
  } catch (cv::Exception& e) {
    std::cerr << "Error cropping the image due to dimension size." << std::endl;
    std::cout << "Return from line 173" << std::endl;
    return;
  }

  const int32_t windowWidth = 640/2;
  const int32_t windowHeight = 400/2;

  // Set number of windows to display
  //cv::Mat display[9];
  cv::Mat display[3];

  cv::Mat visualImpression = croppedImg.clone();
   // Delete the Matrix
  croppedImg.release();

  m_visualMemory.push_back(std::make_pair(now,visualImpression));
   // Delete the Matrix
  //visualImpression.release();

  while(m_visualMemory.size() > 0 && (now-m_visualMemory[0].first).toMicroseconds()/1000000.0 > m_memThreshold){
    m_visualMemory.pop_front();
  }

  // Add the limitation lines to the picture
  if(m_debug) {
    cv::line(m_image, cv::Point(0,m_upperLaneLimit), cv::Point(m_screenSize[0],m_upperLaneLimit), cv::Scalar(255,0,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_lowerLaneLimit), cv::Point(m_screenSize[0],m_lowerLaneLimit), cv::Scalar(255,0,0), 3, 1 );
    
    cv::line(m_image, cv::Point(m_roi[0],0), cv::Point(m_roi[0],m_screenSize[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(m_roi[0]+m_roi[2],0), cv::Point(m_roi[0]+m_roi[2],m_screenSize[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_roi[1]), cv::Point(m_screenSize[0],m_roi[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_roi[1]+m_roi[3]), cv::Point(m_screenSize[0],m_roi[1]+m_roi[3]), cv::Scalar(255,255,0), 3, 1 ); 
  }

 // =================================== OUR IMPLEMENTATION =====================================
  // ------------------------ Canny transformation ---------------------------
  cv::Mat tmpImg = visualImpression; // m_visualMemory[0].second.clone();
  cv::Mat cannyImg;
  cv::Mat cannyImgRes;
  cv::Mat blurredImg;
  cv::medianBlur(tmpImg,blurredImg,m_blur);
  
  cv::Canny(blurredImg, cannyImg, m_cannyThreshold, m_cannyThreshold*3, 3);

  // Sobel function that reduce the noice from canny
  int ddepth = -1;//cv::CV_16S;
  cv::Sobel(cannyImg, cannyImgRes, ddepth, 1, 0, 3, 1,0,cv::BORDER_DEFAULT);
  cannyImg.release();
  // ------------------------ Adaptive Threshold ---------------------------
  cv::Mat intenImgRes;
  cv::Mat TresholdRes;
  tmpImg = intenImgRes;
  cv::inRange(blurredImg, cv::Scalar(m_intensityThreshold, m_intensityThreshold, m_intensityThreshold), cv::Scalar(255, 255, 255), tmpImg);
  //cv::cvtColor(m_image, intenImg, CV_GRAY2BGR);
  blurredImg.release();

  cv::adaptiveThreshold(tmpImg,intenImgRes,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV,11,2);
  tmpImg.release();
  // Use sobel here as well? for the adaptive threshold
  cv::Sobel(intenImgRes, TresholdRes, ddepth, 1, 0, 3, 1,0,cv::BORDER_DEFAULT); 
  intenImgRes.release();

  // ------------------------ Hough Transform ---------------------------
  // Vector holder for each line (rho,theta)
  cv::vector<cv::Vec2f> detectedLines;
  cv::vector<cv::Vec2f> detectedLinesCanny;
  cv::vector<cv::Vec2f> detectedLinesIntensitive;

  // OpenCV function that uses the Hough transform and finds the "strongest" lines in the transformation    
  cv::HoughLines(cannyImgRes, detectedLinesCanny, 1, opendlv::Constants::PI/180, m_houghThreshold);
  cv::HoughLines(TresholdRes, detectedLinesIntensitive, 1, opendlv::Constants::PI/180, m_houghThreshold);
  //cv::HoughLinesP(TresholdRes, detectedLinesIntensitive, 1, opendlv::Constants::PI/180, 80, 30, 10 );
  std::cout << "From Hough, detectedLinesCanny: "<< detectedLinesCanny.size() << std::endl;
  std::cout << "From Hough, detectedLinesIntensitive: "<< detectedLinesIntensitive.size() << std::endl;

  // ------------------------ Print outs ---------------------------
  /*if(m_debug) {
      cv::resize(cannyImgRes, display[1], cv::Size(windowWidth, windowHeight), 0, 0, cv::INTER_CUBIC);
      cv::imshow("Canny transform", display[1]);
      cv::waitKey(1);
    }
    //return;
  }*/

  cannyImgRes.release();  
  TresholdRes.release();
  // ------------------------ Add all found lines to image & -------------------------
  //----------------------------- Grouping Canny lines -------------------------------

  detectedLines = detectedLinesCanny;

  std::vector<cv::Vec2f> tmpVec;
  float angleTresh = 85 * opendlv::Constants::PI/180;
  for(uint16_t i = 0; i != detectedLines.size(); i++){
    if(angleTresh > detectedLines[i][1] || 3.14f-angleTresh < detectedLines[i][1]){
      tmpVec.push_back(detectedLines[i]);
    }
  }
  detectedLines = tmpVec;
  //Grouping the vectors
  std::vector<cv::Vec2f> groups;
  GetGrouping(groups, detectedLines,100);
  detectedLinesCanny = groups; 
  
  std::cout << "Filtered Canny: "<< detectedLinesCanny.size() << std::endl;

  // ------------------------ Add all found lines to image & ---------------------------
  //---------------------------- Grouping Threshold lines ------------------------------

  detectedLines = detectedLinesIntensitive;

  std::vector<cv::Vec2f> tmpVec2;
  for(uint16_t i = 0; i != detectedLines.size(); i++){
    if(angleTresh > detectedLines[i][1] || 3.14f-angleTresh < detectedLines[i][1]){
      tmpVec2.push_back(detectedLines[i]);
    }
  }
  detectedLines = tmpVec2;
  
  cv::Mat img1 = m_image.clone();
  cv::Mat img2 = m_image.clone();
  // Print out all lines
  if(m_debug) {
    
    for( size_t i = 0; i < detectedLines.size(); i++ ) {
      float rho = detectedLines[i][0];
      float theta = detectedLines[i][1];

      float a = cos(theta), b = sin(theta);
      float x0 = a*rho, y0 = b*rho;
      cv::Point pt1(m_roi[0] + x0 + 2000*(-b), m_roi[1] + y0 + 2000*(a));
      cv::Point pt2(m_roi[0] + x0 - 2000*(-b), m_roi[1] + y0 - 2000*(a));

      cv::line(img1, pt1, pt2, cv::Scalar(0,0,255), 2, 1 );
    }
  }

  //Grouping the vectors
  std::vector<cv::Vec2f> groups2;
  GetGrouping(groups2, detectedLines,100);
  detectedLinesIntensitive = groups2; 

    // Print out all lines
  if(m_debug) {
    for( size_t i = 0; i < detectedLinesIntensitive.size(); i++ ) {
      float rho = detectedLinesIntensitive[i][0];
      float theta = detectedLinesIntensitive[i][1];

      float a = cos(theta), b = sin(theta);
      float x0 = a*rho, y0 = b*rho;
      cv::Point pt1(m_roi[0] + x0 + 2000*(-b), m_roi[1] + y0 + 2000*(a));
      cv::Point pt2(m_roi[0] + x0 - 2000*(-b), m_roi[1] + y0 - 2000*(a));

      cv::line(img2, pt1, pt2, cv::Scalar(0,0,255), 2, 1 );
    }
  }
  if(m_debug) {
  cv::resize(img1, display[1], cv::Size(windowWidth*2, windowHeight*2), 0, 0, cv::INTER_CUBIC);
  cv::imshow("Unfiltered", display[1]);
  cv::resize(img2, display[2], cv::Size(windowWidth*2, windowHeight*2), 0, 0, cv::INTER_CUBIC);
  cv::imshow("Filtered", display[2]);
  
  cv::waitKey(1);
  }
  std::cout << "Filtered Threshold: "<< detectedLinesIntensitive.size() << std::endl;
  // --------------------- Merge Canny & threshold lines ---------------------------
  
  detectedLines = detectedLinesIntensitive;
  //detectedLines = detectedLinesCanny;

  //detectedLinesIntensitive.insert( detectedLinesIntensitive.end(), detectedLinesCanny.begin(), detectedLinesCanny.end() );
  //GetGrouping(groups2, detectedLinesIntensitive,100);
  //detectedLines = groups2;
  //std::cout << "Merged lines: " << detectedLines.size() << std::endl;

  // Get parametric line representation
  std::vector<cv::Vec2f> p, m;

  GetParametricRepresentation(p, m, detectedLines);

  // Get points on lines
  std::vector<cv::Vec2f> xScreenP,yScreenP;
  std::vector<cv::Vec2f> xWorldP, yWorldP;
  GetPointsOnLine(xScreenP, yScreenP, xWorldP, yWorldP, p, m);
  
  
  std::cout << "Coordinates: ";
  for(uint8_t i = 0; i < yWorldP.size(); i++){
    std::cout << yWorldP[i][1] << " , ";
  } 
  std::cout << std::endl;
  

  //std::cout << "Coordinates:: " << yWorldP.size() << std::endl;
  
  // Filter out lines that are close to each other
  std::vector<int> toBeRemoved;
  for(uint8_t i = 0; i < yWorldP.size(); i++){
    for(uint8_t j = i+1; j < yWorldP.size(); j++){
      if( (yWorldP[i][1] < 0) && (yWorldP[j][1] < 0) ){ // if smaller than 0, it is a negative number
        //std::cout << "Two negative coordinates: " << yWorldP[i][1] << ", " << yWorldP[j][1] << std::endl;
        if(std::abs(yWorldP[i][1] - yWorldP[j][1]) < m_lineDiff ){
          // Remove the one that is largest
          //std::cout << "Small diff, remove a line" << std::endl;
          if(yWorldP[i][1] > yWorldP[j][1]){
            toBeRemoved.push_back(yWorldP[j][1]);
            std::cout << "Removed: " << yWorldP[j][1] << std::endl;
            continue;
          }
          else{
            toBeRemoved.push_back(yWorldP[i][1]);
            std::cout << "Removed: " << yWorldP[i][1] << std::endl;
            continue;
          }
        }
      }
      else if(yWorldP[i][1] > 0 && yWorldP[j][1] > 0){ // if bigger than 0, it is a positive number
        //std::cout << "Two positive coordinates: " << yWorldP[i][1] << ", " << yWorldP[j][1] << std::endl;
        if(std::abs(yWorldP[i][1] - yWorldP[j][1]) < m_lineDiff ){
          // Remove the one that is smallest
          //std::cout << "Small diff, remove a line" << std::endl;
          if(yWorldP[i][1] > yWorldP[j][1]){
            toBeRemoved.push_back(yWorldP[i][1]);
            std::cout << "Removed: " << yWorldP[i][1] << std::endl;
            continue;
          }
          else{
            toBeRemoved.push_back(yWorldP[j][1]);
            std::cout << "Removed: " << yWorldP[j][1] << std::endl;
            continue;
          }
        }
      }
    }
  }

  // Print out all lines
  if(m_debug) {
    for( size_t i = 0; i < detectedLines.size(); i++ ) {
      float rho = detectedLines[i][0];
      float theta = detectedLines[i][1];

      float a = cos(theta), b = sin(theta);
      float x0 = a*rho, y0 = b*rho;
      cv::Point pt1(m_roi[0] + x0 + 2000*(-b), m_roi[1] + y0 + 2000*(a));
      cv::Point pt2(m_roi[0] + x0 - 2000*(-b), m_roi[1] + y0 - 2000*(a));

      cv::line(m_image, pt1, pt2, cv::Scalar(0,0,255), 2, 1 );
    }
  }

  /* DETECT LANE STARTS HERE  */

  // Pair up lines to form a surface
  std::vector<int8_t> groupIds;
  std::vector<int8_t> groupIdsCurrentLane;
  GetLinePairs(xScreenP, groupIdsCurrentLane);
  //GetAllLines(xScreenP, groupIds);

  for(uint8_t i = 0; i < xScreenP.size(); i++){    
    bool hit = false;
    for(uint8_t j = 0; j < toBeRemoved.size(); j++){
      if(j == i){
        hit = true;
      }
    }
    if(!hit){
      groupIds.push_back(i);  
    }
    else{
      std::cout << "Skipped line: " << i << std::endl;
    }
    
  }

  if(groupIds.size() == 0){
    std::cout << "No groups found" << std::endl;
    std::cout << "Return from line 546" << std::endl;
    return;
  }

  if(m_debug) {
    for(uint8_t i = 0; i < groupIds.size(); i++) {
      for(uint8_t k = 0; k < 2; k++) {
        //cv::circle(Thresh_res2, cv::Point(xScreenP[groupIds[i]][k],yScreenP[groupIds[i]][k]), 10, cv::Scalar(0,255,0), 3, 8);
        cv::circle(m_image, cv::Point(xScreenP[groupIds[i]][k],yScreenP[groupIds[i]][k]), 10, cv::Scalar(0,255,0), 3, 8);
        //std::cout << xWorldP[groupIds[i]][k] << "x" << yWorldP[groupIds[i]][k] << ",";
      }
      //std::cout << std::endl;
    }
    for(uint8_t i = 0; i < groupIdsCurrentLane.size(); i++) {
      for(uint8_t k = 0; k < 2; k++) {
        //cv::circle(Thresh_res2, cv::Point(xScreenP[groupIds[i]][k],yScreenP[groupIds[i]][k]), 10, cv::Scalar(0,255,0), 3, 8);
        cv::circle(m_image, cv::Point(xScreenP[groupIdsCurrentLane[i]][k],yScreenP[groupIdsCurrentLane[i]][k]), 10, cv::Scalar(0,0,255), 3, 8);
        //std::cout << xWorldP[groupIds[i]][k] << "x" << yWorldP[groupIds[i]][k] << ",";
      }
      //std::cout << std::endl;
    }
    //cv::resize(cannyImgRes, display[1], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
    //cv::INTER_CUBIC);
    //cv::imshow("Canny2", display[1]);
    

    // Now considering the adaptive threshold algorithm only, should fix correct image to merge with canny
    //cv::resize(Thresh_res2, display[1], cv::Size(windowWidth*2, windowHeight*2), 0, 0, cv::INTER_CUBIC);
    cv::resize(m_image, display[0], cv::Size(windowWidth*2, windowHeight*2), 0, 0, cv::INTER_CUBIC);
    cv::imshow("Result", display[0]);
    
    cv::waitKey(1);
  }
  //Thresh_res2.release();
  m_image.release();



  odcore::data::TimeStamp imageTimeStamp = a_c.getSentTimeStamp();
  std::string type = "surface";
  float typeConfidence = 1;

  std::vector<opendlv::model::Cartesian3> edges;

  // Print out all lines found
  for(uint8_t i=0; i < groupIds.size(); i++){
    edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[i]][0],yWorldP[groupIds[i]][0],0));
    edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[i]][1],yWorldP[groupIds[i]][1],0));
  }

  /*
  //topleft
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[0]][0],yWorldP[groupIds[0]][0],0));
  //bottomleft
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[0]][1],yWorldP[groupIds[0]][1],0));
  //bottomright
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[1]][1],yWorldP[groupIds[1]][1],0));
  //topright
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[1]][0],yWorldP[groupIds[1]][0],0));
  */
  // std::cout << "+++++++++++++++++++++++++++" << std::endl;
  // for(uint8_t i = 0; i < edges.size(); i++) { 
  //   std::cout << edges[i].toString() << std::endl;
  // }
  // std::cout << "+++++++++++++++++++++++++++" << std::endl;

  float edgesConfidence = 1;

  bool traversable = true;
  float confidence = 1;
  std::vector<std::string> sources;
  sources.push_back(mySharedImg.getName());

  std::vector<std::string> properties;

  int16_t surfaceId = 0;

  std::vector<int16_t> connectedWidth;
  std::vector<int16_t> traversableTo;

  if(edges.size() == 6 || edges.size() == 4){
    std::cout << "Found all lines" << std::endl;
    std::cout<<"Detected frame: " << m_counter << std::endl;
  }
  else{
    std::cout << "Failed, detected: " << (edges.size()/2) << std::endl;
  }

  opendlv::perception::Surface detectedSurface(imageTimeStamp,
      type,
      typeConfidence,
      edges,
      edgesConfidence,
      traversable,
      confidence,
      sources,
      properties,
      surfaceId,
      connectedWidth,
      traversableTo);


  odcore::data::Container objectContainer(detectedSurface);
  getConference().send(objectContainer);


}


// Gruppera linjer som är närliggande till en enda, en average linje som tar alla liknande
void DetectLane::GetGrouping(std::vector<cv::Vec2f> &a_groups, std::vector<cv::Vec2f> &a_lines, double a_groupingRadius)
{
  if(a_lines.size() < 1) {
    return;
  }
  std::vector< std::vector<cv::Vec2f> > group;
  std::vector<cv::Vec2f> groupMean, groupSum;
  std::vector<cv::Vec2f> tmp;
  group.push_back(tmp);
  group.at(0).push_back(a_lines.at(0));
  groupMean.push_back(group.at(0).at(0));
  groupSum.push_back(group.at(0).at(0));
  
  for(uint16_t i = 1; i < a_lines.size(); i++) {
    bool groupAssigned = false;
    for(uint16_t j = 0; j < group.size(); j++) {
      double xDiff = a_lines[i][0] - groupMean[j][0];
      double yDiff = a_lines[i][1] - groupMean[j][1];
      double absDiff = sqrt(pow(xDiff,2) + pow(yDiff,2));

      if( absDiff <= a_groupingRadius) {
        group.at(j).push_back(a_lines.at(i));
        groupSum.at(j) += a_lines.at(i);
        groupMean.at(j)[0] = groupSum.at(j)[0] / group.at(j).size();
        groupMean.at(j)[1] = groupSum.at(j)[1] / group.at(j).size();
        groupAssigned = true;
        break;
      }
    }
    if(!groupAssigned) {
      group.push_back(tmp);
      group.at(group.size()-1).push_back(a_lines.at(i));
      groupMean.push_back(a_lines.at(i));
      groupSum.push_back(a_lines.at(i));
    }
  }
  a_groups = groupMean;
}

// konvertera till räta linjens ekvation istället för rho och theta
void DetectLane::GetParametricRepresentation(std::vector<cv::Vec2f> &a_p
    , std::vector<cv::Vec2f> &a_m
    , std::vector<cv::Vec2f> &a_groups)
{
  for(uint i = 0; i < a_groups.size(); i++){
    float rho = a_groups[i][0];
    float theta = a_groups[i][1];
    
    float heading = -(static_cast<float>(opendlv::Constants::PI) / 2.0f - theta);
    float a = cos(theta), b = sin(theta);
    float x0 = a*rho, y0 = b*rho;
    
    a_p.push_back(cv::Vec2f(cos(heading),sin(heading)));
    a_m.push_back(cv::Vec2f(x0,y0));
  }
}

// hitta röda linjen med upper och lower lanelimit för de gröna prickarna
// returnerar både världs och pixel koordinaterna
void DetectLane::GetPointsOnLine(std::vector<cv::Vec2f> &a_xScreenP
    , std::vector<cv::Vec2f> &a_yScreenP
    , std::vector<cv::Vec2f> &a_xWorldP
    , std::vector<cv::Vec2f> &a_yWorldP
    , std::vector<cv::Vec2f> &a_p
    , std::vector<cv::Vec2f> &a_m)
{
  for (uint16_t i = 0; i < a_p.size(); i++) {
    float t1,t2;
    // To handle special case of dividing 0  
    if(fabs(a_p[i][1]) > 0.000001f){
      t1 =  (static_cast<float>(m_upperLaneLimit - m_roi[1]) - a_m[i][1]) / sin(a_p[i][1]);
      t2 =  (static_cast<float>(m_lowerLaneLimit - m_roi[1]) - a_m[i][1]) / sin(a_p[i][1]);
    }
    else {
      t1 = 0;
      t2 = 0;
    }
    Eigen::Vector3d point1, point2;
    
    float x1 = (t1 * a_p[i][0] + a_m[i][0]) + m_roi[0]; 
    point1 << x1, m_upperLaneLimit, 1;
    TransformPointToGlobalFrame(point1);

    float x2 =  (t2 * a_p[i][0] + a_m[i][0] + m_roi[0]); 
    point2 << x2, m_lowerLaneLimit, 1;
    TransformPointToGlobalFrame(point2);

    a_xWorldP.push_back(cv::Vec2f(point1(0),point2(0)));
    a_yWorldP.push_back(cv::Vec2f(point1(1),point2(1)));

    a_xScreenP.push_back(cv::Vec2f(x1,x2));
    a_yScreenP.push_back(cv::Vec2f(m_upperLaneLimit,m_lowerLaneLimit));
    
  }
}

// Hitta alla linjer
void DetectLane::GetAllLines(std::vector<cv::Vec2f> &a_xPoints
  , std::vector<int8_t> &a_groupIds){
  
  for(uint8_t i = 0; i < a_xPoints.size(); i++){    
    a_groupIds.push_back(i);
  }

}


// Hitta mittersta paret för att hitta aktuell lane
void DetectLane::GetLinePairs(std::vector<cv::Vec2f> &a_xPoints
  , std::vector<int8_t> &a_groupIds)
{
  // Den ska vara för två, borde gå att expandera för fler, hittar en left och right, men borde gå att lösa för att hitta fler
  // float leftId, rightId;
  // for(uint16_t i = 0; i < a_xPoints.size()-1; i++) {
  //   for(uint16_t j= i+1; j < a_xPoints.size(); j++) {
  //     if(a_xPoints[i][0] < a_xPoints[j][1]) {
  //       leftId = i, rightId = j;
  //     } else {
  //       leftId = j, rightId = i;
  //     }
  //     a_groupIds.push_back(cv::Vec2f(leftId,rightId));
      
  //     // float xDiff1 = xPoints[rightId][0] - xPoints[leftId][0];
  //     // float xDiff2 = xPoints[rightId][1] - xPoints[leftId][1];
  //     // Lane width has to be  2 < w < 5 to be valid
  //     //if( xDiff1 > 0 && xDiff1 < 10 && xDiff2 > 0 && xDiff2 < 10){
  //     //}
  //   }
  // }
  int8_t leftLineId = -1;
  int8_t rightLineId = -1;
  for(uint8_t i = 0; i < a_xPoints.size(); i++){
    if(a_xPoints[i][1] < m_screenSize[0]/2) {
      if(leftLineId == -1 || a_xPoints[i][1] > a_xPoints[leftLineId][1]){
        leftLineId = i;
      }
    } else {
      if(rightLineId == -1 ||a_xPoints[i][1] < a_xPoints[rightLineId][1]){
        rightLineId = i;
      }
    }
  }
  if(leftLineId != rightLineId && leftLineId != -1 && rightLineId != -1) {
    a_groupIds.push_back(leftLineId);
    a_groupIds.push_back(rightLineId);
  }
}

Eigen::MatrixXd DetectLane::ReadMatrix(std::string fileName, uint8_t nRows,
    uint8_t nCols)
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
    // std::cout << matrix << std::endl;
  } else {
    std::cout << "Couldn't read the projection matrix." << std::endl;
  }
  file.close();
  return matrix;
}
void DetectLane::TransformPointToGlobalFrame(Eigen::Vector3d &point)
{
  // std::cout<<"point before \n";
  // std::cout<<point<<std::endl;
  // point = point.cwiseProduct(m_scale);
  // std::cout<<"point after 1\n";
  // std::cout<<point<<std::endl;
  point = m_transformationMatrix * point;

  // std::cout<<"point final before \n";
  // std::cout<<point<<std::endl;

  point = point / point(2);
  // std::cout<<"point final \n";
  // std::cout<<point<<std::endl;

}

} // detectlane
} // perception
} // opendlv
