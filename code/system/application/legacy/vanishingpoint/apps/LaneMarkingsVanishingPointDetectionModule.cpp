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

#include "LaneMarkingsVanishingPointDetectionModule.h"

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

#include "LmvpTypes.h"

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

using odcore::base::module::DataTriggeredConferenceClientModule;
using odcore::data::Container;
using odcore::data::image::SharedImage;
using odcore::exceptions::ValueForKeyNotFoundException;

void onMouse(int event, int x, int y, int flags, void * userdata) {
    LMVP_UNUSED(flags);
    LMVP_UNUSED(userdata);

    if(event == cv::EVENT_MOUSEMOVE) {
        std::cout << "x: " << x << " y: " << y << std::endl;
    }
}

LaneMarkingsVanishingPointDetectionModule::
LaneMarkingsVanishingPointDetectionModule(int32_t argc, char ** argv)
    : DataTriggeredConferenceClientModule(argc, argv, MODULE_NAME)
    , lowpass_(.125f)
    , sharedImageName_()
    , leftScanRegion_()
    , rightScanRegion_()
    , vanishingPointDetection_()
    , sharedImageAdapter_() {}

void LaneMarkingsVanishingPointDetectionModule::setUp() {
    sharedImageName_ = getSharedImageName();

    auto & kvConfig = getKeyValueConfiguration();

    leftScanRegion_.reset(new ScanRegion(
        kvConfig.getValue<ScanRegionBuilder>(MODULE_NAME + ".leftscanregion")));

    rightScanRegion_.reset(new ScanRegion(
        kvConfig.getValue<ScanRegionBuilder>(MODULE_NAME + ".rightscanregion")));

    vanishingPointDetection_.reset(new VanishingPointDetection(
        kvConfig.getValue<RoiBuilder>(MODULE_NAME + ".roi"),
        kvConfig.getValue<int>(MODULE_NAME + ".blurradius"),
        static_cast<uint8_t>(kvConfig.getValue<int>(MODULE_NAME + ".threshold")),
        *leftScanRegion_, *rightScanRegion_,
        kvConfig.getValue<ColumnIndex>(MODULE_NAME + ".maxlanemarkingwidth"),
        kvConfig.getValue<ColumnIndex>(MODULE_NAME + ".maxmarkingrowcount"),
        kvConfig.getValue<double>(MODULE_NAME + ".maxmarkinginclination"),
        kvConfig.getValue<size_t>(MODULE_NAME + ".minpixelcount")));

    if(DEBUG_IS_SET) {
        cv::namedWindow(MODULE_NAME);
        cv::setMouseCallback(MODULE_NAME, onMouse);
    }
}

void LaneMarkingsVanishingPointDetectionModule::nextContainer(Container & container) {
    if(SharedImage::ID() == container.getDataType()) {
        SharedImage image = container.getData<SharedImage>();

        if(isDesiredSharedImage(image)) {
            if(!attachToSharedMemory(image)) {
                return;
            }

            cv::Mat cvImage = sharedImageAdapter_->fetchImage();
            processImage(cvImage);
        }
    }
}

void LaneMarkingsVanishingPointDetectionModule::tearDown() {
    if(DEBUG_IS_SET) {
        cv::destroyWindow(MODULE_NAME);
    }
}

void LaneMarkingsVanishingPointDetectionModule::drawVanishingPoint(cv::Mat & image, const cv::Point2f & vanishingPoint, const cv::Scalar & color) {
    int radius = 10;
    float offset = 1.5f * static_cast<float>(radius);
    cv::circle(image, vanishingPoint, radius, color, 2);
    cv::line(image, vanishingPoint - cv::Point2f(0, offset), vanishingPoint + cv::Point2f(0, offset),
             color, 1);
    cv::line(image, vanishingPoint - cv::Point2f(offset, 0), vanishingPoint + cv::Point2f(offset, 0),
             color, 1);
}

const std::string LaneMarkingsVanishingPointDetectionModule::MODULE_NAME("vanishingpoint");

std::string LaneMarkingsVanishingPointDetectionModule::getSharedImageName() const {
    try {
        return getKeyValueConfiguration().getValue<std::string>(MODULE_NAME + ".sharedimagename");
    }
    catch(const ValueForKeyNotFoundException & e) {
        CLOG << "Didn't find key " << MODULE_NAME + ".sharedimagename" << std::endl;
        return std::string();
    }
}

bool LaneMarkingsVanishingPointDetectionModule::isDesiredSharedImage(const SharedImage & image) const {
    if(sharedImageName_.empty()) {
        CLOG << "no \"sharedImageName\" config value set, using shared image " << image.getName() << std::endl;
        sharedImageName_ = image.getName();
        return true;
    }

    return 0 == image.getName().compare(sharedImageName_);
}

bool LaneMarkingsVanishingPointDetectionModule::attachToSharedMemory(const SharedImage & image) {
    if(!sharedImageAdapter_) {
        try {
            sharedImageAdapter_.reset(new SharedImageCvAdapter(image));
        }
        catch(const FailedAttachingToSharedMemoryException & e) {
            return false;
        }
    }

    return true;
}

void LaneMarkingsVanishingPointDetectionModule::processImage(cv::Mat & image) {
    std::shared_ptr<cv::Point2f> vanishingPoint = vanishingPointDetection_->detectVanishingPoint(image);
    vanishingPoint = lowpass_.lowpass(vanishingPoint);

    if(DEBUG_IS_SET) {
        if(DEBUG_SHOW_SCAN_REGIONS) {
            leftScanRegion_->draw(image, cv::Scalar(0,212,89));
            rightScanRegion_->draw(image, cv::Scalar(212,195,0));
        }

        if(DEBUG_SHOW_VANISHING_POINT) {
            if(vanishingPoint) {
                drawVanishingPoint(image, *vanishingPoint, cv::Scalar(0,0,255));
            }
        }

        cv::imshow(MODULE_NAME, image);

        int waitDelay = pause_ ? WAIT_DELAY_PAUSE : WAIT_DELAY_PLAY;
        char key = cv::waitKey(waitDelay);
        if(key == ' ') {
            pause_ = !pause_;
        }
    }
}

}
}
}

int32_t main(int32_t argc, char ** argv) {
    using namespace opendlv::legacy::vanishingpoint;

    LaneMarkingsVanishingPointDetectionModule vpModule(argc, argv);
    vpModule.runModule();

    return EXIT_SUCCESS;
}
