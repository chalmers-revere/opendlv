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
    , m_inputWidth(200)
    , m_inputHeight(112)
    , m_normAvgR()
    , m_normStdR()
    , m_normAvgG()
    , m_normStdG()
    , m_normAvgB()
    , m_normStdB()
    , m_isInitialized(false)
{
}


ConvNeuralNet::~ConvNeuralNet()
{
}


// Public functions

void ConvNeuralNet::SetUp()
{
  bool isTrainingNetwork = false;
  this->GetConvNet(m_inputWidth, m_inputHeight, &m_cnn, isTrainingNetwork);

  std::cout << "Loading net from file..." << std::endl;
  std::ifstream input(m_pathTrainedCnn);

  if (input.fail()) {
    std::cout << "WARNING! Could not open file " << m_pathTrainedCnn << std::endl;
  }

  input >> m_cnn;
  std::cout << "nn.depth(): " << m_cnn.depth() << std::endl;
  std::cout << "Loaded." << std::endl;


  std::cout << "Reading normalization constants..." << std::endl;
  std::ifstream normInput(m_pathNormalizationConstants);

  if (normInput.fail()) {
    std::cout << "WARNING! Could not open file " << m_pathNormalizationConstants << std::endl;
  }
  // RGB images
  normInput >> m_normAvgR;
  normInput >> m_normStdR;
  normInput >> m_normAvgG;
  normInput >> m_normStdG;
  normInput >> m_normAvgB;
  normInput >> m_normStdB;
  std::cout << "(avgR, stdR): (" << m_normAvgR << ", " << m_normStdR << ")" << std::endl;
  std::cout << "(avgG, stdG): (" << m_normAvgG << ", " << m_normStdG << ")" << std::endl;
  std::cout << "(avgB, stdB): (" << m_normAvgB << ", " << m_normStdB << ")" << std::endl;
  std::cout << "Normalization constants read." << std::endl;

  m_isInitialized = true;
}

bool ConvNeuralNet::IsInitialized()
{
  return m_isInitialized;
}


void ConvNeuralNet::TearDown()
{
}


