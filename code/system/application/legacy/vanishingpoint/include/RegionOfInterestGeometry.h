#ifndef REGION_OF_INTEREST_GEOMETRY_H_
#define REGION_OF_INTEREST_GEOMETRY_H_

#include <opencv2/core/core.hpp>

namespace lmvp {

class RegionOfInterestGeometry {
public:
    RegionOfInterestGeometry(const cv::Rect & roi) : roi_(roi){ }

    cv::Rect get() const {
        return roi_;
    }

    template<typename T>
    cv::Point_<T> toImageCoordinates(const cv::Point_<T> & roiCoordinates) const {
        // Workaround: copy constructor for different types does not work, whereas assignment does
        cv::Point_<T> topLeft = roi_.tl();
        return roiCoordinates + topLeft;
    }

    template<typename T>
    cv::Point_<T> toRoiCoordinates(const cv::Point_<T> & imageCoordinates) const {
        return imageCoordinates - cv::Point_<T>(roi_.tl());
    }

private:
    cv::Rect roi_;
};

}

#endif /* REGION_OF_INTEREST_GEOMETRY_H_ */
