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

#ifndef LOGIC_LEGACY_VEHICLEFOLLOWER_VEHICLE_HPP
#define LOGIC_LEGACY_VEHICLEFOLLOWER_VEHICLE_HPP

#include "opendlv/data/environment/EgoState.h"
#include "opendlv/data/environment/Point3.h"
#include "opendlv/data/environment/WGS84Coordinate.h"
#include "opendavinci/odcore/base/Mutex.h"

namespace opendlv {
namespace logic {
namespace legacy {

class Vehicle {
 public:
  Vehicle(opendlv::data::environment::WGS84Coordinate);
  Vehicle(Vehicle const &) = delete;
  Vehicle &operator=(Vehicle const &) = delete;
  virtual ~Vehicle();

  opendlv::data::environment::EgoState getEgoState();
  double getSpeed();
  bool isUpdated() const;
  double updatePosition(opendlv::data::environment::WGS84Coordinate const &);
  void updateSpeed(double);
  void updateReference(opendlv::data::environment::WGS84Coordinate);

 private:
  opendlv::data::environment::EgoState m_egoState;
  opendlv::data::environment::Point3 m_oldPosition;
  opendlv::data::environment::Point3 m_oldPositionForDirection;
  opendlv::data::environment::WGS84Coordinate m_wgs84Reference;
  odcore::base::Mutex m_egoStateMutex;
  odcore::base::Mutex m_speedMutex;
  double m_speed;
  bool m_isUpdatedHeading;
  bool m_isUpdatedSpeed;
  bool m_receveivedFirstWgs84Position;
};

}
}
}

#endif
