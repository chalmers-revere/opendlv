#ifndef LANE_MARKINGS_DETECTION_H_
#define LANE_MARKINGS_DETECTION_H_

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImagePreprocessing.h"
#include "LaneMarkingsScan.h"
#include "RegionOfInterestGeometry.h"
#include "ScanRegion.h"

namespace lmvp {

class LaneBorderLowpass {
public:
    LaneBorderLowpass(double dampingFactor) : dampingFactor_(dampingFactor), lastBorder_() { }

    std::shared_ptr<DirInfLinef> lowpass(std::shared_ptr<DirInfLinef> border) {
        if(!border) {
            return border;
        }

        if(!lastBorder_) {
            lastBorder_ = border;
            return border;
        }

        cv::Point2f distance = border->getOrigin() - lastBorder_->getOrigin(),
                    origin = lastBorder_->getOrigin() + dampingFactor_ * distance;

        double dPhi = border->getAngle() - lastBorder_->getAngle(),
               phi = lastBorder_->getAngle() + dampingFactor_ * dPhi;

        lastBorder_ = std::make_shared<DirInfLinef>(origin, phi, border->getRadius());

        return lastBorder_;
    }

private:
    const double dampingFactor_;

    std::shared_ptr<DirInfLinef> lastBorder_;
};

/**
 * Combines all processing steps into a single unit for detecting lane markings.
 */
class VanishingPointDetection {
public:
    VanishingPointDetection(// preprocessing parameters
                          const RegionOfInterestGeometry & roiGeometry, int blurSize,
                          uint8_t threshold,
                          // scanning parameters
                          const ScanRegion & leftScanRegion, const ScanRegion & rightScanRegion,
                          ColumnIndex maxMarkingWidth, ColumnIndex maxMarkingRowCount,
                          double maxMarkingInclination, size_t minPixelCount)

        : roiGeometry_(roiGeometry)
        , roiLeftScanRegion_(leftScanRegion.toRoiCoordinates(roiGeometry))
        , roiRightScanRegion_(rightScanRegion.toRoiCoordinates(roiGeometry))
        , imagePreprocessing_(blurSize, threshold)
        , laneMarkingsScan_(roiLeftScanRegion_, roiRightScanRegion_,
                            maxMarkingWidth, maxMarkingRowCount, maxMarkingInclination, minPixelCount)
        , leftLowpass_(.0625)
        , rightLowpass_(.0625) {

        if(maxMarkingRowCount > roiGeometry.get().height) {
            // FIXME: use CLOG once integrated into the ODV stack
            std::clog << "Warning: maxMarkingLength is set to a higher value (" << maxMarkingRowCount <<
                    ") than the ROI height (" << roiGeometry.get().height << "), therefore only the"
                    " ROI size decides about abortion of the scanning." << std::endl;
        }
    }

    std::shared_ptr<cv::Point2f> detectVanishingPoint(cv::Mat & image) {
        auto laneMarkings = detectLaneMarkings(image);

        return computeVanishingPoint(image, laneMarkings);
    }

private:
    template<typename T>
    static void swapValues(T & v1, T & v2) {
        T originalV1 = v1;

        v1 = v2;
        v2 = originalV1;
    }

    /**
     * Makes the angle of the rotatedRect always be measured along the longer of the two sides and
     * the width always greater than the height of the rectangle.
     */
    static cv::RotatedRect & normalizeRotatedRectAngle(cv::RotatedRect & rotatedRect) {
        if(rotatedRect.size.width > rotatedRect.size.height) {
            return rotatedRect;
        }

        rotatedRect.angle -= 90.0f;
        swapValues(rotatedRect.size.height, rotatedRect.size.width);

        return rotatedRect;
    }

    static cv::Point roundPoint(const cv::Point2f & point) {
        return cv::Point(static_cast<int>(std::round(point.x)),
                         static_cast<int>(std::round(point.y)));
    }

    static float toRad(float degree) {
        return degree / 180.0f * static_cast<float>(M_PI);
    }

#ifndef NDEBUG
    static void drawVanishingPoint(cv::Mat & image, const cv::Point2f & vanishingPoint, const cv::Scalar & color) {
        float radius = 5;
        cv::line(image, vanishingPoint - cv::Point2f(0, radius), vanishingPoint + cv::Point2f(0, radius),
                 color, 1);
        cv::line(image, vanishingPoint - cv::Point2f(radius, 0), vanishingPoint + cv::Point2f(radius, 0),
                 color, 1);
    }
#endif

#ifndef NDEBUG
    void drawRotatedRect(cv::Mat & image, const cv::RotatedRect & rotatedRect, const cv::Scalar & color) {
        cv::Point2f vertices[4];
        rotatedRect.points(vertices);

        for(int i = 0; i < 4; ++i) {
            cv::Point2f start = roiGeometry_.toImageCoordinates(vertices[i]),
                        end = roiGeometry_.toImageCoordinates(vertices[(i+1)%4]);
            cv::line(image, start, end, color, 1);
        }
    }
#endif

