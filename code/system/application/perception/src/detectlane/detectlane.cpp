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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "detectlane/Drawing.h"
#include "detectlane/DecisionMaking.h"
#include "detectlane/ExtractingRegions.h"
#include "detectlane/ProcessImage.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/odcore/base/KeyValueConfiguration.h"


#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "detectlane/detectlane.hpp"

namespace opendlv {
namespace perception {
namespace detectlane {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DetectLane::DetectLane(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-detectlane"),
    m_setup(false),
    m_width(640),
    m_height(480),
    m_outputWidth(),
    m_outputHeight(),
    m_maxRow(450),
    m_minRow(200),
    m_midRegion(),
    m_threshold(180),
    m_houghThreshold(100),
    m_cannyThresholdTrue(1),
    m_standardLaneWidth(3.75),
    m_initialized(false),
    m_regions(),
    m_leftCameraRegions(),
    m_rightCameraRegions(),
    m_lines(),
    m_leftLines(),
    m_rightLines(),
    m_nPoints(),
    m_nRegions(),
    m_recoveredPoints(),
    m_k(),
    m_m(),
    m_K(),
    m_M(),
    m_pointsPerRegion(),
    m_regionIndex(),
    m_laneLocation2(),
    m_leftIpm(),
    m_rightIpm(),
    m_transformationMatrix(),
    m_leftTransformationMatrix(),
    m_rightTransformationMatrix(),
    m_scale(),
    m_sourceName()

{
}

DetectLane::~DetectLane()
{
}

/**
 * Receives .
 * Sends .
 */
void DetectLane::nextContainer(odcore::data::Container &c)
{
	// Code to read data
	// TODO: Implement a way to see if shared image is from left or right camera.
	// Algorithm is needed to set the search regions depending on the 
	if (c.getDataType() != odcore::data::image::SharedImage::ID()) {
    return;
  }
//  std::cout << "Received container of type " << c.getDataType() << 
//      " sent at " <<   c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << 
//      std::endl;
    
  if(m_setup == true)
  {
    odcore::data::image::SharedImage mySharedImg = 
        c.getData<odcore::data::image::SharedImage>();


    std::string cameraName = mySharedImg.getName();
    //std::cout << "Received image from camera " << cameraName  << "!" << std::endl;

    if (m_sourceName.compare(cameraName) != 0) {
      // Received image from a source that this instance should not care about
      //std::cout << "Received image came from wrong source. Expected " << m_sourceName << std::endl;
      return;
    }

    std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
          odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
              mySharedImg.getName()));
    
    const uint32_t nrChannels = 3;//mySharedImg.getBytesPerPixel();;
    int imgWidth = mySharedImg.getWidth();
    int imgHeight = mySharedImg.getHeight();
    cv::Mat image;
   
    IplImage* myIplImage;
    myIplImage = cvCreateImage(cvSize(imgWidth, imgHeight),
        IPL_DEPTH_8U, nrChannels);
    cv::Mat src(myIplImage);
    cv::Mat color_dst;

    if (!sharedMem->isValid()) {
      cvReleaseImage(&myIplImage);
      std::cout<<"Invalid memory\n";
      return;
    }
    
    sharedMem->lock();
    {
      memcpy(src.data, sharedMem->getSharedMemory(),
          imgWidth*imgHeight*nrChannels);
    }
    sharedMem->unlock();
    
    m_initialized = false;
    if(mySharedImg.getName() == "front-left"){
      m_transformationMatrix = m_leftTransformationMatrix;
      m_leftIpm->ApplyHomography(src,src);
      m_initialized = true;
    } else if(mySharedImg.getName() == "front-right"){
      m_transformationMatrix = m_rightTransformationMatrix;
      m_rightIpm->ApplyHomography(src,src);
      m_initialized = true;
    }

    if(!m_initialized){
      cvReleaseImage(&myIplImage);
      return;
    }

    if(m_cannyThresholdTrue == 1){
      Canny(src,src,m_threshold,3*m_threshold,3);
      //medianBlur(src,src,3);
    }
    else
    {
      cv::inRange(src, cv::Scalar(m_threshold, m_threshold, m_threshold), cv::Scalar(255, 255, 255), src);
    }

    // Make output image into a 3 channel BGR image
    cvtColor( src, color_dst, CV_GRAY2BGR );
    
    // Vector holder for each line (rho,theta)
    vector<Vec2f> lines;
  
    // OpenCV function that uses the Hough transform and finds the "strongest" lines in the transformation
    
    //TODO: It is possible to filter out lines with certain theta angles. Thus we know that our lane will
    //      consist of lines that will probably look like: // | \\ but not ---
    cv::HoughLines( src, lines, 1, CV_PI/180, m_houghThreshold );

    // Holder for the mean (rho,theta) for each group
    vector<Vec2f> groups;
    if(lines.size() != 0){

      // Draw all of the lines found in red
      for( size_t i = 0; i < lines.size(); i++ )
      {
        float rho = lines[i][0];
        float theta = lines[i][1];
      
      
        float a = cos(theta), b = sin(theta);
        float x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( color_dst, pt1, pt2, Scalar(0,0,255), 3, 8 );
      }




      GetGrouping(groups,lines);   

      std::cout<<"Group size " << groups.size()<<std::endl;
      // Get parametric line representation
      std::vector<cv::Vec2f> p, m;

      GetParametricRepresentation(p,m,groups);    
    
      // Get points on lines
      std::vector<cv::Vec2f> xPoints,yPoints;
      std::vector<cv::Vec2f> X, Y;
      float row1 = 100, row2 = 480;
      GetPointsOnLine(xPoints,yPoints,X,Y,p,m,row1,row2);
    
      // Pair up lines to form a surface
      std::vector<cv::Vec2i> groupIds;
      GetLinePairs(xPoints,yPoints,groupIds);


      int leftLane =  groupIds[0][0];
      int rightLane = groupIds[0][1];

      std::cout << "Leftlane\n" << X[leftLane][1] << " " << Y[leftLane][1]<<std::endl;
      std::cout<< X[leftLane][0] << " " << Y[leftLane][0] << std::endl;
      std::cout << "Rightlane\n" << X[rightLane][1] << " " << Y[rightLane][1]<<std::endl;
      std::cout << X[rightLane][0] << " " << Y[rightLane][0]<<std::endl;
      cv::Mat tmp;
      color_dst.copyTo(tmp);

      for(uint i = 0; i < groups.size(); i++){

        float rho = groups[i][0];
        float theta = groups[i][1];
      
      
        float a = cos(theta), b = sin(theta);
        float x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( tmp, pt1, pt2, Scalar(0,255,0), 3, 8 );


      }

      for(uint i = 0; i < groupIds.size(); i++){
    
        Point p1(xPoints[groupIds[i][0]][1],yPoints[groupIds[i][0]][1]);
        Point p2(xPoints[groupIds[i][1]][1],yPoints[groupIds[i][1]][1]);
        Point p3(xPoints[groupIds[i][1]][0],yPoints[groupIds[i][1]][0]);
        Point p4(xPoints[groupIds[i][0]][0],yPoints[groupIds[i][0]][0]);
                
        line( tmp, p1, p2, Scalar(255,0,255), 3, 8 );
        line( tmp, p2, p3, Scalar(255,0,255), 3, 8 );
        line( tmp, p3, p4, Scalar(255,0,255), 3, 8 );
        line( tmp, p4, p1, Scalar(255,0,255), 3, 8 );
    
        //waitKey(0);

        

        odcore::data::TimeStamp imageTimeStamp = c.getSentTimeStamp();
        std::string type = "surface";
        float typeConfidence = 1;

        std::vector<opendlv::model::Cartesian3> edges;

        float xBotLeft = X[groupIds[i][0]][1];
        float xBotRight = X[groupIds[i][1]][1];
        float xTopRight = X[groupIds[i][1]][0];
        float xTopLeft = X[groupIds[i][0]][0];

        float yBotLeft = Y[groupIds[i][0]][1];
        float yBotRight = Y[groupIds[i][1]][1];
        float yTopRight = Y[groupIds[i][1]][0];
        float yTopLeft = Y[groupIds[i][0]][0];

        edges.push_back(opendlv::model::Cartesian3(xBotLeft,yBotLeft,1));
        edges.push_back(opendlv::model::Cartesian3(xBotRight,yBotRight,1));
        edges.push_back(opendlv::model::Cartesian3(xTopRight,yTopRight,1));
        edges.push_back(opendlv::model::Cartesian3(xTopLeft,yTopLeft,1));

        float edgesConfidence = 1;

        bool traversable = true;
        float confidence = 1;
        std::vector<std::string> sources;
        sources.push_back(mySharedImg.getName());

        std::vector<std::string> properties;

        int16_t surfaceId = i;

        std::vector<int16_t> connectedWidth;
        std::vector<int16_t> traversableTo;

        opendlv::perception::Surface detectedSurface(imageTimeStamp,
            type,
            typeConfidence,
            edges,
            edgesConfidence,
            traversable,
            confidence,
            sources,
            properties,
            surfaceId,
            connectedWidth,
            traversableTo);


        odcore::data::Container objectContainer(detectedSurface);
        getConference().send(objectContainer);

      
      }
      namedWindow( "Tmp", 3 );
      imshow("Tmp",tmp);
    
    }


    char key = cv::waitKey(1);
    if( key == 'w')
      m_threshold += 10;
    else if( key == 's')
      m_threshold = std::max(10,m_threshold-10);
    else if( key == 'p')
      m_cannyThresholdTrue *= -1;
    else if( key == 'i')
      m_houghThreshold += 10;
    else if( key == 'k')
      m_houghThreshold = std::max(10,m_houghThreshold-10);

    cvReleaseImage(&myIplImage);


     
  }
}

void DetectLane::GetLinePairs(std::vector<cv::Vec2f> &xPoints,std::vector<cv::Vec2f> &,std::vector<cv::Vec2i> &groupIds)
{
  for(uint i = 0; i <xPoints.size()-1; i++)
  {
    for(uint j= i+1; j < xPoints.size(); j++)
    {
      float leftId, rightId;
      if(xPoints[i][1] < xPoints[j][1])
        leftId = i, rightId = j;
      else
        leftId = j, rightId = i;
      
      // float xDiff1 = xPoints[rightId][0] - xPoints[leftId][0];
      // float xDiff2 = xPoints[rightId][1] - xPoints[leftId][1];
      // Lane width has to be  2 < w < 5 to be valid
      //if( xDiff1 > 0 && xDiff1 < 10 && xDiff2 > 0 && xDiff2 < 10){
          groupIds.push_back(cv::Vec2f(leftId,rightId));
      //}
    }
  }
}


void DetectLane::GetParametricRepresentation(std::vector<cv::Vec2f> &p,std::vector<cv::Vec2f> &m,std::vector<cv::Vec2f> &groups)
{
  for(uint i = 0; i < groups.size(); i++){
    float rho = groups[i][0];
    float theta = groups[i][1];
    
    float heading = -((float)CV_PI / 2.0f - theta);
    float a = cos(theta), b = sin(theta);
    float x0 = a*rho, y0 = b*rho;
    
    p.push_back(cv::Vec2f(cos(heading),sin(heading)));
    m.push_back(cv::Vec2f(x0,y0));

  }
  
}

void DetectLane::GetPointsOnLine(std::vector<cv::Vec2f> &xPoints,std::vector<cv::Vec2f> &yPoints,
    std::vector<cv::Vec2f> &X, std::vector<cv::Vec2f> &Y,
    std::vector<cv::Vec2f> &p,std::vector<cv::Vec2f> &m,float row1, float row2)
{
  for (uint i = 0; i < p.size(); i++)
  {
  
    // y1 = t * sin(heading) + y0
    // -> t = y1 - y0 / (sin(heading))
    // x1 = t * cos(heading) + x0
    
    float t1,t2;
    if(fabs(p[i][1]) > 0.000001f){
      t1 = ( row1 - m[i][1]) / sin(p[i][1]);
      t2 = ( row2 - m[i][1]) / sin(p[i][1]);
    }
    else {
      t1 = 0;
      t2 = 0;
    }
    Eigen::Vector3d point1, point2;
    
    float x1 = (t1 * p[i][0] + m[i][0]); 
    point1 << x1, row1, 1;
    TransformPointToGlobalFrame(point1);

    float x2 = (t2 * p[i][0] + m[i][0]); 
    point2 << x2, row2, 1;
    TransformPointToGlobalFrame(point2);

    X.push_back(cv::Vec2f(point1(0),point2(0)));
    Y.push_back(cv::Vec2f(point1(1),point2(1)));

    xPoints.push_back(cv::Vec2f(x1,x2));
    yPoints.push_back(cv::Vec2f(row1,row2));
    
  }
}


void DetectLane::ReadWarpPointsMatrix(std::vector<cv::Point2f> & a_points,std::string a_fileName)
{
  std::ifstream file(a_fileName);

  if(file.is_open()){

    for(uint i = 0; i < a_points.size(); i++){
      file >> a_points.at(i).x;
      file >> a_points.at(i).y;
    }

  }

  file.close();

}

void DetectLane::ReadInputWindowSize(std::string a_inputSizeString)
{
  std::stringstream stringStream(a_inputSizeString);
  std::vector<std::string> result;

  while( stringStream.good() )
  {
    std::string subString;
    getline(stringStream, subString, ',' );
    result.push_back( subString );
  }
  m_width = atoi(result.at(0).c_str());
  m_height = atoi(result.at(1).c_str());

}

void DetectLane::ReadOutputWindowSize(std::string a_outputSizeString)
{
  std::stringstream stringStream(a_outputSizeString);
  std::vector<std::string> result;

  while( stringStream.good() )
  {
    std::string subString;
    getline(stringStream, subString, ',' );
    result.push_back( subString );
  }
  m_outputWidth = atoi(result.at(0).c_str());
  m_outputHeight = atoi(result.at(1).c_str());

}


void DetectLane::setUp()
{

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  m_sourceName = kv.getValue<std::string>("perception-detectlane.source");
  std::cout << "This DetectLane instance will receive images from " << m_sourceName << "." << std::endl;

  std::string inputSizeString = kv.getValue<std::string>("perception-detectlane.inputSize");
  ReadInputWindowSize(inputSizeString);

  m_width = 1280;
  m_height = 720;
  std::string outputSizeString = kv.getValue<std::string>("perception-detectlane.outputSize");
  ReadOutputWindowSize(outputSizeString);

  std::vector<cv::Point2f> leftRegionPoints(4);
  ReadWarpPointsMatrix(leftRegionPoints,"./share/opendlv/tools/vision/projection/leftCameraWarpPoints.csv");
  std::cout<<"Left region warp points\n";
  std::cout<<leftRegionPoints<<std::endl;
  std::vector<cv::Point2f> rightRegionPoints(4);
  ReadWarpPointsMatrix(rightRegionPoints, "./share/opendlv/tools/vision/projection/rightCameraWarpPoints.csv");

  std::cout<<"Right region warp points\n";
  std::cout<<rightRegionPoints<<std::endl;
  std::vector<cv::Point2f> outputPoints;
  cv::Size inputSize(m_width,m_height);
  cv::Size outputSize(m_outputWidth, m_outputHeight);
  outputPoints.push_back(cv::Point2f(0, m_outputHeight) );
  outputPoints.push_back(cv::Point2f(m_outputWidth, m_outputHeight) );
  outputPoints.push_back(cv::Point2f(m_outputWidth, 0) );
  outputPoints.push_back(cv::Point2f(0, 0) ); 


  m_leftIpm.reset(new InversePerspectiveMapping(
          inputSize,outputSize,
          leftRegionPoints,outputPoints));
  m_rightIpm.reset(new InversePerspectiveMapping(
          inputSize, outputSize,
          rightRegionPoints, outputPoints));
    
  m_regions = Eigen::MatrixXd(7,4);

  //-----------------------------
  // Scaling: Calibrations were made for 640x480 resolution
  //-----------------------------
  m_minRow = 150 * m_outputHeight  / 480.0;
  m_maxRow = 450 * m_outputHeight / 480.0;
  /* OLD CODE 
  std::string leftCameraRegionsFile = 
    "./share/opendlv/system/application/perception/detectlane/leftCameraRegions.csv";
  m_leftCameraRegions = ReadMatrix(leftCameraRegionsFile,
      m_regions.rows(),m_regions.cols());

  std::string rightCameraRegionsFile = 
    "./share/opendlv/system/application/perception/detectlane/rightCameraRegions.csv";
  m_rightCameraRegions = ReadMatrix(rightCameraRegionsFile,
      m_regions.rows(),m_regions.cols());
  */

  m_leftCameraRegions = Eigen::MatrixXd(7,4);
  m_leftCameraRegions <<
88, 196, 88, 227,
240, 201, 240, 368,
285, 206, 285, 442,
305, 201, 305, 474,
325, 202, 325, 454,
400, 199, 400, 298,
540, 174, 540, 198;


    // 221, 122, 13, 233,
    // 267, 114, 33, 461,
    // 293, 112, 327, 462,
    // 299, 101, 444, 474,
    // 303, 100, 528, 469,
    // 319, 105, 609, 332,
    // 336, 87, 630, 201;


  m_rightCameraRegions = Eigen::MatrixXd(7,4);
  m_rightCameraRegions <<
    88, 196, 88, 227,
270, 201, 270, 368,
360, 201, 360, 474,
380, 202, 380, 454,
400, 202, 400, 454,
500, 199, 500, 298,
540, 174, 540, 198;

  m_leftCameraRegions.col(0) *= (m_outputWidth / 640.0);
  m_leftCameraRegions.col(2) *= (m_outputWidth / 640.0);
  m_leftCameraRegions.col(1) *= (m_outputHeight / 480.0);
  m_leftCameraRegions.col(3) *= (m_outputHeight / 480.0);

  m_rightCameraRegions.col(0) *= (m_outputWidth / 640.0);
  m_rightCameraRegions.col(2) *= (m_outputWidth / 640.0);
  m_rightCameraRegions.col(1) *= (m_outputHeight / 480.0);
  m_rightCameraRegions.col(3) *= (m_outputHeight / 480.0);

  m_leftLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_leftCameraRegions,m_leftLines);

