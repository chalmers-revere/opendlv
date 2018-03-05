/**
 * Copyright (C) 2017 Chalmers Revere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *cmake -E remove_directory .

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
#ifndef ORBMAPPOINT_HPP
#define ORBMAPPOINT_HPP

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include <orbframe.hpp>
#include <orbmap.hpp>

namespace opendlv {
namespace logic {
namespace sensation {

class OrbFrame;
class OrbMap;

class OrbMapPoint
{
public:
    OrbMapPoint(const cv::Mat &position, std::shared_ptr<OrbFrame> refenceKeyFrame, std::shared_ptr<OrbMap> map);
    OrbMapPoint(const cv::Mat &position, std::shared_ptr<OrbFrame> refenceKeyFrame, std::shared_ptr<OrbMap> map, const int &keyPointIndex);
    ~OrbMapPoint();

    std::map<std::shared_ptr<OrbFrame>,size_t> GetObservingKeyframes();

    void SetWorldPosition(const cv::Mat &Position);
    cv::Mat GetWorldPosition();

    cv::Mat GetMeanViewingDirection();
    std::shared_ptr<OrbFrame> GetReferenceKeyFrame();

    int GetObservingKeyFrameCount();
    int GetSequenceId();
    void AddObservingKeyframe(std::shared_ptr<OrbFrame> keyFrame,size_t idx);
    void EraseObservingKeyframe(std::shared_ptr<OrbFrame> keyFrame);

    int GetObeservationIndexOfKeyFrame(std::shared_ptr<OrbFrame> keyFrame);
    bool KeyFrameInObservingKeyFrames(std::shared_ptr<OrbFrame> keyFrame);

    void SetCorruptFlag();
    bool IsCorrupt();

    void Replace(std::shared_ptr<OrbMapPoint> orbMapPoint);    
    std::shared_ptr<OrbMapPoint> GetReplaced();

    void IncreaseVisible(int n=1);
    void IncreaseFound(int n=1);
    float GetFoundRatio();
    inline int GetFound() { return m_foundCounter; }
    float getTrackProjX() { return mTrackProjX; }
    float getTrackProjY() { return mTrackProjY; }
    float getTrackProjXR() { return mTrackProjXR; }

    void ComputeDistinctiveDescriptors();

    cv::Mat GetDescriptor();

    void UpdateMeanAndDepthValues();

    float GetMinDistanceInvariance();
    float GetMaxDistanceInvariance();
    int PredictScale(const float &currentDist, std::shared_ptr<OrbFrame> keyFrame);

    static long unsigned int m_nextId;

    int GetTrackScaleLevel();
    bool GetTrackInView();
    float GTrackViewCos();
    long unsigned int GetTrackReferenceForFrame();
    long unsigned int GetLastFrameSeen();
    long unsigned int GetBALocalForKF();
    long unsigned int GetFuseCandidateForKF();
    long unsigned int GetLoopPointForKF();
    long unsigned int GetCorrectedByKF();
    long unsigned int GetCorrectedReference();
    cv::Mat GetPosGBA();
    long unsigned int GetBAGlobalForKF();

    void SetTrackScaleLevel(long unsigned int TrackScaleLevel);
    void SetTrackInView(long unsigned int TrackInView);
    void SetackViewCos(long unsigned int ackViewCos);
    void SetTrackReferenceForFrame(long unsigned int TrackReferenceForFrame);
    void SetLastFrameSeen(long unsigned int LastFrameSeen);
    void SetBALocalForKF(long unsigned int BALocalForKF);
    void SetFuseCandidateForKF(long unsigned int FuseCandidateForKF);
    void SetLoopPointForKF(long unsigned int LoopPointForKF);
    void SetCorrectedByKF(long unsigned int CorrectedByKF);
    void SetCorrectedReference(long unsigned int CorrectedReference);
    void SetPosGBA(long unsigned int PosGBA);
    void SetBAGlobalForKF(long unsigned int BAGlobalForKF);

private:
    long unsigned int SequenceId = 0;
    long int m_firstKeyframeId;
    long int m_FirstKeyFrame;
    int m_observingKeyFramesCount;

    // Variables used by the tracking
    float mTrackProjX;
    float mTrackProjY;
    float mTrackProjXR;
    bool mbTrackInView;
    int mnTrackScaleLevel;
    float mTrackViewCos;
    long unsigned int mnTrackReferenceForFrame;
    long unsigned int mnLastFrameSeen;

    // Variables used by local mapping
    long unsigned int mnBALocalForKF;
    long unsigned int mnFuseCandidateForKF;

    // Variables used by loop closing
    long unsigned int mnLoopPointForKF;
    long unsigned int mnCorrectedByKF;
    long unsigned int mnCorrectedReference;    
    cv::Mat mPosGBA;
    long unsigned int mnBAGlobalForKF;


    static std::mutex mGlobalMutex;

    // mutexed below

     // Position in absolute coordinates
     cv::Mat m_worldPosition;

     // Keyframes observing the point and associated index in keyframe
     std::map<std::shared_ptr<OrbFrame>,size_t> m_observingKeyframes;

     // Mean viewing direction
     cv::Mat m_meanViewingDirection;

     // Best descriptor to fast matching
     cv::Mat m_descriptor;

     // Reference KeyFrame
     std::shared_ptr<OrbFrame> m_refenceKeyFrame;

     // Tracking counters
     int m_visibleCounter;
     int m_foundCounter;

     // Bad flag (we do not currently erase MapPoint from memory)
     bool m_corrupt;
    std::shared_ptr<OrbMapPoint> m_replaced;

     // Scale invariance distances
     float m_minDistance;
     float m_maxDistance;

     std::shared_ptr<OrbMap> m_map;

    std::mutex m_constructorMutex;
    std::mutex mMutexPos;
    std::mutex mMutexFeatures;
};

}
}
}

#endif // ORBMAPPOINT_HPP