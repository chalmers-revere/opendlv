#ifndef LANE_MARKINGS_SCAN_REGION_H_
#define LANE_MARKINGS_SCAN_REGION_H_

#include <vector>
#include <map>

#include <opencv2/core/core.hpp>

#include "RegionOfInterestGeometry.h"
#include "LmvpTypes.h"

namespace lmvp {

/**
 * Represents a directed infinite line.
 */
template<typename T>
class DirectedInfiniteLine {
public:
    DirectedInfiniteLine toRoiCoordinates(const RegionOfInterestGeometry & roi) const {
        DirectedInfiniteLine roiLine(*this);

        roiLine.origin_ = roi.toRoiCoordinates(roiLine.origin_);

        return roiLine;
    }

    DirectedInfiniteLine toImageCoordinates(const RegionOfInterestGeometry & roi) const {
        DirectedInfiniteLine imageLine(*this);

        imageLine.origin_ = roi.toImageCoordinates(imageLine.origin_);

        return imageLine;
    }

    /**
     * Calculates the intersection point of this line with another.
     */
    cv::Point_<T> intersection(const DirectedInfiniteLine & other) const {
        T numerator = (other.origin_.y - origin_.y) * direction_.x +
                      (origin_.x - other.origin_.x) * direction_.y,

          denominator = other.direction_.x * direction_.y - other.direction_.y * direction_.x;

        double parameter2 = static_cast<double>(numerator)/static_cast<double>(denominator),
               // FIXME: use std::round on x and y when T == int
               x = static_cast<double>(other.origin_.x) + static_cast<double>(other.direction_.x) * parameter2,
               y = static_cast<double>(other.origin_.y) + static_cast<double>(other.direction_.y) * parameter2;

        return cv::Point_<T>(static_cast<T>(x), static_cast<T>(y));
    }

    /**
     * Create a directed infinite line.
     *
     * @param origin The point used as the origin of the line.
     * @param destination A second point on the line, which is used to compute the direction vector.
     *                    The direction vector is computed as destination - origin.
     */
    DirectedInfiniteLine(const cv::Point_<T> & origin, const cv::Point_<T> & destination)
        : origin_(origin)
        , direction_(destination - origin) {}


    /**
     * @param phiRad Angle between x axis and direction vector, in radians
     * @param radius Present such that the <int> template version is still usable. Defines the
     *               length of the distance vector.
     */
    DirectedInfiniteLine(const cv::Point_<T> & origin, double phiRad, double radius)
        : origin_(origin)
        , direction_(static_cast<T>(radius * std::cos(phiRad)),
                     static_cast<T>(radius * std::sin(phiRad))) { }

    /**
     * The length of the direction vector, i.e. the distance between origin and destination.
     */
    double getRadius() const {
        return cv::norm(direction_);
    }

    /**
     * The angle of the direction vector to the x axis, in radians. As we are in a coordinate system
     * with a swapped y axis, counter-clockwise angles are negative.
     */
    double getAngle() const {
        return std::atan2(direction_.y, direction_.x);
    }

    /**
     * Calculates the x-coordinate of where the line intersects the given row.
     */
    template<typename S>
    S getX(S y) const {
        // FIXME: use std::round in <int> specialization
        return static_cast<S>(static_cast<double>(origin_.x) + static_cast<double>(y - static_cast<S>(origin_.y)) * getSlope());
    }

    /**
     * Calculates the y.coordinate of the point where the line intersects the given x coordinate.
     */
    template<typename S>
    S getY(S x) const {
        // FIXME: use std::round in <int> specialization
        return static_cast<S>(static_cast<double>(origin_.y) + static_cast<double>(x - static_cast<S>(origin_.x)) / getSlope());
    }

    /**
     * get the slope of the line
     */
    double getSlope() const {
        return static_cast<double>(direction_.x)/static_cast<double>(direction_.y);
    }

    /**
     * The distance to the line. In the image coordinate system: If the line is directed upwards,
     * a positive distance means the point is right of the line.
     */
    double distance(const cv::Point_<T> & point) const {
        cv::Point_<T> shiftedPoint = point - origin_;
        cv::Point_<T> directionNormal(-direction_.y, direction_.x);

        return directionNormal.dot(shiftedPoint) / cv::norm(directionNormal);
    }

    cv::Point_<T> getOrigin() const { return origin_; }
    cv::Point_<T> getDirection() const { return direction_; }

