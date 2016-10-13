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

#ifndef DETECTLANE_DETECTLANE_HPP_
#define DETECTLANE_DETECTLANE_HPP_

#include <deque>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

namespace opendlv {
namespace perception {
namespace detectlane {

/**
 * This class provides the ability to detect lanes given an image source.
 */
class DetectLane
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  DetectLane(int32_t const &, char **);
  DetectLane(DetectLane const &) = delete;
  DetectLane &operator=(DetectLane const &) = delete;
  virtual ~DetectLane();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  bool m_initialized;
  cv::Mat m_image;
  std::deque<std::pair<odcore::data::TimeStamp, cv::Mat>> m_visualMemory;

  uint16_t m_intensityThreshold;
  uint16_t m_cannyThreshold;
  uint16_t m_houghThreshold;
  double m_memThreshold;

  void GetGrouping(std::vector<cv::Vec2f> &, std::vector<cv::Vec2f> &, double);

};

} // detectlane
} // perception
} // opendlv

#endif
