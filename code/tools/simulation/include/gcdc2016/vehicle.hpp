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

#ifndef GCDC2016_VEHICLE_HPP
#define GCDC2016_VEHICLE_HPP

#include <vector>

namespace opendlv {
namespace tools {
namespace simulation {
namespace gcdc2016 {

class Vehicle {
  public:
    Vehicle();
    Vehicle(Vehicle const &) = delete;
    Vehicle &operator=(Vehicle const &) = delete;
    virtual ~Vehicle();
    double GetHeading() const;
    double GetLateralPosition() const;
    double GetYawRate() const;
    double GetSpeed() const;
    void SetHeading(double);
    void SetRoadVheelAngle(double a_roadWheelAngle);
    void SetThrottlePedalPosition(double a_throttlePedalPosition);
    void SetDeceleraionRequest(double a_deceleraionRequest);
    void SetLateralPosition(double);
    void Update(double);

  private:
    void SetSpeed(double a_throttlePedalPosition, double a_deceleraionRequest, double a_dt);

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
    double m_roadWheelAngle;
    double m_throttlePedalPosition;
    double m_deceleraionRequest;

};

} // gcdc2016
} // simulation
} // tools
} // opendlv

#endif
