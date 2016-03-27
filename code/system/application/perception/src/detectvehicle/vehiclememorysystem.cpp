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

#include "detectvehicle/vehiclememorysystem.hpp"

// TODO add documentation



namespace opendlv {
namespace perception {
namespace detectvehicle {

VehicleMemorySystem::VehicleMemorySystem()
    : m_rememberedVehicles()
{
}
  
VehicleMemorySystem::~VehicleMemorySystem()
{
	
}


void VehicleMemorySystem::UpdateMemory(
    std::vector<std::shared_ptr<DetectedVehicle>>* a_verifiedVehicles,
    double timeStamp)
{
  std::cout << "\n";
  std::cout << "timeStamp: " << timeStamp << "\n";
  this->CleanMemory(timeStamp);
  
  
  std::vector<int32_t> indexHolder(a_verifiedVehicles->size()); // Should be size of candidat points..
  std::vector<double> newPoints;
  //std::cout << "a_verifiedVehicles->size(): " << a_verifiedVehicles->size() << "\n";
  //std::cout << "m_rememberedVehicles->size(): " << m_rememberedVehicles.size() << "\n";

  for (uint32_t i=0; i<a_verifiedVehicles->size(); i++) {
    cv::Rect detRect = a_verifiedVehicles->at(i)->GetDetectionRectangle();
    newPoints.push_back(detRect.x + detRect.width/2);
    newPoints.push_back(detRect.y + detRect.height/2);
  }
                                    
  std::vector<double> origPoints;

  for (uint32_t i=0; i<m_rememberedVehicles.size(); i++) {
    cv::Rect detRect = m_rememberedVehicles.at(i).GetLatestDetection()->GetDetectionRectangle();
    origPoints.push_back(detRect.x + detRect.width/2);
    origPoints.push_back(detRect.y + detRect.height/2);
  }

  AssociatePointsToMemory(&indexHolder,&newPoints, &origPoints);

  for (uint32_t i=0; i<indexHolder.size(); i++) {
    int32_t currentIndex = indexHolder.at(i);
    if (currentIndex == -1) {
      // new detection, not associated with an old memory
      // TODO Is this really correct? not just placed on the stack and removed?
      RememberedVehicle tmp;
      tmp.AddMemory(a_verifiedVehicles->at(i));
      m_rememberedVehicles.push_back(tmp);
    }
    else {
      m_rememberedVehicles.at(currentIndex).AddMemory(a_verifiedVehicles->at(i));
    }
  }
}

void VehicleMemorySystem::GetMemorizedVehicles(std::vector<RememberedVehicle>* a_returnContainer)
{
  a_returnContainer->clear();
  for (uint32_t i=0; i<m_rememberedVehicles.size(); i++) {
    a_returnContainer->push_back(m_rememberedVehicles.at(i));
  }
}

int32_t VehicleMemorySystem::GetNrMemorizedVehicles()
{
  return m_rememberedVehicles.size();
}

int32_t VehicleMemorySystem::GetTotalNrVehicleRects()
{
  int32_t sum = 0;
  for (uint32_t i=0; i<m_rememberedVehicles.size(); i++) {
    sum += m_rememberedVehicles.at(i).GetNrMemories();
  }
  return sum;
}

void VehicleMemorySystem::CleanMemory(double timeStamp)
{
  //std::cout << " Cleaning memory \n";
  for (uint32_t i=0; i<m_rememberedVehicles.size(); i++) {
    RememberedVehicle* vehicle = &(m_rememberedVehicles.at(i));
    vehicle->CleanMemory(timeStamp);
    //std::cout << "  #memories in vehicle: " << vehicle->GetNrMemories() << "\n";
    if (vehicle->GetNrMemories() == 0) {
      m_rememberedVehicles.erase(m_rememberedVehicles.begin() + i);
      i--;
      //std::cout << "  Removed vehicle mem... \n";
    }
  }
}

void VehicleMemorySystem::AssociatePointsToMemory(
    std::vector<int32_t>* index, 
    std::vector<double>* newPoints, 
    std::vector<double>* origPoints)
{

  // Calculate euclidic distance between new points and original points
  int32_t nOriginalPoints = origPoints->size() / 2;
  int32_t nNewPoints = newPoints->size() / 2;
  std::vector<double> distance(nOriginalPoints*nNewPoints);
  
  // Distance holding for each row the distance of newPoint i the distance to all original points
  for ( int32_t i = 0; i < nNewPoints; i ++){
    for ( int32_t j = 0; j < nOriginalPoints; j ++){
      distance.at(i*nOriginalPoints+j) = sqrt(
          pow(newPoints->at(i*2)-origPoints->at(j*2),2) +
          pow(newPoints->at(i*2+1)-origPoints->at(j*2+1),2));
    } 
  }
  
  //Decide number of iterations for assigning index vector.
  int32_t nIterations = std::min(nOriginalPoints,nNewPoints);
  for (int32_t k = 0; k < nIterations; k++)
  {
    //Find min for each newPoint and its corresponding originalPoint index
    std::vector<double> minVector(nNewPoints*2);
    for (int32_t i = 0; i < nNewPoints; i++){
      minVector.at(i*2) = std::numeric_limits<double>::max();
      //minVector.at(i*2) = 5;
      for ( int32_t j = 0; j < nOriginalPoints; j++){
        if ( distance.at(i*nOriginalPoints+j) < minVector.at(i*2) ){
          minVector.at(i*2) = distance.at(i*nOriginalPoints+j);
          minVector.at(i*2+1) = j;
        }
      }
    }
    // Take out lowest value and assign it to its corresponding original point
    double totalMin = std::numeric_limits<double>::max();
    int32_t minIndex = 0;
    int32_t currentPoint = 0;
    for (int32_t i = 0; i < nNewPoints; i++){
      if (minVector.at(i*2) < totalMin){
        totalMin = minVector.at(i*2);
        minIndex = minVector.at(i*2+1);
        currentPoint = i;
      }
    }
    
    // Update the distance for the assigned original point to something large so that it won't be selected again
    for(int32_t i = 0; i < nNewPoints; i ++) {
      distance.at(i*nOriginalPoints+minIndex) = std::numeric_limits<double>::max();
    }
    for(int32_t i = 0; i < nOriginalPoints; i++) {
      distance.at(currentPoint*nOriginalPoints+i) = std::numeric_limits<double>::max();
    }
        
    // Update the index holder with the best match
    index->at(k) = minIndex;
    
  }
  // If nNewPoints larger than nIterations, pad rest of the vector with -1
  if(nIterations < nNewPoints)
  {
    for(int32_t i = nIterations; i < nNewPoints; i++) {
      index->at(i) = -1;
    }
  }
}


} // detectvehicle
} // perception
} // opendlv

