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

#include "opendavinci/odcore/data/TimeStamp.h"

#include "imageextractor.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
ImageExtractor::ImageExtractor(odcore::base::KeyValueConfiguration kv/*, std::shared_ptr<OrbVocabulary> m_pVocabulary, std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase, std::shared_ptr<Map> m_pMap*/)
: m_imGrey()
, m_RGB()
, m_saveCounter()
{
  


  	m_RGB = (kv.getValue<int32_t>("logic-sensation-selflocalization.camera.channelorder.RGB") == 1);
	std::cout << "RGB is " << m_RGB << std::endl;
}

cv::Mat ImageExtractor::ImageToGreyscaleStereo(cv::Mat &imgL, cv::Mat &imgR, double &timeStamp)
{
	m_imGrey = imgL;
	cv::Mat imGrayRight = imgR;
	timeStamp = timeStamp;

	if(m_imGrey.channels() == 3){

		if(m_RGB){

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_RGB2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_RGB2GRAY);
			
		}else{

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_BGR2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_BGR2GRAY);

		}

	}else if(m_imGrey.channels() == 4){ //In case of Alpha channel


		if(m_RGB){

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_RGBA2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_RGBA2GRAY);
			
		}else{

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_BGRA2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_BGRA2GRAY);

		}

	}

	saveImg(m_imGrey);
	saveImg(imGrayRight);

	/*mCurrentFrame = Frame(mImGray,imGrayRight,timestamp,mpORBextractorLeft,mpORBextractorRight,mpORBVocabulary,mK,mDistCoef,mbf,mThDepth);

    Track();

	return mCurrentFrame.mTcw.clone();*/



	return m_imGrey; //CHANGE RETURN TO FRAME

}

cv::Mat ImageExtractor::ImageToGreyscaleMono(cv::Mat &img, double &timeStamp)
{
	m_imGrey = img;
	timeStamp = timeStamp;

	if(m_imGrey.channels() == 3){

		if(m_RGB){

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_RGB2GRAY);
			
		}else{

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_BGR2GRAY);

		}

	}else if(m_imGrey.channels() == 4){ //In case of Alpha channel


		if(m_RGB){

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_RGBA2GRAY);
			
		}else{

			cv::cvtColor(m_imGrey,m_imGrey,cv::COLOR_BGRA2GRAY);

		}

	}

	saveImg(m_imGrey);
	/*mCurrentFrame = Frame(mImGray,imGrayRight,timestamp,mpORBextractorLeft,mpORBextractorRight,mpORBVocabulary,mK,mDistCoef,mbf,mThDepth);

    Track();

	return mCurrentFrame.mTcw.clone();*/



	return m_imGrey; //CHANGE RETURN TO FRAME

}

void ImageExtractor::saveImg(cv::Mat &img) {
	if(m_saveCounter < 50){
  		std::string img_name = std::to_string(m_saveCounter);
  		cv::imwrite("Images/"+img_name+".png", img);
		m_saveCounter++;
	}
}

ImageExtractor::~ImageExtractor()
{
}


}
}
}
