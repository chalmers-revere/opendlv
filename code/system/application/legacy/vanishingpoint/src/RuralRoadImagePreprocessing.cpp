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

#include "RuralRoadImagePreprocessing.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

RuralRoadImagePreprocessing::RuralRoadImagePreprocessing(int blurSize, uint8_t threshold)
    : blurSize_(blurSize)
    , threshold_(threshold) { }

cv::Mat RuralRoadImagePreprocessing::preprocessImage(cv::Mat & coloredImage) const {
    cv::Mat image;

    // convert to grayscale
    cv::cvtColor(coloredImage, image, CV_BGR2GRAY);

    // crop car
    cv::Point roiTopLeft(0,169);
    cv::rectangle(image, cv::Point(268,240) - roiTopLeft, cv::Point(527,408) - roiTopLeft, cv::Scalar(0,0,0), CV_FILLED);
//        cv::Mat labImage;
//        cv::cvtColor(coloredImage, labImage, CV_BGR2Lab);
//        cv::Mat labChannels[3];
//        cv::split(labImage, labChannels);
//        image = labChannels[0];

    //cv::blur(image, image, cv::Size(blurSize_, blurSize_));
    //cv::equalizeHist(image, image);

    cv::threshold(image, image, threshold_, 255, cv::THRESH_BINARY);

    //const int dilateRadius = 2;
    //cv::dilate(image, image, cv::getStructuringElement(cv::MORPH_ELLIPSE,
             //                       cv::Size(2 * dilateRadius + 1, 2 * dilateRadius + 1)) );

    return image;
}

}
}
}
