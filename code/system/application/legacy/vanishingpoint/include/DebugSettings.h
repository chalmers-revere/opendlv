#ifndef DEBUG_SETTINGS_H_
#define DEBUG_SETTINGS_H_

namespace lmvp {

#ifndef NDEBUG

// change as desired
const bool DEBUG_SHOW_SCAN_COURSE     = false;
const bool DEBUG_SHOW_SCAN_REGIONS    = false;
const bool DEBUG_SHOW_DETECTED_RECTS  = true;
const bool DEBUG_SHOW_LANE_BOUNDARIES = true;
const bool DEBUG_SHOW_PREPROCESSING   = true;
const bool DEBUG_SHOW_ROI             = true;
const bool DEBUG_SHOW_DETECTED_POINTS = false;
const bool DEBUG_SHOW_VANISHING_POINT = true;

// don't change
const bool DEBUG_IS_SET = true;

#else

// don't change
const bool DEBUG_IS_SET = false;
const bool DEBUG_SHOW_SCAN_COURSE     = false;
const bool DEBUG_SHOW_SCAN_REGIONS    = false;
const bool DEBUG_SHOW_DETECTED_RECTS  = false;
const bool DEBUG_SHOW_LANE_BOUNDARIES = false;
const bool DEBUG_SHOW_PREPROCESSING   = false;
const bool DEBUG_SHOW_ROI             = false;
const bool DEBUG_SHOW_DETECTED_POINTS = false;
const bool DEBUG_SHOW_VANISHING_POINT = false;

#endif

}

#endif
