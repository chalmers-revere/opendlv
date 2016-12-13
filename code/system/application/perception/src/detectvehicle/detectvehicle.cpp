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


//---
#include <ctype.h>
#include <cstring>
#include <iostream>

#include <string>
#include <stdio.h>
#include <math.h> /* sqrt */
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

//#include "detectvehicle/vehicle_s.hpp"
#include "detectvehicle/detectvehicle.hpp"





namespace opendlv {
namespace perception {
namespace detectvehicle {


/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
DetectVehicle::DetectVehicle(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "perception-detectvehicle")
    , m_initialised(false)
    , m_debug()
{
}

DetectVehicle::~DetectVehicle()
{
}

void DetectVehicle::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = (kv.getValue<int32_t> ("perception-detectvehicle.debug") == 1);
  const std::string cascade_xml = kv.getValue<std::string>("perception-detectvehicle.cascade");

  ///HOW TO DO THIS???? (from core->videocapture.cpp)==> not working

  std::string path = "/opt/Cascade/"+cascade_xml;
  cascade = (CvHaarClassifierCascade*)cvLoad(path.c_str(), 0, 0, 0);

  // if( !cascade.load( path ) ){ printf("--(!)Error loading\n"); return -1; };

  storage = cvCreateMemStorage(0);
  /*This is generating an error idecating that the cascade is empty*/
  assert(cascade && storage);
  std::cout << "Setup complete." << std::endl;
  m_initialised = true;
}
void DetectVehicle::tearDown()
{
}
/**
 * Receives SharedImage from camera.
 * Sends .
 */
void DetectVehicle::nextContainer(odcore::data::Container &c)
{
  if (c.getDataType() != odcore::data::image::SharedImage::ID() || !m_initialised) {
    return;
  }


  odcore::data::image::SharedImage mySharedImg = c.getData<odcore::data::image::SharedImage>();


  std::string cameraName = mySharedImg.getName();
  //std::cout << "Received image from camera " << cameraName  << "!" << std::endl;

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


//COPY MAT TO IPLIMAGE
IplImage* frame = cvCreateImage(cvSize(myImage.cols, myImage.rows), 8, 3);
IplImage frame2 = myImage;
cvCopy(&frame2, frame);


//  IplImage* frame = new IplImage( myImage );

//THE RESIZE FRAME
  IplImage* frame1;
  frame1 = cvCreateImage(
      cvSize((int)((frame->width * input_resize_percent) / 100),
             (int)((frame->height * input_resize_percent) / 100)),
      frame->depth, frame->nChannels);
  cvResize(frame, frame1);

  //std::cout << "DETECTING!" << std::endl;

  detect(frame1,c.getReceivedTimeStamp());
cv::waitKey(1);
//cvWaitKey(1);
  cvReleaseImage(&myIplImage);
}


/**
  *  Detects vehicles in a fram
  *
  * @param img The frame captured from the camera
  */
void DetectVehicle::detect(IplImage* img,odcore::data::TimeStamp timeStampOfFrame){
  CvSize img_size = cvGetSize(img);

  CvSeq* object = cvHaarDetectObjects(
      img, cascade, storage,
      1.1,  // 1.1,//1.5, //-------------------SCALE FACTOR
      NEIGHBOURS,  // 2        //------------------MIN NEIGHBOURS
      0,  // CV_HAAR_DO_CANNY_PRUNING
      cvSize(0, 0),  // cvSize( 30,30), // ------MINSIZE
      img_size  // cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
      );

  std::vector<vehicle_t> detected_vehicles = std::vector<vehicle_t>();
  for (int i = 0; i < (object ? object->total : 0); i++) {
    CvRect* r = (CvRect*)cvGetSeqElem(object, i);
    vehicle_t vehicle;

    vehicle.x = r->x;
    vehicle.y = r->y;
    vehicle.w = r->width;
    vehicle.h = r->height;
    vehicle.found = 1;

    if (vehicle_buffer.empty()) {
      id_counter++;
      vehicle.id = id_counter;

      // this is the first run or its just empty ==> cant compare just draw and
      // add to buffer
      detected_vehicles.push_back(vehicle);
      vehicle_buffer.push_back(vehicle);
    } else {
      int corresponding_vehicle = checkMatch(vehicle);
      if (corresponding_vehicle != -1) {
        vehicle.found = vehicle_buffer.at(corresponding_vehicle).found + 2;
        vehicle.id = vehicle_buffer.at(corresponding_vehicle).id;
        vehicle_buffer.at(corresponding_vehicle) = vehicle;
      } else {
        id_counter++;
        vehicle.id = id_counter;
        vehicle_buffer.push_back(vehicle);
      }
      detected_vehicles.push_back(vehicle);
    }
  }  // end_for

  updateBuffer();
  int active_tracking = 0;
  int active_sccess=0;
  for (unsigned i = 0; i < detected_vehicles.size(); i++) {
    if (detected_vehicles.at(i).found > SHOW_POINT) {
      drawRec(img, detected_vehicles.at(i));
      active_tracking++;
      if (detected_vehicles.at(i).found > SUCCESS_POINT){
        active_sccess++;
        //BRODCAST VEHICLE WITH TIME STAMP BECAUSE ITS HERE
        sendVehicle(detected_vehicles.at(i),timeStampOfFrame);
      }
    }
  }
  if(active_tracking==0 && active_sccess==0){
    id_counter=0;
  }

  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 2, CV_AA);
  char buffer2[128];
  snprintf(buffer2, sizeof(buffer2), "%s%d%s%d%s%d", "Detected(Now): ",
           object->total, " Tracking:", (int)vehicle_buffer.size(), " Active:",
           active_tracking);
  cvPutText(img, buffer2, cvPoint(0, img->height - 10), &font,
            cvScalar(0x0, 0x0, 0x0));

