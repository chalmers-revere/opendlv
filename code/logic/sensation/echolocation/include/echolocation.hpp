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

#ifndef LOGIC_SENSATION_ECHOLOCATION_ECHOLOCATION_HPP
#define LOGIC_SENSATION_ECHOLOCATION_ECHOLOCATION_HPP

#include <deque>
#include <vector>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

namespace opendlv {
namespace logic {
namespace sensation {

class Echolocation
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  Echolocation(int32_t const &, char **);
  Echolocation(Echolocation const &) = delete;
  Echolocation &operator=(Echolocation const &) = delete;
  virtual ~Echolocation();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  double PointDistance(float, double, float, double);

  std::deque<float> m_listAzimuth;
  std::deque<float> m_listZenith;
  std::deque<double> m_listDistance;
  std::deque<odcore::data::TimeStamp> m_listTime;
  float m_memoryThreshold;
  double m_pointCloudRadius;
  uint32_t m_pointCloudSizeMinimum;
  bool m_initialised;
};

}
}
}

#endif
