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

// #include "detectlane/Drawing.h"
// #include "detectlane/DecisionMaking.h"
// #include "detectlane/ExtractingRegions.h"
// #include "detectlane/ProcessImage.h"

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
    m_initialized(false),
    m_image(),
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
 * Receives .
 * Sends .
 */
void DetectLane::nextContainer(odcore::data::Container &a_c)
{
	if (!m_initialized || a_c.getDataType() != odcore::data::image::SharedImage::ID()) {
    return;
  }

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

  cv::Mat cannyImg;
  cv::Mat intenImg;
  cv::Mat color_dst;

  cv::Canny(m_image, cannyImg, m_cannyThreshold, m_cannyThreshold*3, 3);
  //medianBlur(src,src,3);
  cv::inRange(m_image, cv::Scalar(m_intensityThreshold, m_intensityThreshold, m_intensityThreshold), cv::Scalar(255, 255, 255), intenImg);

  // Make output image into a 3 channel BGR image
  // cv::cvtColor(m_image, color_dst, CV_GRAY2BGR);

  // Vector holder for each line (rho,theta)
  cv::vector<cv::Vec2f> detectedLines;

  // OpenCV function that uses the Hough transform and finds the "strongest" lines in the transformation    
  cv::HoughLines(cannyImg, detectedLines, 1, 3.14f/180, m_houghThreshold );
  // std::cout << "Unfiltered: "<< detectedLines.size() << std::endl;
  std::vector<cv::Vec2f> test;
  float angleTresh = 60 * 3.14f/180;
  for(uint16_t i = 0; i != detectedLines.size(); i++){
    if(angleTresh > detectedLines[i][1] || 3.14f-angleTresh < detectedLines[i][1]){
      test.push_back(detectedLines[i]);
    }
  }
  detectedLines = test;
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

    line(m_image, pt1, pt2, cv::Scalar(0,0,255), 1, 1 );
  }
  const int32_t windowWidth = 640;
  const int32_t windowHeight = 480;
  cv::Mat display;
  cv::Mat displa2;
  cv::resize(cannyImg, display, cv::Size(windowWidth, windowHeight), 0, 0,
    cv::INTER_CUBIC);
  cv::imshow("FOE", display);
  cv::resize(m_image, displa2, cv::Size(windowWidth, windowHeight), 0, 0,
    cv::INTER_CUBIC);
  cv::imshow("FOE2", displa2);
  cv::waitKey(1);

}


void DetectLane::setUp()
{

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_intensityThreshold = kv.getValue<uint16_t>("perception-detectlane.intensityThreshold");
  m_cannyThreshold = kv.getValue<uint16_t>("perception-detectlane.cannyThreshold");
  m_houghThreshold = kv.getValue<uint16_t>("perception-detectlane.houghThreshold");
  m_initialized = true;

}

void DetectLane::tearDown()
{
}


} // detectlane
} // perception
} // opendlv
