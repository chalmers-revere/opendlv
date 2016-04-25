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

#ifndef GEOLOCATION_KINEMATICMODEL_HPP_
#define GEOLOCATION_KINEMATICMODEL_HPP_

#include "kalman/LinearizedSystemModel.hpp"


namespace opendlv {
namespace sensation{
namespace geolocation {

/**
 * @brief System state vector-type for a 3DOF Ackermann steering truck - including position and velocity
 *
 * This is a system state for a simple kinematic Ackermann steering single-truck vehicle that
 * is characterized by its (x,y)-Position and angular orientation.
 *
 * State vector descriptiom :  
 *     X  =  state vector at the current time k
 *           where (x, y, theta) is the pose of the vehicle with heading (theta)
 *           [x, x_dot, y, y_dot, theta, theta_dot ], 6-by-1
 *
 * @param T Numeric scalar type
 */
template<typename T>
class State : public Kalman::Vector<T, 6>
{
public:
    KALMAN_VECTOR(State, T, 6)    //the kalman vector for our state will be 6 (x, x_dot, y, y_dot, theta, theta_dot)

    //! X-position
    static constexpr size_t X = 0;
    //! X-Velocity
    static constexpr size_t X_DOT = 1;

    //! Y-Position
    static constexpr size_t Y = 2;
    //! Y-Velocity
    static constexpr size_t Y_DOT = 3;

    //! Orientation
    static constexpr size_t THETA = 4;
    //! Angular Velocity
    static constexpr size_t THETA_DOT = 5;


    T x()          const { return (*this)[ X ]; }
    T x_dot()      const { return (*this)[ X_DOT ]; }
    T y()          const { return (*this)[ Y ]; }
    T y_dot()      const { return (*this)[ Y_DOT ]; }
    T theta()      const { return (*this)[ THETA ]; }
    T theta_dot()  const { return (*this)[ THETA_DOT ]; }


    T& x()          { return (*this)[ X ]; }
    T& x_dot()      { return (*this)[ X_DOT ]; }
    T& y()          { return (*this)[ Y ]; }
    T& y_dot()      { return (*this)[ Y_DOT ]; }
    T& theta()      { return (*this)[ THETA ]; }
    T& theta_dot()  { return (*this)[ THETA_DOT ]; }

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
class Control : public Kalman::Vector<T, 2>
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
 * @brief System model for a simple 3DOF Ackermann steering truck
 *
 * This is the system model defining how our vehicle moves from one
 * time-step to the next, i.e. how the system state evolves over time.
 *
 * @param T Numeric scalar type
 * @param CovarianceBase Class template to determine the covariance representation
 *                       (as covariance matrix (StandardBase) or as lower-triangular
 *                       coveriace square root (SquareRootBase))
 */
template<typename T, template<class> class CovarianceBase = Kalman::StandardBase>
class SystemModel : public Kalman::LinearizedSystemModel<State<T>, Control<T>, CovarianceBase>
{
public:

    SystemModel(): 
        m_vehicleParams()
    { 
    }

    //! State type shortcut definition
    typedef State<T> S;

    //! Control type shortcut definition
    typedef Control<T> C;


    //! expose the time stamp update to the user
    void updateDeltaT(double _current_time){ delta_t = _current_time; }

    //! get the delta_t for the current iteration
    double getDeltaT ( ) {return delta_t;}


    /**
     * @brief Definition of the parameters for the truck kinematic model
     *
     *
     * According to the following equations the Ackermann Steering vehicle:
     *
     *     x_dot = v * cos (theta)
     *     y_dot = v * sin (theta)
     *     theta_dot = (v / b) * tan (phi)
     *
     * Where b = vehicle wheelbase and phi=steering angle
     *
     * @param [ ] wheelbase = vehiche wheelbase, default value = 3.8 (m)
     */
    struct vehicleParams {
      double wheelbase;   ///--> distance between the front axle and the rear axle of a vechicle

      vehicleParams() : wheelbase(3.8) {}

      /**
       * @brief Espose the wheelbase to be set by the user
       *
       * @param [in] wheelbase = vehiche wheelbase, default value = 3.8 (m)
       */
      void setWheelbase (double _wheelbase) { wheelbase = _wheelbase; }

    };
    vehicleParams m_vehicleParams;

