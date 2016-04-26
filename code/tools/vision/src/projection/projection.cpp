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

#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>

#include <opendavinci/odcore/data/Container.h>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "projection/projection.hpp"



namespace opendlv {
namespace tools {
namespace vision {
namespace projection {

void LogMouseClicks(int32_t a_event, int32_t a_x, int32_t a_y, int32_t, void* a_userdata)
{  

  opendlv::tools::vision::projection::MouseParams* click = 
      (opendlv::tools::vision::projection::MouseParams*) a_userdata;
  if (a_event == cv::EVENT_LBUTTONDOWN && click->iterator < 4)
  {

    std::cout << "(" << a_x << ", " << a_y << ")" << std::endl;
    click->points(0,click->iterator) = a_x;
    click->points(1,click->iterator) = a_y;
    click->iterator = click->iterator +1;
    std::cout << "Points: " + std::to_string(click->iterator) << std::endl;
    std::cout << (click->points) << std::endl;
  }
  if (a_event == cv::EVENT_RBUTTONDOWN)
  {
    click->iterator = 0;
  }
}
void ProjectMouseClicks(int32_t a_event, int32_t a_x, int32_t a_y, int32_t, void* a_userdata)
{
  Eigen::Vector3d v;
  Eigen::MatrixXd* m = (Eigen::MatrixXd*) a_userdata;
  
  if(a_event == cv::EVENT_LBUTTONDOWN){
    v << a_x,a_y,1;
    std::cout << *m << std::endl;
    std::cout << v << std::endl;
    v = *m*v;

    std::cout << v << std::endl;
    // v = v * 1/v(3);

    // std::cout << v << std::endl;

  } 
}

MouseParams::MouseParams() : 
    points(),
    iterator() 
{
  points = Eigen::MatrixXd(2,4);
}
MouseParams::~MouseParams()
{
}


Projection::Projection(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-vision-projection"),
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
    const uint32_t nrChannels = 3;
    const uint32_t imgWidth = mySharedImg.getWidth();
    const uint32_t imgHeight = mySharedImg.getHeight();

    // std::cout << imgWidth << "    "<< imgHeight << std::endl;

    IplImage* myIplImage;
    
    myIplImage = cvCreateImage(cvSize(imgWidth,imgHeight), IPL_DEPTH_8U,
        nrChannels);
    cv::Mat feed(myIplImage);

    if(!sharedMem->isValid()){
      return;
    }

    sharedMem->lock();
    {
      memcpy(feed.data, sharedMem->getSharedMemory(),
          imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
    // const int32_t windowWidth = 640;
    // const int32_t windowHeight = 480;
    // cv::Mat display;
    // cv::resize(m_feed, display, cv::Size(windowWidth, windowHeight), 0, 0,
    //   cv::INTER_CUBIC);

    putText(feed, "Rectangle width: " + std::to_string(m_recWidth), cvPoint(30,30), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);
    putText(feed, "Rectangle height: " + std::to_string(m_recHeight), cvPoint(30,40), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);
    putText(feed, "Position (x,y): (" + std::to_string(m_recPosX) + ","  + std::to_string(m_recPosY) + ")" , cvPoint(30,50), 
    1, 0.8, cvScalar(0,0,254), 1, CV_AA);

    cv::imshow("Calibration", feed);

    m_option = (char) cv::waitKey(1);

    cvReleaseImage(&myIplImage);
    return;
  }
}
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Projection::body(){
  Config();
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING){
    switch(m_option){
      case 'c':
        std::cout<<"Enter Calibration" << std::endl;
        Calibrate();
        break;
      case'r':
        std::cout<<"Enter Configuration" << std::endl;
        Config();
        break;
      case's':
        std::cout<<"Calculating projection matrix and saving to file" << std::endl;
        Save();
        break;
      case 'p':
        std::cout<< "Projecting points" << std::endl;
        Project();
      default:
        break;
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Projection::Config()
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

void Projection::Calibrate()
{

  MouseParams mouseClick;
  mouseClick.iterator = 0;
  cv::setMouseCallback("Calibration", LogMouseClicks, (void *) &mouseClick);
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
void Projection::Save()
{
  m_projectionMatrix =  m_aMatrix * m_bMatrix.inverse();
  std::cout << m_projectionMatrix << std::endl;

  struct stat st;
  if (stat("var/tools/vision/projection", &st) == -1) {
    system("mkdir -p ./var/tools/vision/projection");
    // std::cout<<"Created dir"<<std::endl;
  }

  std::string name;
  std::cout<< "filename";
  std::cin >> name;

  std::ofstream file("var/tools/vision/projection/"+name+".csv");
  if(file.is_open()){
    file << m_projectionMatrix;
  }
  file.close();
}


void Projection::Project()
{
  cv::setMouseCallback("Calibration", ProjectMouseClicks, (void *) &m_projectionMatrix);
  cv::waitKey(0);
  cv::setMouseCallback("Calibration", NULL, NULL);
  std::cout<< "Exit point projection" << std::endl;
}

} // projection
} // vision
} // tools
} // opendlv
