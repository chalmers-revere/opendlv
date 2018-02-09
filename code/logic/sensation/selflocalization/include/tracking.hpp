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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_TRACKING_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_TRACKING_HPP

#include <memory>

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "selflocalization.hpp"


namespace opendlv {
namespace logic {
namespace sensation {


class Selflocalization;
class Tracking
 {
 public:
  Tracking(std::shared_ptr<Selflocalization> pSelfLocalization/*, std::shared_ptr<auto> kv, std::shared_ptr<OrbVocabulary> m_pVocabulary, std::shared_ptr<KeyFrameDatabase> m_pKeyFrameDatabase, std::shared_ptr<Map> m_pMap*/);
  Tracking(Tracking const &) = delete;
  Tracking &operator=(Tracking const &) = delete;
  virtual ~Tracking();
  cv::Mat ImageToGreyscaleStereo(cv::Mat &imgL, cv::Mat &imgR, double &timeStamp);

 private:
 cv::Mat m_imGreyLeft;
 bool m_RGB; //Order of colour channels
 std::shared_ptr<Selflocalization> m_pSelfLocalization;
	
};

}
}
}

#endif
