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

#include <orbframe.hpp>
#include <orbmappoint.hpp>
#include <mutex>

namespace opendlv {
namespace logic {
namespace sensation {

class OrbMap {
 public:
    OrbMap();
    ~OrbMap();
    void pushOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame);
    void pushOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint);
    void deleteOrbMapPoint(std::shared_ptr<OrbMapPoint> orbMapPoint);
    void deleteOrbKeyFrame(std::shared_ptr<OrbFrame> orbKeyFrame);
    void setReferenceMapPoints(std::vector<std::shared_ptr<OrbMapPoint>> referenceMapPoints);
    std::vector<std::shared_ptr<OrbFrame>> GetAllKeyFrames();
    std::vector<std::shared_ptr<OrbMapPoint>> GetAllMapPoints();
    std::vector<std::shared_ptr<OrbMapPoint>> GetReferenceMapPoints();

    void InformNewBigChange();
    int GetLastBigChangeIdx();

    long unsigned int MapPointsInMap();
    long unsigned  KeyFramesInMap();

    long unsigned int GetMaxKFid();

    void clear();

    std::vector<std::shared_ptr<OrbFrame>> mvpKeyFrameOrigins;

    std::mutex mMutexMapUpdate = {};

    
    std::mutex mMutexPointCreation = {};

 private:
    std::vector<std::shared_ptr<OrbFrame>> m_keyFrames;
    std::vector<std::shared_ptr<OrbMapPoint>> m_mapPoints;
    std::vector<std::shared_ptr<OrbMapPoint>> m_referenceMapPoints;

    std::mutex mMutexMap = {};
    long unsigned int mnMaxKFid;

    int mnBigChangeIdx;
};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif //ORBMAP_HPP