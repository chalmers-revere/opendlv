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


#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "vision/opticalflow/opticalflow.hpp"

namespace opendlv {
namespace sensation {
namespace vision {
namespace opticalflow {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
OpticalFlow::OpticalFlow(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-vision-opticalflow"),
      m_termcrit(),
      m_searchSize(),
      m_maxLevel(),
      m_minEigThreshold(),
      m_nAxisPoints(),
      m_grayImage(),
      m_prevGrayImage(),
      m_image(),
      m_frame(),
      m_staticImagePoints(),
      m_endImagePoints()
{
}

OpticalFlow::~OpticalFlow()
{
}

/**
 * Receives raw images from cameras.
 * Sends an RGB matrix of optical flow.
 */
void OpticalFlow::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() != odcore::data::image::SharedImage::ID()){
    return;
  }
  odcore::data::image::SharedImage mySharedImg =
      a_c.getData<odcore::data::image::SharedImage>();

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
      odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
          mySharedImg.getName()));
  const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
  const uint32_t imgWidth = mySharedImg.getWidth();
  const uint32_t imgHeight = mySharedImg.getHeight();

  IplImage* myIplImage = cvCreateImage(cvSize(imgWidth,imgHeight), IPL_DEPTH_8U,
      nrChannels);
  m_image = cv::Mat(myIplImage);

  if(!sharedMem->isValid()){
    return;
  }

  sharedMem->lock();
  {
    memcpy(m_image.data, sharedMem->getSharedMemory(),
        imgWidth*imgHeight*nrChannels);
  }
  sharedMem->unlock();

  double x,y;
  for(uint32_t i = 1; i <= m_nAxisPoints; ++i){
      for(uint32_t j = 1; j <= m_nAxisPoints; ++j){
          x = imgWidth*j/(1+m_nAxisPoints);
          y = imgHeight*i/(1+m_nAxisPoints);
          m_staticImagePoints.push_back(cv::Point2f(x,y));
          // constPoints->row(k) << x,y;
      }
  }

  //Converts the image to grayscale and output in "gray"
  cv::cvtColor(m_image, m_grayImage, cv::COLOR_BGR2GRAY);
  if(m_prevGrayImage.empty()){
    m_grayImage.copyTo(m_prevGrayImage);
  }


  std::vector<uchar> status;
  std::vector<float> err;

  cv::calcOpticalFlowPyrLK(m_prevGrayImage, m_grayImage, m_staticImagePoints,
      m_endImagePoints, status, err, m_searchSize, m_maxLevel, m_termcrit,
      0, m_minEigThreshold);

  for(uint32_t i = 0; i < m_staticImagePoints.size(); i++){
    cv::circle(m_image, m_staticImagePoints[i], 3, cv::Scalar(255,0,0), -1, 8);            
  }
  for(uint32_t i = 0; i < m_endImagePoints.size(); i++){
    cv::circle(m_image, m_endImagePoints[i], 3, cv::Scalar(0,0,255), -1, 8);            
  }

  const int32_t windowWidth = 640;
  const int32_t windowHeight = 480;
  cv::resize(m_image, m_image, cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);

  cv::imshow("LK Demo", m_image); 
  cv::waitKey(10);

  //TODO Send the information

}

void OpticalFlow::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  uint32_t termCritMaxCount = kv.getValue<uint32_t>(
      "sensation-vision-opticalflow.termCritMaxCount");
  double termCritEpsilon = kv.getValue<double>(
      "sensation-vision-opticalflow.termCritEpsilon");
  m_termcrit = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,
        termCritMaxCount,termCritEpsilon);
  uint32_t lkSearchWindowWidth = kv.getValue<uint32_t>(
      "sensation-vision-opticalflow.lkSearchWindowWidth");
  m_searchSize = cv::Size(lkSearchWindowWidth,lkSearchWindowWidth);
  m_maxLevel = kv.getValue<uint32_t>(
      "sensation-vision-opticalflow.maxLevel");
  m_minEigThreshold = kv.getValue<double>(
      "sensation-vision-opticalflow.minEigThreshold");
  m_nAxisPoints = kv.getValue<uint32_t>(
      "sensation-vision-opticalflow.nAxisPoints");

  cv::namedWindow( "LK Demo", 1 );
}

void OpticalFlow::tearDown()
{
}

} // opticalflow
} // vision
} // sensation
} // opendlv
