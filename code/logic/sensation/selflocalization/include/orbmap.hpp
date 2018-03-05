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

#ifndef ORBMAP_HPP
#define ORBMAP_HPP


#include <mutex>
#include <algorithm>
#include <memory>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <list>
#include <utility>

namespace opendlv {
namespace logic {
namespace sensation {

class OrbFrame;
class OrbMapPoint;

class OrbMap {
 public:
    /**
     *  Çonstructor
     */
    OrbMap();
    ~OrbMap();
    /**
     * PushOrbKeyFrame - Push Keyframe to Map.
     */
    void PushOrbKeyFrame(std::shared_ptr<OrbFrame> keyFrame);
    /**
     * PushOrbMapPoint - Push OrbMapPoint to Map.
     */
    void PushOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint);
    /**
     * DeleteOrbMapPoint - Deletes OrbMapPoint from if it exists in Map.
     */
    void DeleteOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint);
    /**
     * DeleteOrbKeyFrame - Deletes OrbKeyFrame from if it exists in Map.
     */
    void DeleteOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame);
    /**
     * SetReferenceMapPoints - Set reference Map Points in Map.
     */
    void SetReferenceMapPoints(std::vector<std::shared_ptr<OrbMapPoint>> referenceMapPoints);
    /**
     * GetAllKeyFrames - Returns all OrbKeyFrames
     */
    std::vector<std::shared_ptr<OrbFrame>> GetAllKeyFrames();
    /**
     * GetAllMapPoints - Returns all OrbMapPoints
     */
    std::vector<std::shared_ptr<OrbMapPoint>> GetAllMapPoints();
    /**
     * GetReferenceMapPoints - Returns all ReferenceMapPoints
     */
    std::vector<std::shared_ptr<OrbMapPoint>> GetReferenceMapPoints();
    void IncrementMajorChangeIndex();
    int LastMajorChangeIndex();
    /**
     *  OrbMapPointsCount - return total number of OrbMapPoints in Map.
     */
    long unsigned int OrbMapPointsCount();
    /**
     *  OrbKeyFramesCount - return total number of OrbKeyFrames in Map.
     */
    long unsigned  OrbKeyFramesCount();
    /**
     * MaxKeyFrameId - Return the maximum OrbKeyFrame Id.
     */
    long unsigned int MaxKeyFrameId();
    /**
     * Reset - Resets the Map.
     */
    void Reset();


 private:
    std::vector<std::shared_ptr<OrbFrame>> m_OrbKeyFrameOrigins;
    std::vector<std::shared_ptr<OrbFrame>> m_keyFrames;
    std::vector<std::shared_ptr<OrbMapPoint>> m_mapPoints;
    std::vector<std::shared_ptr<OrbMapPoint>> m_referenceMapPoints;
    long unsigned int m_maxOrbKeyFrameId;
    int m_majorChangeIndex;
    
    std::mutex m_mapMutex = {};
    std::mutex m_MapUpdateMutex = {};
    std::mutex m_orbMapPointCreationMutex = {};
};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif //ORBMAP_HPP