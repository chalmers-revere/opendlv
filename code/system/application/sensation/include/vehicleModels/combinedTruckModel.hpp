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

#ifndef COMBINEDTRUCKMODEL_HPP_
#define COMBINEDTRUCKMODEL_HPP_

#include <math.h>
#include "LinearizedSystemModel.hpp"


namespace opendlv {
namespace system {
namespace application {
namespace sensation{
namespace combinedTruckModel
{

/**
 * @brief System state vector-type for a Planar Bicycle Model of the lateral forces on a truck
 *
 * This is a system state for a Planar Bicycle Model of a single-truck vehicle that
 * is characterized by its yaw rate and lateral and longitudinal velocities.
 *
 * State vector descriptiom :
 *     X  =  state vector at the current time k
 *           where (x, y, uy, theta, r) is the position, lateral velocity, heading and yaw rate of the vehicle
 *           [x, y, uy, uy_dot, theta, r, r_dot ], 7-by-1
 *
 * @param T Numeric scalar type
 */
template<typename T>
class State : public opendlv::system::libs::kalman::Vector<T, 7>
{
public:
    KALMAN_VECTOR(State, T, 7)    //the kalman vector for our state will be 7 (x, y, uy, uy_dot, theta, r, r_dot)

    //! Position X
    static constexpr size_t X = 0;

    //! Position Y
    static constexpr size_t Y = 1;

    //! Lateral velocity
    static constexpr size_t UY = 2;
    //! Lateral acceleration
    static constexpr size_t UY_DOT = 3;

    //!Heading
    static constexpr size_t THETA = 4;
    //! Yaw rate
    static constexpr size_t R = 5;
    //! Yaw rate change
    static constexpr size_t R_DOT = 6;


    T x()          const { return (*this)[ X ]; }
    T y()          const { return (*this)[ Y ]; }
    T uy()          const { return (*this)[ UY ]; }
    T uy_dot()      const { return (*this)[ UY_DOT ]; }
    T theta()          const { return (*this)[ THETA ]; }
    T r()          const { return (*this)[ R ]; }
    T r_dot()      const { return (*this)[ R_DOT ]; }


    T& x()          { return (*this)[ X ]; }
    T& y()          { return (*this)[ Y ]; }
    T& uy()          { return (*this)[ UY ]; }
    T& uy_dot()      { return (*this)[ UY_DOT ]; }
    T& theta()          { return (*this)[ THETA ]; }
    T& r()          { return (*this)[ R ]; }
    T& r_dot()      { return (*this)[ R_DOT ]; }

}; // end - class State




/**
 * @brief System control-input vector-type for a 3DOF Ackermann steering truck
 *
 * This is the system control-input of a very simple kinematic Ackermann steering single-truck
 * vehicle that can control the velocity in its current direction as well as the steering angle
 * change in direction.
 *
 * U_k:  (v_k, phi_k) Input commands
 * Commands:
 *           U_k(1) = longitudinal velocity in (m/s)
 *           U_k(2) = steering angle of the wheels in (rad)
 *
 * @param T Numeric scalar type
 */
template<typename T>
class Control : public opendlv::system::libs::kalman::Vector<T, 2>
{
public:
    KALMAN_VECTOR(Control, T, 2)

    //! Longitudinal Velocity
    static constexpr size_t V = 0;
    //! Steering angle
    static constexpr size_t PHI = 1;

    T v()       const { return (*this)[ V ]; }
    T phi()     const { return (*this)[ PHI ]; }

    T& v()      { return (*this)[ V ]; }
    T& phi()    { return (*this)[ PHI ]; }

}; // end - class Control

/**
 * @brief System model for Planar Bicycle Model of the lateral forces on a 3DOF Ackermann steering truck
 *
 * This is the system model defining how our vehicle moves laterally from one
 * time-step to the next, i.e. how the system state evolves over time.
 *
 * @param T Numeric scalar type
 * @param CovarianceBase Class template to determine the covariance representation
 *                       (as covariance matrix (StandardBase) or as lower-triangular
 *                       coveriace square root (SquareRootBase))
 */
template<typename T, template<class> class CovarianceBase = opendlv::system::libs::kalman::StandardBase>
class SystemModel : public opendlv::system::libs::kalman::LinearizedSystemModel<State<T>, Control<T>, CovarianceBase>
{
public:
    //! State type shortcut definition
    typedef State<T> S;

    //! Control type shortcut definition
    typedef Control<T> C;

