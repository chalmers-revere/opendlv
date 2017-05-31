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

#ifndef LOGIC_SENSATION_OPTICALFLOW_OPTICALFLOW_HPP
#define LOGIC_SENSATION_OPTICALFLOW_OPTICALFLOW_HPP

#include <Eigen/Dense>
#include <memory>

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"

namespace opendlv {
namespace logic {
namespace sensation {

class OpticalFlow
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  OpticalFlow(int32_t const &, char **);
  OpticalFlow(OpticalFlow const &) = delete;
  OpticalFlow &operator=(OpticalFlow const &) = delete;
  virtual ~OpticalFlow();
  virtual void nextContainer(odcore::data::Container &);

  std::string m_name;
  uint32_t m_size;

 private:
  void setUp();
  void tearDown();

  void SendContainer();

  cv::TermCriteria m_termcrit;
  cv::Size m_searchSize;
  uint32_t m_maxLevel;
  double m_minEigThreshold;
  uint32_t m_nAxisPoints;
  cv::Mat m_grayImage;
  cv::Mat m_prevGrayImage;
  cv::Mat m_image;
  cv::Mat m_flow;
  std::vector<cv::Point2f> m_staticImagePoints;
  std::vector<cv::Point2f> m_endImagePoints;
  odcore::data::image::SharedImage m_outputSharedImage;
  std::shared_ptr<odcore::wrapper::SharedMemory> m_sharedMemory;
};

}
}
}

#endif
