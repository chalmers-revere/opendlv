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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_ORBFRAME_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_ORBFRAME_HPP

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/generated/odcore/data/CompactPointCloud.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include <memory>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <list>
#include <utility>

#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/wrapper/Eigen.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <orbkeypoint.hpp>
#include <orbmappoint.hpp>

namespace opendlv {
namespace logic {
namespace sensation {

class OrbMapPoint;

class OrbFrame 
{
public:
    // Copy constructor.
    OrbFrame(cv::Mat leftGreyImage, cv::Mat rightGreyImage, std::vector<OrbKeyPoint>, cv::Mat tcw);
    ~OrbFrame();

    void SetPose(const cv::Mat &Tcw);
    cv::Mat GetPose();
    cv::Mat GetPoseInverse();
    cv::Mat GetCameraCenter();
    cv::Mat GetStereoCenter();
    cv::Mat GetRotation();
    cv::Mat GetTranslation();

    void ComputeBoW();

    void AddConnection(std::shared_ptr<OrbFrame> frame, const int &weight);
    void EraseConnection(std::shared_ptr<OrbFrame> frame);
    void UpdateConnections();
    void UpdateBestCovisibles();

    std::set<std::shared_ptr<OrbFrame>> GetConnectedKeyFrames();
    std::vector<std::shared_ptr<OrbFrame>> GetVectorCovisibleKeyFrames();
    std::vector<std::shared_ptr<OrbFrame>> GetBestCovisibilityKeyFrames(const int &N);
    std::vector<std::shared_ptr<OrbFrame>> GetCovisiblesByWeight(const int &weight);
    int GetWeight(std::shared_ptr<OrbFrame> frame);

    void AddChild(std::shared_ptr<OrbFrame> frame);
    void EraseChild(std::shared_ptr<OrbFrame> frame);
    void ChangeParent(std::shared_ptr<OrbFrame> frame);
    std::set<std::shared_ptr<OrbFrame>> GetChilds();
    std::shared_ptr<OrbFrame> GetParent();
    bool HasChild(std::shared_ptr<OrbFrame> frame);

    void AddLoopEdge(std::shared_ptr<OrbFrame> pKF);
    std::set<std::shared_ptr<OrbFrame>> GetLoopEdges();

    void AddMapPoint(std::shared_ptr<OrbMapPoint> pMP, const size_t &idx);
    void EraseMapPointMatch(const size_t &idx);
    void EraseMapPointMatch(std::shared_ptr<OrbMapPoint> pMP);
    void ReplaceMapPointMatch(const size_t &idx, std::shared_ptr<OrbMapPoint> pMP);
    std::set<std::shared_ptr<OrbMapPoint>> GetMapPoints();
    std::vector<std::shared_ptr<OrbMapPoint>> GetMapPointMatches();
    int TrackedMapPoints(const int &minObs);
    std::shared_ptr<OrbMapPoint> GetMapPoint(const size_t &idx);

    std::vector<cv::KeyPoint> GetUndistortedKeyPoints() { return m_undistortedKeyPoints; }

    cv::Mat GetDescriptors() { return m_descriptors; }
    void SetDescriptors(cv::Mat descriptors) { m_descriptors = descriptors; }

    int GetScaleLevels() { return m_scaleLevels; }
    void SetScaleLevels(int scaleLevels) { m_scaleLevels = scaleLevels; }

    float GetScaleFactor() { return m_scaleFactor; }
    void SetScaleFactor(float scaleFactor) { m_scaleFactor = scaleFactor; }

    float GetLogScaleFactor() { return m_logScaleFactor; }
    void SetLogScaleFactor(float logScaleFactor) { m_logScaleFactor = logScaleFactor; }

    std::vector<float> GetScaleFactors() { return m_scaleFactors; }
    void SetScaleFactors(std::vector<float> scaleFactors) { m_scaleFactors = scaleFactors; }

    std::vector<float> GetLevelSigma2() { return m_levelSigma2; }
    void SetLevelSigma2(std::vector<float> levelSigma2) { m_levelSigma2 = levelSigma2; }

    std::vector<float> GetInverseLevelSigma2() { return m_inverseLevelSigma2; }
    void SetInverseLevelSigma2(std::vector<float> inverseLevelSigma2) { m_inverseLevelSigma2 = inverseLevelSigma2; }

    static bool WeightComp( int a, int b)
    {
        return a>b;
    }

    long unsigned int Id;
    static long unsigned int NextId;

private:

    int m_numberOfKeypoints = 0;

    std::vector<cv::KeyPoint> m_keyPoints, m_keyPointsRight;
    std::vector<cv::KeyPoint> m_undistortedKeyPoints;
    std::vector<float> m_right; // negative value for monocular points
    std::vector<float> m_depth; // negative value for monocular points
    cv::Mat m_descriptors;

    std::vector<OrbKeyPoint> m_keypoints;
    cv::Mat m_leftGreyImage, m_rightGreyImage;

    int m_scaleLevels;
    float m_scaleFactor;
    float m_logScaleFactor;
    std::vector<float> m_scaleFactors;
    std::vector<float> m_levelSigma2;
    std::vector<float> m_inverseLevelSigma2;

    cv::Mat m_tcw = {};
    cv::Mat m_twc = {};
    cv::Mat m_ow = {};

    cv::Mat m_cw = {};

    std::vector<std::shared_ptr<OrbMapPoint> > m_mapPoints = {};

    std::map<std::shared_ptr<OrbFrame>,int> m_connectedKeyFrameWeights = {};
    std::vector<std::shared_ptr<OrbFrame>> m_orderedConnectedKeyFrames = {};
    std::vector<int> m_orderedWeights = {};

    bool m_firstConnection = true;
    std::shared_ptr<OrbFrame> m_parent = {};
    std::set<std::shared_ptr<OrbFrame> > m_spanningChildren = {};
    std::set<std::shared_ptr<OrbFrame> > m_loopEdges = {};

    bool m_dontErase = false;

    float m_halfBaseline = 0.0f;

    std::mutex m_mutexPose = {};
    std::mutex m_mutexConnections = {};
    std::mutex m_mutexFeatures = {};
};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif