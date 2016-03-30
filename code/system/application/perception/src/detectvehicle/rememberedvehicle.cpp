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

#include "detectvehicle/rememberedvehicle.hpp"

// TODO add documentation


namespace opendlv {
namespace perception {
namespace detectvehicle {

RememberedVehicle::RememberedVehicle()
    : m_memoryOverTime()
    , m_dummyColor()
{
  m_memoryOverTime = std::shared_ptr<std::vector<std::shared_ptr<DetectedVehicle>>>(new std::vector<std::shared_ptr<DetectedVehicle>>);

  int32_t b = rand() % 256;
  int32_t g = rand() % 256;
  int32_t r = rand() % 256;
  m_dummyColor = cv::Scalar(b,g,r);
}

RememberedVehicle::~RememberedVehicle()
{
  //std::cout << "RememberedVehicle::~RememberedVehicle()" << std::endl;
}

void RememberedVehicle::AddMemory(std::shared_ptr<DetectedVehicle> a_detectedVehiclePtr)
{
  m_memoryOverTime->push_back(a_detectedVehiclePtr);
}

void RememberedVehicle::GetMemoryOverTime(std::vector<std::shared_ptr<DetectedVehicle>>* containerVector)
{
  containerVector->clear();
  for (uint32_t i=0; i<m_memoryOverTime->size(); i++) {
    containerVector->push_back(m_memoryOverTime->at(i));
  }
}

int32_t RememberedVehicle::GetNrMemories()
{
  return m_memoryOverTime->size();
}

std::shared_ptr<DetectedVehicle> RememberedVehicle::GetLatestDetection()
{
  return m_memoryOverTime->back();
}

void RememberedVehicle::CleanMemory(double timeStamp)
{
  for (uint32_t i=0; i<m_memoryOverTime->size(); i++) {
    // TODO memory duration is hardcoded here
    if (m_memoryOverTime->at(i)->GetTimeStamp() < timeStamp - 3) {
      //std::cout << "    timeStamp: " << timeStamp << "\n";
      //std::cout << "    age      : " << m_memoryOverTime.at(i)->GetTimeStamp() << "\n";
      // this memory is old, remove it
      //std::cout << "    m_memoryOverTime.size(): " << m_memoryOverTime.size() << "\n";
      //std::cout << "    i: " << i << "\n";
      m_memoryOverTime->erase(m_memoryOverTime->begin() + i);
      i--; // TODO will this cause a possible "underflow" since unsigned???
      //std::cout << "    m_memoryOverTime.size(): " << m_memoryOverTime.size() << "\n";
      //std::cout << "    i: " << i << "\n";
    }
  }
}

cv::Scalar RememberedVehicle::GetDummyColor()
{
  return m_dummyColor;
}


} // detectvehicle
} // perception
} // opendlv

