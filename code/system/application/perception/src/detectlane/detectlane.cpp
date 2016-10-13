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
    m_initialized(false),
    m_image(),
    m_visualMemory(),
    m_intensityThreshold(),
    m_cannyThreshold(),
    m_houghThreshold()
{
}

DetectLane::~DetectLane()
{
  m_image.release();
}

/**
 * Receives images from vision source and detect lanes.
 * Sends drivalble lanes objects.
 */
void DetectLane::nextContainer(odcore::data::Container &a_c)
{
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

  cv::Rect myROI(0, 450, 1080, 270);
  // cv::Rect myROI(0, 450, 1, 1);
  m_image = m_image(myROI);

  const int32_t windowWidth = 640;
  const int32_t windowHeight = 240;
  cv::Mat display[3];

  cv::Mat visualImpression = m_image.clone();
  m_visualMemory.push_back(std::make_pair(now,visualImpression));
  
  double memThreshold = 1.0;
  while(m_visualMemory.size() > 0 && (now-m_visualMemory[0].first).toMicroseconds()/1000000.0 > memThreshold){
    m_visualMemory.pop_front();
  }
  cv::Mat exposedImage = m_visualMemory[0].second.clone();
  if(m_visualMemory.size() > 1) {
    cv::Mat tmp;
    // cv::Mat exposedImage;// =  m_visualMemory[0].second.clone();
    double alpha;
    double beta;
    for(uint16_t i = 1; i < m_visualMemory.size(); i++){
      // exposedImage +=  m_visualMemory[i].second;
      // cv::AddWeighted
      alpha = i/(i+1.0);
      // std::cout << alpha << std::endl;
      beta = 1-alpha;
      cv::addWeighted(exposedImage, alpha, m_visualMemory[i].second, beta, 0, tmp, -1);
      exposedImage = tmp;
    }
    // exposedImage = exposedImage/m_visualMemory.size();
    // std::cout <<"o" << m_visualMemory[0].second << std::endl;
    

    // std::cout << m_visualMemory[0].second << std::endl;
    // std::cout << m_visualMemory[1].second << std::endl;
    // std::cout << exposedImage << std::endl;
    cv::resize(exposedImage, display[2], cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE3", display[2]);
  }
  cv::Mat cannyImg;
  cv::Canny(exposedImage, cannyImg, m_cannyThreshold, m_cannyThreshold*3, 3);

  // std::cout<< "Memory length: " << m_visualMmeory.size() << " Type: " << m_image.type()<< "," << m_image.depth() << "," << m_image.channels() << std::endl;



  cv::Mat intenImg;
  cv::Mat color_dst;


  //medianBlur(src,src,3);
  // cv::inRange(m_image, cv::Scalar(m_intensityThreshold, m_intensityThreshold, m_intensityThreshold), cv::Scalar(255, 255, 255), intenImg);

  // Make output image into a 3 channel BGR image
  // cv::cvtColor(m_image, color_dst, CV_GRAY2BGR);

  // Vector holder for each line (rho,theta)
  cv::vector<cv::Vec2f> detectedLines;

  // OpenCV function that uses the Hough transform and finds the "strongest" lines in the transformation    
  cv::HoughLines(cannyImg, detectedLines, 1, 3.14f/180, m_houghThreshold );
  // std::cout << "Unfiltered: "<< detectedLines.size() << std::endl;
  std::vector<cv::Vec2f> tmpVec;
  float angleTresh = 85 * 3.14f/180;
  for(uint16_t i = 0; i != detectedLines.size(); i++){
    if(angleTresh > detectedLines[i][1] || 3.14f-angleTresh < detectedLines[i][1]){
      tmpVec.push_back(detectedLines[i]);
    }
  }
  detectedLines = tmpVec;
  // std::cout << "Filtered: "<< detectedLines.size() << std::endl;


  for( size_t i = 0; i < detectedLines.size(); i++ ) {
    float rho = detectedLines[i][0];
    float theta = detectedLines[i][1];
    float a = cos(theta), b = sin(theta);
    float x0 = a*rho, y0 = b*rho;
    cv::Point pt1(cvRound(x0 + 2000*(-b)),
               cvRound(y0 + 2000*(a)));
    cv::Point pt2(cvRound(x0 - 2000*(-b)),
               cvRound(y0 - 2000*(a)));

    cv::line(m_image, pt1, pt2, cv::Scalar(0,0,255), 1, 1 );
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

  cv::resize(cannyImg, display[0], cv::Size(windowWidth, windowHeight), 0, 0,
    cv::INTER_CUBIC);
  cv::imshow("FOE", display[0]);
  cv::resize(m_image, display[1], cv::Size(windowWidth, windowHeight), 0, 0,
    cv::INTER_CUBIC);
  cv::imshow("FOE2", display[1]);

  cv::waitKey(1);
  // warpedImg.release();

}


void DetectLane::setUp()
{

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_intensityThreshold = kv.getValue<uint16_t>("perception-detectlane.intensityThreshold");
  m_cannyThreshold = kv.getValue<uint16_t>("perception-detectlane.cannyThreshold");
  m_cannyThreshold = 25;
  m_houghThreshold = kv.getValue<uint16_t>("perception-detectlane.houghThreshold");
  m_houghThreshold = 130;
  m_initialized = true;

}

void DetectLane::tearDown()
{
}


} // detectlane
} // perception
} // opendlv
