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

#ifndef LOGIC_KNOWLEDGE_SCENE_SCENE_HPP
#define LOGIC_KNOWLEDGE_SCENE_SCENE_HPP

#include <memory>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace logic {
namespace knowledge {

class Scene : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Scene(int32_t const &, char **);
  Scene(Scene const &) = delete;
  Scene &operator=(Scene const &) = delete;
  virtual ~Scene();
  virtual void nextContainer(odcore::data::Container &);

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();
  void MergeSurfaces(opendlv::perception::Surface, uint32_t);
  std::vector<opendlv::model::Cartesian3> MergePoints(std::vector<opendlv::model::Cartesian3>, std::vector<opendlv::model::Cartesian3>, float);
  opendlv::model::Cartesian3 CrossingPoint(std::vector<opendlv::model::Cartesian3>);
  bool IsInRectangle(opendlv::model::Cartesian3, std::vector<opendlv::model::Cartesian3>);
  void SendStuff();
  double PointDistance(float a_angle1, double a_dist1, float a_angle2, double a_dist2);
  void MergeObjects(opendlv::perception::Object a_object, uint32_t a_index);
  void TimeCheck();

  bool m_initialised;
  std::vector<opendlv::perception::Surface> m_savedSurfaces;
  std::vector<opendlv::perception::Object> m_savedObjects;
  uint32_t m_objectCounter;
  uint32_t m_surfaceCounter;
  odcore::base::Mutex m_mutex;
  float m_mergeDistance;
  int32_t m_validUntilDuration;
  float m_memoryCapacity;
  bool m_debug;
};

}
}
}

#endif