    /**
     * @brief Definition of (non-linear) state transition function
     *
     * This function defines how the system state is propagated through time,
     * i.e. it defines in which state \f$\hat{x}_{k+1}\f$ is system is expected to
     * be in time-step \f$k+1\f$ given the current state \f$x_k\f$ in step \f$k\f$ and
     * the system control input \f$u\f$.
     *
     *
     * For the Planar Bicycle Model of lateral forces the kinematic equations are:
     *     uy_dot = c1 * uy/ux - (ux + c2/ux) * r + c3 * phi
     *     r_dot = c4 * uy/ux + c5 * r/ux + c6 * phi
     *
     * Where ux = longitudinal velocity and phi = steering angle
     * And where c1-c6 = constants depending on tire cornering stiffness, moment of inertia, mass and
     * wheelbase distances
     *
     * c1 = (-s1 -s2) /m                s1 = cornering stiffness of front wheel          s += 0.15
     * c2 = (s2*b -s1*a)/m              s2 = cornering stiffness of rear wheel
     * c3 = s1/m                        m = mass of truck                                m += 2000
     * c4 = (s2*b -s1*a)/I              a = distance from front wheel to center of mass  a + b = 3.8
     * c5 = (-s1*a² - s2*b²)/I          b = distance from rear wheel to center of mass
     * c6 = s1*a/I                      I = moment of inertia of truck                   I += 4000
     *
     *
     * @param [in] x The system state in current time-step
     * @param [in] u The control vector input
     * @returns The (predicted) system state in the next time-step
     */
    S f(const S& x, const C& u) const
    {
        //! Predicted state vector after transition
        S x_p;

        // New yaw rate given by old yaw rate plus yaw rate change

        // New lateral velocity given by old lateral velocity plus change in lateral velocity

        // TODO: Find constants and set them somewhere else
        double delta_t = 0.05;

        double s1 = 0.15;
        double s2 = 0.15;
        double m = 2000;
        double a = 1;
        double b = 2.8;
        double I = 4000;

        double c1 = (-s1 -s2) /m ;
        double c2 = (s2*b -s1*a)/m ;
        double c3 = s1/m ;
        double c4 = (s2*b -s1*a)/I ;
        double c5 = (-s1*pow(a,2) - s2*pow(b,2))/I;
        double c6 = s1*a/I ;
        //double ux = sqrt(pow(u.v(),2)-pow(x.uy(),2));

        double v = u.v();
        if(u.v() < 0.0001) { v = 0.0001; }

        x_p.x() = x.x() + std::cos(x.theta())*u.v()*delta_t - std::sin(x.theta())*x.uy()*delta_t;
        x_p.y() = x.y() + std::sin(x.theta())*u.v()*delta_t + std::cos(x.theta())*x.uy()*delta_t;
        x_p.uy() = x.uy() + delta_t * x.uy_dot();
        x_p.uy_dot() = c1 * x.uy()/v - (u.v() + c2/v) * x.r() + c3 * u.phi();
        x_p.theta() = x.theta() + x.r()*delta_t;
        x_p.r() = x.r() + delta_t * x.r_dot();
        x_p.uy_dot() = c4 * x.uy()/v + c5 * x.r()/v + c6 * u.phi();

        // Return transitioned state vector
        return x_p;
    }


protected:


