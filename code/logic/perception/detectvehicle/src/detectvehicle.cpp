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

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "detectvehicle.hpp"

namespace opendlv {
namespace logic {
namespace perception {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DetectVehicle::DetectVehicle(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "logic-perception-detectvehicle")
    , m_initialised(false)
    , m_convNeuralNet()
    , m_scale(1, 1, 1)
    , m_sourceName()
    , m_debugMode()
    , m_pixel2World()
{
  m_convNeuralNet = std::shared_ptr<ConvNeuralNet>(
      new ConvNeuralNet);

}

DetectVehicle::~DetectVehicle()
{
}

void DetectVehicle::setUp()
{
  std::cout << "DetectVehicle::setUp()" << std::endl;
  m_convNeuralNet->SetUp();
  // float scale = 1280.0f/800.0f;
  // m_scale = Eigen::Vector3d(scale, scale, 1);
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_sourceName = kv.getValue<std::string>("logic-perception-detectvehicle.source");

  std::string path = "/opt/opendlv/share/opendlv/tools/vision/projection/";

  if (m_sourceName == "front-left") {
    m_pixel2World = ReadMatrix(path+"leftCameraTransformationMatrix.csv",3,3);
  }
  else if (m_sourceName == "front-right") {
    m_pixel2World = ReadMatrix(path+"rightCameraTransformationMatrix.csv",3,3);
  }
  else {
    std::cout << "Cannot load transformation matrix for " << m_sourceName << ". Undefined camera source." << std::endl;
  }
  m_debugMode = (kv.getValue<int32_t> ("logic-perception-detectvehicle.debug") == 1);
  std::cout << "Debug mode: " << m_debugMode << std::endl;
  std::cout << "This DetectVehicle instance will receive images from " << m_sourceName << "." << std::endl;
  std::cout << "Setup complete." << std::endl;
  m_initialised = true;
}

void DetectVehicle::nextContainer(odcore::data::Container &c)
{
  if (!m_convNeuralNet->IsInitialized()) {
    std::cout << "Convolutional Neural Net not yet initialized" << std::endl;
    return;
  }
  if (c.getDataType() != odcore::data::image::SharedImage::ID() || !m_initialised) {
    return;
  }


  odcore::data::image::SharedImage mySharedImg = 
      c.getData<odcore::data::image::SharedImage>();


  std::string cameraName = mySharedImg.getName();
  // std::cout << "Received image from camera " << cameraName  << "!" << std::endl;

  // Receive image from a source that this instance should not care about
  if (m_sourceName.compare(cameraName) != 0) {
    //std::cout << "Received image came from wrong source. Expected " << m_sourceName << std::endl;
    return;
  }

  odcore::data::TimeStamp timeStampOfImage = c.getReceivedTimeStamp();
  // odcore::data::TimeStamp start;

  //double tmpSeconds = (nowTimeStamp - timeStampOfImage).toMicroseconds() / 1000000.0;
  //std::cout << "tmpSeconds: " << tmpSeconds << std::endl;
  //std::cout << "timeStampOfImage: " << timeStampOfImage << std::endl;
  //std::cout << "timeStampOfImage (s): " << (timeStampOfImage.toMicroseconds() / 1000000.0) << std::endl;

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


  // odcore::data::TimeStamp start;
  m_convNeuralNet->Update(&myImage, m_debugMode);
  // odcore::data::TimeStamp end;
  // double duration = end.toMicroseconds() - start.toMicroseconds();
  // std::cout << "The update took " << duration << " microseconds (" << duration/1000000 << " seconds)." << std::endl;

  std::vector<cv::Rect> detections;
  m_convNeuralNet->GetDetectedVehicles(&detections);

  sendObjectInformation(&detections, timeStampOfImage);

  cvReleaseImage(&myIplImage);
}

void DetectVehicle::tearDown()
{
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

void DetectVehicle::TransformPointToGlobalFrame(Eigen::Vector3d &point)
{
  // std::cout<<"point before \n";
  // std::cout<<point<<std::endl;
  //Eigen::Vector3d scale(1.6, 1.6, 1);
  point = point.cwiseProduct(m_scale);
  // std::cout<<"point after 1\n";
  // std::cout<<point<<std::endl;
  point = m_pixel2World * point;

  // std::cout<<"point final before \n";
  // std::cout<<point<<std::endl;

  point = point / point(2);
  // std::cout<<"point final \n";
  // std::cout<<point<<std::endl;
}

void DetectVehicle::sendObjectInformation(std::vector<cv::Rect>* detections, 
    odcore::data::TimeStamp timeStampOfImage)
{
  for (uint32_t i=0; i<detections->size(); i++) {
    
    cv::Rect currentBoundingBox = detections->at(i);

    odcore::data::TimeStamp lastSeen = timeStampOfImage;
    
    std::string type = "vehicle";

    //Hardcoded confidence
    float typeConfidence = 0.5f;

    Eigen::Vector3d pointBottomLeft(
        currentBoundingBox.x, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    Eigen::Vector3d pointBottomRight(
        currentBoundingBox.x + currentBoundingBox.width, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    Eigen::Vector3d pointBottomMid(
        currentBoundingBox.x + currentBoundingBox.width / 2.0f, 
        currentBoundingBox.y + currentBoundingBox.height, 
        1);
    TransformPointToGlobalFrame(pointBottomLeft);
    TransformPointToGlobalFrame(pointBottomRight);
    TransformPointToGlobalFrame(pointBottomMid);

    float headingOfBottomLeft = std::atan2(pointBottomLeft(1), pointBottomLeft(0));
    float headingOfBottomRight = std::atan2(pointBottomRight(1), pointBottomRight(0));
    float angularSize = headingOfBottomLeft - headingOfBottomRight;
    //Hardcoded confidence
    float angularSizeConfidence = -1.0f;


    float azimuth = headingOfBottomRight + angularSize / 2.0f; //midpoint of box
    float zenith = 0;
    opendlv::model::Direction direction(azimuth, zenith);
    //Hardcoded confidence
    float directionConfidence = 0.1f;

    float azimuthRate = 0;
    float zenithRate = 0;
    opendlv::model::Direction directionRate(azimuthRate, zenithRate);
    float directionRateConfidence = -1.0f;


    float distance = static_cast<float>(sqrt(pow(pointBottomMid(0),2) + pow(pointBottomMid(1),2)));
    //Hardcoded confidence
    float distanceConfidence = 0.5f;


    float angularSizeRate = 0;
    float angularSizeRateConfidence = -1.0f;


    float confidence = 0.2f;
    std::vector<std::string> sources;
    sources.push_back(m_sourceName);

    std::vector<std::string> properties;

    uint16_t objectId = -1;

    float detectionWidth = static_cast<float>(
        sqrt(pow(pointBottomRight(0) - pointBottomLeft(0),2) + 
        pow(pointBottomRight(1) - pointBottomLeft(1),2)));

    //std::cout << "Object width: " << detectionWidth << " m" << std::endl;
    //Should detection sort be in the this module?
    if (angularSize < 0 
        || distance > 150 
        || detectionWidth < 0.5f 
        || detectionWidth > 10) {
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
    std::cout << "    azimuth (deg): " << (azimuth*static_cast<float>(opendlv::Constants::RAD2DEG)) << std::endl;
    std::cout << "    angularSize:   " << angularSize << std::endl;
    std::cout << "    angSize (deg): " << (angularSize*static_cast<float>(opendlv::Constants::RAD2DEG)) << std::endl;
    std::cout << "    distance (m):  " << distance << std::endl;
    std::cout << "    width (m):     " << detectionWidth << std::endl;
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

}
}
}