  m_rightLines = Eigen::MatrixXd(m_regions.rows(),2);
  GetRegionLinesV2(m_rightCameraRegions, m_rightLines);

  //-----------------------------
  // Image processing parameters
  m_nPoints = (m_maxRow - m_minRow) / 2;
  
  //-----------------------------
  // Initializations
  //-----------------------------
  m_lines = Eigen::MatrixXd(m_regions.rows(), 2);

  // Middle region line index
  m_midRegion = (int)(m_lines.rows()-1)/2;

  m_recoveredPoints = Eigen::MatrixXd(m_nPoints, m_lines.rows()+2);
  
  m_nRegions = m_recoveredPoints.cols()-1;
   
  // Counts the number of points per each region
  m_pointsPerRegion = Eigen::VectorXd(m_nRegions,1);
  
  // Holds the index that decides the left and right road track
  m_regionIndex = Eigen::MatrixXd(2,1);

  m_k = Eigen::MatrixXd(m_regions.rows(),1);
  m_m = Eigen::MatrixXd(m_regions.rows(),1);

  m_K = Eigen::MatrixXd(m_nRegions,1);
  m_M = Eigen::MatrixXd(m_nRegions,1);

  /* OLD CODE 
  m_leftTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/leftCameraTransformationMatrixWarped.csv",3,3);
  std::cout<<m_leftTransformationMatrix;
  std::cout<<"\n------\n----\n---\n";
  m_rightTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/rightCameraTransformationMatrixWarped.csv",3,3);
  */