    /**
     * @brief Update jacobian matrices for the system state transition function using current state
     *
     * This will re-compute the (state-dependent) elements of the jacobian matrices
     * to linearize the non-linear state transition function \f$f(x,u)\f$ around the
     * current state \f$x\f$.
     *
     * @note This is only needed when implementing a LinearizedSystemModel
     *       for usage with an ExtendedKalmanFilter or SquareRootExtendedKalmanFilter.
     *       When using a fully non-linear filter such as the UnscentedKalmanFilter
     *       or its square-root form then this is not needed.
     *
     * @param x The current system state around which to linearize
     * @param u The current system control input
     */
    void updateJacobians( const S& x, const C& u )
    {
        // F = df/dx (Jacobian of state transition w.r.t. the state)
        this->F.setZero();

//           d f    |
// J_f = -----------|                 : Linearize state equation, J_f is the
//           d X    |X=Xp               Jacobian of the process model
//
//       dx   dy   duy               duy_dot               dtheta                    dr       dr_dot
//
//J_f = [1    0    -sin(theta)*delta_t  0    (-sin(theta)*v - cos(theta)*uy)*delta_t  0         0      dx
//       0    1    cos(theta)*delta_t   0    (cos(theta)*v - sin(theta)*uy)*delta_t   0         0      dy
//       0    0    1                 delta_t                 0                        0         0      duy
//       0    0    c1/ux                0                    0                    ux + c2/ux    0      duy_dot
//       0    0    0                    0                    1                      delta_t     0      dtheta
//       0    0    0                    0                    0                        1       delta_t  dr
//       0    0    c4/ux                0                    0                      c5/ux       0    ] dr_dot

        //TODO fix constants
        double delta_t = 0.05;
        double s1 = 0.15;
        double s2 = 0.15;
        double m = 2000;
        double a = 1;
        double b = 2.8;
        double I = 4000;

        double c1 = (-s1 -s2) /m ;
        double c2 = (s2*b -s1*a)/m ;

        double c4 = (s2*b -s1*a)/I ;
        double c5 = (-s1*pow(a,2) - s2*pow(b,2))/I;


        double v = u.v();
        if(u.v() < 0.0001) { v = 0.0001; }

        // partial derivative of x.x() w.r.t. x.x()
        this->F( S::X, S::X ) = 1;
        // partial derivative of x.x() w.r.t. x.uy()
        this->F( S::X, S::UY ) = -std::sin(x.theta())*delta_t;
        // partial derivative of x.x() w.r.t. x.theta()
        this->F( S::X, S::THETA ) = (-std::sin(x.theta())*u.v() - std::cos(x.theta())*x.uy())*delta_t;

        // partial derivative of x.y() w.r.t. x.y()
        this->F( S::Y, S::Y ) = 1;
        // partial derivative of x.y() w.r.t. x.uy()
        this->F( S::Y, S::UY ) = std::cos(x.theta())*delta_t;
        // partial derivative of x.y() w.r.t. x.theta()
        this->F( S::Y, S::THETA ) = (std::cos(x.theta())*u.v() - std::sin(x.theta())*x.uy())*delta_t;

        // partial derivative of x.uy() w.r.t. x.uy()
        this->F( S::UY, S::UY ) = 1;
        // partial derivative of x.uy() w.r.t. x.uy_dot()
        this->F( S::UY, S::UY_DOT ) = delta_t;

        // partial derivative of x.uy_dot() w.r.t. x.uy()
        this->F( S::UY_DOT, S::UY ) = c1/v;
        // partial derivative of x.uy_dot() w.r.t. x.uy()
        this->F( S::UY_DOT, S::R ) = u.v() + c2/v;

        // partial derivative of x.theta() w.r.t. x.theta()
        this->F( S::THETA, S::THETA ) = 1;
        // partial derivative of x.theta() w.r.t. x.r()
        this->F( S::THETA, S::R ) = delta_t;

        // partial derivative of x.r() w.r.t. x.r()
        this->F( S::R, S::R ) = 1;
        // partial derivative of x.r() w.r.t. x.r_dot()
        this->F( S::R, S::R_DOT ) = delta_t;

        // partial derivative of x.r_dot() w.r.t. x.uy()
        this->F( S::R_DOT, S::UY ) = c4/v;
        // partial derivative of x.r_dot() w.r.t. x.r()
        this->F( S::R_DOT, S::R ) = c5/v;

        // W = df/dw (Jacobian of state transition w.r.t. the noise)
        this->W.setIdentity();
        // TODO: more sophisticated noise modelling
        //       i.e. The noise affects the the direction in which we move as
        //       well as the velocity (i.e. the distance we move)
        // Set Q,
        // Q = eye(8)*0.1;                    % process model noise variance
        // Q = [ (1/3)*delta_t^3 (1/2)*delta_t^2
        //       (1/2)*delta_t^2 delta_t];
        //delta_t = 0.005;        % simulation discrete time
        //%sigma=0.01;%5;         % state transition variance
        //sigma_q = [0.1 0.01];
        //Qxyz = sigma_q(1)^2 * [delta_t^3/3 delta_t^2/2;
        //                       delta_t^2/2 delta_t];

        //Qtheta = sigma_q(2)^2 * [delta_t^3/3 delta_t^2/2;
        //                         delta_t^2/2 delta_t];
        //Q_init = blkdiag(Qxyz, Qxyz, Qtheta, Qtheta);

    }
};
} // combinedTruckModel
} // Sensation
} // application
} // system
} // opendlv
#endif
