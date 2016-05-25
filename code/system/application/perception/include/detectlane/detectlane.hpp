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

#include <memory>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "detectlane/InversePerspectiveMapping.hpp"

namespace opendlv {
namespace perception {
namespace detectlane {

/**
 * This class provides...
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
  Eigen::MatrixXd ReadMatrix(std::string fileName,int nRows,
      int nCols);
  void TransformPointToGlobalFrame(Eigen::Vector3d &point);
  double GetLaneOffset(double kLeft,double mLeft, double kRight,
      double mRight,double col, int p1, int p2);
  double GetHeadingAngle(double kLeft,double mLeft, double kRight,
      double mRight, double row1, double row2, int p1, int p2);
  void ReadWarpPointsMatrix(std::vector<cv::Point2f> & a_points,
      std::string a_fileName);
  void ReadInputWindowSize(std::string a_stringStream);
  void ReadOutputWindowSize(std::string a_stringStream);
  void GetGrouping(std::vector<cv::Vec2f> &groups,
      std::vector<cv::Vec2f> &lines);
  void GetParametricRepresentation(std::vector<cv::Vec2f> &p,std::vector<cv::Vec2f> &m,std::vector<cv::Vec2f> &groups);
  void GetPointsOnLine(std::vector<cv::Vec2f> &xPoints,std::vector<cv::Vec2f> &yPoints,
    std::vector<cv::Vec2f> &X, std::vector<cv::Vec2f> &Y,
    std::vector<cv::Vec2f> &p,std::vector<cv::Vec2f> &m,float row1, float row2);
  void GetLinePairs(std::vector<cv::Vec2f> &xPoints,std::vector<cv::Vec2f> &yPoints,std::vector<cv::Vec2i> &groupIds);



  bool m_setup;
  int m_width;
  int m_height;
  int m_outputWidth;
  int m_outputHeight;
  int m_maxRow;
  int m_minRow;
  int m_midRegion;
  int m_threshold;
  int m_houghThreshold;
  int m_cannyThresholdTrue;
  
  double m_standardLaneWidth;  
  bool m_initialized;


  // Matrix holding region lines
  Eigen::MatrixXd m_regions;
  Eigen::MatrixXd m_leftCameraRegions;
  Eigen::MatrixXd m_rightCameraRegions;
  
  // Matrix holding the lines col = row * k + m for the region lines
  Eigen::MatrixXd m_lines;
  Eigen::MatrixXd m_leftLines;
  Eigen::MatrixXd m_rightLines;
  
  // Number of scan lines / points
  int m_nPoints;
  // Number of search regions
  long m_nRegions;

  // Matrix holding the mean column for each region on each row
  Eigen::MatrixXd m_recoveredPoints;

  // Holds the K and M parameters for each region
  Eigen::MatrixXd m_k, m_m;

  Eigen::MatrixXd m_K, m_M;
  // Counts the number of points per each region
  Eigen::VectorXd m_pointsPerRegion;
  
  // Holds the index that decides the left and right road track
  Eigen::MatrixXd m_regionIndex;
  
  // Holds the location of found lanes
  Eigen::VectorXd m_laneLocation2;

  // Inverse perspective mapping class
  std::unique_ptr<InversePerspectiveMapping> m_leftIpm;
  std::unique_ptr<InversePerspectiveMapping> m_rightIpm;

  Eigen::Matrix3d m_transformationMatrix;
  Eigen::Matrix3d m_leftTransformationMatrix;
  Eigen::Matrix3d m_rightTransformationMatrix;


  Eigen::Vector3d m_scale;


};

} // detectlane
} // perception
} // opendlv

#endif
