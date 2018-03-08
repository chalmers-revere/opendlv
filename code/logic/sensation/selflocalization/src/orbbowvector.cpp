/**
 * Copyright (C) 2017 Chalmers Revere
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
#include "orbbowvector.hpp"


namespace opendlv {
namespace logic {
namespace sensation {
OrbBowVector::OrbBowVector()
{
}
OrbBowVector::~OrbBowVector()
{
}

void OrbBowVector::addWeight(uint32_t id, double v)
{
  OrbBowVector::iterator vit = this->lower_bound(id);
  
  if(vit != this->end() && !(this->key_comp()(id, vit->first)))
  {
    vit->second += v;
  }
  else
  {
    this->insert(vit, OrbBowVector::value_type(id, v));
  }
}

// --------------------------------------------------------------------------

void OrbBowVector::addIfNotExist(uint32_t id, double v)
{
  OrbBowVector::iterator vit = this->lower_bound(id);
  
  if(vit == this->end() || (this->key_comp()(id, vit->first)))
  {
    this->insert(vit, OrbBowVector::value_type(id, v));
  }
}

// --------------------------------------------------------------------------

void OrbBowVector::normalize()
{
  double norm = 0.0; 
  OrbBowVector::iterator it;

  
    for(it = begin(); it != end(); ++it)
      norm += fabs(it->second);

  if(norm > 0.0)
  {
    for(it = begin(); it != end(); ++it)
      it->second /= norm;
  }
}

// --------------------------------------------------------------------------

std::ostream& operator<< (std::ostream &out, const OrbBowVector &v)
{
  OrbBowVector::const_iterator vit;
  std::vector<uint32_t>::const_iterator iit;
  uint32_t i = 0; 
  const uint32_t N = v.size();
  for(vit = v.begin(); vit != v.end(); ++vit, ++i)
  {
    out << "<" << vit->first << ", " << vit->second << ">";
    
    if(i < N-1) out << ", ";
  }
  return out;
}

} // namespace sensation
} // namespace logic
} // namespace opendlv