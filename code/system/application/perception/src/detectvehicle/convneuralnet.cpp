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

#include "detectvehicle/convneuralnet.hpp"

// TODO add documentation


namespace opendlv {
namespace perception {
namespace detectvehicle {


ConvNeuralNet::ConvNeuralNet()
    : m_cnn()
    , m_normAvgR()
    , m_normStdR()
    , m_normAvgG()
    , m_normStdG()
    , m_normAvgB()
    , m_normStdB()
    , m_pathTrainedCnn()
    , m_pathNormalizationConstants()
{
}


ConvNeuralNet::~ConvNeuralNet()
{
}


// Public functions

void ConvNeuralNet::setUp()
{
  int32_t width = 200;
  int32_t height = 112;
  bool isTrainingNetwork = false;
  this->GetConvNet(width, height, &m_cnn, isTrainingNetwork);

  std::cout << "Loading net from file..." << std::endl;
  std::ifstream input(m_pathTrainedCnn);
  input >> m_cnn;
  std::cout << "Loaded." << std::endl;


  std::cout << "Reading normalization constants..." << std::endl;
  std::ifstream normInput(m_pathNormalizationConstants);

  // RGB images
  double normAvgR;
  double normStdR;
  double normAvgG;
  double normStdG;
  double normAvgB;
  double normStdB;
  normInput >> normAvgR;
  normInput >> normStdR;
  normInput >> normAvgG;
  normInput >> normStdG;
  normInput >> normAvgB;
  normInput >> normStdB;
  std::cout << "(avgR, stdR): (" << normAvgR << ", " << normStdR << ")" << std::endl;
  std::cout << "(avgG, stdG): (" << normAvgG << ", " << normStdG << ")" << std::endl;
  std::cout << "(avgB, stdB): (" << normAvgB << ", " << normStdB << ")" << std::endl;
  std::cout << "Normalization constants read." << std::endl;
}


void ConvNeuralNet::tearDown()
{
}


void ConvNeuralNet::update(const cv::Mat* /*a_imageFrame*/)
{
}





// Private functions


void ConvNeuralNet::GetConvNet(
    int32_t /*a_width*/, int32_t /*a_height*/,
    tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent>* /*a_cnn*/,
    bool /*a_isTrainingNetwork*/)
{
}



} // detectvehicle
} // perception
} // opendlv

