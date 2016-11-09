#ifndef LMVP_TYPES_H_
#define LMVP_TYPES_H_

#include <vector>
#include <stdexcept>

#include <opencv2/core/core.hpp>

// from http://stackoverflow.com/a/3599170
#define LMVP_UNUSED(x) (void)(x)

namespace lmvp {

typedef uint8_t PixelValue;
typedef int ColumnIndex;
typedef int RowIndex;
typedef std::vector<std::vector<cv::Point> > Contours;

/**
 * Enum for left and right. Values are chosen to represent the human-perceived direction in the
 * image coordinate system (x axis). Makes the code completely unreadable for Guugu Yimithirr
 *
 * https://en.wikipedia.org/wiki/Relative_direction#Cultures_without_relative_directions
 */
enum class RelativeDirection {
    LEFT = -1,
    RIGHT = 1
};

/**
 * Convenience vector for iterating over the relative directions.
 */
const std::vector<RelativeDirection> RELATIVE_DIRECTIONS{RelativeDirection::LEFT, RelativeDirection::RIGHT};

class InvalidRelativeDirection : public std::invalid_argument {
public:
    InvalidRelativeDirection() : std::invalid_argument("Passed a direction that's neither LEFT nor RIGHT") {}
};

RelativeDirection oppositeDirection(RelativeDirection otherDirection);

/**
 * Calculates the sign of a number (-1 for negative, +1 for positive).
 *
 * from http://stackoverflow.com/a/4609795
 */
template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

const float EPS = 1e-6;

}

#endif /* LMVP_TYPES_H_ */
