#ifndef CONVNETFUNCTIONS_HPP
#define CONVNETFUNCTIONS_HPP

#include <iostream>
#include <cmath>


void GetConvNet(
    int32_t w, int32_t h,
    tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent>* nn,
    bool isTrainingNetwork);


void ConvertImage(
    const std::string& directory,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data);

void ConvertImage(
    cv::Mat img,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data);

void ConvertImageRGB(
    const std::string& directory,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data);

void ConvertImageRGB(
    cv::Mat img,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data);

void ConvertImageDirectory(
    const std::string& directory,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data,
    std::vector<tiny_cnn::label_t>& labels);

void NormalizeData(std::vector<tiny_cnn::vec_t>& data);

void NormalizeDataRGB(std::vector<tiny_cnn::vec_t>& data);

void ApplyNormalization(std::vector<tiny_cnn::vec_t>& data,
    double normAvg, double normStd);

void ApplyNormalizationRGB(std::vector<tiny_cnn::vec_t>& data,
    double normAvgR, double normStdR,
    double normAvgG, double normStdG,
    double normAvgB, double normStdB);


bool endsWith(const std::string &str, const std::string &suffix);
cv::Mat image2mat(tiny_cnn::image<>& img, int32_t sizeIncrease);


////////////////////////////////////////////////////////////////////////////////



void GetConvNet(
    int32_t w, int32_t h,
    tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent>* nn,
    bool isTrainingNetwork)
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
  

  (*nn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(w, h, 5, nrChannels, nrFeat1, tiny_cnn::padding::same) // wxh in, 5x5 kernel, 1-6 fmaps conv
        << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(w, h, nrFeat1, 2) // 28x28 in, 6 fmaps, 2x2 subsampling
        << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(w/2, h/2, 3, nrFeat1, nrFeat2, tiny_cnn::padding::same)
        << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(w/2, h/2, nrFeat2, 2)
        << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(w/4, h/4, 3, nrFeat2, nrFeat3, tiny_cnn::padding::same);

  if (isTrainingNetwork) {
    (*nn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(w/4, h/4, 1, nrFeat3, 2, tiny_cnn::padding::same)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(w/4, h/4, 2, 1)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::identity>(w/4, h/4, 2, w/4);
  }
  else { // if for actual usage
    (*nn) << tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>(w/4, h/4, 1, nrFeat3, 2, tiny_cnn::padding::same)
          << tiny_cnn::max_pooling_layer<tiny_cnn::activation::softmax>(w/4, h/4, 2, 1);
  }
}


// convert all images found in directory to vec_t
void ConvertImageDirectory(
    const std::string& directory,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data,
    std::vector<tiny_cnn::label_t>& labels)
{
  std::string posSamplesPath = "pos";
  std::string negSamplesPath = "neg";

  if (endsWith(directory, "/")) {
    posSamplesPath = directory + posSamplesPath;
    negSamplesPath = directory + negSamplesPath;
  } else {
    posSamplesPath = directory + "/" + posSamplesPath;
    negSamplesPath = directory + "/" + negSamplesPath;
  }
  std::vector<std::string> posFileNames;
  std::vector<std::string> negFileNames;
  std::cout << "Searching for positive samples in: " << posSamplesPath  << "\n";
  cv::glob(posSamplesPath, posFileNames);
  std::cout << "Searching for negative samples in: " << negSamplesPath  << "\n";
  cv::glob(negSamplesPath, negFileNames);
  // total training size = sum of #positives and #negatives
  int32_t totalTrainingSetSize = posFileNames.size() + negFileNames.size();
  std::cout << "Training data consists of " << totalTrainingSetSize << " data points: " << posFileNames.size() << " positives and " << negFileNames.size() << " negatives.\n";

  if (posFileNames.size() > 0 && negFileNames.size() > 0) {
    // valid
  } else {
    // non-valid!
    std::cout << "WARNING: There are either no positive samples or no negative samples. Aborting!\n";
    return;
  }


  // Process the positive training data
  for (uint32_t i=0; i<posFileNames.size(); i++) {
    //ConvertImage(posFileNames[i], w, h, data);
    ConvertImageRGB(posFileNames[i], w, h, data);
    labels.push_back(1);
  }

  // Process the negative training data
  for (uint32_t i=0; i<negFileNames.size(); i++) {
    //ConvertImage(negFileNames[i], w, h, data);
    ConvertImageRGB(negFileNames[i], w, h, data);
    labels.push_back(0);
  }

  //NormalizeData(data);
  NormalizeDataRGB(data);
}

void ConvertImage(
    const std::string& imagefilename,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data)
{
  cv::Mat img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE);

  ConvertImage(img, w, h, data);
}



void ConvertImage(
    cv::Mat img,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data)
{
  if (img.data == nullptr) {
    return; // cannot open, or it's not an image
  }

  cv::Mat_<uint8_t> resized;
  //cv::imshow("Before eq", img);
  cv::equalizeHist(img, img);
  //cv::imshow("After eq", img);
  //cv::waitKey(0);

  cv::resize(img, resized, cv::Size(w, h));
  tiny_cnn::vec_t d;

  std::transform(resized.begin(), resized.end(), std::back_inserter(d),
      [=](uint8_t c) { return c; });
  data.push_back(d);
}

