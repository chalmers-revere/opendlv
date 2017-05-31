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
 
#ifndef DETECTVEHICLE_VEHICLEMEMORYSYSTEM_HPP
#define DETECTVEHICLE_VEHICLEMEMORYSYSTEM_HPP


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detectvehicle/detectedvehicle.hpp"
#include "detectvehicle/rememberedvehicle.hpp"


// TODO add documentation

namespace opendlv {
namespace perception {
namespace detectvehicle {


 /**
 * @brief Class for handling vehicle detection.
 */
class VehicleMemorySystem {
public:
  VehicleMemorySystem();
  virtual ~VehicleMemorySystem();

/* Public methods */
public:
	void UpdateMemory(
      std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>> a_verifiedVehicles,
      double timeStamp);
  void GetMemorizedVehicles(std::vector<std::shared_ptr<RememberedVehicle>>* a_returnContainer);
  int32_t GetNrMemorizedVehicles();
  int32_t GetTotalNrVehicleRects();


/* Public fields */
public:


/* Private methods */
private:
  void CleanMemory(double timeStamp);
  void AssociatePointsToMemory(
      std::vector<int32_t>* index, 
      std::vector<double>* newPoints, 
      std::vector<double>* origPoints);


/* Private fields */
private:
  std::shared_ptr<std::vector<std::shared_ptr<RememberedVehicle>>> m_rememberedVehicles;

};

} // detectvehicle
} // perception
} // opendlv


#endif
