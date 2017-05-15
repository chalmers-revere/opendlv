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

#ifndef LOGIC_PERCEPTION_DETECTVEHICLE_DETECTVEHICLE_HPP
#define LOGIC_PERCEPTION_DETECTVEHICLE_DETECTVEHICLE_HPP

#include <memory>
#include <vector>
#include <cmath>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
 
#include <Eigen/Dense>

#include "convneuralnet.hpp"

namespace opendlv {
namespace logic {
namespace perception {

class DetectVehicle
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  DetectVehicle(int32_t const &, char **);
  DetectVehicle(DetectVehicle const &) = delete;
  DetectVehicle &operator=(DetectVehicle const &) = delete;
  virtual ~DetectVehicle();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  Eigen::MatrixXd ReadMatrix(std::string fileName,int nRows, int nCols);
  void TransformPointToGlobalFrame(Eigen::Vector3d &point);
  void sendObjectInformation(std::vector<cv::Rect>* detections, 
      odcore::data::TimeStamp timeStampOfImage);

  bool m_initialised;
  std::shared_ptr<ConvNeuralNet> m_convNeuralNet;
  Eigen::Vector3d m_scale;
  std::string m_sourceName;
  bool m_debugMode;
  Eigen::Matrix3d m_pixel2World;
};

}
}
}

#endif
