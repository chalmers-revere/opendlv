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
 
#ifndef LOGIC_PERCEPTION_DETECTVEHICLE_ARTIFICIALNEURALNETWORK_HPP
#define LOGIC_PERCEPTION_DETECTVEHICLE_ARTIFICIALNEURALNETWORK_HPP


#include <memory>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace opendlv {
namespace logic {
namespace perception {

class ArtificialNeuralNetwork {
 public:
	ArtificialNeuralNetwork();
  virtual ~ArtificialNeuralNetwork();

  void setUp(const int a_nrInputs);
  void tearDown();

  float doClassification(const cv::Mat* a_subImage);
  void saveNetworkToFile();
  void loadNetworkFromFile();

 private:
  float evaluate(cv::Mat& predicted, cv::Mat& actual);

  std::shared_ptr<cv::Mat> m_layers;
  std::shared_ptr<CvANN_MLP> m_mlp;
  std::shared_ptr<CvANN_MLP_TrainParams> m_params;
  std::shared_ptr<CvTermCriteria> m_criteria;
};

}
}
}

#endif
