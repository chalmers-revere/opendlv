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
  /*
  m_vehicleDetectionSystem = std::shared_ptr<VehicleDetectionSystem>(
      new VehicleDetectionSystem);
  m_verifiedVehicles = 
      std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>>(
      new std::vector<std::shared_ptr<DetectedVehicle>>);
  m_vehicleMemorySystem = std::shared_ptr<VehicleMemorySystem>(
      new VehicleMemorySystem);
  */
  m_convNeuralNet = std::shared_ptr<ConvNeuralNet>(
      new ConvNeuralNet);

  m_convNeuralNet->SetUp();
}

DetectVehicle::~DetectVehicle()
{
  std::cout << "DetectVehicle::~DetectVehicle()" << std::endl;
}

void DetectVehicle::setUp()
{
  std::cout << "DetectVehicle::setUp()" << std::endl;
  //m_vehicleDetectionSystem->setUp();
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

  Eigen::Matrix3d transformationMatrix;

  std::string cameraName = mySharedImg.getName();
  std::cout << "Received image from camera \"" << cameraName  << "\"!" << std::endl;

  if (cameraName == "front-left") {
    transformationMatrix = ReadMatrix(
        "/opt/opendlv/share/opendlv/tools/vision/projection/leftCameraTransformationMatrix.csv",3,3);
  }
  else if (cameraName == "front-right") {
    transformationMatrix = ReadMatrix(
        "/opt/opendlv/share/opendlv/tools/vision/projection/rightCameraTransformationMatrix.csv",3,3);
  }
  else {
    std::cout << "Cannot load transformation matrix for " << cameraName 
        << ", aborting image analysis..." << std::endl;

    // Release memory, no memory leaks for the people!
    cvReleaseImage(&myIplImage);
    return;
  }


  m_convNeuralNet->Update(&myImage);

  std::vector<cv::Rect> detections;
  m_convNeuralNet->GetDetectedVehicles(&detections);

  sendObjectInformation(&detections, timeStampOfImage, transformationMatrix);



  // Old stuff below
  

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
  //m_vehicleDetectionSystem->tearDown();
  m_convNeuralNet->TearDown();
}

Eigen::MatrixXd DetectVehicle::ReadMatrix(std::string fileName, int nRows,
    int nCols)
{
  std::ifstream file(fileName);

  Eigen::MatrixXd matrix(nRows,nCols);

  if (file.is_open()) {
    for(int i = 0; i < nRows; i++){
      for(int j = 0; j < nCols; j++){
        double item = 0.0;
        file >> item;
        matrix(i,j) = item;
      }
    }
  }  
  file.close();

  return matrix;
}

void DetectVehicle::TransformPointToGlobalFrame(Eigen::Vector3d &point, 
    Eigen::Matrix3d transformationMatrix)
{
  // std::cout<<"point before \n";
  // std::cout<<point<<std::endl;
  // point = point.cwiseProduct(m_scale); // should not be needed...
  // std::cout<<"point after 1\n";
  // std::cout<<point<<std::endl;
  point = transformationMatrix * point;

  // std::cout<<"point final before \n";
  // std::cout<<point<<std::endl;

  point = point / point(2);
  // std::cout<<"point final \n";
  // std::cout<<point<<std::endl;
}

void DetectVehicle::sendObjectInformation(std::vector<cv::Rect>* detections, 
    odcore::data::TimeStamp timeStampOfImage, 
    Eigen::Matrix3d transformationMatrix)
{
  for (uint32_t i=0; i<detections->size(); i++) {
    
    cv::Rect currentBoundingBox = detections->at(i);

    odcore::data::TimeStamp lastSeen = timeStampOfImage;
    
    std::string type = "vehicle";
    float typeConfidence = 0.5f;

    Eigen::Vector3d pointBottomLeft(
        currentBoundingBox.x, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    TransformPointToGlobalFrame(pointBottomLeft, transformationMatrix);
    
    Eigen::Vector3d pointBottomRight(
        currentBoundingBox.x + currentBoundingBox.width, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    TransformPointToGlobalFrame(pointBottomRight, transformationMatrix);

    Eigen::Vector3d pointBottomMid(
        currentBoundingBox.x + currentBoundingBox.width / 2.0f, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    TransformPointToGlobalFrame(pointBottomMid, transformationMatrix);

    float headingOfBottomLeft = std::atan2(pointBottomLeft(1), pointBottomLeft(0));
    float headingOfBottomRight = std::atan2(pointBottomRight(1), pointBottomRight(0));
    float angularSize = headingOfBottomLeft - headingOfBottomRight;
    float angularSizeConfidence = -1.0f;


    float azimuth = headingOfBottomRight + angularSize / 2.0f; //midpoint of box
    float zenith = -1.0f;
    opendlv::model::Direction direction(azimuth, zenith);
    float directionConfidence = 0.1f;

    float azimuthRate = -1.0f;
    float zenithRate = -1.0f;
    opendlv::model::Direction directionRate(azimuthRate, zenithRate);
    float directionRateConfidence = -1.0f;


    float distance = (float) sqrt(pow(pointBottomMid(0),2) + pow(pointBottomMid(1),2));
    float distanceConfidence = 0.1;


    float angularSizeRate = -1.0f;
    float angularSizeRateConfidence = -1.0f;


    float confidence = 0.1f;
    uint16_t sources = 1;

    std::vector<std::string> properties;

    uint16_t objectId = -1;
/*
    float detectionWidth = (float) 
        sqrt(pow(pointBottomRight(0) - pointBottomLeft(0),2) + 
        pow(pointBottomRight(1) - pointBottomLeft(1),2));
*/
    if (angularSize < 0 || distance > 150) {
      // Something fishy with this detection
      continue;
    }

    opendlv::perception::Object detectedObject(lastSeen, type, typeConfidence,
        direction, directionConfidence, directionRate, directionRateConfidence,
        distance, distanceConfidence, angularSize, angularSizeConfidence,
        angularSizeRate, angularSizeRateConfidence, confidence, sources,
        properties, objectId);
    odcore::data::Container objectContainer(detectedObject);
    getConference().send(objectContainer);

    std::cout << "Sending DetectedObject:" << std::endl;
    std::cout << "    type:          " << type << std::endl;
    std::cout << "    azimuth:       " << azimuth << std::endl;
    std::cout << "    azimuth (deg): " << (azimuth*(float)opendlv::Constants::RAD2DEG) << std::endl;
    std::cout << "    angularSize:   " << angularSize << std::endl;
    std::cout << "    angSize (deg): " << (angularSize*(float)opendlv::Constants::RAD2DEG) << std::endl;
    std::cout << "    distance (m):  " << distance << std::endl;
    /*
    std::cout << "    size:          " << size << std::endl;
    std::cout << "    angle (deg):   " << (angle*(float)opendlv::Constants::RAD2DEG) << std::endl;
    std::cout << "    size (deg):    " << (size*(float)opendlv::Constants::RAD2DEG) << std::endl;
    std::cout << "    objectIndex:   " << objectIndex << std::endl;
    std::cout << "    azimuth:       " << azimuth << std::endl;
    std::cout << "    angular size:  " << angularSize << std::endl;
    */
  }
}


float DetectVehicle::PixelPosToHeading(float pixelPosX)
{
  float assumedImageWidth = 800;
  float midPoint = assumedImageWidth/2;
  float assumedFov = opendlv::Constants::DEG2RAD*60;
  float pixelsPerRadian = assumedImageWidth/assumedFov;

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
