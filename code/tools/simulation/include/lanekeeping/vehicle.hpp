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

#ifndef LANEKEEPING_VEHICLE_HPP
#define LANEKEEPING_VEHICLE_HPP

#include <vector>

namespace opendlv {
namespace tools {
namespace simulation {
namespace lanekeeping {

class Vehicle {
  public:
    Vehicle();
    Vehicle(Vehicle const &) = delete;
    Vehicle &operator=(Vehicle const &) = delete;
    virtual ~Vehicle();
    double GetHeading() const;
    double GetLateralPosition() const;
    double GetYawRate() const;
    void SetHeading(double);
    void SetLateralPosition(double);
    void SetSpeed(double);
    void Update(double, double);

  private:
    double const m_cmToFront;
    double const m_cmToRear;
    double const m_corneringStiffnessFront;
    double const m_corneringStiffnessRear;
    double const m_inertia;
    double const m_mass;
    double m_heading;
    double m_lateralPosition;
    double m_lateralVelocity;
    double m_longitudinalPosition;
    double m_speed;
    double m_yawRate;
};

} // lanekeeping
} // simulation
} // tools
} // opendlv

#endif
