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
#include "orbframe.hpp"


namespace opendlv {
namespace logic {
namespace sensation {

long unsigned int OrbFrame::NextId=0;

    //Stereo constructor
OrbFrame::OrbFrame(cv::Mat leftGreyImage, cv::Mat rightGreyImage, std::vector<OrbKeyPoint> keyPoints, cv::Mat tcw):
        Id(NextId++), m_keypoints(keyPoints), m_leftGreyImage(leftGreyImage), m_rightGreyImage(rightGreyImage), m_tcw(tcw)
{
    SetPose(m_tcw);
}

OrbFrame::~OrbFrame()
{

}

void OrbFrame::SetPose(const cv::Mat &Tcw_)
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    Tcw_.copyTo(m_tcw);
    cv::Mat rcw = m_tcw.rowRange(0,3).colRange(0,3);
    cv::Mat tcw = m_tcw.rowRange(0,3).col(3);
    cv::Mat rwc = rcw.t();
    m_ow = -rwc*tcw;

    m_twc = cv::Mat::eye(4,4,m_tcw.type());
    rwc.copyTo(m_tcw.rowRange(0,3).colRange(0,3));
    m_ow.copyTo(m_tcw.rowRange(0,3).col(3));
    cv::Mat center = (cv::Mat_<float>(4,1) << m_halfBaseline, 0 , 0, 1);
    m_cw = m_twc*center;
}

cv::Mat OrbFrame::GetPose()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_tcw.clone();
}

cv::Mat OrbFrame::GetPoseInverse()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_twc.clone();
}

cv::Mat OrbFrame::GetCameraCenter()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_ow.clone();
}

cv::Mat OrbFrame::GetStereoCenter()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_cw.clone();
}

cv::Mat OrbFrame::GetRotation()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_tcw.rowRange(0,3).colRange(0,3).clone();
}

cv::Mat OrbFrame::GetTranslation()
{
    std::unique_lock<std::mutex> lock(m_mutexPose);
    return m_tcw.rowRange(0,3).col(3).clone();
}

void OrbFrame::ComputeBoW()
{

}

void OrbFrame::AddConnection(std::shared_ptr<OrbFrame> keyFrame, const int &weight)
{
    {
        std::unique_lock<std::mutex> lock(m_mutexConnections);
        if(!m_connectedKeyFrameWeights.count(keyFrame))
        {
            m_connectedKeyFrameWeights[keyFrame]=weight;
        }
        else if(m_connectedKeyFrameWeights[keyFrame]!=weight)
        {
            m_connectedKeyFrameWeights[keyFrame]=weight;
        }
        else
        {
            return;
        }
    }

    UpdateBestCovisibles();
}

void OrbFrame::EraseConnection(std::shared_ptr<OrbFrame> keyFrame)
{
    bool update = false;
    {
        std::unique_lock<std::mutex> lock(m_mutexConnections);
        if(m_connectedKeyFrameWeights.count(keyFrame))
        {
            m_connectedKeyFrameWeights.erase(keyFrame);
            update = true;
        }
    }

    if(update)
    {
        UpdateBestCovisibles();
    }
}

