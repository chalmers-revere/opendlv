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
#include "orbmappoint.hpp"


namespace opendlv {
namespace logic {
namespace sensation {


OrbMapPoint::OrbMapPoint(const cv::Mat &position, std::shared_ptr<OrbFrame> refenceKeyFrame, std::shared_ptr<OrbMap> map)
: mnFirstKFid(refenceKeyFrame->Id), mnFirstFrame(refenceKeyFrame->Id), m_refenceKeyFrame(refenceKeyFrame), m_map(map)
{
    position.copyTo(m_worldPosition);
    m_meanViewingDirection = cv::Mat::zeros(3,1,CV_32F);

    // MapPoints can be created from Tracking and Local Mapping. This mutex avoid conflicts with id.
    std::unique_lock<std::mutex> lock(m_map->m_constructorMutex);
    m_sequenceId=m_nextId++;
}

OrbMapPoint::OrbMapPoint(const cv::Mat &position, std::shared_ptr<OrbFrame> frame, std::shared_ptr<OrbMap> map, const int &keyPointIndex)
{
    position.copyTo(m_worldPosition);
    cv::Mat cameraCenter = frame->GetCameraCenter();
    m_meanViewingDirection = m_worldPosition - cameraCenter;
    m_meanViewingDirection = m_meanViewingDirection/cv::norm(m_meanViewingDirection);

    cv::Mat offset = position - cameraCenter;
    const float distance = cv::norm(offset);
    const int level = frame->GetUndistortedKeyPoints()[keyPointIndex].octave;
    const float levelScaleFactor = frame->GetScaleFactors()[level];
    const int levels = frame->GetScaleLevels();

    m_maxDistance = distance*levelScaleFactor;
    m_minDistance = m_maxDistance/frame->GetScaleFactors()[levels-1];

    frame->GetDescriptors().row(keyPointIndex).copyTo(m_descriptor);

    // MapPoints can be created from Tracking and Local Mapping. This mutex avoid conflicts with id.
    std::unique_lock<std::mutex> lock(m_constructorMutex);
    m_sequenceId=m_nextId++;
}

OrbMapPoint::~OrbMapPoint()
{

}

std::map<std::shared_ptr<OrbFrame>,size_t> OrbMapPoint::GetObservingKeyframes()
{
    return m_observingKeyframes;
}

bool OrbMapPoint::IsCorrupt()
{
    return false;
}

} // namespace sensation
} // namespace logic
} // namespace opendlv