void ConvNeuralNet::Update(const cv::Mat* a_imageFrame)
{
  std::cout << "convnet : In update" << std::endl;

  std::vector<tiny_cnn::vec_t> cnnImageData;

  std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

  cv::Mat workingImage;
  a_imageFrame->copyTo(workingImage);
  
  // RGB images
  this->ConvertImageRGB(workingImage, m_inputWidth, m_inputHeight, cnnImageData);
  this->ApplyNormalizationRGB(cnnImageData, m_normAvgR, m_normStdR, m_normAvgG, m_normStdG, m_normAvgB, m_normStdB);

  // For displaying (each pixel increased in size)
  double myTestFactor = 4;
  cv::resize(workingImage, workingImage, cv::Size(m_inputWidth*myTestFactor, m_inputHeight*myTestFactor), 0, 0, cv::INTER_CUBIC);

  // Do the actual CNN prediction
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  tiny_cnn::vec_t result = m_cnn.predict(cnnImageData.at(0));
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();


  // Pick final layer image
  auto out_img = m_cnn[m_cnn.depth()-1]->output_to_image();

  cv::Mat spatialMap = this->ImageToMat(out_img, 1);

  // Pick only the "vehicle" (not the "non-vehicle") feature
  cv::Rect roi(spatialMap.cols/2+1, 1, spatialMap.cols-1, spatialMap.rows-1);
  roi.width -= roi.x;
  roi.height -= roi.y;
  spatialMap = spatialMap(roi);
  
  // Draw black lines along the edges to
  // get rid of the edge effects...
  cv::line(spatialMap, cv::Point(0, 0), cv::Point(0, spatialMap.rows-1), cv::Scalar(0, 0, 0), 1, 8);
  cv::line(spatialMap, cv::Point(0, 0), cv::Point(spatialMap.cols-1, 0), cv::Scalar(0, 0, 0), 1, 8);
  cv::line(spatialMap, cv::Point(spatialMap.cols-1, spatialMap.rows-1), cv::Point(0, spatialMap.rows-1), cv::Scalar(0, 0, 0), 1, 8);
  cv::line(spatialMap, cv::Point(spatialMap.cols-1, spatialMap.rows-1), cv::Point(spatialMap.cols-1, 0), cv::Scalar(0, 0, 0), 1, 8);
  

  int32_t resizeFactor = 4;
  cv::resize(spatialMap, spatialMap, cv::Size(), resizeFactor, resizeFactor, cv::INTER_AREA);

  cv::Mat thresholdedImg;
  cv::Mat thresholdedImgBinary;

  double thresh = 70;
  double maxval = 255;
  cv::threshold(spatialMap, thresholdedImg, thresh, maxval, cv::THRESH_TOZERO);
  cv::threshold(spatialMap, thresholdedImgBinary, thresh, maxval, cv::THRESH_BINARY);

  // Find connected components in thresholded image
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  findContours(thresholdedImgBinary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // Draw bounding boxes around the detected connected components
  std::vector<cv::Rect> boundingRects;
  std::cout << "Nr of connected components: " << contours.size() << std::endl;
  for (uint32_t i=0; i<contours.size(); i++) {
    cv::Rect bRect = cv::boundingRect(contours.at(i));

    // Ignore all bounding boxes that are only one (resized) pixel wide or high
    if (bRect.width <= 1*resizeFactor || bRect.height <= 1*resizeFactor) {
      continue;
    }

    boundingRects.push_back(bRect);
    
    // Increase the size of the bounding box to fit the displayed image
    bRect.x *= myTestFactor;
    bRect.y *= myTestFactor;
    bRect.width *= myTestFactor;
    bRect.height *= myTestFactor;
    // Draw bounding boxes in red
    cv::rectangle(workingImage, bRect, cv::Scalar(0, 0, 255));
  }


  cv::imshow("Final spatial map", spatialMap);
  cv::imshow("Thresholded spatial map", thresholdedImg);
  cv::imshow("CNN Output", workingImage);

  cv::waitKey(10);


  std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

  auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
  auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
  auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();

  std::cout << "Preprocess time:  " <<  ((float)duration0)/1000 << " ms" << std::endl;
  std::cout << "CNN time:         " <<  ((float)duration1)/1000 << " ms" << std::endl;
  std::cout << "Postprocess time: " <<  ((float)duration2)/1000 << " ms" << std::endl << std::endl;
}





// Private functions


void ConvNeuralNet::GetConvNet(
    int32_t a_width, int32_t a_height,
    tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent>* a_cnn,
    bool a_isTrainingNetwork)
{
  // works ~ 93 %
  /*
  int nrFeat1 = 12;
  int nrFeat2 = 32;
  int nrFeat3 = 128;
  */

  int32_t nrChannels = 3;
  int32_t nrFeat1 = 12;
  int32_t nrFeat2 = 48;
  int32_t nrFeat3 = 148;
  

  (*a_cnn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(a_width, a_height, 5, nrChannels, nrFeat1, tiny_cnn::padding::same) 
        << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(a_width, a_height, nrFeat1, 2)
        << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(a_width/2, a_height/2, 3, nrFeat1, nrFeat2, tiny_cnn::padding::same)
        << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(a_width/2, a_height/2, nrFeat2, 2)
        << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(a_width/4, a_height/4, 3, nrFeat2, nrFeat3, tiny_cnn::padding::same);

  if (a_isTrainingNetwork) {
    (*a_cnn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(a_width/4, a_height/4, 1, nrFeat3, 2, tiny_cnn::padding::same)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(a_width/4, a_height/4, 2, 1)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(a_width/4, a_height/4, 2, a_width/4);
  }
  else { // if for actual usage
    (*a_cnn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(a_width/4, a_height/4, 1, nrFeat3, 2, tiny_cnn::padding::same)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::softmax>(a_width/4, a_height/4, 2, 1);
  }
}


void ConvNeuralNet::ConvertImageRGB(
    const std::string& a_imagefilename,
    int32_t a_width,
    int32_t a_height,
    std::vector<tiny_cnn::vec_t>& a_data)
{
  cv::Mat img = cv::imread(a_imagefilename, CV_LOAD_IMAGE_COLOR);

  ConvertImageRGB(img, a_width, a_height, a_data);
}

void ConvNeuralNet::ConvertImageRGB(
    cv::Mat a_img,
    int32_t a_width,
    int32_t a_height,
    std::vector<tiny_cnn::vec_t>& a_data)
{
  if (a_img.data == nullptr) {
    std::cout << "WARNING: ConvNeuralNet::ConvertImageRGB() - cannot open, or it's not an image!" << std::endl;
    return; // cannot open, or it's not an image
  }
  else if (a_img.channels() != 3) {
    std::cout << "WARNING: #channels was not 3!" << std::endl;
    return;
  }


  cv::Mat_<cv::Vec3b> resized;
  //cv::imshow("Before eq", a_img);
  // Possibly equalize image, see http://stackoverflow.com/questions/15007304/histogram-equalization-not-working-on-color-image-opencv
  //cv::imshow("After eq", a_img);
  //cv::waitKey(0);

  cv::resize(a_img, resized, cv::Size(a_width, a_height));


  float denominator = 255;
  float min = -1;
  float max = 1;
  int32_t padding = 0;
  tiny_cnn::vec_t d((3*a_height + 2*padding)*(a_width + 2*padding), min);
  for (int32_t i = 0; i < a_height; i++) { // Go over all rows
    for (int32_t j = 0; j < a_width; j++) { // Go over all columns
      for (int32_t c = 0; c < 3; c++) { // Go through all channels
        d[a_width*a_height*c + a_width*(i + padding) + (j + padding)] = resized.at<cv::Vec3b>(i, j)[c]/denominator*(max - min) + min;
      }
    }
  }

  a_data.push_back(d);
}


void ConvNeuralNet::NormalizeDataRGB(std::vector<tiny_cnn::vec_t>& a_data)
{
  std::cout << "Normalizing data..." << std::endl;
  double sumR = 0;
  double sumG = 0;
  double sumB = 0;
  long counter = 0;
  for (uint32_t i=0; i<a_data.size(); i++) {
    tiny_cnn::vec_t currentImg = a_data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      sumR += currentImg.at(j + imgSize*0); // red
      sumG += currentImg.at(j + imgSize*1); // blue
      sumB += currentImg.at(j + imgSize*2); // green
      counter++;
    }
  }

  double dataAvgR = sumR/counter;
  double dataAvgG = sumG/counter;
  double dataAvgB = sumB/counter;
  std::cout << "dataAvgR: " << dataAvgR << std::endl;
  std::cout << "dataAvgG: " << dataAvgG << std::endl;
  std::cout << "dataAvgB: " << dataAvgB << std::endl;

  double varSumR = 0;
  double varSumG = 0;
  double varSumB = 0;
  counter = 0;
  for (uint32_t i=0; i<a_data.size(); i++) {
    tiny_cnn::vec_t currentImg = a_data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      varSumR += pow(dataAvgR - currentImg.at(j + imgSize*0), 2);
      varSumG += pow(dataAvgG - currentImg.at(j + imgSize*1), 2);
      varSumB += pow(dataAvgB - currentImg.at(j + imgSize*2), 2);
      counter++;
    }
  }

  double dataStdR = sqrt(varSumR/counter);
  double dataStdG = sqrt(varSumG/counter);
  double dataStdB = sqrt(varSumB/counter);
  std::cout << "dataStdR: " << dataStdR << std::endl;
  std::cout << "dataStdG: " << dataStdG << std::endl;
  std::cout << "dataStdB: " << dataStdB << std::endl;

  for (uint32_t i=0; i<a_data.size(); i++) {
    tiny_cnn::vec_t currentImg = a_data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      // Transform data to zero-mean, unit-variance
      a_data.at(i).at(j + imgSize*0) = (currentImg.at(j + imgSize*0)-dataAvgR)/dataStdR;
      a_data.at(i).at(j + imgSize*1) = (currentImg.at(j + imgSize*1)-dataAvgG)/dataStdG;
      a_data.at(i).at(j + imgSize*2) = (currentImg.at(j + imgSize*2)-dataAvgB)/dataStdB;
    }
  }

  std::ofstream ofs("normalization_constants");
  ofs << dataAvgR << " " << dataStdR << " "
      << dataAvgG << " " << dataStdG << " "
      << dataAvgB << " " << dataStdB << "\n";

  std::cout << "Data normalized!" << std::endl;
}

void ConvNeuralNet::ApplyNormalizationRGB(std::vector<tiny_cnn::vec_t>& a_data,
    double normAvgR, double normStdR,
    double normAvgG, double normStdG,
    double normAvgB, double normStdB)
{
  for (uint32_t i=0; i<a_data.size(); i++) {
    tiny_cnn::vec_t currentImg = a_data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      // Transform data to zero-mean, unit-variance
      a_data.at(i).at(j + imgSize*0) = (currentImg.at(j + imgSize*0)-normAvgR)/normStdR;
      a_data.at(i).at(j + imgSize*1) = (currentImg.at(j + imgSize*1)-normAvgG)/normStdG;
      a_data.at(i).at(j + imgSize*2) = (currentImg.at(j + imgSize*2)-normAvgB)/normStdB;
    }
  }
}


cv::Mat ConvNeuralNet::ImageToMat(tiny_cnn::image<>& a_img, int a_sizeIncrease) {
  cv::Mat ori(a_img.height(), a_img.width(), CV_8U, &a_img.at(0, 0));
  cv::Mat resized;
  cv::resize(ori, resized, cv::Size(), a_sizeIncrease, a_sizeIncrease, cv::INTER_AREA);
  return resized;
}



} // detectvehicle
} // perception
} // opendlv

