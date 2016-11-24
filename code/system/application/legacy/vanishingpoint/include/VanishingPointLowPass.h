/**
 * Copyright (C) 2016 Crispin Kirchner
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef OPENDLV_LEGACY_VANISHINGPOINT_VANISHING_POINT_LOW_PASS_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_VANISHING_POINT_LOW_PASS_H_

#include <memory>
#include <opencv2/core/core.hpp>

namespace lmvp {

class VanishingPointLowPass {
public:
    VanishingPointLowPass(float dampingFactor);

    std::shared_ptr<cv::Point2f> lowpass(std::shared_ptr<cv::Point2f> vanishingPoint);

private:
    const float dampingFactor_;

    std::shared_ptr<cv::Point2f> lastVanishingPoint_;
};

}


#endif /* OPENDLV_LEGACY_VANISHINGPOINT_VANISHING_POINT_LOW_PASS_H_ */
