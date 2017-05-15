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

#ifndef LOGIC_PERCEPTION_DETECTLANE_DETECTLANE_HPP
#define LOGIC_PERCEPTION_DETECTLANE_DETECTLANE_HPP

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


#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

namespace opendlv {
namespace logic {
namespace perception {

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

  bool ExtractSharedImage(odcore::data::image::SharedImage *);
  void UpdateVisualMemory();
  void UpdateVisualLines();
  std::vector<cv::Vec2f> GetGrouping(std::vector<cv::Vec2f>, double);
  std::vector<std::pair<cv::Vec2f, cv::Vec2f>> GetParametricRepresentation(std::vector<cv::Vec2f>);
  void UpdatePointsOnLines(std::vector<std::pair<cv::Vec2f, cv::Vec2f>>);
  std::vector<uint16_t> GetLanes() const;
  std::vector<uint16_t> GetCurrentLane() const;
  Eigen::MatrixXd ReadMatrix(std::string const, uint8_t const, uint8_t const) const;
  Eigen::Vector3d TransformPointToGlobalFrame(Eigen::Vector3d) const;
  void DrawWindows();

  bool m_initialized;
  cv::Mat m_currentImg;
  uint16_t m_blurKernelSize; 
  cv::Mat m_cannyImg;
  cv::Mat m_adapThreshImg;
  std::deque<std::pair<odcore::data::TimeStamp, cv::Mat>> m_visualMemory;
  uint8_t m_adapThreshKernelSize;
  uint8_t m_adapThreshConst;
  uint16_t m_cannyThreshold;
  uint16_t m_houghThreshold;
  std::vector<cv::Vec2f> m_linesRaw;
  std::vector<cv::Vec2f> m_linesProcessed;
  std::vector<uint16_t> m_laneLineIds;
  std::vector<uint16_t> m_currentLaneLineIds;
  std::vector<cv::Vec2f> m_xScreenP;
  std::vector<cv::Vec2f> m_yScreenP;
  std::vector<cv::Vec2f> m_xWorldP;
  std::vector<cv::Vec2f> m_yWorldP;
  float m_lineDiff;
  float m_OneLineDiff;
  float m_HorisontalLimit;
  double m_memThreshold;
  double m_upperLaneLimit;
  double m_lowerLaneLimit;
  int16_t m_screenSize[2];
  int16_t m_roi[4];
  odcore::base::Mutex m_mtx;
  bool m_debug;
  std::string m_cameraName; 
  Eigen::Matrix3d m_transformationMatrix;

};

}
}
}

#endif