    /**
     * @brief Definition of (non-linear) state transition function
     *
     * This function defines how the system state is propagated through time,
     * i.e. it defines in which state \f$\hat{x}_{k+1}\f$ is system is expected to
     * be in time-step \f$k+1\f$ given the current state \f$x_k\f$ in step \f$k\f$ and
     * the system control input \f$u\f$.
     *
     * 
     * For an Ackermann Steering vehicle the kinematic equations are:
     *     x_dot = v * cos (theta)
     *     y_dot = v * sin (theta)
     *     theta_dot = (v / b) * tan (phi)
     * 
     * Where b = vehicle wheelbase and phi=steering angle
     * 
     * @param [in] x The system state in current time-step
     * @param [in] u The control vector input
     * @returns The (predicted) system state in the next time-step
     */
    S f(const S& x, const C& u) const
    {
        //! Predicted state vector after transition
        S x_p;


        ///  TODO: describe the vehicle model
        ///
        //x_.x() = x.x() + std::cos( x.theta ) * u.v();
        //x_.y() = x.y() + std::sin( x.theta ) * u.v();
        //x_.theta = x.theta() + delta_t * (u.v() / wheelbase) * std::tan(u.phi())


        //double wheelbase = 3.8; // TODO: this must be set by the user
        x_p.x() = x.x() + delta_t * x.x_dot();
        x_p.x_dot() = u.v() * std::cos(x.theta());
        x_p.y() = x.y() + delta_t * x.y_dot();
        x_p.y_dot() = u.v() * std::sin (x.theta());
        x_p.theta() = x.theta() + delta_t * x.theta_dot();
        //x_p.theta_dot() = (u.v() / wheelbase) * std::tan(u.phi());
        x_p.theta_dot() = (u.v() / m_vehicleParams.wheelbase ) * std::tan(u.phi());

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
     * @note This is only needed when implementing a LinearizedSystemModel,
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
        //      dx     dx_dot     dy   dydot      dtheta                 dthetadot
        //
        //J_f = [1     delta_t    0     0          0                     0                  dx
        //       0     0          0     0         -U_k(1)*sin(X_k(5))    0                  dxdot
        //       0     0          1     delta_t    0                     0                  dy
        //       0     0          0     0          U_k(1)*cos(X_k(5))    0                  dydot
        //       0     0          0     0          1                     delta_t            dtheta
        //       0     0          0     0          0                     0              ];  dthetadot

        // partial derivative of x.x() w.r.t. x.x()
        this->F( S::X, S::X ) = 1;
        // partial derivative of x.x() w.r.t. x.x_dot()
        this->F( S::X, S::X_DOT ) = delta_t;
        // partial derivative of x.x() w.r.t. x.theta()
        this->F( S::X_DOT, S::THETA ) = -std::sin( x.theta() ) * u.v();

        // partial derivative of x.y() w.r.t. x.y()
        this->F( S::Y, S::Y ) = 1;
        // partial derivative of x.y() w.r.t. x.y_dot()
        this->F( S::Y, S::Y_DOT ) = delta_t;
        // partial derivative of x.y() w.r.t. x.theta()
        this->F( S::Y_DOT, S::THETA ) = std::cos( x.theta() ) * u.v();

        // partial derivative of x.theta() w.r.t. x.theta()
        this->F( S::THETA, S::THETA ) = 1;
        // partial derivative of x.theta() w.r.t. x.theta()
        this->F( S::THETA, S::THETA_DOT ) = delta_t;

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

        this->W( S::X, S::X ) = std::pow(delta_t,3) / 3;
        this->W( S::X, S::X_DOT ) = std::pow(delta_t,2) / 2;
        this->W( S::X_DOT, S::X ) = std::pow(delta_t,2) / 2;
        this->W( S::X_DOT, S::X_DOT ) = delta_t;

        this->W( S::Y, S::Y ) = std::pow(delta_t,3) / 3;
        this->W( S::Y, S::Y_DOT ) = std::pow(delta_t,2) / 2;
        this->W( S::Y_DOT, S::Y ) = std::pow(delta_t,2) / 2;
        this->W( S::Y_DOT, S::Y_DOT ) = delta_t;

        this->W( S::THETA, S::THETA ) = std::pow(delta_t,3) / 3;
        this->W( S::THETA, S::THETA_DOT ) = std::pow(delta_t,2) / 2;
        this->W( S::THETA_DOT, S::THETA ) = std::pow(delta_t,2) / 2;
        this->W( S::THETA_DOT, S::THETA_DOT ) = delta_t;

        //double sigma_q = 0.1;   /// variance of the model noise
        //this->W = this->W * sigma_q;
    }

    //! Variables to handle a variable timestamp in the data
    double delta_t = 0.0;
};

} // geolocation
} // sensation
} // opendlv

#endif
