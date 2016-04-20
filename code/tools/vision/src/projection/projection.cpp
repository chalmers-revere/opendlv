/**
 * Copyright (C) 2016 Chalmers REVERE
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

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include <opendavinci/odcore/data/Container.h>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "projection/projection.hpp"

struct MouseParams
{
  MouseParams()
    : points(),
    iterator() 
  {
    points = Eigen::MatrixXd(2,4);
  }
  ~MouseParams() {}

  Eigen::MatrixXd points;
  uint8_t iterator;
};

void LoggMouseClicks(int32_t event, int32_t x, int32_t y, int32_t, void* userdata)
{  

  MouseParams* click = (MouseParams*)userdata;
  if  ( event == cv::EVENT_LBUTTONDOWN && click->iterator < 4)
  {

    std::cout << "(" << x << ", " << y << ")" << std::endl;
    click->points(0,click->iterator) = x;
    click->points(1,click->iterator) = y;
    click->iterator = click->iterator +1;
    std::cout << "Points: " + std::to_string(click->iterator) << std::endl;
    std::cout << (click->points) << std::endl;
  }
  if  ( event == cv::EVENT_RBUTTONDOWN)
  {
    click->iterator = 0;
  }
}
void projectMouseClicks(int32_t event, int32_t x, int32_t y, int32_t, void* userdata)
{
  Eigen::Vector3d v;
  Eigen::MatrixXd* m = (Eigen::MatrixXd*) userdata;
  
  if(event == cv::EVENT_LBUTTONDOWN){
    v << x,y,1;
    std::cout << *m << std::endl;
    std::cout << v << std::endl;
    v = *m*v;

    std::cout << v << std::endl;
    // v = v * 1/v(3);

    // std::cout << v << std::endl;

  } 
}

namespace opendlv {
namespace tools {
namespace vision {
namespace projection {

Projection::Projection(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-vision-projection"),
      m_feed(),
      m_option(),
      m_recHeight(),
      m_recWidth(),
      m_recPosX(),
      m_recPosY(),
      m_aMatrix(),
      m_bMatrix(),
      m_projectionMatrix()
{
  m_aMatrix = Eigen::MatrixXd(3,3);
  m_bMatrix = Eigen::MatrixXd(3,3);
  m_projectionMatrix = Eigen::MatrixXd(3,3);
}

Projection::~Projection()
{
}
void Projection::setUp()
{
  cv::namedWindow("Calibration", 1 );
}

void Projection::tearDown()
{
}


void Projection::nextContainer(odcore::data::Container &a_c)
{
  if(a_c.getDataType() == odcore::data::image::SharedImage::ID()){
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
    m_feed = cv::Mat(myIplImage);

    if(!sharedMem->isValid()){
      return;
    }

    sharedMem->lock();
    {
      memcpy(m_feed.data, sharedMem->getSharedMemory(),
          imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
    const int32_t windowWidth = 640;
    const int32_t windowHeight = 480;
    cv::Mat display;
    cv::resize(m_feed, display, cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);

    putText(display, "Rectangle width: " + std::to_string(m_recWidth), cvPoint(30,30), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);
    putText(display, "Rectangle height: " + std::to_string(m_recHeight), cvPoint(30,40), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);
    putText(display, "Position (x,y): (" + std::to_string(m_recPosX) + ","  + std::to_string(m_recPosY) + ")" , cvPoint(30,50), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);

    cv::imshow("Calibration", display);

    m_option = (char) cv::waitKey(1);
    return;
  }
}
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Projection::body(){
  config();
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING){
    switch(m_option){
      case 'c':
        std::cout<<"Enter Calibration" << std::endl;
        calibrate();
        break;
      case'r':
        std::cout<<"Enter Configuration" << std::endl;
        config();
        break;
      case's':
        std::cout<<"Calculating projection matrix and saving to file" << std::endl;
        save();
        break;
      case 'p':
        std::cout<< "Projecting points" << std::endl;
        project();
      default:
        break;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Projection::config()
{
  std::cout<< "rectangle width: ";
  std::cin >> m_recWidth;
  std::cout<< "rectangle height: ";
  std::cin >> m_recHeight;
  std::cout<< "x position: ";
  std::cin >> m_recPosX;
  std::cout<< "y position: ";
  std::cin >> m_recPosY;
  Eigen::MatrixXd q(3,3), w(3,1);
  q <<  m_recPosX, m_recPosX, m_recPosX+m_recWidth,
        m_recPosY + m_recHeight, m_recPosY, m_recPosY,
        1,1,1;
  std::cout<<q<<std::endl;
  w <<  m_recPosX+m_recWidth,
        m_recPosY + m_recHeight,
        1;
  std::cout<<w<<std::endl;

  Eigen::Vector3d scale = q.colPivHouseholderQr().solve(w);
  std::cout<<scale<<std::endl;

  m_aMatrix << scale(0)*q.col(0) ,scale(1)*q.col(1),scale(2)*q.col(2);
  std::cout<< m_aMatrix << std::endl;

  std::cout<<"Configuration done." << std::endl;

}

void Projection::calibrate()
{

  MouseParams mouseClick;
  mouseClick.iterator = 0;
  cv::setMouseCallback("Calibration", LoggMouseClicks, (void *) &mouseClick);
  cv::waitKey(0);
  cv::setMouseCallback("Calibration", NULL, NULL);
  if(mouseClick.iterator > 3){
    Eigen::MatrixXd q(3,3), w(3,1);
    q << mouseClick.points(0,0),mouseClick.points(0,1),mouseClick.points(0,2),
        mouseClick.points(1,0),mouseClick.points(1,1),mouseClick.points(1,2),
        1,1,1;
    w << mouseClick.points(0,3), mouseClick.points(1,3), 1;
    std::cout<<q<< std::endl;
    std::cout<<w<< std::endl;

    Eigen::Vector3d scale = q.colPivHouseholderQr().solve(w);
    std::cout<<scale<<std::endl;

    m_bMatrix << scale(0)*q.col(0),scale(1)*q.col(1),scale(2)*q.col(2);
    std::cout<< m_bMatrix << std::endl;

    std::cout << "Calibration done." << std::endl;
  }
  else{
    std::cout << "Calibration cancelled." << std::endl;
  }
}
void Projection::save()
{
  m_projectionMatrix =  m_aMatrix * m_bMatrix.inverse();
  std::cout << m_projectionMatrix << std::endl;
}


void Projection::project()
{
  cv::setMouseCallback("Calibration", projectMouseClicks, (void *) &m_projectionMatrix);
  cv::waitKey(0);
  cv::setMouseCallback("Calibration", NULL, NULL);
  std::cout<< "Exit point projection" << std::endl;
}

} // projection
} // vision
} // tools
} // opendlv
