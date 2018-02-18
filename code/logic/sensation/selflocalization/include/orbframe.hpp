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

#ifndef LOGIC_SENSATION_SELFLOCALIZATION_ORBFRAME_HPP
#define LOGIC_SENSATION_SELFLOCALIZATION_ORBFRAME_HPP

#include <memory>

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "opendavinci/generated/odcore/data/CompactPointCloud.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "opendavinci/odcore/wrapper/SharedMemory.h"
#include "opendavinci/odcore/wrapper/SharedMemoryFactory.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/wrapper/Eigen.h>
#include <vector>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <orbkeypoint.hpp>

namespace opendlv {
namespace logic {
namespace sensation {

class OrbFrame 
{
public:
    // Copy constructor.
    OrbFrame(cv::Mat leftGreyImage, cv::Mat rightGreyImage, std::vector<OrbKeyPoint>);
    ~OrbFrame();

private:
    std::vector<OrbKeyPoint> m_keypoints;
    cv::Mat m_leftGreyImage, m_rightGreyImage;

};

} // namespace sensation
} // namespace logic
} // namespace opendlv

#endif