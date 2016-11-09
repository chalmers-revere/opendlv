#include "MatVisitInterface.h"

namespace lmvp {

#ifndef NDEBUG
// used for Debugging, copied from StackOverflow
// http://stackoverflow.com/a/17820615
std::string type2str(int type) {
    using std::string;
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = static_cast<uchar>(1 + (type >> CV_CN_SHIFT));

    switch (depth) {
    case CV_8U:
        r = "8U";
        break;
    case CV_8S:
        r = "8S";
        break;
    case CV_16U:
        r = "16U";
        break;
    case CV_16S:
        r = "16S";
        break;
    case CV_32S:
        r = "32S";
        break;
    case CV_32F:
        r = "32F";
        break;
    case CV_64F:
        r = "64F";
        break;
    default:
        r = "User";
        break;
    }

    r += "C";
    r += static_cast<char>(chans + '0');

    return r;
}

#endif
}