void OrbFrame::UpdateConnections()
{
    std::map<std::shared_ptr<OrbFrame>,int> frameCounter;

    std::vector<std::shared_ptr<OrbMapPoint> > mapPoints;

    {
        std::unique_lock<std::mutex> lockMapPoints(m_mutexConnections);
        mapPoints = m_mapPoints;
    }

    //For all map points in keyframe check in which other keyframes are they seen
    //Increase counter for those keyframes
    for(std::vector<std::shared_ptr<OrbMapPoint>>::iterator vectorIterator=mapPoints.begin(), vectorEnd=mapPoints.end(); vectorIterator!=vectorEnd; vectorIterator++)
    {
        std::shared_ptr<OrbMapPoint> mapPoint = *vectorIterator;

        if(!mapPoint)
        {
            continue;
        }
        if(mapPoint->IsCorrupt())
        {
            continue;
        }

        std::map<std::shared_ptr<OrbFrame>,size_t> observations = mapPoint->GetObservingKeyframes();

        for(std::map<std::shared_ptr<OrbFrame>,size_t>::iterator mapIterator=observations.begin(), mapEnd=observations.end(); mapIterator!=mapEnd; mapIterator++)
        {
            if(mapIterator->first->Id==Id)
            {
                continue;
            }

            frameCounter[mapIterator->first]++;
        }
    }

    // This should not happen
    if(frameCounter.empty())
    {
        return;
    }

    //If the counter is greater than threshold add connection
    //In case no keyframe counter is over threshold add the one with maximum counter
    int max=0;
    std::shared_ptr<OrbFrame> maxKeyFrame=NULL;
    int th = 15;

    std::vector<std::pair<int,std::shared_ptr<OrbFrame>>> vectorPairs;
    vectorPairs.reserve(frameCounter.size());
    for(std::map<std::shared_ptr<OrbFrame>,int>::iterator mapIterator=frameCounter.begin(), mapEnd=frameCounter.end(); mapIterator!=mapEnd; mapIterator++)
    {
        if(mapIterator->second>max)
        {
            max=mapIterator->second;
            maxKeyFrame=mapIterator->first;
        }
        if(mapIterator->second>=th)
        {
            vectorPairs.push_back(make_pair(mapIterator->second,mapIterator->first));
            (mapIterator->first)->AddConnection(std::shared_ptr<OrbFrame>(this),mapIterator->second);
        }
    }

    if(vectorPairs.empty())
    {
        vectorPairs.push_back(make_pair(max,maxKeyFrame));
        maxKeyFrame->AddConnection(std::shared_ptr<OrbFrame>(this),max);
    }

    std::sort(vectorPairs.begin(),vectorPairs.end());
    std::list<std::shared_ptr<OrbFrame>> keyFrameList;
    std::list<int> weightList;
    for(size_t i=0; i<vectorPairs.size();i++)
    {
        keyFrameList.push_front(vectorPairs[i].second);
        weightList.push_front(vectorPairs[i].first);
    }

    {
        std::unique_lock<std::mutex> lock(m_mutexConnections);

        m_connectedKeyFrameWeights = frameCounter;
        m_orderedConnectedKeyFrames = std::vector<std::shared_ptr<OrbFrame>>(keyFrameList.begin(),keyFrameList.end());
        m_orderedWeights = std::vector<int>(weightList.begin(), weightList.end());

        if(m_firstConnection && Id!=0)
        {
            m_parent = m_orderedConnectedKeyFrames.front();
            m_parent->AddChild(std::shared_ptr<OrbFrame>(this));
            m_firstConnection = false;
        }
    }
}

void OrbFrame::UpdateBestCovisibles()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);

    std::vector<std::pair<int,std::shared_ptr<OrbFrame>>> vectorPairs;

    vectorPairs.reserve(m_connectedKeyFrameWeights.size());

    for(std::map<std::shared_ptr<OrbFrame>,int>::iterator mapIterator=m_connectedKeyFrameWeights.begin(), mapEnd=m_connectedKeyFrameWeights.end(); mapIterator!=mapEnd; mapIterator++)
    {
        vectorPairs.push_back(std::make_pair(mapIterator->second,mapIterator->first));
    }

    std::sort(vectorPairs.begin(),vectorPairs.end());
    std::list<std::shared_ptr<OrbFrame>> keyFrameList;
    std::list<int> weightList;
    for(size_t i=0, iend=vectorPairs.size(); i<iend;i++)
    {
        keyFrameList.push_front(vectorPairs[i].second);
        weightList.push_front(vectorPairs[i].first);
    }

    m_orderedConnectedKeyFrames = std::vector<std::shared_ptr<OrbFrame>>(keyFrameList.begin(),keyFrameList.end());
    m_orderedWeights = std::vector<int>(weightList.begin(), weightList.end());
}

std::set<std::shared_ptr<OrbFrame>> OrbFrame::GetConnectedKeyFrames()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    std::set<std::shared_ptr<OrbFrame>> set;
    for(std::map<std::shared_ptr<OrbFrame>,int>::iterator mapIterator=m_connectedKeyFrameWeights.begin();mapIterator!=m_connectedKeyFrameWeights.end();mapIterator++)
        set.insert(mapIterator->first);
    return set;
}

std::vector<std::shared_ptr<OrbFrame>> OrbFrame::GetVectorCovisibleKeyFrames()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_orderedConnectedKeyFrames;
}

std::vector<std::shared_ptr<OrbFrame>> OrbFrame::GetBestCovisibilityKeyFrames(const int &n)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    if((int)m_orderedConnectedKeyFrames.size()<n)
    {
        return m_orderedConnectedKeyFrames;
    }
    else
    {
        return std::vector<std::shared_ptr<OrbFrame>>(m_orderedConnectedKeyFrames.begin(),m_orderedConnectedKeyFrames.begin()+n);
    }
}

