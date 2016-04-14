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

#include "detectvehicle/vehicledetectionsystem.hpp"

// TODO add documentation


namespace opendlv {
namespace perception {
namespace detectvehicle {

VehicleDetectionSystem::VehicleDetectionSystem() 
    : m_regionOfInterest()
    , m_windowWidth()
    , m_windowHeight()
    , m_roiTop()
    , m_haarClassifier()
    , m_potentialVehicles()
    , m_ann()
{
  std::cout << "VehicleDetectionSystem::VehicleDetectionSystem()" << "\n";
}

VehicleDetectionSystem::~VehicleDetectionSystem()
{
  std::cout << "VehicleDetectionSystem::~VehicleDetectionSystem()" << "\n";
}


void VehicleDetectionSystem::setUp()
{
  std::cout << "VehicleDetectionSystem::setUp()" << "\n";
	m_haarClassifier = cv::CascadeClassifier(m_pathHaarClassCars3);
  if (m_haarClassifier.empty()) {
    std::cout << "LOADED HAAR CLASSIFIER WAS EMPTY!!!\n";
  }

  // TODO this affects the x/y coord of every rectangle
  m_windowWidth = 640;
  m_windowHeight = 480;
  m_roiTop = 200;
  m_regionOfInterest = 
      cv::Rect(0, m_roiTop, m_windowWidth, m_windowHeight-m_roiTop);

  m_ann.loadNetworkFromFile();
}

void VehicleDetectionSystem::tearDown()
{
  std::cout << "VehicleDetectionSystem::tearDown()" << "\n";
  m_ann.tearDown();
}

void VehicleDetectionSystem::update(const cv::Mat* a_imageFrame, 
    std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>> verifiedVehicles, 
    double timeStamp)
{
  //clock_t timeBegin = clock();


  // Save a copy of the source image so that we don't change the original one
  cv::Mat srcImg(a_imageFrame->size(), a_imageFrame->type());
  a_imageFrame->copyTo(srcImg);

  //cv::Mat srcImg = a_imageFrame->clone();
  cv::resize(srcImg, srcImg, cv::Size(m_windowWidth, m_windowHeight), 
      0, 0, cv::INTER_CUBIC);

  cv::Mat outputImg(srcImg.size(), srcImg.type());
  srcImg.copyTo(outputImg);

  //cv::Mat outputImg = srcImg.clone();
  //GaussianBlur(srcImg, srcImg, cv::Size(3,3), 1);

  // Extract the sub image of the region of interest
  cv::Mat imageROI = srcImg(m_regionOfInterest);
  m_haarClassifier.detectMultiScale(imageROI, m_potentialVehicles);
  //haarClassifier.detectMultiScale(imageROI, detectedVehicles, 1.03, 3, 0 | CASCADE_FIND_BIGGEST_OBJECT, Size(10, 10), Size(150, 150));
  //haarClassifier.detectMultiScale(imageROI, detectedVehicles, 1.03, 1, 0, Size(10, 10), Size(150, 150));
  //haarClassifier.detectMultiScale(imageROI, detectedVehicles, 1.1, 3, 0 | CASCADE_FIND_BIGGEST_OBJECT, Size(20, 20), Size(150, 150));

  //clock_t timeHaar = clock();
  //std::cout << "        Detected " << m_potentialVehicles.size() << " potential vehicles\n";


  cv::Mat subImage;

  std::cout << "Potential #vehicles: " << m_potentialVehicles.size() << 
      std::endl;

  for (size_t i = 0; i < m_potentialVehicles.size(); i++) {
    //cout << "Inside for loop \n";
    cv::Rect r = m_potentialVehicles[i];
    r.y += m_roiTop;
    
    //cv::imshow("Debug", srcImg(r));
    //cv::waitKey(0);
    //cv::Mat featureMat(nrOfFeatures, 1, CV_32FC1, featureVector);
    subImage = cv::Mat(srcImg(r).size(), srcImg(r).type());
    srcImg(r).copyTo(subImage);
    //subImage = srcImg(r).clone();
    float result = m_ann.doClassification(&subImage);
    std::cout << "ann result: " << result << std::endl;
    bool saveImages = false;
    std::string saveImgPath = "tmp_img/";

    if (result > 0) {
      // if this subimage gets verified by SVM, draw green
      cv::rectangle(outputImg, r, cv::Scalar(0,255,0));
      std::shared_ptr<DetectedVehicle> detectedVehicle(
          new DetectedVehicle(r, timeStamp));
      verifiedVehicles->push_back(detectedVehicle);
      if (saveImages) {
        std::string imgFileName = saveImgPath + "pos" + 
            std::to_string(m_imgCounter) + ".jpg";
        imwrite(imgFileName, subImage);
        m_imgCounter++;
      }
    } else {
      // otherwise, if not verified, draw gray
      cv::rectangle(outputImg, r, cv::Scalar(0,0,255));
      if (saveImages) {
        std::string imgFileName = saveImgPath + "neg" + 
            std::to_string(m_imgCounter) + ".jpg";
        imwrite(imgFileName, subImage);
        m_imgCounter++;
      }
    }

    //std::cout << "ANN response: " << result << "\n";
    //cv::imshow("Debug", subImage);
    //cv::waitKey(10);
    //cv::rectangle(outputImg, r, cv::Scalar(0,255,0));
    subImage.release();
  }
  //clock_t timeAnn = clock();

  //float f_timeHaar = (float) (timeHaar-timeBegin)/CLOCKS_PER_SEC;
  //float f_timeAnn = (float) (timeAnn-timeHaar)/CLOCKS_PER_SEC;
  //std::cout << "    (timeHaar, timeAnn): (" << f_timeHaar << ", " << f_timeAnn << ")\n";

  // Create the window
  //cv::imshow("VehicleDetectionSystem", outputImg);
  //cv::moveWindow("VehicleDetectionSystem", 100, 100);
  //cv::waitKey(10);
  subImage.release();
  imageROI.release();
  outputImg.release();
  srcImg.release();
}

void VehicleDetectionSystem::setShowImage(bool a_showImage)
{
  showImage = a_showImage;
}

bool VehicleDetectionSystem::isShowImage()
{
  return showImage;
}


} // detectvehicle
} // perception
} // opendlv

