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

#include <opendavinci/odcore/base/Lock.h>
#include "automotivedata/GeneratedHeaders_AutomotiveData.h"
#include "vehicle.hpp"

namespace opendlv {
namespace logic {
namespace legacy {

Vehicle::Vehicle(opendlv::data::environment::WGS84Coordinate a_wgs84Reference):
  m_egoState(),
  m_oldPosition(),
  m_oldPositionForDirection(),
  m_wgs84Reference(a_wgs84Reference),
  m_egoStateMutex(),
  m_speedMutex(),
  m_speed(),
  m_isUpdatedHeading(false),
  m_isUpdatedSpeed(false),
  m_receveivedFirstWgs84Position(false)
{
}

Vehicle::~Vehicle()
{
}

opendlv::data::environment::EgoState Vehicle::getEgoState()
{
  odcore::base::Lock l(m_egoStateMutex);
  return m_egoState;
}

double Vehicle::getSpeed()
{
  odcore::base::Lock l(m_speedMutex);
  return m_speed;
}

bool Vehicle::isUpdated() const
{
  return m_isUpdatedHeading && m_isUpdatedSpeed;
}

double Vehicle::updatePosition(
    opendlv::data::environment::WGS84Coordinate const &a_wgs84CurrentPosition)
{
  odcore::base::Lock l(m_egoStateMutex);

  double const positionUpdateDeltaThreshold = 2.0;

  opendlv::data::environment::Point3 currentPosition = 
    m_wgs84Reference.transform(a_wgs84CurrentPosition);

  if (m_receveivedFirstWgs84Position) {

    double const d = (currentPosition - m_oldPositionForDirection).lengthXY();
    if (d > positionUpdateDeltaThreshold) {
      opendlv::data::environment::Point3 direction = 
        (currentPosition - m_oldPositionForDirection);
      m_egoState.setRotation(direction);
      m_oldPositionForDirection = currentPosition;
      m_isUpdatedHeading = true;
    }

    m_egoState.setPosition(currentPosition);
  }

  m_oldPosition = currentPosition;
  m_receveivedFirstWgs84Position = true;

  return currentPosition.lengthXY();
}

void Vehicle::updateReference(opendlv::data::environment::WGS84Coordinate a_wgs84Reference)
{
  odcore::base::Lock l(m_egoStateMutex);
  m_wgs84Reference = a_wgs84Reference;
}

void Vehicle::updateSpeed(double a_speed)
{
  odcore::base::Lock l(m_speedMutex);
  m_speed = a_speed;
  m_isUpdatedSpeed = true;
}

}
}
}
