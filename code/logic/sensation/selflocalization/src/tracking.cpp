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

#include "tracking.hpp"

namespace opendlv {
namespace logic {
namespace sensation {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Tracking::Tracking(std::shared_ptr<Selflocalization> pSelfLocalization/*, std::shared_ptr<auto> kv, std::shared_ptr<OrbVocabulary> m_pVocabulary, std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase, std::shared_ptr<Map> m_pMap*/)
: m_imGreyLeft()
, m_RGB()
, m_pSelfLocalization(pSelfLocalization)
{
  

  //auto kv = getKeyValueConfiguration();

  //m_RGB = (kv->getValue<int32_t>("logic-sensation-selflocalization.camera.channelorder.RGB") == 1);
	m_RGB = true;
}

cv::Mat Tracking::ImageToGreyscaleStereo(cv::Mat &imgL, cv::Mat &imgR, double &timeStamp)
{
	m_imGreyLeft = imgL;
	cv::Mat imGrayRight = imgR;
	timeStamp = timeStamp;

	if(m_imGreyLeft.channels() == 3){

		if(m_RGB){

			cv::cvtColor(m_imGreyLeft,m_imGreyLeft,cv::COLOR_RGB2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_RGB2GRAY);
			
		}else{

			cv::cvtColor(m_imGreyLeft,m_imGreyLeft,cv::COLOR_BGR2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_BGR2GRAY);

		}

	}else if(m_imGreyLeft.channels() == 4){ //In case of Alpha channel


		if(m_RGB){

			cv::cvtColor(m_imGreyLeft,m_imGreyLeft,cv::COLOR_RGBA2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_RGBA2GRAY);
			
		}else{

			cv::cvtColor(m_imGreyLeft,m_imGreyLeft,cv::COLOR_BGRA2GRAY);
			cv::cvtColor(imGrayRight,imGrayRight,cv::COLOR_BGRA2GRAY);

		}

	}

	m_pSelfLocalization->saveImg(m_imGreyLeft);
	m_pSelfLocalization->saveImg(imGrayRight);
	/*mCurrentFrame = Frame(mImGray,imGrayRight,timestamp,mpORBextractorLeft,mpORBextractorRight,mpORBVocabulary,mK,mDistCoef,mbf,mThDepth);

    Track();

	return mCurrentFrame.mTcw.clone();*/



	return m_imGreyLeft; //CHANGE RETURN TO FRAME

}

Tracking::~Tracking()
{
}


}
}
}
