/*
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

#ifndef OPENDLV_LEGACY_VANISHINGPOINT_RURAL_ROAD_IMAGE_PREPROCESSING_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_RURAL_ROAD_IMAGE_PREPROCESSING_H_

#include "ImagePreprocessing.h"

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

class RuralRoadImagePreprocessing : public ImagePreprocessing {
public:
    RuralRoadImagePreprocessing(int blurSize, uint8_t threshold);

    cv::Mat preprocessImage(cv::Mat & coloredImage) const;

private:
    int blurSize_;
    uint8_t threshold_;
};

}
}
}

#endif /* OPENDLV_LEGACY_VANISHINGPOINT_RURAL_ROAD_IMAGE_PREPROCESSING_H_ */
