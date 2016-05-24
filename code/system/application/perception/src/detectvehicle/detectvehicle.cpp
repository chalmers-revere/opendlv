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
    , m_convNeuralNet()
{
  m_vehicleDetectionSystem = std::shared_ptr<VehicleDetectionSystem>(
      new VehicleDetectionSystem);
  m_verifiedVehicles = 
      std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>>(
      new std::vector<std::shared_ptr<DetectedVehicle>>);
  m_vehicleMemorySystem = std::shared_ptr<VehicleMemorySystem>(
      new VehicleMemorySystem);
  m_convNeuralNet = std::shared_ptr<ConvNeuralNet>(
      new ConvNeuralNet);

}

DetectVehicle::~DetectVehicle()
{
  std::cout << "DetectVehicle::~DetectVehicle()" << std::endl;
}

void DetectVehicle::setUp()
{
  std::cout << "DetectVehicle::setUp()" << std::endl;
  m_vehicleDetectionSystem->setUp();
  m_convNeuralNet->SetUp();
}

/**
 * Receives SharedImage from camera.
 * Sends .
 */
void DetectVehicle::nextContainer(odcore::data::Container &c)
{
  if (c.getDataType() != odcore::data::image::SharedImage::ID()) {
    // received container that we don't care about
    return;
  }
  std::cout << std::endl;

  if (!m_convNeuralNet->IsInitialized()) {
    std::cout << "Convolutional Neural Net not yet initialized" << std::endl;
    return;
  }

  odcore::data::image::SharedImage mySharedImg = 
      c.getData<odcore::data::image::SharedImage>();

  // extract time stamp from container
  odcore::data::TimeStamp timeStampOfImage = c.getSentTimeStamp();

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
  
  sharedMem->lock();
  {
    memcpy(myImage.data, sharedMem->getSharedMemory(), 
        imgWidth*imgHeight*nrChannels);
  }
  sharedMem->unlock();

  //////////////////////////////////////////////////////////////////////////////

  // Nr of seconds
  // TODO use something else as timestamp?
  //double timeStamp = ((double)c.getSentTimeStamp().toMicroseconds())/1000000;
  //std::cout << "timeStamp: " << timeStamp << std::endl;


  m_convNeuralNet->Update(&myImage);

  std::vector<cv::Rect> detections;
  m_convNeuralNet->GetDetectedVehicles(&detections);


  for (uint32_t i=0; i<detections.size(); i++) {
    cv::Rect currentBoundingBox = detections.at(i);

    odcore::data::TimeStamp lastSeen = timeStampOfImage;
    
    std::string type = "vehicle";
    float typeConfidence = 0.8f;

    float headingOfXmin = this->PixelPosToHeading(currentBoundingBox.x);
    float headingOfXmax = this->PixelPosToHeading(currentBoundingBox.x + currentBoundingBox.width);
    float angularSize = headingOfXmin - headingOfXmax;
    float angularSizeConfidence = 0.8f;

    float azimuth = headingOfXmin - angularSize / 2.0f; //midpoint of box
    float zenith = 0.0f;
    opendlv::model::Direction direction(azimuth, zenith);
    float directionConfidence = 1.0f;

    float azimuthRate = 0.0f;
    float zenithRate = 0.0f;
    opendlv::model::Direction directionRate(azimuthRate, zenithRate);
    float directionRateConfidence = -1.0f;

    float distance = 0.0f;
    float distanceConfidence = -1.0;

    float angularSizeRate = 0.0f;
    float angularSizeRateConfidence = -1.0f;

    float confidence = 0.8f;
    std::vector<std::string> sources;
    sources.push_back("Camera");

    std::vector<std::string> properties;

    uint16_t objectId = -1;

    opendlv::perception::Object detectedObject(lastSeen, type, typeConfidence,
        direction, directionConfidence, directionRate, directionRateConfidence,
        distance, distanceConfidence, angularSize, angularSizeConfidence,
        angularSizeRate, angularSizeRateConfidence, confidence, sources,
        properties, objectId);
    odcore::data::Container objectContainer(detectedObject);
    getConference().send(objectContainer);

    std::cout << "    type:        " << type << std::endl;
    std::cout << "    azimuth:       " << azimuth << std::endl;
    std::cout << "    angular size:        " << angularSize << std::endl;
  }

  /*
  m_verifiedVehicles->clear();
  m_vehicleDetectionSystem->update(&myImage, m_verifiedVehicles, timeStamp);


  m_vehicleMemorySystem->UpdateMemory(m_verifiedVehicles, timeStamp);


  
  //  ***   plot stuff ***
  cv::Mat outputImg(myImage.size(),myImage.type());
  myImage.copyTo(outputImg);
  //cv::Mat outputImg = myImage.clone();

  int32_t windowWidth = 640;
  int32_t windowHeight = 480;


  cv::resize(outputImg, outputImg, cv::Size(windowWidth, windowHeight), 0, 0, cv::INTER_CUBIC);
  */

  /*
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
  */

  /*
  //std::cout << "Nr of memorized vehicles: " << m_vehicleMemorySystem->GetNrMemorizedVehicles() << std::endl;
  //std::cout << "Total nr of vehicle rectangles: " << m_vehicleMemorySystem->GetTotalNrVehicleRects() << std::endl;
  cv::imshow("VehicleDetection", outputImg);
  cv::moveWindow("VehicleDetection", 100, 100);
  cv::waitKey(10);

  outputImg.release();
  */

  
  // end of plot stuff
  //myImage->release();
  cvReleaseImage(&myIplImage);
}

void DetectVehicle::tearDown()
{
  std::cout << "DetectVehicle::tearDown()" << std::endl;
  m_vehicleDetectionSystem->tearDown();
  m_convNeuralNet->TearDown();
}


float DetectVehicle::PixelPosToHeading(float pixelPosX)
{
  float assumedImageWidth = 800;
  float midPoint = assumedImageWidth/2;
  float assumedFov = 3.1416f*2/3;
  float pixelsPerRadian = assumedImageWidth/assumedFov;

  std::cout << std::endl;
  /*
  std::cout << "pixelPosX:          " << pixelPosX << std::endl;
  std::cout << "midPoint:           " << midPoint << std::endl;
  std::cout << "assumedFov:         " << assumedFov << std::endl;
  std::cout << "pixelsPerRadian:    " << pixelsPerRadian << std::endl;
  std::cout << "midPoint-pixelPosX: " << (midPoint-pixelPosX) << std::endl;
  std::cout << "(midPoint-pixelPosX)/pixelsPerRadian: " << ((midPoint-pixelPosX)/pixelsPerRadian) << std::endl;
  */
  float heading = (midPoint-pixelPosX)/pixelsPerRadian;
  //std::cout << "heading: " << heading << std::endl;
  return heading;
}

} // detectvehicle
} // perception
} // opendlv
