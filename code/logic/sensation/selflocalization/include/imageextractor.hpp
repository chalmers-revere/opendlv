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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_IMAGEEXTRACTOR_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_IMAGEEXTRACTOR_HPP

#include <memory>

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "opendavinci/odcore/base/KeyValueConfiguration.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
//#include "selflocalization.hpp"


namespace opendlv {
namespace logic {
namespace sensation {



class ImageExtractor
 {
 public:
  ImageExtractor(odcore::base::KeyValueConfiguration kv);
  ImageExtractor(ImageExtractor const &) = delete;
  ImageExtractor &operator=(ImageExtractor const &) = delete;
  virtual ~ImageExtractor();
  cv::Mat ImageToGreyscaleStereo(cv::Mat &imgL, cv::Mat &imgR, double &timeStamp);
  cv::Mat ImageToGreyscaleMono(cv::Mat &img, double &timeStamp);
  void saveImg(cv::Mat &img);

 private:
 cv::Mat m_imGrey;
 bool m_RGB; //Order of colour channels
 int m_saveCounter = 0;
	
};

}
}
}

#endif
