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

#ifndef DETECTVEHICLE_DETECTVEHICLE_HPP_
#define DETECTVEHICLE_DETECTVEHICLE_HPP_

#include <memory>
#include <vector>
#include <cmath>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
 
#include <Eigen/Dense>

#include "detectvehicle/convneuralnet.hpp"




namespace opendlv {
namespace perception {
namespace detectvehicle {

/**
 * This class provides a module with ability of detect vehicle. 
 * It imports a trained ANN externally.
 */
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
<<<<<<< HEAD
  bool m_debugMode;
=======
  Eigen::Matrix3d m_pixel2World;
>>>>>>> 1f04980f8ed63d7fdd53d104baf82a53b11fd194
};

} // detectvehicle
} // perception
} // opendlv

#endif