std::vector<std::shared_ptr<OrbFrame>> OrbFrame::GetCovisiblesByWeight(const int &weight)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);

    if(m_orderedConnectedKeyFrames.empty())
    {
        return std::vector<std::shared_ptr<OrbFrame>>();
    }

    std::vector<int>::iterator iterator = upper_bound(m_orderedWeights.begin(),m_orderedWeights.end(),weight,OrbFrame::WeightComp);
    if(iterator==m_orderedWeights.end())
    {
        return std::vector<std::shared_ptr<OrbFrame>>();
    }
    else
    {
        int n = iterator-m_orderedWeights.begin();
        return std::vector<std::shared_ptr<OrbFrame>>(m_orderedConnectedKeyFrames.begin(), m_orderedConnectedKeyFrames.begin()+n);
    }
}

int OrbFrame::GetWeight(std::shared_ptr<OrbFrame>keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    if(m_connectedKeyFrameWeights.count(keyFrame))
        return m_connectedKeyFrameWeights[keyFrame];
    else
        return 0;
}

void OrbFrame::AddChild(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    m_spanningChildren.insert(keyFrame);
}

void OrbFrame::EraseChild(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    m_spanningChildren.erase(keyFrame);
}

void OrbFrame::ChangeParent(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    m_parent = keyFrame;
    keyFrame->AddChild(std::shared_ptr<OrbFrame>(this));
}

std::set<std::shared_ptr<OrbFrame>> OrbFrame::GetChilds()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_spanningChildren;
}

std::shared_ptr<OrbFrame> OrbFrame::GetParent()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_parent;
}

bool OrbFrame::HasChild(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_spanningChildren.count(keyFrame);
}

void OrbFrame::AddLoopEdge(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    m_dontErase = true;
    m_loopEdges.insert(keyFrame);
}

std::set<std::shared_ptr<OrbFrame>> OrbFrame::GetLoopEdges()
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_loopEdges;
}

void OrbFrame::AddMapPoint(std::shared_ptr<OrbMapPoint> mapPoint, const size_t &index)
{
    std::unique_lock<std::mutex> lockFeatures(m_mutexFeatures);
    m_mapPoints[index]=mapPoint;
}

void OrbFrame::EraseMapPointMatch(const size_t &index)
{
    std::unique_lock<std::mutex> lockFeatures(m_mutexFeatures);
    m_mapPoints[index]=NULL;
}

void OrbFrame::EraseMapPointMatch(std::shared_ptr<OrbMapPoint> mapPoint)
{
    int index = mapPoint->GetObeservationIndexOfKeyFrame(std::shared_ptr<OrbFrame>(this));
    if(index>=0)
    {
        m_mapPoints[index]=static_cast<std::shared_ptr<OrbMapPoint>>(NULL);
    }
}

void OrbFrame::ReplaceMapPointMatch(const size_t &index, std::shared_ptr<OrbMapPoint> mapPoint)
{
    m_mapPoints[index]=mapPoint;
}

std::vector<std::shared_ptr<OrbMapPoint>> OrbFrame::GetMapPointMatches()
{
    std::unique_lock<std::mutex> lockFeatures(m_mutexFeatures);
    return m_mapPoints;
}

int OrbFrame::TrackedMapPoints(const int &minimumObservations)
{
    std::unique_lock<std::mutex> lockFeatures(m_mutexFeatures);

    int points=0;
    const bool checkObservations = minimumObservations>0;
    for(int i=0; i<m_numberOfKeypoints; i++)
    {
        std::shared_ptr<OrbMapPoint> mapPoint = m_mapPoints[i];
        if(mapPoint)
        {
            if(!mapPoint->IsCorrupt())
            {
                if(checkObservations)
                {
                    if(m_mapPoints[i]->GetObservingKeyFrameCount()>=minimumObservations)
                    {
                        points++;
                    }
                }
                else
                {
                    points++;
                }
            }
        }
    }

    return points;
}

std::shared_ptr<OrbMapPoint> OrbFrame::GetMapPoint(const size_t &index)
{
    std::unique_lock<std::mutex> lockFeatures(m_mutexFeatures);
    return m_mapPoints[index];
}

} // namespace sensation
} // namespace logic
} // namespace opendlv