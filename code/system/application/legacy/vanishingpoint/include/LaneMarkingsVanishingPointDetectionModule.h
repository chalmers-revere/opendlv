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

#ifndef OPENDLV_LEGACY_VANISHINGPOINT_LANE_MARKINGS_VANISHING_POINT_DETECTION_MODULE_H_
#define OPENDLV_LEGACY_VANISHINGPOINT_LANE_MARKINGS_VANISHING_POINT_DETECTION_MODULE_H_

#include <memory>
#include <string>

#include <opencv2/core/core.hpp>

#include <opendavinci/generated/odcore/data/image/SharedImage.h>
#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>

#include "ConfigDeserialization.h"
#include "ScanRegion.h"
#include "SharedImageCvAdapter.h"
#include "VanishingPointDetection.h"
#include "VanishingPointLowPass.h"

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

void onMouse(int event, int x, int y, int flags, void * userdata);

class LaneMarkingsVanishingPointDetectionModule
     : public odcore::base::module::DataTriggeredConferenceClientModule {

public:
    LaneMarkingsVanishingPointDetectionModule(int32_t argc, char ** argv);

    void setUp();

    void nextContainer(odcore::data::Container & container);

    void tearDown();

private:
    static const std::string MODULE_NAME;

    static const int WAIT_DELAY_PLAY = 25,
                     WAIT_DELAY_PAUSE = 0;

    static void drawVanishingPoint(cv::Mat & image, const cv::Point2f & vanishingPoint, const cv::Scalar & color);

    /**
     * Run upon construction to fetch the desired sharedImage name from the configuration
     *
     * @return The sharedImageName configuration value. Empty string if not set.
     */
    std::string getSharedImageName() const;

    /**
     * Checks whether the given image has the name set in sharedImageName_. If it is not set, it is
     * overwritten with the name of the given shared image, so that this is used in the future.
     */
    bool isDesiredSharedImage(const odcore::data::image::SharedImage & image) const;

    /**
     * Try to attach to the shared memory, initializing the sharedImageAdapter_ member.
     *
     * @return true if attaching succeeded or already done, false if it failed
     */
    bool attachToSharedMemory(const odcore::data::image::SharedImage & image);

    void processImage(cv::Mat & image);


    bool pause_ = false;

    VanishingPointLowPass lowpass_;

    mutable std::string sharedImageName_;

    // the following members are defined as unique_ptr to allow for delayed initialization
    // (in setUp() instead of the constructor)
    std::unique_ptr<ScanRegion> leftScanRegion_,
                                rightScanRegion_;

    std::unique_ptr<VanishingPointDetection> vanishingPointDetection_;
    std::unique_ptr<SharedImageCvAdapter> sharedImageAdapter_;

};

}
}
}

#endif /* OPENDLV_LEGACY_VANISHINGPOINT_LANE_MARKINGS_VANISHING_POINT_DETECTION_MODULE_H_ */