 /*
  if(active_tracking==0){
    cvPutText(img, "EMPTY ROAD", cvPoint(0,20), &font,
            cvScalar(0x0, 0xff, 0x0));
  }else if(active_tracking>0 && active_sccess==0){
    cvPutText(img, "POSSIBLE OCCUPIED ROAD", cvPoint(0, 20), &font,
            cvScalar(0xff, 0x0, 0x0));
  }else if( active_sccess>0){
    cvPutText(img, "OCCUPIED ROAD", cvPoint(0, 20), &font,
            cvScalar(0x0, 0x0, 0xff));
  }*/

  //std::cout << "SHOWING IMAGE!" << std::endl;
  cvShowImage("video", img);
  if (SHOW_RIO)
     drawROI(img);

}

/**
  *  Checks for a match in the buffer
  *
  * @param vehicle  The vehicle struct to check
  */
int  DetectVehicle::checkMatch(vehicle_t vehicle){
  double min_distance = 1000;
  int return_index = -1, index = -1;

  for (unsigned i = 0; i < vehicle_buffer.size(); i++) {
    vehicle_t stored_vehicle = vehicle_buffer.at(i);
    double distance_b =
        distance(stored_vehicle.x, vehicle.x, stored_vehicle.y, vehicle.y);
    if (min_distance > distance_b) {
      min_distance = distance_b;
      index = i;
    }
  }
  if (min_distance <= MAX_DISTANCE)
    return_index = index;

  return return_index;
}

/**
  *  Updates the short term memory(The buffer)
  */
void DetectVehicle::updateBuffer(){
  if (vehicle_buffer.empty() == false) {
    for (int i = vehicle_buffer.size() - 1; i >= 0; i--) {
      vehicle_buffer.at(i).found--;
      if (vehicle_buffer.at(i).found < REMOVE_POINT) {
        vehicle_buffer.erase(vehicle_buffer.begin() + i);
      }
    }
  }
}
/**
  *  Draws a rectangel in the frame for the found vehicles
  *
  * @param img      The frame to draw on
  * @param vehicle  The vehicle struct to draw a frame around
  */
void  DetectVehicle::drawRec(IplImage* img, vehicle_t vehicle){
  int red = 255, green = 0;

  if (vehicle.found > SUCCESS_POINT) {
    red = 0;
    green = 255;
  }

  CvFont font;
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);
  char buffer[60], buffer2[60];
  // DRAW ON THE FRAME
  cvRectangle(img, cvPoint(vehicle.x, vehicle.y),
              cvPoint(vehicle.x + vehicle.w, vehicle.y + vehicle.h),
              CV_RGB(red, green, 0), 2, 8, 0);
  snprintf(buffer, sizeof(buffer), "%s%d", "Vehicle_ID:", vehicle.id);
  snprintf(buffer2, sizeof(buffer2), "%s%d", " Rank:", vehicle.found);
  cvPutText(img, buffer, cvPoint(vehicle.x, vehicle.y), &font, cvScalar(255));
  cvPutText(img, buffer2, cvPoint(vehicle.x, vehicle.y + vehicle.h + 5), &font,
            cvScalar(255));
}

/**
  *  Calculates the distance between to points in the frame
  * @param x1,y1  first point cooardinates
  * @param x2,y2  scound point cooardinates
  */
double DetectVehicle::distance(int x1, int x2, int y1, int y2){
  return sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
}

/**
  *  Draws a reagion of interest @BETA
  *  @param frame     The frame
  */
void DetectVehicle::drawROI(IplImage* frame){
  int vrio = (int)((HORIZON * input_resize_percent) / 100);
  IplImage* img_src_cpy =
      cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

  img_src_cpy = cvCloneImage(frame);

  cvSetImageROI(img_src_cpy, cvRect(0, img_src_cpy->height - vrio,
                                    img_src_cpy->width, vrio));
  cvShowImage("video2", img_src_cpy);
}

/**
  *  Sends a vehicle to the tracker to track
  *  @param vehicle     The vehicle to send
  * Message [id=201 ] VehicleVision
  */
void DetectVehicle::sendVehicle(vehicle_t vehicle,odcore::data::TimeStamp timeStampOfFrame){
std::cout << "BRODCASTING VEHICLE ID=" << vehicle.id << std::endl;
  opendlv::perception::VehicleVision detectedObject(
      timeStampOfFrame,
      vehicle.id,
      vehicle.x,
      vehicle.y,
      vehicle.w,
      vehicle.h,
      vehicle.found);
  odcore::data::Container objectContainer(detectedObject);
  getConference().send(objectContainer);

}


} // detectvehicle
} // perception
} // opendlv
