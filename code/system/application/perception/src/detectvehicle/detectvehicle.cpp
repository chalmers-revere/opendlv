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

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "detectvehicle/detectvehicle.hpp"

namespace opendlv {
namespace perception {
namespace detectvehicle {


// TODO add documentation


/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DetectVehicle::DetectVehicle(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-detectvehicle")
    , m_vehicleDetectionSystem()
    , m_verifiedVehicles()
    , m_vehicleMemorySystem()
{
  m_vehicleDetectionSystem = std::shared_ptr<VehicleDetectionSystem>(new VehicleDetectionSystem);
  m_verifiedVehicles = std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>>(new std::vector<std::shared_ptr<DetectedVehicle>>);
  m_vehicleMemorySystem = std::shared_ptr<VehicleMemorySystem>(new VehicleMemorySystem);
}

DetectVehicle::~DetectVehicle()
{
  std::cout << "DetectVehicle::~DetectVehicle()" << std::endl;
}

/**
 * Receives .
 * Sends .
 */
void DetectVehicle::nextContainer(odcore::data::Container &c)
{
  if (c.getDataType() == odcore::data::image::SharedImage::ID()) {
   // std::cout << "Received container of type " << c.getDataType() << 
   //     " sent at " <<   c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() << 
   //     " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << 
   //     std::endl;


    // TODO Start of pretty bad-performing block that extracts image from memory 

    odcore::data::image::SharedImage mySharedImg = 
        c.getData<odcore::data::image::SharedImage>();
  //  cout << "Received a SharedImage of size: (" << mySharedImg.getWidth() << 
  //      ", " << mySharedImg.getHeight() << ")" << endl;

    std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(mySharedImg.getName()));
    
    const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
    const uint32_t imgWidth = mySharedImg.getWidth();
    const uint32_t imgHeight = mySharedImg.getHeight();

    //std::shared_ptr<cv::Mat> myImage = std::shared_ptr<cv::Mat>(cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels));
    IplImage* myIplImage;
    myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels);
    cv::Mat myImage(myIplImage);

    if (!sharedMem->isValid()) {
      return;
    }
    
    sharedMem->lock();
    {
      memcpy(myImage.data, sharedMem->getSharedMemory(), imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
    

    // end of slow / bad-performing block

    // Nr of seconds
    // TODO use something else as timestamp?
    double timeStamp = ((double)c.getSentTimeStamp().toMicroseconds())/1000000;
   // std::cout << "timeStamp: " << timeStamp << std::endl;
    
    m_verifiedVehicles->clear();
    m_vehicleDetectionSystem->update(&myImage, m_verifiedVehicles, timeStamp);


    m_vehicleMemorySystem->UpdateMemory(m_verifiedVehicles, timeStamp);


    
    //  ***   plot stuff ***
    cv::Mat outputImg(myImage.size(),myImage.type());
    myImage.copyTo(outputImg);
    //cv::Mat outputImg = myImage.clone();

    int32_t windowWidth = 640;
    int32_t windowHeight = 480;

    /*
    if (showBlackImgOutput) {
      outputImg = cv::Scalar(0, 0, 0); //set image black
    }
    */

    cv::resize(outputImg, outputImg, cv::Size(windowWidth, windowHeight), 0, 0, cv::INTER_CUBIC);

    std::vector<std::shared_ptr<RememberedVehicle>> memorized;
    m_vehicleMemorySystem->GetMemorizedVehicles(&memorized);
    for (uint32_t i=0; i<memorized.size(); i++) {
      // Show memory of vehicles
      for (int32_t j=0; j<memorized.at(i)->GetNrMemories(); j++) {
        std::vector<std::shared_ptr<DetectedVehicle>> memOverTime;
        memorized.at(i)->GetMemoryOverTime(&memOverTime);
        std::shared_ptr<DetectedVehicle> vehRect = memOverTime.at(j);
        cv::rectangle(outputImg, vehRect->GetDetectionRectangle(), memorized.at(i)->GetDummyColor());
      }
    }
    for (uint32_t i=0; i<m_verifiedVehicles->size(); i++) {
      // Show vehicles that were verified this frame (green)
      //cv::rectangle(outputImg, m_verifiedVehicles.at(i)->GetDetectionRectangle(), cv::Scalar(0,255,0));
    }

  //  std::cout << "Nr of memorized vehicles: " << m_vehicleMemorySystem->GetNrMemorizedVehicles() << std::endl;
   // std::cout << "Total nr of vehicle rectangles: " << m_vehicleMemorySystem->GetTotalNrVehicleRects() << std::endl;
    cv::imshow("VehicleDetection", outputImg);
    cv::moveWindow("VehicleDetection", 100, 100);
    cv::waitKey(10);

    outputImg.release();

    
    // end of plot stuff
    //myImage->release();
    cvReleaseImage(&myIplImage);
  }
}

void DetectVehicle::setUp()
{
  std::cout << "DetectVehicle::setUp()" << std::endl;
  m_vehicleDetectionSystem->setUp();
}

void DetectVehicle::tearDown()
{
  std::cout << "DetectVehicle::tearDown()" << std::endl;
  m_vehicleDetectionSystem->tearDown();
}

} // detectvehicle
} // perception
} // opendlv
