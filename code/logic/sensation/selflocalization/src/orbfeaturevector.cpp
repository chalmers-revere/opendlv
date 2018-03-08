/**
 * File: orbfeaturevector.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: feature vector
 * License: see the LICENSE.txt file
 * Adapted for OpenDLV in 2018
 */




#include "orbfeaturevector.hpp"
#include <map>
#include <vector>
#include <iostream>

namespace opendlv {
namespace logic {
namespace sensation {

OrbFeatureVector::OrbFeatureVector()
{
}

OrbFeatureVector::~OrbFeatureVector()
{
}

void OrbFeatureVector::addFeature(uint32_t id, uint32_t iFeature){

  OrbFeatureVector::iterator featureIterator = this->lower_bound(id);
  
  if(featureIterator != this->end() && featureIterator->first == id)
  {
    featureIterator->second.push_back(iFeature);
  }
  else
  {
    featureIterator = this->insert(featureIterator, OrbFeatureVector::value_type(id, 
      std::vector<uint32_t>()));
    featureIterator->second.push_back(iFeature);
  }
}

std::ostream& operator<<(std::ostream &out, const OrbFeatureVector &featureVector){

  if(!featureVector.empty())
  {
    OrbFeatureVector::const_iterator featureIterator = featureVector.begin();
    
    const std::vector<uint32_t>* fPointer = &featureIterator->second;

    out << "<" << featureIterator->first << ": [";
    if(!fPointer->empty()) out << (*fPointer)[0];
    for(uint32_t i = 1; i < fPointer->size(); ++i)
    {
      out << ", " << (*fPointer)[i];
    }
    out << "]>";
    
    for(++featureIterator; featureIterator != featureVector.end(); ++featureIterator)
    {
      fPointer = &featureIterator->second;
      
      out << ", <" << featureIterator->first << ": [";
      if(!fPointer->empty()) out << (*fPointer)[0];
      for(uint32_t i = 1; i < fPointer->size(); ++i)
      {
        out << ", " << (*fPointer)[i];
      }
      out << "]>";
    }
  }
  
  return out;  
}

} //namespace opendlv
} //namespace logic
} //namespace sensation
