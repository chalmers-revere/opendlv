/**
 * Copyright (C) 2016 Chalmers REVERE
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

#ifndef LANEKEEPING_LANEKEEPING_HPP_
#define LANEKEEPING_LANEKEEPING_HPP_

#include <memory>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

namespace opendlv {
namespace tools {
namespace simulation {
namespace lanekeeping {

class Vehicle;

class LaneKeeping : 
    public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  LaneKeeping(int32_t const &, char **);
  LaneKeeping(LaneKeeping const &) = delete;
  LaneKeeping &operator=(LaneKeeping const &) = delete;
  virtual ~LaneKeeping();

  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  std::unique_ptr<Vehicle> m_vehicle;
  double m_steering;
};

} // lanekeeping
} // simulation
} // tools
} // opendlv

#endif
