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
#include "orbmap.hpp"


namespace opendlv {
namespace logic {
namespace sensation {
OrbMap::OrbMap():m_OrbKeyFrameOrigins(), m_keyFrames(), m_mapPoints(), m_referenceMapPoints(), m_maxOrbKeyFrameId(), m_majorChangeIndex()
{
}
OrbMap::~OrbMap()
{

}
void OrbMap::PushOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame) {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    this->m_keyFrames.push_back(orbKeyFrame);
    // TODO increment max id 
}
void OrbMap::PushOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint) {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    this->m_mapPoints.push_back(orbMapPoint);
}
void OrbMap::DeleteOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint) {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    m_mapPoints.erase(std::remove(m_mapPoints.begin(), m_mapPoints.end(), orbMapPoint),
               m_mapPoints.end());
}
void OrbMap::DeleteOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame) {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    m_keyFrames.erase(std::remove(m_keyFrames.begin(), m_keyFrames.end(), orbKeyFrame),
               m_keyFrames.end());
}
void OrbMap::SetReferenceMapPoints(std::vector<std::shared_ptr<OrbMapPoint>> referenceMapPoints) {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    this->m_referenceMapPoints = referenceMapPoints;
}
std::vector<std::shared_ptr<OrbFrame>> OrbMap::GetAllKeyFrames() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return this->m_keyFrames;
}
std::vector<std::shared_ptr<OrbMapPoint>> OrbMap::GetAllMapPoints() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return this->m_mapPoints;
}
std::vector<std::shared_ptr<OrbMapPoint>> OrbMap::GetReferenceMapPoints() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return this->m_referenceMapPoints;
}
void OrbMap::OnMajorChange(){

}
int  OrbMap::LastMajorChangeIndex() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return m_majorChangeIndex;
}
long unsigned int OrbMap::OrbMapPointsCount() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return m_mapPoints.size();
}
long unsigned  OrbMap::OrbKeyFramesCount() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return m_keyFrames.size();
}
long unsigned int OrbMap::MaxKeyFrameId() {
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    return m_maxOrbKeyFrameId;
}
void OrbMap::Reset(){
    std::lock_guard<std::mutex> lock(this->m_mapMutex);
    m_keyFrames.erase(m_keyFrames.begin(), m_keyFrames.end());
    m_mapPoints.erase(m_mapPoints.begin(), m_mapPoints.end());
    m_referenceMapPoints.erase(m_referenceMapPoints.begin(), m_referenceMapPoints.end());
    m_OrbKeyFrameOrigins.erase(m_OrbKeyFrameOrigins.begin(), m_OrbKeyFrameOrigins.end());
    m_keyFrames.clear();
    m_mapPoints.clear();
    m_referenceMapPoints.clear();
    m_OrbKeyFrameOrigins.clear();

}
} // namespace sensation
} // namespace logic
} // namespace opendlv