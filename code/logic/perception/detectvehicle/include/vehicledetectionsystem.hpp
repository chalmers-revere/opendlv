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
 
#ifndef LOGIC_PERCEPTION_DETECTVEHICLE_VEHICLEDETECTIONSYSTEM_HPP
#define LOGIC_PERCEPTION_DETECTVEHICLE_VEHICLEDETECTIONSYSTEM_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "artificialneuralnetwork.hpp"
#include "detectedvehicle.hpp"

namespace opendlv {
namespace logic {
namespace perception {

class VehicleDetectionSystem {
 public:
	VehicleDetectionSystem();
  //VehicleDetectionSystem(VehicleDetectionSystem const &) = delete;
  //VehicleDetectionSystem &operator=(VehicleDetectionSystem const &) = delete;
  virtual ~VehicleDetectionSystem();
  void setUp();
  void tearDown();
  void update(const cv::Mat* a_imageFrame,
      std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>> verifiedVehicles,
      double timeStamp);

  void setShowImage(bool a_showImage);
  bool isShowImage();

 private:
  uint32_t m_imgCounter = 0;

  const std::string m_pathHaarClassOwn = "haar_classifiers/own_201512.xml";
  const std::string m_pathHaarClassCars3 = "share/opendlv/system/application/perception/detectvehicle/haar_cars3.xml";

  cv::Rect m_regionOfInterest;
  int32_t m_windowWidth;
  int32_t m_windowHeight;
  int32_t m_roiTop;
  cv::CascadeClassifier m_haarClassifier;
  std::vector<cv::Rect> m_potentialVehicles;

  ArtificialNeuralNetwork m_ann;

  bool showImage = false;
};

}
}
}

#endif