    void draw(cv::Mat & image, const cv::Scalar color) const {
        auto intersections = getRectIntersections(cv::Rect(cv::Point(0,0), image.size()));

        cv::line(image, intersections[0], intersections[1], color, 1);

        cv::circle(image, origin_, 4, color, 2);
        cv::circle(image, origin_ + direction_, 2, color, 2);
    }

private:
    /**
     * Helper for drawing the line on an image
     */
    std::vector<cv::Point> getRectIntersections(const cv::Rect & rectangle) const {
        std::vector<cv::Point> allIntersections;

        const cv::Point bottomRight = rectangle.br() - cv::Point(1,1);

        allIntersections.push_back(cv::Point(rectangle.x, getY(rectangle.x)));
        allIntersections.push_back(cv::Point(getX(rectangle.y), rectangle.y));
        allIntersections.push_back(cv::Point(bottomRight.x, getY(bottomRight.x)));
        allIntersections.push_back(cv::Point(getX(bottomRight.y), bottomRight.y));

        std::vector<cv::Point> relevantIntersections;

        for(const auto & intersection : allIntersections) {
            if(rectangle.contains(intersection)) {
                relevantIntersections.push_back(intersection);
            }

            if(relevantIntersections.size() == 2) {
                break;
            }
        }

        return relevantIntersections;
    }

    cv::Point_<T> origin_,
                  direction_;
};

typedef DirectedInfiniteLine<int> DirInfLinei;
typedef DirectedInfiniteLine<float> DirInfLinef;

/**
 * Defines a corridor in which to look for lane markings. The boundaries are formed by two directed
 * infinite lines. A point is deemed in the corridor if it is right of the left border and left of
 * the right border.
 */
class ScanRegion {
public:
    typedef std::vector<std::pair<cv::Point,cv::Point>> Lines;

    ScanRegion toRoiCoordinates(const RegionOfInterestGeometry & roi) const {
        ScanRegion roiRegion(*this);

        DirInfLinei & leftBorder = roiRegion.borders_.at(RelativeDirection::LEFT);
        leftBorder = leftBorder.toRoiCoordinates(roi);

        DirInfLinei & rightBorder = roiRegion.borders_.at(RelativeDirection::RIGHT);
        rightBorder = rightBorder.toRoiCoordinates(roi);

        return roiRegion;
    }

    ScanRegion(const cv::Point & lowerLeft, const cv::Point & upperLeft,
               const cv::Point & upperRight, const cv::Point & lowerRight)
        : borders_() {

        borders_.insert(std::make_pair(RelativeDirection::LEFT,
                                       DirInfLinei(lowerLeft, upperLeft)));

        borders_.insert(std::make_pair(RelativeDirection::RIGHT,
                                       DirInfLinei(lowerRight, upperRight)));
    }

    /**
     * Returns true if the given point is right of the left border and vice versa. Makes use of the
     * fact that the signs of the relativeDirections enum and getBorder() correlate, as they are
     * defined in the same coordinate system
     */
    bool isInside(const cv::Point & point) const {
        for(const auto relativeDirection : RELATIVE_DIRECTIONS) {
            if(sgn(getBorder(relativeDirection).distance(point)) == sgn(relativeDirection)) {
                return false;
            }
        }

        return true;
    }

    cv::Point linesIntersection() const {
        return getBorder(RelativeDirection::LEFT).intersection(getBorder(RelativeDirection::RIGHT));
    }

    Lines getLineVectors() const {
        Lines points;

        for(const auto relativeDirection : RELATIVE_DIRECTIONS) {
            auto border = getBorder(relativeDirection);
            auto origin = border.getOrigin();
            points.push_back(std::make_pair(origin, origin + border.getDirection()));
        }

        return points;
    }

    const DirInfLinei & getBorder(RelativeDirection relativeDirection) const {
        return borders_.at(relativeDirection);
    }

    void draw(cv::Mat & image, const cv::Scalar & color) const {
        // tint area
        cv::Scalar fillColor = .5 * color;
        for(RowIndex y = 0; y < image.size().height; ++y) {
            cv::Vec3b * row = image.ptr<cv::Vec3b>(y);

            for(int x = 0; x < image.size().width; ++x) {
                if(isInside(cv::Point(x, y))) {
                    cv::Vec3b & pixelValue = row[x];

                    for(int i = 0; i < 3; ++i) {
                        pixelValue[i] = static_cast<uchar>(pixelValue[i] | static_cast<uchar>(fillColor[i]));
                    }
                }
            }
        }

        for(const auto & border : borders_) {
            border.second.draw(image, color);
        }
    }

private:
    std::map<RelativeDirection, DirInfLinei> borders_;
};

}

#endif /* LANE_MARKINGS_SCAN_REGION_H_ */