void ConvertImageRGB(
    const std::string& imagefilename,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data)
{
  cv::Mat img = cv::imread(imagefilename, CV_LOAD_IMAGE_COLOR);

  ConvertImageRGB(img, w, h, data);
}

void ConvertImageRGB(
    cv::Mat img,
    int32_t w,
    int32_t h,
    std::vector<tiny_cnn::vec_t>& data)
{
  if (img.data == nullptr) {
    return; // cannot open, or it's not an image
  }
  else if (img.channels() != 3) {
    std::cout << "WARNING: #channels was not 3!" << std::endl;
    return;
  }

  cv::Mat_<cv::Vec3b> resized;
  //cv::imshow("Before eq", img);
  // Possibly equalize image, see http://stackoverflow.com/questions/15007304/histogram-equalization-not-working-on-color-image-opencv
  //cv::imshow("After eq", img);
  //cv::waitKey(0);

  cv::resize(img, resized, cv::Size(w, h));
  
  //cv::imshow("DEBUG", img);
  //cv::waitKey(0);


  float denominator = 255;
  float min = -1;
  float max = 1;
  int32_t padding = 0;
  tiny_cnn::vec_t d((3*h + 2*padding)*(w + 2*padding), min);
  for (int32_t i = 0; i < h; i++) { // Go over all rows
    for (int32_t j = 0; j < w; j++) { // Go over all columns
      for (int32_t c = 0; c < 3; c++) { // Go through all channels
        d[w*h*c + w*(i + padding) + (j + padding)] = resized.at<cv::Vec3b>(i, j)[c]/denominator*(max - min) + min;
      }
    }
  }

  data.push_back(d);
}

void NormalizeData(std::vector<tiny_cnn::vec_t>& data)
{
  std::cout << "Normalizing data..." << std::endl;
  double sum = 0;
  long counter = 0;
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    for (uint32_t j=0; j<currentImg.size(); j++) {
      sum += currentImg.at(j);
      counter++;
    }
  }

  double dataAvg = sum/counter;
  std::cout << "dataAvg: " << dataAvg << std::endl;

  double varSum = 0;
  counter = 0;
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    for (uint32_t j=0; j<currentImg.size(); j++) {
      varSum += pow(dataAvg - currentImg.at(j), 2);
      counter++;
    }
  }

  double dataStd = sqrt(varSum/counter);
  std::cout << "dataStd: " << dataStd << std::endl;

  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    for (uint32_t j=0; j<currentImg.size(); j++) {
      // Transform data to zero-mean, unit-variance
      data.at(i).at(j) = (currentImg.at(j)-dataAvg)/dataStd;
    }
  }

  std::ofstream ofs("normalization_constants");
  ofs << dataAvg << " " << dataStd << "\n";

  std::cout << "Data normalized!" << std::endl;
}

void NormalizeDataRGB(std::vector<tiny_cnn::vec_t>& data)
{
  std::cout << "Normalizing data..." << std::endl;
  double sumR = 0;
  double sumG = 0;
  double sumB = 0;
  long counter = 0;
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
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
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
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

  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      // Transform data to zero-mean, unit-variance
      data.at(i).at(j + imgSize*0) = (currentImg.at(j + imgSize*0)-dataAvgR)/dataStdR;
      data.at(i).at(j + imgSize*1) = (currentImg.at(j + imgSize*1)-dataAvgG)/dataStdG;
      data.at(i).at(j + imgSize*2) = (currentImg.at(j + imgSize*2)-dataAvgB)/dataStdB;
    }
  }

  std::ofstream ofs("normalization_constants");
  ofs << dataAvgR << " " << dataStdR << " "
      << dataAvgG << " " << dataStdG << " "
      << dataAvgB << " " << dataStdB << "\n";

  std::cout << "Data normalized!" << std::endl;
}

void ApplyNormalization(std::vector<tiny_cnn::vec_t>& data,
    double normAvg, double normStd)
{
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    for (uint32_t j=0; j<currentImg.size(); j++) {
      // Transform data to zero-mean, unit-variance
      data.at(i).at(j) = (currentImg.at(j)-normAvg)/normStd;
    }
  }
}

void ApplyNormalizationRGB(std::vector<tiny_cnn::vec_t>& data,
    double normAvgR, double normStdR,
    double normAvgG, double normStdG,
    double normAvgB, double normStdB)
{
  for (uint32_t i=0; i<data.size(); i++) {
    tiny_cnn::vec_t currentImg = data.at(i);
    int32_t imgSize = currentImg.size()/3;
    for (int32_t j=0; j<imgSize; j++) {
      // Transform data to zero-mean, unit-variance
      data.at(i).at(j + imgSize*0) = (currentImg.at(j + imgSize*0)-normAvgR)/normStdR;
      data.at(i).at(j + imgSize*1) = (currentImg.at(j + imgSize*1)-normAvgG)/normStdG;
      data.at(i).at(j + imgSize*2) = (currentImg.at(j + imgSize*2)-normAvgB)/normStdB;
    }
  }
}





bool endsWith(const std::string &str, const std::string &suffix)
{
  return str.size() >= suffix.size() &&
      str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}



cv::Mat image2mat(tiny_cnn::image<>& img, int sizeIncrease) {
  cv::Mat ori(img.height(), img.width(), CV_8U, &img.at(0, 0));
  cv::Mat resized;
  cv::resize(ori, resized, cv::Size(), sizeIncrease, sizeIncrease, cv::INTER_AREA);
  return resized;
}


#endif
