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


//Stereo constructor
OrbFrame::OrbFrame(cv::Mat leftGreyImage, cv::Mat rightGreyImage, std::vector<OrbKeyPoint> keyPoints, cv::Mat tcw): 
m_keypoints(keyPoints), m_leftGreyImage(leftGreyImage), m_rightGreyImage(rightGreyImage), m_tcw(tcw)
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

}

void OrbFrame::UpdateBestCovisibles()
{

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

bool OrbFrame::hasChild(std::shared_ptr<OrbFrame> keyFrame)
{
    std::unique_lock<std::mutex> lockConnections(m_mutexConnections);
    return m_spanningChildren.count(keyFrame);
}




} // namespace sensation
} // namespace logic
} // namespace opendlv