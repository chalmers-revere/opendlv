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

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "trackvehicle/trackvehicle.hpp"

namespace opendlv {
namespace perception {
namespace trackvehicle {


/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
TrackVehicle::TrackVehicle(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-trackvehicle")
    , m_initialised(false)
    , m_debug()
{
}

TrackVehicle::~TrackVehicle()
{
}

void TrackVehicle::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = (kv.getValue<int32_t> ("perception-trackvehicle.debug") == 1);
  std::cout << "Setup complete." << std::endl;
  m_initialised = true;
}
void TrackVehicle::tearDown()
{
}
/**
 * Receives SharedImage from camera.
 * Sends .
 */
void TrackVehicle::nextContainer(odcore::data::Container &c)
{
  if (c.getDataType() != odcore::data::image::SharedImage::ID() || !m_initialised) {
    return;
  }


  odcore::data::image::SharedImage mySharedImg = c.getData<odcore::data::image::SharedImage>();


  std::string cameraName = mySharedImg.getName();
  // std::cout << "Received image from camera " << cameraName  << "!" << std::endl;

  //TODO compare the source name to keep track different camera sources

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
      odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
          mySharedImg.getName()));
  
  const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
  const uint32_t imgWidth = mySharedImg.getWidth();
  const uint32_t imgHeight = mySharedImg.getHeight();

  IplImage* myIplImage;
  myIplImage = cvCreateImage(cvSize(
      imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels);
  cv::Mat myImage(myIplImage);

  if (!sharedMem->isValid()) {
    return;
  }
  
  {
    sharedMem->lock();
    memcpy(myImage.data, sharedMem->getSharedMemory(), 
        imgWidth*imgHeight*nrChannels);
    sharedMem->unlock();
  }

  cvReleaseImage(&myIplImage);
}




} // trackvehicle
} // perception
} // opendlv
