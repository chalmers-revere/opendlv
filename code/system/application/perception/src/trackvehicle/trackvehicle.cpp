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

#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/features2d/features2d.hpp"


#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "detectvehicle/vehicle_s.hpp"

#include "trackvehicle/trackvehicle.hpp"
#include "trackvehicle/vehicle_tex.hpp"

namespace opendlv {
namespace perception {
namespace trackvehicle {


/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
TrackVehicle::TrackVehicle(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-trackvehicle")
    , m_initialised(false)
    , m_debug()
{
}

TrackVehicle::~TrackVehicle()
{
}

void TrackVehicle::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = (kv.getValue<int32_t> ("perception-trackvehicle.debug") == 1);
  std::cout << "Setup complete." << std::endl;
  m_initialised = true;
}
void TrackVehicle::tearDown()
{
}
/**
 * Receives SharedImage from camera.
 * Sends .
 */
void TrackVehicle::nextContainer(odcore::data::Container &c)
{

  if (c.getDataType() == opendlv::perception::VehicleVision::ID() && m_initialised) {
     opendlv::perception::VehicleVision vehicleToTrack = c.getData<opendlv::perception::VehicleVision>();
    std::cout<<vehicleToTrack.getVehicleId()<<std::endl;

     vehicle_t data;
     data.id=vehicleToTrack.getVehicleId();
     data.x=vehicleToTrack.getVehicleX();
     data.y=vehicleToTrack.getVehicleY();
     data.w=vehicleToTrack.getVehicleW();
     data.h=vehicleToTrack.getVehicleH();
     data.found=vehicleToTrack.getVehicleFound();
     updateBuffer(data);

    return;
  }

  if (c.getDataType() != odcore::data::image::SharedImage::ID() || !m_initialised) {

    return;
  }





  odcore::data::image::SharedImage mySharedImg = c.getData<odcore::data::image::SharedImage>();


  std::string cameraName = mySharedImg.getName();
  // std::cout << "Received image from camera " << cameraName  << "!" << std::endl;

  //TODO compare the source name to keep track different camera sources

  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(
      odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(
          mySharedImg.getName()));

  const uint32_t nrChannels = mySharedImg.getBytesPerPixel();
  const uint32_t imgWidth = mySharedImg.getWidth();
  const uint32_t imgHeight = mySharedImg.getHeight();

  IplImage* myIplImage;
  myIplImage = cvCreateImage(cvSize(
      imgWidth, imgHeight), IPL_DEPTH_8U, nrChannels);
  cv::Mat myImage(myIplImage);

  if (!sharedMem->isValid()) {
    return;
  }

  {
    sharedMem->lock();
    memcpy(myImage.data, sharedMem->getSharedMemory(),
        imgWidth*imgHeight*nrChannels);
    sharedMem->unlock();
  }



  cv::imshow("frames",myImage);
  cv::waitKey(1);
  cvReleaseImage(&myIplImage);
}

void TrackVehicle::doTrack(cv::Mat frame){
  // LOOP IN ALL THE VEHICLE_T
      //EXTRACT IMAGE from  the frame  getVehicleFromFrame
      // PUSH / UPDATE to the buufer pushToBuffer
      // matchFeatures
      //show the image for each vehicle
  for (unsigned i = 0; i < vehicle_buffer_t.size(); i++){
   cv::Mat car_image=getVehicleFromFrame(frame,vehicle_buffer_t.at(i));
   vehicle_tex car_tex;
   car_tex.vehicle=vehicle_buffer_t.at(i);
   car_tex.image=car_image;
   //pushToBuffer(car_tex);
   cv::Mat matched=matchFeatures(car_image,frame);
   char buffer[60];
   snprintf(buffer, sizeof(buffer), "%s%d", "Vehicle_ID:", vehicle_buffer_t.at(i).id);
   cv::imshow(buffer,matched);
  }

}

cv::Mat TrackVehicle::getVehicleFromFrame(cv::Mat frame, vehicle_t data){
	  //CUT THE FRAME AND PUT IT INTO CROPPEDVEHICLE
    cv::Rect myROI(data.x, data.y, data.w, data.h);
    cv::Mat croppedImage = frame(myROI);
	  return croppedImage;
  }


void TrackVehicle::updateBuffer(vehicle_t data){
  //TODO: remove from buffer for memory effec

  int index=-1;
  for (unsigned i = 0; i < vehicle_buffer_t.size(); i++){
       if(vehicle_buffer_t.at(i).id==data.id){
         index=i;
       }
  }
  if (index!=-1){
     vehicle_buffer_t.at(index).x=data.x;
     vehicle_buffer_t.at(index).y=data.y;
     vehicle_buffer_t.at(index).w=data.w;
     vehicle_buffer_t.at(index).h=data.h;
     vehicle_buffer_t.at(index).found=data.found;
  }else{
    vehicle_buffer_t.push_back(data);
  }
}

void TrackVehicle::pushToBuffer(vehicle_tex data){
  vehicle_buffer.push_back(data);
}

// returns img_matches
cv::Mat TrackVehicle::matchFeatures(cv::Mat img_object,cv::Mat img_scene){
  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 10;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector.detect( img_object, keypoints_object );
  detector.detect( img_scene, keypoints_scene );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  cv::Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double max_dist = 1000; double min_dist = 10;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

//  printf("-- Max dist : %f \n", max_dist );
//  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 3*min_dist )
     { good_matches.push_back( matches[i]); }
  }





  //--DRAW MATCHES
  cv::Mat img_matches;
  drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Localize the object
  std::vector<Point2f> obj;
  std::vector<Point2f> scene;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
  }
   //????????????????????????? What if the good_matches is 0?
   cv::Mat H = findHomography( obj, scene, CV_RANSAC );

  //-- Get the corners from the image_1 ( the object to be "detected" )
  std::vector<Point2f> obj_corners(4);
  obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
  obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
  std::vector<Point2f> scene_corners(4);

  perspectiveTransform( obj_corners, scene_corners, H);

  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
  line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

  return img_matches;
}


} // trackvehicle
} // perception
} // opendlv
