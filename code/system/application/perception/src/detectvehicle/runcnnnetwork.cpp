
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

#include <tiny_cnn/tiny_cnn.h>

#include "convnetfunctions.hpp"



void VisualizeNetwork(int32_t w, int32_t h);

void CnnTest();


void CnnTest() {
  std::cout << "Inside the CNN stuff!" << std::endl;
}



////////////////////////////////////////////////////////////////////////////////

/*
int main(int argc, const char * argv[]) {
  std::cout << "... Starting program \"runcnnnetwork.exe\"..." << std::endl;


  int32_t w = 200;
  int32_t h = 112;
  //int32_t w = 100;
  //int32_t h = 56;
  //int32_t w = 160;
  //int32_t h = 84;

  VisualizeNetwork(w, h);
}
*/

////////////////////////////////////////////////////////////////////////////////


void VisualizeNetwork(int32_t width, int32_t height)
{
  tiny_cnn::network<tiny_cnn::mse, tiny_cnn::gradient_descent> nn;


  bool isTrainingNetwork = false;
  GetConvNet(width, height, &nn, isTrainingNetwork);

  
  
  std::cout << "load net from file..." << std::endl;

  std::ifstream input("trained_cnn");
  input >> nn;


  std::cout << "loaded." << std::endl;

  std::cout << "Reading normalization constants..." << std::endl;
  std::ifstream normInput("normalization_constants");

  /* // Grayscale images
  double normAvg;
  double normStd;
  normInput >> normAvg;
  normInput >> normStd;
  std::cout << "(avg, std): (" << normAvg << ", " << normStd << ")" << std::endl;
  */

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


  // test and show results
  std::vector<tiny_cnn::vec_t> testImage;

  // Testing against video
  const char* videoPath = "/home/plankton/Videos/GCDC_videos/testrun_gbg.mkv";
  CvCapture* capture;
  capture = cvCreateFileCapture(videoPath);
  cv::Mat src;

  // Testing against specific image files
  std::string testImgPath = "/home/plankton/Desktop/testImg3.png";


  // Continue until breaking out
  while(1) {
    int32_t frameSkip = 100;
    for (int32_t i=0; i<frameSkip; i++) {
      src = cvQueryFrame(capture);
    }

    if (src.empty()) {
      // Reached the end of the video
      break;
    }

    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

    cv::Mat myTestImage = src.clone(); // Using video
    //cv::Mat myTestImage = cv::imread(testImgPath); // Using specific file

    /* // Grayscale images
    cv::cvtColor(src, src, CV_BGR2GRAY);
    ConvertImage(src, width, height, testImage); // Using video
    ConvertImage(testImgPath, width, height, testImage); // Using specific file
    ApplyNormalization(testImage, normAvg, normStd);
    */
    
    // RGB images
    ConvertImageRGB(src, width, height, testImage); // Using video
    //ConvertImageRGB(testImgPath, width, height, testImage); // Using specific file
    ApplyNormalizationRGB(testImage, normAvgR, normStdR, normAvgG, normStdG, normAvgB, normStdB);

    // For displaying (each pixel increased in size)
    double myTestFactor = 4;
    cv::resize(myTestImage, myTestImage, cv::Size(width*myTestFactor, height*myTestFactor), 0, 0, cv::INTER_CUBIC);

    // Do the actual CNN prediction
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    tiny_cnn::vec_t result = nn.predict(testImage.at(0));
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    // Pick final layer image
    auto out_img = nn[nn.depth()-1]->output_to_image();

    cv::Mat spatialMap = image2mat(out_img, 1);

    // Pick only the "vehicle" feature
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
      cv::rectangle(myTestImage, bRect, cv::Scalar(0, 0, 255));
    }


    cv::imshow("Final spatial map", spatialMap);
    cv::imshow("Thresholded spatial map", thresholdedImg);
    //cv::imshow("Original image", myTestImage);

    /* // Display the intermediate layers of the network
    for (size_t i = 0; i < nn.depth(); i++) {
      if (i==0 || i==2 || i==4) {
        auto img = nn.at<tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>>(i).weight_to_image();
        //cv::imshow("conv layer: " + std::to_string(i), image2mat(img, 5));
      }
      else if (i==5) {
        auto img = nn.at<tiny_cnn::convolutional_layer<tiny_cnn::activation::relu>>(i).weight_to_image();
        //cv::imshow("conv layer: " + std::to_string(i), image2mat(img, 5));
      }
      else if (i!=3) {
        auto out_img = nn[i]->output_to_image();
        //cv::imshow("layer:" + std::to_string(i), image2mat(out_img, 4));
      }
    }
    */


    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

    auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>( t1 - t0 ).count();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();

    std::cout << "Preprocess time:  " <<  ((float)duration0)/1000 << " ms" << std::endl;
    std::cout << "CNN time:         " <<  ((float)duration1)/1000 << " ms" << std::endl;
    std::cout << "Postprocess time: " <<  ((float)duration2)/1000 << " ms" << std::endl << std::endl;
    
    testImage.clear();
    cv::waitKey(10);
    //cv::waitKey(0);
  }

}



