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

#include "opendavinci/odcore/base/Lock.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "vision/directionofmovement/directionofmovement.hpp"


namespace opendlv {
namespace sensation {
namespace vision {
namespace directionofmovement {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DirectionOfMovement::DirectionOfMovement(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-vision-directionofmovement"),
      m_FOE(),
      m_image()
{
}

DirectionOfMovement::~DirectionOfMovement()
{
}

/**
 * Receives .
 * Sends .
 */
void DirectionOfMovement::nextContainer(odcore::data::Container &a_c)
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
    return;
  }
  if(a_c.getDataType() == opendlv::sensation::OpticalFlow::ID()){
    opendlv::sensation::OpticalFlow message = 
        a_c.getData<opendlv::sensation::OpticalFlow>();
    std::vector<float> x,y,u,v;
    uint16_t nPoints = message.getNPoint();
    x = message.getListOfX();
    y = message.getListOfY();
    u = message.getListOfU();
    v = message.getListOfV();

    Eigen::MatrixXd flow(nPoints, 4);
    Eigen::MatrixXd A(nPoints,2);
    Eigen::MatrixXd B(nPoints,1);
    for(uint8_t i = 0; i < nPoints; ++i){
      flow.row(i) << x[i],y[i],u[i],v[i];
    }
    A.col(0) = flow.col(3);
    A.col(1) = -flow.col(2);
    B.col(0) = flow.col(3).cwiseProduct(flow.col(0))-flow.col(1).cwiseProduct(flow.col(2));
    
    // FOE = (A * A^T)^-1 * A^T * B
    m_FOE = (A.transpose()*A).inverse() * A.transpose() * B;
    // std::cout<< m_FOE << std::endl;
    cv::circle(m_image, cv::Point2f(m_FOE(0),m_FOE(1)), 3, cv::Scalar(0,255,0), -1, 8);
    const int32_t windowWidth = 640;
    const int32_t windowHeight = 480;
    cv::Mat display;
    cv::resize(m_image, display, cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);
    cv::imshow("FOE", display);

    cv::waitKey(1);
    return;
  }
  
}

void DirectionOfMovement::setUp()
{
  cv::namedWindow( "FOE", 1 );
}

void DirectionOfMovement::tearDown()
{
}

} // directionofmovement
} // vision
} // sensation
} // opendlv
