/**
 * Copyright (C) 2015 Chalmers REVERE
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
 
#ifndef DETECTEDVEHICLE_HPP
#define DETECTEDVEHICLE_HPP


#include <iostream>
#include <opencv2/core/core.hpp>


// TODO add documentation

namespace opendlv {
namespace perception {
namespace detectvehicle {


 /**
 * @brief Class for handling vehicle detection.
 */
class DetectedVehicle {
public:
  DetectedVehicle(cv::Rect a_detectionRectangle, double a_timeStamp);
  virtual ~DetectedVehicle();

/* Public methods */
public:
  cv::Rect GetDetectionRectangle();
  double GetTimeStamp();


/* Public fields */
public:


/* Private methods */
private:


/* Private fields */
private:
  cv::Rect m_detectionRectangle;
  double m_timeStamp;

};


} // detectvehicle
} // perception
} // opendlv

#endif