  // NEW CODE
  m_leftTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/leftCameraTransformationMatrixWarped.csv",3,3);
  std::cout<<m_leftTransformationMatrix;
  std::cout<<"\n------\n----\n---\n";
  m_rightTransformationMatrix = ReadMatrix(
          "/opt/opendlv/share/opendlv/tools/vision/projection/rightCameraTransformationMatrixWarped.csv",3,3);
  // --------------------------------

  m_scale << m_width / (double)m_outputWidth, m_height / (double)m_outputHeight, 1;
 
  if(false)
  {
    std::cout<<"Input size\n";
    std::cout<<m_width<< " " << m_height << std::endl;
    std::cout<<"Output size\n",
    std::cout << m_outputWidth << " " << m_outputHeight<<std::endl;
    std::cout<<"Left camera regions:\n";
    std::cout<<m_leftCameraRegions;
    std::cout<<std::endl;
    std::cout<<"Right camear regions:\n";
    std::cout<<m_rightCameraRegions<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Left camera transformation matrix\n";
    std::cout<<m_leftTransformationMatrix<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Right camera transformation matrix\n";
    std::cout<<m_rightTransformationMatrix<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Left camera warp points :\n";
    for(int i = 0; i < 4; i++)
      std::cout<<leftRegionPoints.at(i).x << " " << leftRegionPoints.at(i).y<<std::endl;
    std::cout<<"Right camera warp points :\n";
    for(int i = 0; i < 4; i++)
      std::cout<<rightRegionPoints.at(i).x << " " << rightRegionPoints.at(i).y<<std::endl;


  }
  m_setup = true;
 }

void DetectLane::tearDown()
{
}

Eigen::MatrixXd DetectLane::ReadMatrix(std::string fileName, int nRows,
    int nCols)
{
  std::ifstream file(fileName);

  Eigen::MatrixXd matrix(nRows,nCols);


  if (file.is_open()) {
    for(int i = 0; i < nRows; i++){
      for(int j = 0; j < nCols; j++){
        double item = 0.0;
        file >> item;

   
        matrix(i,j) = item;

      }
    }
  }
  
  file.close();

  return matrix;

}

void DetectLane::TransformPointToGlobalFrame(Eigen::Vector3d &point)
{
  // std::cout<<"point before \n";
  // std::cout<<point<<std::endl;
  point = point.cwiseProduct(m_scale);
  // std::cout<<"point after 1\n";
  // std::cout<<point<<std::endl;
  point = m_transformationMatrix * point;

  // std::cout<<"point final before \n";
  // std::cout<<point<<std::endl;

  point = point / point(2);
  // std::cout<<"point final \n";
  // std::cout<<point<<std::endl;

}

double DetectLane::GetHeadingAngle(double kLeft,double mLeft, double kRight,
    double mRight, double row1, double row2, int leftPointIndex, 
    int rightPointIndex )

{
  if( leftPointIndex == rightPointIndex )
  {
    double col1 = kLeft * row1 + mLeft;
    Eigen::Vector3d point1(col1, row1, 1);
    TransformPointToGlobalFrame(point1);

    double dxPoint1 = point1(0);
    double dyPoint1 = point1(1);

    double col2 = kLeft * row2 + mLeft;
    Eigen::Vector3d point2(col2,row2,1);
    TransformPointToGlobalFrame(point2);

    double dxPoint2 = point2(0);
    double dyPoint2 = point2(1);

    return std::atan2(dyPoint1-dyPoint2,dxPoint1-dxPoint2);

  }
  else 
  {
    //Find x,y point of middle of lane for row1
    double colLeft1 = kLeft * row1 + mLeft;
    double colRight1 = kRight * row1 + mRight;

    Eigen::Vector3d leftPoint1(colLeft1, row1, 1);
    TransformPointToGlobalFrame(leftPoint1);
    
    Eigen::Vector3d rightPoint1(colRight1, row1, 1);  
    TransformPointToGlobalFrame(rightPoint1);

    double dxPoint1 = (leftPoint1(0) + rightPoint1(0) ) / 2.0;
    double dyPoint1 = (leftPoint1(1) + rightPoint1(1) ) / 2.0;

    //Find x,y point of middle of lane for row2
    double colLeft2 = kLeft * row2 + mLeft;
    double colRight2 = kRight * row2 + mRight;

    Eigen::Vector3d leftPoint2(colLeft2, row2, 1);
    TransformPointToGlobalFrame(leftPoint2);


    Eigen::Vector3d rightPoint2(colRight2, row2, 1);  
    TransformPointToGlobalFrame(rightPoint2);

    double dxPoint2 = (leftPoint2(0) + rightPoint2(0) ) / 2.0;
    double dyPoint2 = (leftPoint2(1) + rightPoint2(1) ) / 2.0;

    return std::atan2(dyPoint1-dyPoint2,dxPoint1-dxPoint2);
  }
}



double DetectLane::GetLaneOffset(double kLeft,double mLeft, double kRight,
    double mRight,double row, int leftPointIndex,int rightPointIndex)
{
  if(leftPointIndex == rightPointIndex)
  {
    std::cout<<"Using only one lane marking\n";
    double col = kLeft * row + mLeft;
    Eigen::Vector3d point(col, row, 1);
    TransformPointToGlobalFrame(point);

    std::cout<<"Lane is at position : " << point(1) <<std::endl;
    if(leftPointIndex <= m_nRegions / 2 - 1)
      return ( point(1) - m_standardLaneWidth / 2 );
    else
      return ( point(1) + m_standardLaneWidth / 2 ); 
  
  }
  else 
  {
    double colLeft = kLeft * row + mLeft;
    double colRight = kRight * row + mRight;

    Eigen::Vector3d leftPoint(colLeft, row, 1);
    TransformPointToGlobalFrame(leftPoint);
   
    Eigen::Vector3d rightPoint(colRight, row, 1);  
    TransformPointToGlobalFrame(rightPoint);
    return (leftPoint(1) + rightPoint(1)) / 2.0;
  }
}

void DetectLane::GetGrouping(std::vector<cv::Vec2f> &groups, std::vector<cv::Vec2f> &lines)
{
  std::vector< std::vector<cv::Vec2f> > group;
  std::vector<cv::Vec2f> groupMean, groupSum;
  std::vector<cv::Vec2f> tmp;
  group.push_back(tmp);
  group.at(0).push_back(lines.at(0));
  groupMean.push_back(group.at(0).at(0));
  groupSum.push_back(group.at(0).at(0));
  
  double radius = 100;
  
  for(uint i = 1; i < lines.size(); i++)
  {
    bool groupAssigned = false;
    for(uint j = 0; j < group.size(); j++)
    {
      double xDiff = lines[i][0] - groupMean[j][0];
      double yDiff = lines[i][1] - groupMean[j][1];
      double absDiff = sqrt(pow(xDiff,2) + pow(yDiff,2));

      if( absDiff <= radius)
      {
        group.at(j).push_back(lines.at(i));
        groupSum.at(j) += lines.at(i);
        groupMean.at(j)[0] = groupSum.at(j)[0] / group.at(j).size();
        groupMean.at(j)[1] = groupSum.at(j)[1] / group.at(j).size();
        groupAssigned = true;
        break;
      }
      
    }
    if(!groupAssigned)
    {
      group.push_back(tmp);
      group.at(group.size()-1).push_back(lines.at(i));
      groupMean.push_back(lines.at(i));
      groupSum.push_back(lines.at(i));
    }
  }
  groups = groupMean;
    
}




} // detectlane
} // perception
} // opendlv
