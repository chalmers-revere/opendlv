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

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "projection/projection.hpp"



namespace opendlv {
namespace tools {
namespace vision {
namespace projection {

void LogMouseClicks(int32_t a_event, int32_t a_x, int32_t a_y, int32_t,
    void* a_userdata)
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

void ProjectMouseClicks(int32_t a_event, int32_t a_x, int32_t a_y, int32_t,
    void* a_userdata)
{
  Eigen::Vector3d v;
  Eigen::MatrixXd* m = (Eigen::MatrixXd*) a_userdata;
  
  if(a_event == cv::EVENT_LBUTTONDOWN){
    v << a_x,a_y,1;
    // std::cout << *m << std::endl;
    // std::cout << v << std::endl;
    v = *m*v;
    v = v / v(2);


    std::cout << v << std::endl;

  } 
}

MouseParams::MouseParams() : 
    points(),
    iterator() 
{
  points = Eigen::MatrixXd(2,4);
  cv::namedWindow("Calibration", 1 );
}

MouseParams::~MouseParams()
{
}


Projection::Projection(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-vision-projection"),
      m_recHeight(),
      m_recWidth(),
      m_recPosX(),
      m_recPosY(),
      m_aMatrix(),
      m_bMatrix(),
      m_projectionMatrix(),
      m_applyWarp(false),
      m_point(),
      m_regionPoints(),
      m_outputPoints(),
      m_inputSize(),
      m_outputSize(),
      m_warpPointsFileName(),
      m_leftWarpPointsFileName(),
      m_rightWarpPointsFileName(),
      m_transformationMatrixFileName(),
      m_leftTransformationMatrixFileName(),
      m_rightTransformationMatrisFileName(),
      m_path(),
      m_initialized(false)
{
  m_aMatrix = Eigen::MatrixXd(3,3);
  m_bMatrix = Eigen::MatrixXd(3,3);
  m_projectionMatrix = Eigen::MatrixXd(3,3);

  m_leftWarpPointsFileName = "leftCameraWarpPoints.csv";
  m_rightWarpPointsFileName = "rightCameraWarpPoints.csv";

  m_leftTransformationMatrixFileName = "leftCameraTransformationMatrix";
  m_rightTransformationMatrisFileName = "rightCameraTransformationMatrix";
}

Projection::~Projection()
{
}

void Projection::setUp()
{
  cv::namedWindow("Calibration", 1 );

  uint32_t inputWidth = 1280;
  uint32_t inputHeight = 720;

  uint32_t outputWidth = inputWidth;
  uint32_t outputHeight = inputHeight;

  m_inputSize = cv::Size(inputWidth, inputHeight);
  m_outputSize = cv::Size(outputWidth, outputHeight);

  m_regionPoints.push_back(cv::Point2f(0, inputHeight));
  m_regionPoints.push_back(cv::Point2f(inputWidth, inputHeight));
  m_regionPoints.push_back(cv::Point2f(inputWidth, 0));
  m_regionPoints.push_back(cv::Point2f(0,0));

  m_outputPoints.push_back(cv::Point2f(0, outputHeight));
  m_outputPoints.push_back(cv::Point2f(outputWidth, outputHeight));
  m_outputPoints.push_back(cv::Point2f(outputWidth, 0));
  m_outputPoints.push_back(cv::Point2f(0, 0));
  m_initialized = true;
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
    
    myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U,
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

    if(m_initialized){
      if(mySharedImg.getName() == "front-left"){
        m_warpPointsFileName = m_leftWarpPointsFileName;
        m_transformationMatrixFileName = m_leftTransformationMatrixFileName;
      }
      else if(mySharedImg.getName() == "front-right"){
        m_warpPointsFileName = m_rightWarpPointsFileName;
        m_transformationMatrixFileName = m_rightTransformationMatrisFileName;
      }
      // cv::resize(feed,feed,m_inputSize);
       
      if(m_applyWarp){
        cv::Mat warped;
        cv::resize(feed,warped, m_inputSize);
        InversePerspectiveMapping ipm(m_inputSize, m_outputSize, m_regionPoints,
            m_outputPoints);
        ipm.ApplyHomography(warped, warped);
        ipm.DrawPoints(m_regionPoints, feed);
        cv::namedWindow("Warped");
        cv::imshow("Warped", warped);
      }
      // const int32_t windowWidth = 640;
      // const int32_t windowHeight = 480;
      // cv::Mat display;
      // cv::resize(m_feed, display, cv::Size(windowWidth, windowHeight), 0, 0,
      //   cv::INTER_CUBIC);

      putText(feed, "Rectangle width: " + std::to_string(m_recWidth),
          cvPoint(30,30), 1, 0.8, cvScalar(0,0,254), 1, CV_AA);
      putText(feed, "Rectangle height: " + std::to_string(m_recHeight),
          cvPoint(30,40), 1, 0.8, cvScalar(0,0,254), 1, CV_AA);
      putText(feed, "Position (x,y): (" + std::to_string(m_recPosX) + "," 
          + std::to_string(m_recPosY) + ")" , cvPoint(30,50), 1, 0.8,
          cvScalar(0,0,254), 1, CV_AA);

      cv::imshow("Calibration", feed);

      cvReleaseImage(&myIplImage);
    }
    return;
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Projection::body(){
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING){
      if(!m_applyWarp){
        char key = (char) cv::waitKey(1);
        switch(key){
          case 'c':
            std::cout << "Enter Calibration" << std::endl;
            Calibrate();
            break;
          case'r':
            std::cout << "Enter Configuration" << std::endl;
            Config();
            break;
          case 'e':
            std::cout << "Read file" << std::endl;
            ReadMatrix();
            break;
          case's':
            std::cout << "Calculating projection matrix and saving to file" 
                << std::endl;
            Save();
            break;
          case 'p':
            std::cout << "Projecting points" << std::endl;
            Project();
            break;
          case 'w':
            m_applyWarp = !m_applyWarp;
            if(m_applyWarp == true){
              std::cout<<"Warp drive" << std::endl;
            }
            break;
          case 'q':
          case 27:
            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
          default:
            break;
        }
      }
      else{
        char key = (char) cv::waitKey(1); //time interval for reading key input;
        switch(key){
          case '1':
            m_point = 0;
            break;
          case '2':
            m_point = 1;
            break;
          case '3':
            m_point = 2;
            break;
          case '4':
            m_point = 3;
            break;
          case 'i':
            m_regionPoints.at(m_point).y -= 5;
            break;
          case 'k':
            m_regionPoints.at(m_point).y += 5;
            break;
          case 'j':
            m_regionPoints.at(m_point).x -= 5;
            break;
          case 'l':
            m_regionPoints.at(m_point).x += 5;
            break;
          case 's':
            std::cout << 
                "Calculating projection matrix and saving to file in warp mode"
                << std::endl;
            Save();
            break;
          case 'c':
            std::cout << "Enter Calibration in warp mode" << std::endl;
            Calibrate();
            break;
          case 'p':
            std::cout << "Projecting points in warp mode" << std::endl;
            Project();
            break;
          case 'q':
          case 27:
            m_applyWarp = !m_applyWarp;
          default:
            break;
        }
      
    }
  }
  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

void Projection::SavePerspectivePoints(std::string a_path)
{
  Eigen::MatrixXd points(4,2);
  for(int i = 0; i < 4; i ++){
    points(i,0) = m_regionPoints.at(i).x;
    points(i,1) = m_regionPoints.at(i).y;
  }
   
  std::ofstream file(a_path + m_warpPointsFileName);

  if(file.is_open())
    file << points;
  file.close();
 
}

void Projection::ReadMatrix()
{
  Eigen::MatrixXd m(3,3);
  std::ifstream indata;
  std::string filename;
  std::cout << "Input filename: " << std::endl;
  std::cin >> filename;
  std::string filepath = "/opt/opendlv/var/tools/vision/projection/"
      + filename + ".csv";
  indata.open(filepath, std::ifstream::in);
  if(indata.is_open()){
    for(uint8_t i = 0; i < 3; ++i){
      for(uint8_t j = 0; j < 3; ++j){
        double item;
        indata >> item;
        // std::cout<<item<<", ";
        m(i,j) = item;
      }
    }
    std::cout<< m << std::endl;
  }
  else{
    std::cout<< "File not found." << std::endl;
  }
  indata.close();
}

void Projection::Config()
{
  std::cout << "rectangle width: ";
  std::cin >> m_recWidth;
  std::cout << "rectangle height: ";
  std::cin >> m_recHeight;
  std::cout << "x position: ";
  std::cin >> m_recPosX;
  std::cout << "y position: ";
  std::cin >> m_recPosY;
  Eigen::MatrixXd q(3,3), w(3,1);
  q <<  m_recPosX, m_recPosX, m_recPosX+m_recHeight,
        m_recPosY + m_recWidth, m_recPosY, m_recPosY,
        1,1,1;
  std::cout<<q<<std::endl;
  w <<  m_recPosX + m_recHeight,
        m_recPosY + m_recWidth,
        1;
  std::cout << w << std::endl;

  Eigen::Vector3d scale = q.colPivHouseholderQr().solve(w);
  std::cout<<scale<<std::endl;

  m_aMatrix << scale(0)*q.col(0) ,scale(1)*q.col(1), scale(2)*q.col(2);
  std::cout<< m_aMatrix << std::endl;

  std::cout<<"Configuration done." << std::endl;

}

void Projection::Calibrate()
{

  MouseParams mouseClick;
  mouseClick.iterator = 0;
  std::string mode;
  if(m_applyWarp){
    mode = "Warped";
  }
  else{
    mode = "Calibration";
  }
  cv::setMouseCallback(mode, LogMouseClicks, (void *) &mouseClick);
  cv::waitKey(0);
  cv::setMouseCallback(mode, NULL, NULL);
  if(mouseClick.iterator > 3){
    Eigen::MatrixXd q(3,3), w(3,1);
    q << mouseClick.points(0,0),mouseClick.points(0,1),mouseClick.points(0,2),
        mouseClick.points(1,0),mouseClick.points(1,1),mouseClick.points(1,2),
        1,1,1;
    w << mouseClick.points(0,3), mouseClick.points(1,3), 1;
    // std::cout<<q<< std::endl;
    // std::cout<<w<< std::endl;

    Eigen::Vector3d scale = q.colPivHouseholderQr().solve(w);
    // std::cout<<scale<<std::endl;

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
  std::cout << "Enter path to save transformation matrices: ";
  std::cin >> m_path;
  std::cout << "\nEntered path: " << m_path << std::endl;
  
  std::string inputPathString;
  std::cout << "Save transformation matrices?(yes/no): ";
  std::cin >> inputPathString;
  std::cout<<"\n";


  m_projectionMatrix =  m_aMatrix * m_bMatrix.inverse();

  std::string projectionMatrixFileName;
  if(m_applyWarp){
    projectionMatrixFileName = m_transformationMatrixFileName + "Warped.csv";
  } else {
    projectionMatrixFileName = m_transformationMatrixFileName + ".csv";
  }

  if(inputPathString == "yes")
  {
    std::cout << m_projectionMatrix << std::endl;
    // const static Eigen::IOFormat CSVFormat(Eigen::StreamPrecision,
    //     Eigen::DontAlignCols, ", ", "\n");
    const static Eigen::IOFormat saveFormat(Eigen::StreamPrecision,
        Eigen::DontAlignCols, " ", " ", "", "", "", "");
    struct stat st;
    if (stat(m_path.c_str(), &st) == -1) {
      system(("mkdir -p " + m_path).c_str());
      // std::cout<<"Created dir"<<std::endl;
    }

    std::ofstream file(m_path + "/" + projectionMatrixFileName );
    if(file.is_open()){
      file << m_projectionMatrix.format(saveFormat);
    }
    file.close();
    std::cout<<"Saved matrices to: " + m_path << std::endl;

  }

  std::cout << "Save warp matrices also? (yes/no)?: ";
  std::cin >> inputPathString;
  std::cout << "\n";
  if(inputPathString == "yes")
  {
    SavePerspectivePoints(m_path + "/");
    std::cout<<"Saved points to:" + m_path << std::endl;

  }
}


void Projection::Project()
{
  std::string mode;
  if(m_applyWarp){
    mode = "Warped";
  }
  else{
    mode = "Calibration";
  }
  cv::setMouseCallback(mode, ProjectMouseClicks, 
      (void *) &m_projectionMatrix);
  cv::waitKey(0);
  cv::setMouseCallback(mode, NULL, NULL);
  std::cout << "Exit point projection" << std::endl;
}

} // projection
} // vision
} // tools
} // opendlv