    LaneMarkingsScan::LaneMarkings detectLaneMarkings(cv::Mat & image) {
        cv::Mat roi = image(roiGeometry_.get());

        cv::Mat preprocessedRoi = imagePreprocessing_.preprocessImage(roi);

#ifndef NDEBUG
        cv::Mat debugRoi = roi;
        if(DEBUG_SHOW_PREPROCESSING) {
            cv::cvtColor(preprocessedRoi, debugRoi, CV_GRAY2BGR);
        }
        laneMarkingsScan_.setDebugImage(debugRoi);
#endif

        auto contours = laneMarkingsScan_.scanLaneMarkings(preprocessedRoi);

#ifndef NDEBUG
        if(DEBUG_SHOW_DETECTED_POINTS) {
            for(const auto & scannerContours : contours) {
                for(const auto & contour : scannerContours.second) {
                    for(const auto & point : contour) {
                        cv::circle(debugRoi, point, 3, cv::Scalar(255,0,0), CV_FILLED);
                    }
                }
            }
        }

        if(DEBUG_SHOW_ROI) {
            cv::rectangle(image, roiGeometry_.get(), cv::Scalar(0,0,255), 1);
        }
        debugRoi.copyTo(roi);
#endif

        return contours;
    }

    /**
     * @param dashes List of contours of dash markings in the image, sorted by their y-coordinates
     *               (the first dash is the one closest to the bottom in the image)
     *
     * @return An empty pointer if no dashes were recognized
     */
    std::shared_ptr<DirInfLinef> computeLaneBoundary(cv::Mat & debugImage, const Contours & dashes) {
        if(dashes.empty()) {
            return std::shared_ptr<DirInfLinef>();
        }

        cv::RotatedRect firstDash = cv::minAreaRect(*dashes.begin()),
                        lastDash = cv::minAreaRect(*dashes.rbegin());

        if(dashes.size() == 1) {
            if(firstDash.size.width * firstDash.size.height < EPS) {
                return std::shared_ptr<DirInfLinef>();
            }
        }

        normalizeRotatedRectAngle(firstDash);
        normalizeRotatedRectAngle(lastDash);

        cv::Point2f origin = firstDash.center -
                             cv::Point2f(std::cos(toRad(firstDash.angle)) * firstDash.size.width * .5f,
                                         std::sin(toRad(firstDash.angle)) * firstDash.size.width * .5f),

                    destination = lastDash.center +
                                  cv::Point2f(std::cos(toRad(lastDash.angle)) * lastDash.size.width * .5f,
                                              std::sin(toRad(lastDash.angle)) * lastDash.size.width * .5f);

#ifndef NDEBUG
        if(DEBUG_SHOW_DETECTED_RECTS) {
            drawRotatedRect(debugImage, firstDash, cv::Scalar(0,255,0));
            drawRotatedRect(debugImage, lastDash, cv::Scalar(0,255,0));
        }
#endif

        return std::shared_ptr<DirInfLinef>(new DirInfLinef(origin, destination));
    }

    /**
     * @return An empty pointer if the vanishing point could not be determined
     */
    std::shared_ptr<cv::Point2f> computeVanishingPoint(cv::Mat & debugImage, const LaneMarkingsScan::LaneMarkings & laneMarkings) {
        std::shared_ptr<DirInfLinef> leftLaneBoundary = leftLowpass_.lowpass(computeLaneBoundary(debugImage, laneMarkings.at(RelativeDirection::LEFT))),
                                     rightLaneBoundary = rightLowpass_.lowpass(computeLaneBoundary(debugImage, laneMarkings.at(RelativeDirection::RIGHT)));

        if(DEBUG_SHOW_LANE_BOUNDARIES) {
            if(leftLaneBoundary) {
                leftLaneBoundary->toImageCoordinates(roiGeometry_).draw(debugImage, cv::Scalar(0,255,0));
            }
            if(rightLaneBoundary) {
                rightLaneBoundary->toImageCoordinates(roiGeometry_).draw(debugImage, cv::Scalar(0,255,0));
            }
        }

        if(!leftLaneBoundary || !rightLaneBoundary) {
            return std::shared_ptr<cv::Point2f>();
        }

        cv::Point2f roiVanishingPoint = leftLaneBoundary->intersection(*rightLaneBoundary);
        std::shared_ptr<cv::Point2f> vanishingPoint(new cv::Point2f(roiGeometry_.toImageCoordinates(roiVanishingPoint)));

#ifndef NDEBUG
        if(DEBUG_SHOW_VANISHING_POINT) {
            drawVanishingPoint(debugImage, *vanishingPoint, cv::Scalar(255,0,0));
            std::cout << "VP: (" << vanishingPoint->x << ", " << vanishingPoint->y << ")" << std::endl;
        }
#endif

        return vanishingPoint;
    }

    const RegionOfInterestGeometry roiGeometry_;

    /**
     * The scan regions, transformed to ROI coordinates
     */
    const ScanRegion roiLeftScanRegion_,
                     roiRightScanRegion_;

    AstaZeroImagePreprocessing imagePreprocessing_;
    LaneMarkingsScan laneMarkingsScan_;

    LaneBorderLowpass leftLowpass_, rightLowpass_;
};

}

#endif
