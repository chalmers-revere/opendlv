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
    , m_memThreshold()
    , m_upperLaneLimit()
    , m_lowerLaneLimit()
    , m_screenSize()
    , m_roi()
    , m_mtx()
    , m_debug()
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
  // Lägg in rätt namn på matrisen här
  m_transformationMatrix = ReadMatrix(
          "/opt/opendlv.data/front-left-pixel2word-matrix.csv",3,3);
  m_initialized = true;
}

void DetectLane::tearDown()
{
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DetectLane::body()
{
  int8_t sgn = 1;
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING)
  {
    if(m_debug){
      char key = (char) cv::waitKey(1);
      switch(key) {
        case 'u':
          sgn = -sgn;
          break;
        case 'j':    
          {
            odcore::base::Lock l(m_mtx);
            m_roi[0] = m_roi[0] + sgn*5;
            if(m_roi[0] < 0) {
              m_roi[0] = 0;
            } else if(m_roi[0] + m_roi[2] > m_screenSize[0]) {
              m_roi[0] = m_screenSize[0] - m_roi[2];
            }
          }
          m_visualMemory.clear();
          std::cout << m_roi[0]<<" x " << m_roi[1]<<" x " << m_roi[2] <<" x " << m_roi[3] << std::endl;
          break;
        case 'i':
          {
            odcore::base::Lock l(m_mtx);
            m_roi[1] = m_roi[1] + sgn*5;
            if(m_roi[1] < 0) {
              m_roi[1] = 0;
            } else if(m_roi[1] + m_roi[3] > m_screenSize[1]) {
              m_roi[1] = m_screenSize[1] - m_roi[3];
            }
          }
          m_visualMemory.clear();
          std::cout << m_roi[0]<<" x " << m_roi[1]<<" x " << m_roi[2] <<" x " << m_roi[3] << std::endl;
          break;
        case 'l':
          {
            odcore::base::Lock l(m_mtx);
            m_roi[2] = m_roi[2] - sgn*5;
            if(m_roi[2] > m_screenSize[0]) {
              m_roi[2] = m_screenSize[0];
            } else if(m_roi[0] + m_roi[2] > m_screenSize[0]) {
              m_roi[2] = m_screenSize[0] - m_roi[0];
            }
          }
          m_visualMemory.clear();
          std::cout << m_roi[0]<<" x " << m_roi[1]<<" x " << m_roi[2] <<" x " << m_roi[3] << std::endl;
          break;
        case 'k':
          {
            odcore::base::Lock l(m_mtx);
            m_roi[3] = m_roi[3] - sgn*5;
            if(m_roi[3] > m_screenSize[1]) {
              m_roi[3] = m_screenSize[1];
            } else if(m_roi[1] + m_roi[3] > m_screenSize[1]) {
              m_roi[3] = m_screenSize[1] - m_roi[1];
            }
          }
          m_visualMemory.clear();
          std::cout << m_roi[0]<<" x " << m_roi[1]<<" x " << m_roi[2] <<" x " << m_roi[3] << std::endl;
          break;
        case'y':
          m_memThreshold += 0.1;
          std::cout << "Mem: " << m_memThreshold << std::endl;
          break;
        case'h':
          m_memThreshold -= 0.1;
          if (m_memThreshold < 0.1) {
            m_memThreshold = 0.1;
          }
          std::cout << "Mem: " << m_memThreshold << std::endl;
          break;
        case't':
          m_cannyThreshold += 5;
          std::cout << "Canny: " << m_cannyThreshold << std::endl;
          break;
        case'g':
          m_cannyThreshold -= 5;
          if (m_cannyThreshold > 255) {
            m_cannyThreshold = 0;
          }
          std::cout << "Canny: " << m_cannyThreshold << std::endl;
          break;
        case'r':
          m_houghThreshold += 5;
          std::cout << "Hough: " << m_houghThreshold << std::endl;
          break;
        case'f':
          m_houghThreshold -= 5;
          if (m_houghThreshold < 10) {
            m_houghThreshold = 15;
          }
          std::cout << "Hough: " << m_houghThreshold << std::endl;
          break;
        case'w':
          m_upperLaneLimit -= 5;
          std::cout << "UpperLaneLimit: " << m_upperLaneLimit << std::endl;
          break;
        case's':
          m_upperLaneLimit += 5;
          std::cout << "UpperLaneLimit: " << m_upperLaneLimit << std::endl;
          break;
        case'q':
          m_lowerLaneLimit -= 5;
          std::cout << "LowerLaneLimit: " << m_lowerLaneLimit << std::endl;
          break;
        case'a':
          m_lowerLaneLimit += 5;
          std::cout << "LowerLaneLimit: " << m_lowerLaneLimit << std::endl;
        default:
          break;
      }
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}


/**
 * Receives images from vision source and detect lanes.
 * Sends drivalble lanes objects.
 */
void DetectLane::nextContainer(odcore::data::Container &a_c)
{
  // Increase the counter for each frame
  m_counter++;

  if (!m_initialized || a_c.getDataType() != odcore::data::image::SharedImage::ID()) {
    return;
  }
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

  cvReleaseImage(&myIplImage);


  cv::Rect rectROI(m_roi[0], m_roi[1], m_roi[2], m_roi[3]);
  // cv::Rect rectROI(0, 450, 1280, 270);
  cv::Mat croppedImg;
  try {
    odcore::base::Lock l(m_mtx);
    croppedImg = m_image(rectROI);
  } catch (cv::Exception& e) {
    std::cerr << "Error cropping the image due to dimension size." << std::endl;
    return;
  }


  const int32_t windowWidth = 640/2;
  const int32_t windowHeight = 400/2;

  // Set number of windows to display
  cv::Mat display[5];

  cv::Mat visualImpression = croppedImg.clone();
  m_visualMemory.push_back(std::make_pair(now,visualImpression));
  
  while(m_visualMemory.size() > 0 && (now-m_visualMemory[0].first).toMicroseconds()/1000000.0 > m_memThreshold){
    m_visualMemory.pop_front();
  }
  cv::Mat exposedImage = m_visualMemory[0].second.clone();
  
  if(m_visualMemory.size() > 1) {
    cv::Mat tmp;
    double alpha;
    double beta;
    for(uint16_t i = 1; i < m_visualMemory.size(); i++){
      alpha = i/(i+1.0);
      beta = 1-alpha;
      cv::addWeighted(exposedImage, alpha, m_visualMemory[i].second, beta, 0, tmp, -1);
      exposedImage = tmp;
    }
    /*if(m_debug) {
    cv::resize(exposedImage, display[2], cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE3", display[2]); 
    }*/
  }
  cv::Mat cannyImg;
  cv::Mat tmpImg = m_visualMemory[0].second.clone();
  cv::Canny(tmpImg, cannyImg, m_cannyThreshold, m_cannyThreshold*3, 3);

  exposedImage = cannyImg;
  int ddepth = -1;//cv::CV_16S;
  cv::Sobel(exposedImage, cannyImg, ddepth, 1, 0, 3, 1,0,cv::BORDER_DEFAULT);

  //exposedImage = cannyImg;
  //cv::cvtColor(exposedImage, cannyImg, CV_GRAY2BGR);

  if(m_debug) {
    cv::line(m_image, cv::Point(0,m_upperLaneLimit), cv::Point(m_screenSize[0],m_upperLaneLimit), cv::Scalar(255,0,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_lowerLaneLimit), cv::Point(m_screenSize[0],m_lowerLaneLimit), cv::Scalar(255,0,0), 3, 1 );
    
    cv::line(m_image, cv::Point(m_roi[0],0), cv::Point(m_roi[0],m_screenSize[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(m_roi[0]+m_roi[2],0), cv::Point(m_roi[0]+m_roi[2],m_screenSize[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_roi[1]), cv::Point(m_screenSize[0],m_roi[1]), cv::Scalar(255,255,0), 3, 1 );
    cv::line(m_image, cv::Point(0,m_roi[1]+m_roi[3]), cv::Point(m_screenSize[0],m_roi[1]+m_roi[3]), cv::Scalar(255,255,0), 3, 1 ); 
  }
  // std::cout<< "Memory length: " << m_visualMmeory.size() << " Type: " << m_image.type()<< "," << m_image.depth() << "," << m_image.channels() << std::endl;

   //cv::Mat intenImg;
   //cv::Mat color_dst;

  //medianBlur(src,src,3);
  //cv::inRange(m_image, cv::Scalar(m_intensityThreshold, m_intensityThreshold, m_intensityThreshold), cv::Scalar(255, 255, 255), intenImg);

  // Make output image into a 3 channel BGR image
  //cv::cvtColor(m_image, color_dst, CV_GRAY2BGR);

  /*
  cv::vector<cv::Vec4i> detectedLines;
  HoughLinesP(cannyImg, detectedLines, 1, opendlv::Constants::PI/180, 50, 50, 10 );
  for( size_t i = 0; i < detectedLines.size(); i++ )
  {
    Vec4i l = detectedLines[i];
    line( cannyImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, cv::CV_AA);
  }
  */
  
  // Victors implementation
  
  cv::Mat intenImg;
  cv::Mat intenImgRes;
  cv::medianBlur(intenImg,intenImg,3);
  cv::inRange(tmpImg, cv::Scalar(m_intensityThreshold, m_intensityThreshold, m_intensityThreshold), cv::Scalar(255, 255, 255), intenImg);
  //cv::cvtColor(m_image, intenImg, CV_GRAY2BGR);
  
  cv::adaptiveThreshold(intenImg,intenImgRes,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV,11,2);
  

  // Vector holder for each line (rho,theta)
  cv::vector<cv::Vec2f> detectedLines;

  // temporary image to keep track of all filtered lines
  cv::Mat tmp_img = m_image.clone();

  // OpenCV function that uses the Hough transform and finds the "strongest" lines in the transformation    
  //cv::HoughLines(cannyImg, detectedLines, 1, opendlv::Constants::PI/180, m_houghThreshold);
  cv::HoughLines(intenImg, detectedLines, 1, opendlv::Constants::PI/180, m_houghThreshold);
  if(detectedLines.size() < 3){
    if(m_debug) {
    cv::resize(cannyImg, display[0], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE", display[0]);
    cv::resize(m_image, display[1], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE2", display[1]);
    cv::resize(intenImgRes, display[4], cv::Size(windowWidth*2, windowHeight*2), 0, 0, cv::INTER_CUBIC);
    cv::imshow("intensityThreshold", display[4]);
    cv::waitKey(1);

    }
    return;
  }
  std::cout << "detectedLines: "<< detectedLines.size() << std::endl;

    // Print out all detected lines
    if(m_debug) {
    for( size_t i = 0; i < detectedLines.size(); i++ ) {
      float rho = detectedLines[i][0];
      float theta = detectedLines[i][1];

    //std::cout << "rho: "<< rho << std::endl;
      //std::cout << "theta: "<< theta << std::endl;

      float a = cos(theta), b = sin(theta);
      float x0 = a*rho, y0 = b*rho;
      cv::Point pt1(m_roi[0] + x0 + 2000*(-b), m_roi[1] + y0 + 2000*(a));
      cv::Point pt2(m_roi[0] + x0 - 2000*(-b), m_roi[1] + y0 - 2000*(a));

      cv::line(m_image, pt1, pt2, cv::Scalar(0,0,255), 1, 1 );
      }
    }

  // Filtering stuff out
  // std::cout << "Unfiltered: "<< detectedLines.size() << std::endl;
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
  detectedLines = groups; 
  std::cout << "Filtered: "<< detectedLines.size() << std::endl;

  // Print out all detected lines
    if(m_debug) {
    for( size_t i = 0; i < detectedLines.size(); i++ ) {
      float rho = detectedLines[i][0];
      float theta = detectedLines[i][1];

    //std::cout << "rho: "<< rho << std::endl;
      //std::cout << "theta: "<< theta << std::endl;

      float a = cos(theta), b = sin(theta);
      float x0 = a*rho, y0 = b*rho;
      cv::Point pt1(m_roi[0] + x0 + 2000*(-b), m_roi[1] + y0 + 2000*(a));
      cv::Point pt2(m_roi[0] + x0 - 2000*(-b), m_roi[1] + y0 - 2000*(a));

      cv::line(tmp_img, pt1, pt2, cv::Scalar(0,0,255), 1, 1 );
      }

      cv::resize(tmp_img, display[3], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
      cv::INTER_CUBIC);
      cv::imshow("FOE4", display[3]); 
    }


  //  FOR INTENSITY THRESHOLD
  // cv::HoughLines(intenImg, detectedLines, 1, 3.14f/180, m_houghThreshold );
  // // std::cout << "Unfiltered: "<< detectedLines.size() << std::endl;
  // test.clear();
  // for(uint16_t i = 0; i != detectedLines.size(); i++){
  //   if(angleTresh > detectedLines[i][1] || 3.14f-angleTresh < detectedLines[i][1]){
  //     test.push_back(detectedLines[i]);
  //   }
  // }
  // detectedLines = test;
  // // std::cout << "Filtered: "<< detectedLines.size() << std::endl;


  // for( size_t i = 0; i < detectedLines.size(); i++ ) {
  //   float rho = detectedLines[i][0];
  //   float theta = detectedLines[i][1];
  //   float a = cos(theta), b = sin(theta);
  //   float x0 = a*rho, y0 = b*rho;
  //   cv::Point pt1(cvRound(x0 + 2000*(-b)),
  //              cvRound(y0 + 2000*(a)));
  //   cv::Point pt2(cvRound(x0 - 2000*(-b)),
  //              cvRound(y0 - 2000*(a)));

  //   cv::line(intenImg, pt1, pt2, cv::Scalar(255,255,255), 1, 1 );
  // }


  // // Input Quadilateral or Image plane coordinates
  // cv::Point2f inputQuad[4] = {cv::Point2f(350,470),cv::Point2f(750,470),cv::Point2f(2180,720),cv::Point2f(-700,720)};
  // // Output Quadilateral or World plane coordinates
  // cv::Point2f outputQuad[4] = {cv::Point2f(0,0),cv::Point2f(1080,0),cv::Point2f(1080,720),cv::Point2f(0,720)}; 
  // // std::cout << inputQuad << outputQuad << std::endl;
  // cv::Mat lambda = cv::getPerspectiveTransform(inputQuad, outputQuad);
  // cv::Mat warpedImg = m_image.clone();
  // cv::warpPerspective(m_image, warpedImg, lambda, warpedImg.size());
  // cv::Point polypoint[4];
  // for(int i=0;i<4;i++){
  //     polypoint[i]= inputQuad[i];
  // }
  // const cv::Point* countours[1]={polypoint};
  // int v = 4;
  // const int* constCont = &v; 
  // cv::polylines(m_image, countours, constCont, 4, 1, cv::Scalar(0,255,0), 4, 8, 0);


  // Get parametric line representation
  std::vector<cv::Vec2f> p, m;

  GetParametricRepresentation(p, m, detectedLines);

  // Get points on lines
  std::vector<cv::Vec2f> xScreenP,yScreenP;
  std::vector<cv::Vec2f> xWorldP, yWorldP;
  GetPointsOnLine(xScreenP, yScreenP, xWorldP, yWorldP, p, m);

  /* DETECT LANE STARTS HERE  */

  // Pair up lines to form a surface
  std::vector<int8_t> groupIds;
  GetLinePairs(xScreenP, groupIds);
  if(groupIds.size() == 0){
    return;
  }

  std::cout << "Group size: "<< groupIds.size() << std::endl;


  if(m_debug) {
    for(uint8_t i = 0; i < groupIds.size(); i++) {
      for(uint8_t k = 0; k < 2; k++) {
        cv::circle(m_image, cv::Point(xScreenP[groupIds[i]][k],yScreenP[groupIds[i]][k]), 10, cv::Scalar(0,255,0), 3, 8);
        std::cout << xWorldP[groupIds[i]][k] << "x" << yWorldP[groupIds[i]][k] << ",";
      }
      std::cout << std::endl;
    }
    cv::resize(cannyImg, display[0], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE", display[0]);
    cv::resize(m_image, display[1], cv::Size(windowWidth*2, windowHeight*2), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE2", display[1]);
    cv::waitKey(1);
  }


  odcore::data::TimeStamp imageTimeStamp = a_c.getSentTimeStamp();
  std::string type = "surface";
  float typeConfidence = 1;

  std::vector<opendlv::model::Cartesian3> edges;

  //topleft
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[0]][0],yWorldP[groupIds[0]][0],0));
  //bottomleft
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[0]][1],yWorldP[groupIds[0]][1],0));
  //bottomright
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[1]][1],yWorldP[groupIds[1]][1],0));
  //topright
  edges.push_back(opendlv::model::Cartesian3(xWorldP[groupIds[1]][0],yWorldP[groupIds[1]][0],0));
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

  std::cout<<"Detected frame: " << m_counter << std::endl;
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
