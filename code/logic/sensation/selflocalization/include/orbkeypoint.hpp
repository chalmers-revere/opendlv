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

#ifndef OPENDLV_KEYPOINT_HPP
#define OPENDLV_KEYPOINT_HPP
#include <opencv/cv.h>


namespace opendlv {
namespace logic {
namespace sensation {

class OrbKeyPoint
{
public:
    OrbKeyPoint(cv::KeyPoint, float depth, bool outlier);

    ~OrbKeyPoint();

private:
    cv::KeyPoint m_keyPoint;
    float m_depth;
    bool m_outlier;
};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif //OPENDLV_KEYPOINT_HPP