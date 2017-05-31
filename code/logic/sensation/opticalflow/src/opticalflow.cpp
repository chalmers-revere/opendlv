/**
 * Copyright (C) 2017 Chalmers Revere
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
#include "opendavinci/odcore/base/Lock.h"

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opticalflow.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
OpticalFlow::OpticalFlow(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "logic-sensation-opticalflow"),
      m_name(),
      m_size(),
      m_termcrit(),
      m_searchSize(),
      m_maxLevel(),
      m_minEigThreshold(),
      m_nAxisPoints(),
      m_grayImage(),
      m_prevGrayImage(),
      m_image(),
      m_flow(),
      m_staticImagePoints(),
      m_endImagePoints(),
      m_outputSharedImage(),
      m_sharedMemory()
{
  cv::namedWindow( "Optical Flow", 1 );
}

OpticalFlow::~OpticalFlow()
{
  m_image.release();
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
  // std::cout<<mySharedImg.getName()<<std::endl;

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



  //Converts the image to grayscale and output in "gray"
  cv::cvtColor(m_image, m_grayImage, cv::COLOR_BGR2GRAY);

  //First initialisation of the image
  if(m_prevGrayImage.empty()){
    m_grayImage.copyTo(m_prevGrayImage);
    double x,y;
    for(uint32_t i = 1; i <= m_nAxisPoints; ++i){
        for(uint32_t j = 1; j <= m_nAxisPoints; ++j){
            x = imgWidth*j/(1+m_nAxisPoints);
            y = imgHeight*i/(1+m_nAxisPoints);
            m_staticImagePoints.push_back(cv::Point2f(x,y));
            // constPoints->row(k) << x,y;
        }
    }
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
  SendContainer();
  // updateFlow();
  // std::cout<< m_flow << std::endl;

  // if(m_sharedMemory.get() && m_sharedMemory->isValid()){
  //   {
  //     std::cout<< m_size << ", " << m_flow.total() * m_flow.elemSize() << std::endl;
  //     odcore::base::Lock l(m_sharedMemory);
  //     memcpy(static_cast<char *>(m_sharedMemory->getSharedMemory()),m_flow.data,
  //         m_size);
  //   }
  //   odcore::data::Container c(m_outputSharedImage);
  //   getConference().send(c);
  //   // std::cout << "Sent" << std::endl;
  // 
  // }

  // std::cout<< m_outputSharedImage.getName() << std::endl;


  cv::swap(m_prevGrayImage, m_grayImage);
  
  const int32_t windowWidth = 640;
  const int32_t windowHeight = 480;
  cv::Mat display1;
  cv::resize(m_image, display1, cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);
  cv::imshow("Optical Flow", display1); 
  cv::waitKey(1);

  cvReleaseImage(&myIplImage);
}

void OpticalFlow::SendContainer()
{
  uint16_t numberOfPoints = m_staticImagePoints.size();
  std::vector<opendlv::model::Direction> directions;
  std::vector<float> u;
  std::vector<float> v;
  for(uint8_t i = 0; i < m_staticImagePoints.size(); i++){
    // std::cout<< m_staticImagePoints[i].x << std::endl;
    float x = m_staticImagePoints[i].x;
    float y = m_staticImagePoints[i].y;
    opendlv::model::Direction direction(x, y);

    directions.push_back(direction);
    u.push_back(m_endImagePoints[i].x - x);
    v.push_back(m_endImagePoints[i].y - y);
  }
  opendlv::sensation::OpticalFlow nextMessage(numberOfPoints, directions, u, v);
  odcore::data::Container c(nextMessage);
  getConference().send(c);
}

// void OpticalFlow::updateFlow()
// {
//   if(m_staticImagePoints.size() != m_endImagePoints.size()){
//     std::cout<< "Number of points do not match" << std::endl;
//     return;
//   }
//   cv::Mat A(m_staticImagePoints), B(m_endImagePoints);
//   cv::Mat displacement = B-A;
//   // std::cout << displacement.at<float>(0,0)<< ", " << displacement.at<float>(0,1) << std::endl;
//   float blue, green, red;
//   red = 0;
//   for(uint32_t i = 0, k = 0; i < m_nAxisPoints; ++i){
//     for(uint32_t j = 0; j < m_nAxisPoints; ++j, ++k){
//       blue = (displacement.at<float>(k,0)+30)*255/60;
//       green = (displacement.at<float>(k,1)+30)*255/60;
//       cv::Vec3b color(blue,green,red);
//       m_flow.at<cv::Vec3b>(cv::Point(j,i)) = color;
//     }
//   }

// }

void OpticalFlow::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  uint32_t termCritMaxCount = kv.getValue<uint32_t>(
      "logic-sensation-vision-opticalflow.termCritMaxCount");
  double termCritEpsilon = kv.getValue<double>(
      "logic-sensation-vision-opticalflow.termCritEpsilon");
  m_termcrit = cv::TermCriteria(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,
        termCritMaxCount,termCritEpsilon);
  uint32_t lkSearchWindowWidth = kv.getValue<uint32_t>(
      "logic-sensation-vision-opticalflow.lkSearchWindowWidth");
  m_searchSize = cv::Size(lkSearchWindowWidth,lkSearchWindowWidth);
  m_maxLevel = kv.getValue<uint32_t>(
      "logic-sensation-vision-opticalflow.maxLevel");
  m_minEigThreshold = kv.getValue<double>(
      "logic-sensation-vision-opticalflow.minEigThreshold");
  m_nAxisPoints = kv.getValue<uint32_t>(
      "logic-sensation-vision-opticalflow.nAxisPoints");
  
  m_name = "opticalflow";
  m_size = (m_nAxisPoints)*(m_nAxisPoints)*3;
  
  m_sharedMemory =
      odcore::wrapper::SharedMemoryFactory::createSharedMemory(m_name, m_size);
  m_outputSharedImage.setName(m_name);
  m_outputSharedImage.setWidth(m_nAxisPoints);
  m_outputSharedImage.setHeight(m_nAxisPoints);
  m_outputSharedImage.setBytesPerPixel(3);
  m_outputSharedImage.setSize(m_size);

  m_flow = cv::Mat(m_nAxisPoints,m_nAxisPoints,CV_8UC3, cv::Scalar(0,0,0));
}

void OpticalFlow::tearDown()
{
}

}
}
}
