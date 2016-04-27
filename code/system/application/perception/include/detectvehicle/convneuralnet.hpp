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
 
#ifndef CONVNEURALNET_HPP
#define CONVNEURALNET_HPP

#include <opencv2/core/core.hpp>
#include <tiny_cnn/tiny_cnn.h>

namespace opendlv {
namespace perception {
namespace detectvehicle {


class ConvNeuralNet {
public:
  ConvNeuralNet();
  virtual ~ConvNeuralNet();

/* Public methods */
public:
  void setUp();
  void tearDown();
  void update(const cv::Mat* a_imageFrame);

/* Public fields */
public:


/* Private methods */
private:
  void GetConvNet(
      int32_t a_width, int32_t a_height,
      tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent>* a_cnn,
      bool a_isTrainingNetwork);


/* Private fields */
private:
  tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent> m_cnn;

  double m_normAvgR;
  double m_normStdR;
  double m_normAvgG;
  double m_normStdG;
  double m_normAvgB;
  double m_normStdB;

  std::string m_pathTrainedCnn = "trained_cnn";
  std::string m_pathNormalizationConstants = "normalization_constants";

};


} // detectvehicle
} // perception
} // opendlv

#endif
