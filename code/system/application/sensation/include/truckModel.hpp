/**
 * Copyright (C) 2015 Chalmers REVERE
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

#ifndef TRUCKMODEL_HPP_
#define TRUCKMODEL_HPP_

#include "LinearizedSystemModel.hpp"


namespace opendlv {
namespace system {
namespace application {
namespace truckKinematicModel
{



/**
 * @brief System state vector-type for a 3DOF planar robot - including position and velocity
 *
 * This is a system state for a very simple planar robot that
 * is characterized by its (x,y)-Position and angular orientation.
 *
 * @param T Numeric scalar type
 */
template<typename T>
class State : public opendlv::system::libs::kalman::Vector<T, 6>
{
public:
    KALMAN_VECTOR(State, T, 6)    //the kalman vector for our state will be 6 (x, x_dot, y, y_dot, theta, theta_dot)

    //! X-position
    static constexpr size_t X = 0;
    //! X-Velocity
    static constexpr size_t X_dot = 1;

    //! Y-Position
    static constexpr size_t Y = 2;
    //! Y-Velocity
    static constexpr size_t Y_dot = 3;

    //! Orientation
    static constexpr size_t THETA = 4;
    //! Angular Velocity
    static constexpr size_t THETA_dot = 5;


    T x()       const { return (*this)[ X ]; }
    T x_dot()       const { return (*this)[ X_dot ]; }
    T y()       const { return (*this)[ Y ]; }
    T y_dot()       const { return (*this)[ Y_dot ]; }
    T theta()   const { return (*this)[ THETA ]; }
    T theta_dot()   const { return (*this)[ THETA_dot ]; }


    T& x()      { return (*this)[ X ]; }
    T& x_dot()      { return (*this)[ X_dot ]; }
    T& y()      { return (*this)[ Y ]; }
    T& y_dot()      { return (*this)[ Y_dot ]; }
    T& theta()  { return (*this)[ THETA ]; }
    T& theta_dot()  { return (*this)[ THETA_dot ]; }
};

/**
 * @brief System control-input vector-type for a 3DOF planar robot
 *
 * This is the system control-input of a very simple planar robot that
 * can control the velocity in its current direction as well as the
 * change in direction.
 *
 * @param T Numeric scalar type
 */
template<typename T>
class Control : public opendlv::system::libs::kalman::Vector<T, 2>
{
public:
    KALMAN_VECTOR(Control, T, 2)

    //! Velocity
    static constexpr size_t V = 0;
    //! Angular Rate (Orientation-change)
    static constexpr size_t DTHETA = 1;

    T v()       const { return (*this)[ V ]; }
    T dtheta()  const { return (*this)[ DTHETA ]; }

    T& v()      { return (*this)[ V ]; }
    T& dtheta() { return (*this)[ DTHETA ]; }
};



} // truckKinematicModel
} // application
} // system
} // opendlv
#endif
