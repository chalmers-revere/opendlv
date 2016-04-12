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

#include "detectvehicle/artificialneuralnetwork.hpp"



// TODO add documentation


ArtificialNeuralNetwork::ArtificialNeuralNetwork()
    : m_layers()
    , m_mlp()
    , m_params()
    , m_criteria()
{
  std::cout << "ArtificialNeuralNetwork::ArtificialNeuralNetwork()" << "\n";
  m_layers = std::shared_ptr<cv::Mat>(new cv::Mat);
  m_mlp = std::shared_ptr<CvANN_MLP>(new CvANN_MLP);
  m_params = std::shared_ptr<CvANN_MLP_TrainParams>(new CvANN_MLP_TrainParams);
  m_criteria = std::shared_ptr<CvTermCriteria>(new CvTermCriteria);
}

ArtificialNeuralNetwork::~ArtificialNeuralNetwork()
{
  std::cout << "ArtificialNeuralNetwork::~ArtificialNeuralNetwork()" << std::endl;
}


void ArtificialNeuralNetwork::setUp(const int )//a_nrInputs)
{
  std::cout << "ArtificialNeuralNetwork::setUp()" << "\n";
  
  /*
  m_layers = cv::Mat(4, 1, CV_32SC1);
  m_layers.row(0) = cv::Scalar(2);
  m_layers.row(1) = cv::Scalar(10);
  m_layers.row(2) = cv::Scalar(15);
  m_layers.row(3) = cv::Scalar(1);

  m_criteria.max_iter = 100;
  m_criteria.epsilon = 0.00001f;
  m_criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
  m_params.train_method = CvANN_MLP_TrainParams::BACKPROP;
  m_params.bp_dw_scale = 0.05f;
  m_params.bp_moment_scale = 0.05f;
  m_params.term_crit = m_criteria;

  m_mlp.create(m_layers);
  */
}

void ArtificialNeuralNetwork::tearDown()
{
  std::cout << "ArtificialNeuralNetwork::tearDown()" << "\n";
}

float ArtificialNeuralNetwork::doClassification(const cv::Mat* a_subImage)
{
  int32_t imgSize = 30;
  const int32_t nrFeatures = imgSize*imgSize;
  cv::Mat grayImage;
  float* featureVector = new float[nrFeatures];
  cv::resize(*a_subImage, grayImage, cv::Size(imgSize, imgSize), 0, 0, cv::INTER_CUBIC);
  cv::cvtColor(grayImage, grayImage, CV_BGR2GRAY);

  //std::cout << "(width, height): (" << grayImage.cols << ", " << grayImage.rows << ")\n";
  int32_t idx = 0;
  for (int32_t i=0; i<grayImage.rows; i++) {
    for (int32_t j=0; j<grayImage.cols; j++) {
      float rescaledIntensity = ((float)grayImage.at<unsigned char>(i,j) - 127)/255;
      //std::cout << "rescaledIntensity: " << rescaledIntensity << "\n";
      *(featureVector+idx) = rescaledIntensity;
      //std::cout << "*(featureVector+idx): " << *(featureVector+idx) << std::endl;
      idx++;
    }
  }
  cv::Mat featureVectorMat(1, nrFeatures, CV_32FC1, featureVector);
  cv::Mat output = cv::Mat(1, 1, CV_32SC1);

  //std::cout << "*(featureVector): " << *(featureVector) << std::endl;
  //std::cout << "featureVectorMat:" << std::endl << featureVectorMat << std::endl;

  m_mlp->predict(featureVectorMat, output);
  float result = output.at<float>(0);


  delete[] featureVector;
  output.release();
  featureVectorMat.release();
  grayImage.release();

  return result;
}


void ArtificialNeuralNetwork::saveNetworkToFile()
{
  std::cout << "ArtificialNeuralNetwork::saveNetworkToFile()" << "\n";
  cv::FileStorage fs("mlp.xml", cv::FileStorage::WRITE); 
  m_mlp->write(*fs, "mlp"); 
}

void ArtificialNeuralNetwork::loadNetworkFromFile()
{
  std::cout << "ArtificialNeuralNetwork::loadNetworkFromFile()" << "\n";
  m_mlp->load("./share/opendlv/system/application/perception/detectvehicle/trainedAnn.xml","mlp");
}

float ArtificialNeuralNetwork::evaluate(cv::Mat& predicted, cv::Mat& actual) {
  assert(predicted.rows == actual.rows);
  int t = 0;
  int f = 0;
  for(int i = 0; i < actual.rows; i++) {
    float p = predicted.at<float>(i,0);
    float a = actual.at<float>(i,0);
    if((p > 0.0f && a > 0.0f) || (p < 0.0f &&  a < 0.0f)) {
      t++;
    } else {
      f++;
    }
  }
  return (t * 1.0) / (t + f);
}
