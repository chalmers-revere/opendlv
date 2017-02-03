#include "LmvpTypes.h"

namespace opendlv {
namespace legacy {
namespace vanishingpoint {

RelativeDirection oppositeDirection(RelativeDirection otherDirection) {
    switch(otherDirection) {
    case RelativeDirection::LEFT:
        return RelativeDirection::RIGHT;

    case RelativeDirection::RIGHT:
        return RelativeDirection::LEFT;

    default:
        throw InvalidRelativeDirection();
    }
}

}
}
}
