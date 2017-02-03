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

#ifndef OPENDLV_LEGACY_VANISHINGPOINT_IMAGE_PREPROCESSING_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_IMAGE_PREPROCESSING_H_

#include <opencv2/imgproc/imgproc.hpp>

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

/**
 * Abstract base class to allow for different preprocessing implementations, e.g. for real roads and
 * the Carolo environment.
 */
class ImagePreprocessing {
public:
    virtual ~ImagePreprocessing() = default;

    virtual cv::Mat preprocessImage(cv::Mat &) const = 0;
};

}
}
}

#endif
