/**
 * Copyright (C) 2017 Ola Benderius
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LOGIC_LEGACY_VEHICLEFOLLOWER_VEHICLEFOLLOWER_HPP
#define LOGIC_LEGACY_VEHICLEFOLLOWER_VEHICLEFOLLOWER_HPP

#include <memory>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/base/Mutex.h>
#include <opendlv/data/environment/WGS84Coordinate.h>

namespace opendlv {
namespace logic {
namespace legacy {

class Vehicle;

class VehicleFollower : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  VehicleFollower(int32_t const &, char **);
  VehicleFollower(VehicleFollower const &) = delete;
  VehicleFollower &operator=(VehicleFollower const &) = delete;
  virtual ~VehicleFollower();

  virtual void nextContainer(odcore::data::Container &c);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  virtual void setUp();
  virtual void tearDown();

  odcore::base::Mutex m_referenceMutex;
  std::unique_ptr<Vehicle> m_egoVehicle;
  std::unique_ptr<Vehicle> m_targetVehicle;
  double m_steeringWheelAngle;
};

}
}
}

#endif
