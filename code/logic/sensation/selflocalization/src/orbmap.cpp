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
OrbMap::OrbMap():mvpKeyFrameOrigins(), m_keyFrames(), m_mapPoints(), m_referenceMapPoints(), mnMaxKFid(), mnBigChangeIdx()
{
}
OrbMap::~OrbMap()
{

}
void OrbMap::pushOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame) {
    this->m_keyFrames.push_back(orbKeyFrame);
}
void OrbMap::pushOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint) {
    this->m_mapPoints.push_back(orbMapPoint);
}
void OrbMap::deleteOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint) {
    m_mapPoints.erase(std::remove(m_mapPoints.begin(), m_mapPoints.end(), orbMapPoint),
               m_mapPoints.end());
}
void OrbMap::deleteOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame) {
    m_keyFrames.erase(std::remove(m_keyFrames.begin(), m_keyFrames.end(), orbKeyFrame),
               m_keyFrames.end());
}
void OrbMap::setReferenceMapPoints(std::vector<std::shared_ptr<OrbMapPoint>> referenceMapPoints) {
    this->m_referenceMapPoints = referenceMapPoints;
}
std::vector<std::shared_ptr<OrbFrame>> OrbMap::GetAllKeyFrames() {
    return this->m_keyFrames;
}
std::vector<std::shared_ptr<OrbMapPoint>> OrbMap::GetAllMapPoints() {
    return this->m_mapPoints;
}
std::vector<std::shared_ptr<OrbMapPoint>> OrbMap::GetReferenceMapPoints() {
    return this->m_referenceMapPoints;
}
void OrbMap::InformNewBigChange(){

}
int  OrbMap::GetLastBigChangeIdx(){
    return 0;
}
long unsigned int OrbMap::MapPointsInMap(){
    return 0;
}
long unsigned  OrbMap::KeyFramesInMap(){
    return 0;
}
long unsigned int OrbMap::GetMaxKFid(){
    return 0;
}
void OrbMap::clear(){

}
} // namespace sensation
} // namespace logic
} // namespace opendlv