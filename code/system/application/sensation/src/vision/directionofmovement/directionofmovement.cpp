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


#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

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
      a_argc, a_argv, "sensation-vision-directionofmovement")
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
  if(a_c.getDataType() != odcore::data::image::SharedImage::ID()){
    return;
  }
  odcore::data::image::SharedImage mySharedImg =
      a_c.getData<odcore::data::image::SharedImage>();
  // std::cout<<mySharedImg.getName()<<std::endl;

  if(mySharedImg.getName() != "opticalflow"){
    return;
  }
  // std::cout<<"Got correct data"<<std::endl;

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
      odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
          mySharedImg.getName()));
  const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
  const uint32_t imgWidth = mySharedImg.getWidth();
  const uint32_t imgHeight = mySharedImg.getHeight();
  const uint32_t imgSize = mySharedImg.getSize();

  IplImage* myIplImage = cvCreateImage(cvSize(imgWidth,imgHeight), IPL_DEPTH_8U,
      nrChannels);
  cv::Mat image(myIplImage);

  if(!sharedMem->isValid()){
    return;
  }

  sharedMem->lock();
  {
    memcpy(image.data, sharedMem->getSharedMemory(), imgSize);
  }
  sharedMem->unlock();

  std::cout << image << std::endl;

  const int32_t windowWidth = 640;
  const int32_t windowHeight = 480;
  cv::Mat display2;
  cv::resize(image, display2, cv::Size(windowWidth, windowHeight), 0, 0,
      cv::INTER_CUBIC);
  cv::imshow("flow",display2); 
  cv::waitKey(1);
}

void DirectionOfMovement::setUp()
{
  cv::namedWindow( "flow", 1 );
}

void DirectionOfMovement::tearDown()
{
}

} // directionofmovement
} // vision
} // sensation
} // opendlv
