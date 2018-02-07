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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdv2v/GeneratedHeaders_ODVDV2V.h"

#include "selflocalization.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Selflocalization::Selflocalization(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "logic-sensation-selflocalization")
    , m_cameraType()

{

	//Initialization

	//Orb vocabulary - global pointer
	//KeyframDatabase - global pointer
	//Map - global pointer
	//Tracking - global pointer
	//Local mapping - global pointer
	//Loop closing - global pointer

	//System threads
	//Tracking lives in system thread
	//Local mapping - global thread pointer
	//Loop closing - global thread pointer




	/*System::System(const string &strVocFile, const string &strSettingsFile, const eSensor sensor,
               		const bool bUseViewer):mSensor(sensor), mpViewer(static_cast<Viewer*>(NULL)), mbReset(false),mbActivateLocalizationMode(false),
					mbDeactivateLocalizationMode(false)*/
}

Selflocalization::~Selflocalization()
{
}
/*
*Takes data from conference, in our case image?
*/
void Selflocalization::nextContainer(odcore::data::Container &a_container) 
{
	cv::Mat img;
	//Use sharedimage and then proceed??
	if (a_container.getDataType() == odcore::data::image::SharedImage::ID()){

    	odcore::data::image::SharedImage sharedImg = a_container.getData<odcore::data::image::SharedImage>();
    	//Check image plausabilitu???
    	img = ExtractSharedImage(&sharedImg);

   			//std::cout << "[" << getName() << "] " << "[Unable to extract shared image." << std::endl;
   	}

//if mono


//if stereo
   	if(m_cameraType){

	   	int width = img.cols;
  		int height = img.rows;
 		cv::Mat imgL(img, cv::Rect(0, 0, width/2, height));
		cv::Mat imgR(img, cv::Rect(width/2, 0, width/2, height));
	}else{


		//MONO
	}

}

cv::Mat Selflocalization::ExtractSharedImage(odcore::data::image::SharedImage *a_sharedImage)
{
  
  cv::Mat img;
  std::shared_ptr<odcore::wrapper::SharedMemory> sharedMem(odcore::wrapper::SharedMemoryFactory::attachToSharedMemory(a_sharedImage->getName()));
  if (sharedMem->isValid()) {
    const uint32_t nrChannels = a_sharedImage->getBytesPerPixel();
    const uint32_t imgWidth = a_sharedImage->getWidth();
    const uint32_t imgHeight = a_sharedImage->getHeight();
    IplImage* myIplImage = cvCreateImage(cvSize(imgWidth,imgHeight), IPL_DEPTH_8U, nrChannels);
    cv::Mat bufferImage = cv::Mat(myIplImage);
    	{
      		sharedMem->lock();
      		memcpy(bufferImage.data, sharedMem->getSharedMemory()
        	, imgWidth*imgHeight*nrChannels);
      		sharedMem->unlock();
    	}
    	img.release();
    	img = bufferImage.clone();
    	bufferImage.release();
    	cvReleaseImage(&myIplImage);
   
  }else{

    std::cout << "[" << getName() << "] " << "Sharedmem is not valid." << std::endl;
  }

  return img;
}


void Selflocalization::setUp()
{
	  auto kv = getKeyValueConfiguration();
	  //Model uncertainties params
	  std::cout << "test 1" << std::endl;
	  m_cameraType = (kv.getValue<int32_t>("logic-sensation-selflocalization.cameratype") == 1);

	  std::cout << "test 2" << std::endl;
}

void Selflocalization::tearDown()
{
}

}
}
}
