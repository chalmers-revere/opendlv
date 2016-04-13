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
 *TODO: change the notation according to the new code !
 * @param T Numeric scalar type
 */
template<typename T>
class State : public opendlv::system::libs::kalman::Vector<T, 6>
{
public:
    KALMAN_VECTOR(State, T, 6)    //the kalman vector for our state will be 7 (x, y, uy, uy_dot, theta, r, r_dot)

    //! Position X
    static constexpr size_t X = 0;
    //! X-Velocity
    static constexpr size_t X_DOT = 1;

    //! Position Y
    static constexpr size_t Y = 2;
    //! Y-Lateral velocity
    static constexpr size_t Y_DOT = 3;
    //! Y-Lateral acceleration
    //static constexpr size_t Y_DOT_DOT = 4;

    //!Heading
    static constexpr size_t THETA = 5;
    //! Yaw rate
    static constexpr size_t THETA_DOT = 6;
    //! Yaw rate change
    //static constexpr size_t YAW_DOT_DOT = 7;

//    static constexpr size_t X = 0;
//    static constexpr size_t Y = 1;
//    static constexpr size_t UY = 2;
//    static constexpr size_t UY_DOT = 3;
//    static constexpr size_t THETA = 4;
//    static constexpr size_t R = 5;
//    static constexpr size_t R_DOT = 6;

    T x()          const { return (*this)[ X ]; }
    T x_dot()          const { return (*this)[ X_DOT ]; }
    T y()          const { return (*this)[ Y ]; }
    T y_dot()      const { return (*this)[ Y_DOT ]; }
    //T y_dot_dot()      const { return (*this)[ Y_DOT_DOT ]; }
    T theta()          const { return (*this)[ THETA ]; }
    T theta_dot()          const { return (*this)[ THETA_DOT ]; }
    //T yaw_dot_dot()      const { return (*this)[ YAW_DOT_DOT ]; }

    //T x()          const { return (*this)[ X ]; }
    //T y()          const { return (*this)[ Y ]; }
    //T uy()          const { return (*this)[ UY ]; }
    //T uy_dot()      const { return (*this)[ UY_DOT ]; }
    //T theta()          const { return (*this)[ THETA ]; }
    //T r()          const { return (*this)[ R ]; }
    //T r_dot()      const { return (*this)[ R_DOT ]; }

    T& x()          { return (*this)[ X ]; }
    T& x_dot()          { return (*this)[ X_DOT ]; }
    T& y()          { return (*this)[ Y ]; }
    T& y_dot()          { return (*this)[ Y_DOT ]; }
    //T& y_dot_dot()      { return (*this)[ Y_DOT_DOT ]; }
    T& theta()          { return (*this)[ THETA ]; }
    T& theta_dot()          { return (*this)[ THETA_DOT ]; }
    //T& yaw_dot_dot()      { return (*this)[ YAW_DOT_DOT ]; }

    //T& x()          { return (*this)[ X ]; }
    //T& y()          { return (*this)[ Y ]; }
    //T& uy()          { return (*this)[ UY ]; }
    //T& uy_dot()      { return (*this)[ UY_DOT ]; }
    //T& theta()          { return (*this)[ THETA ]; }
    //T& r()          { return (*this)[ R ]; }
    //T& r_dot()      { return (*this)[ R_DOT ]; }

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
class Control : public opendlv::system::libs::kalman::Vector<T, 4>
{
public:
    KALMAN_VECTOR(Control, T, 4)

    //! Longitudinal Velocity
    static constexpr size_t V = 0;
    //! Steering angle
    static constexpr size_t PHI = 1;
    //! Velocity along the y axis in the vehicle reference frame
    static constexpr size_t V_Y = 1;
    //! angular velocity of the vehicle in its own reference frame
    static constexpr size_t YAW_RATE = 1;

    T v()        const { return (*this)[ V ]; }
    T phi()      const { return (*this)[ PHI ]; }
    T v_y()      const { return (*this)[V_Y]; }
    T yaw_rate() const { return (*this)[YAW_RATE]; }

    T& v()        { return (*this)[ V ]; }
    T& phi()      { return (*this)[ PHI ]; }
    T& v_y()      { return (*this)[ V_Y ]; }
    T& yaw_rate() { return (*this)[ YAW_RATE ]; }

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
     * @brief Definition of the parameters for the truck dynamic model
     *
     *     For the Planar Bicycle Model of lateral forces the kinematic equations are:
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
     * @param [ ] wheelbase = vehiche wheelbase, default value = 3.8 (m)
     */
    struct vehicleParams {
      double wheelbase;   ///--> distance between the front axle and the rear axle of a vechicle
      double s1;
      double s2;
      double m;
      double a;
      double b;
      double I;
      double c1;
      double c2;
      double c3;
      double c4;
      double c5;
      double c6;
      //double s1 = 0.15;  ///--> cornering stiffness of front wheel (default value 0.15) UNIT? N/deg N/rad?
      //double s2 = 0.15;  ///--> cornering stiffness of rear wheel (default value 0.15) UNIT? N/deg N/rad?
      //double m = 2000;   ///--> mass of the truck (kg)
      //double a = 1;      ///--> distance between the front wheel and the center of mass 
      //double b = 2.8;    ///--> distance between the rear wheel and the center of mass 
      //double I = 4000;   ///--> moment of inertia of the truck (kg m^2)

        //double c1 = (-s1 -s2) /m ;
        //double c2 = (s2*b -s1*a)/m ;
        //double c3 = s1/m ;
        //double c4 = (s2*b -s1*a)/I ;
        //double c5 = (-s1*pow(a,2) - s2*pow(b,2))/I;
        //double c6 = s1*a/I ;
      vehicleParams() : wheelbase(3.8), s1(0.15), s2(0.15), m(2000), a(1.0), b(2.8), I(4000)  {
           c1 = (-s1 -s2) /m ;
           c2 = (s2*b -s1*a)/m ;
           c3 = s1/m ;
           c4 = (s2*b -s1*a)/I ;
           c5 = (-s1*pow(a,2) - s2*pow(b,2))/I;
           c6 = s1*a/I ;
      }

      /**
       * @brief Espose the wheelbase to be set by the user
       *
       * @param [in] wheelbase = vehiche wheelbase, default value = 3.8 (m)
       */
      void setWheelbase (double _wheelbase) { wheelbase = _wheelbase; }
      //TODO all the parameters must be exposed to be set (all together or separately)

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

        //double s1 = m_vehicleParams.s1;
        //double s2 = m_vehicleParams.s2;
        //double m = m_vehicleParams.m;
        //double a = m_vehicleParams.a;
        //double b = m_vehicleParams.b;
        //double I = m_vehicleParams.I;

        //double c_1 = m_vehicleParams.c1;// (-s1 -s2) /m ;
        //double c_2 = m_vehicleParams.c2;//(s2*b -s1*a)/m ;
        //double c_3 = m_vehicleParams.c3;//s1/m ;
        //double c_4 = m_vehicleParams.c4;//(s2*b -s1*a)/I ;
        //double c_5 = m_vehicleParams.c5;//(-s1*pow(a,2) - s2*pow(b,2))/I;
        //double c_6 = m_vehicleParams.c6;//s1*a/I ;
        //double ux = sqrt(pow(u.v(),2)-pow(x.uy(),2));

        //double v = u.v();
        //if(u.v() < 0.0001) { v = 0.0001; }

 //       x_p.x() = x.x() + std::cos(x.theta())*u.v()*delta_t - std::sin(x.theta())*x.uy()*delta_t;
 //       x_p.y() = x.y() + std::sin(x.theta())*u.v()*delta_t + std::cos(x.theta())*x.uy()*delta_t;
 //       x_p.uy() = x.uy() + delta_t * x.uy_dot();
 //       x_p.uy_dot() = c1 * x.uy()/v - (u.v() + c2/v) * x.r() + c3 * u.phi();
 //       x_p.theta() = x.theta() + x.r()*delta_t;
 //       x_p.r() = x.r() + delta_t * x.r_dot();
 //       x_p.uy_dot() = c4 * x.uy()/v + c5 * x.r()/v + c6 * u.phi();

        double lateral_acceleration;
        double angular_acceleration;
        if(u.v() < 0.0001) {
            lateral_acceleration = 0;
            angular_acceleration = 0;
        }  // if we are stopped it is reasonable to consider the acceleration as 0
        else {
            //lateral_acceleration = c_1 * x.y_dot()/u.v() + x.theta_dot() * (c_2/u.v() - u.v())  + c_3 * u.phi();
            lateral_acceleration = 1/m_vehicleParams.m * (2*m_vehicleParams.s1 * (u.phi() - (u.v_y() + m_vehicleParams.a * u.yaw_rate())/u.v() )  + 2 * m_vehicleParams.s2 * (-(u.v_y() - m_vehicleParams.b * u.yaw_rate() )/ u.v()) - u.yaw_rate() * u.v());
            //angular_acceleration = c_4 * (x.y_dot()/u.v()) + c_5 * (x.yaw_dot()/u.v()) + c_6 * u.phi();
            angular_acceleration = 1/m_vehicleParams.I * (m_vehicleParams.a * 2 * m_vehicleParams.s1 * (u.phi() - (u.v_y() - m_vehicleParams.s1 * u.yaw_rate())/u.v() ) - m_vehicleParams.b * 2 * m_vehicleParams.s2 * (- (u.v_y() - m_vehicleParams.b * u.yaw_rate())/u.v() ) );
        }

        // calculate the X - components
        x_p.x() = x.x() + delta_t * x.x_dot();
        x_p.x_dot() = u.v()*std::cos(x.theta()) + lateral_acceleration * (-std::sin(x.theta())) * delta_t;

        // calculate the Y - components
        x_p.y() = x.y() + delta_t * x.x_dot();
        x_p.y_dot() = u.v()*std::sin(x.theta()) + lateral_acceleration * std::cos(x.theta()) * delta_t;

        // calculate the THETA - components
        x_p.theta() = x.theta() + x.theta_dot()*delta_t;
        x_p.theta_dot() = x.theta_dot() + angular_acceleration * delta_t;



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

//       dx   dx_dot    dy      dy_dot          dy_dot_dot      dtheta                              dtheta_dot       dtheta_dot_dot
//
//J_f = [1    delta_t   0       0               0               0                                  0                0          dx
//       0    0         0      -cos(theta)      0              -v*sin(theta)-y_dot*cos(theta)      0                0          dx_dot
//       0    0         1       delta_t         0               0                                  0                0          dy
//       0    0         0       cos(theta)      delta_t         v*cos(theta)-y_dot*sin(theta)      0                0          dy_dot
//       0    0         0       c1/v            0               0                                  c2/v -v          0          dy_dot_dot
//       0    0         0       0               0               1                                  delta_t          0          dtheta
//       0    0         0       0               0               0                                  1                delta_t    dr
//       0    0         0       c4/v            0               0                                  c5/v             0      ]   dr_dot

        //TODO fix constants
        //double s1 = m_vehicleParams.s1;
        //double s2 = m_vehicleParams.s2;
        //double m = m_vehicleParams.m;
        //double a = m_vehicleParams.a;
        //double b = m_vehicleParams.b;
        //double I = m_vehicleParams.I;

        //double c_1 = m_vehicleParams.c1;// (-s1 -s2) /m ;
        //double c_2 = m_vehicleParams.c2;//(s2*b -s1*a)/m ;
        //double c_3 = m_vehicleParams.c3;//s1/m ;
        //double c_4 = m_vehicleParams.c4;//(s2*b -s1*a)/I ;
        //double c_5 = m_vehicleParams.c5;//(-s1*pow(a,2) - s2*pow(b,2))/I;
        //double c_6 = m_vehicleParams.c6;//s1*a/I ;


        double lateral_acceleration;
        //double angular_acceleration;
        if(u.v() < 0.0001) {
            lateral_acceleration = 0;
            //angular_acceleration = 0;
        }  // if we are stopped it is reasonable to consider the acceleration as 0
        else {
            //lateral_acceleration = c_1 * x.y_dot()/u.v() + x.theta_dot() * (c_2/u.v() - u.v())  + c_3 * u.phi();
            lateral_acceleration = 1/m_vehicleParams.m * (2*m_vehicleParams.s1 * (u.phi() - (u.v_y() + m_vehicleParams.a * u.yaw_rate())/u.v() )  + 2 * m_vehicleParams.s2 * (-(u.v_y() - m_vehicleParams.b * u.yaw_rate() )/ u.v()) - u.yaw_rate() * u.v());
            //angular_acceleration = c_4 * (x.y_dot()/u.v()) + c_5 * (x.yaw_dot()/u.v()) + c_6 * u.phi();
            //angular_acceleration = 1/m_vehicleParams.I * (m_vehicleParams.a * 2 * m_vehicleParams.s1 * (u.phi() - (u.v_y() - m_vehicleParams.s1 * u.yaw_rate())/u.v() ) - m_vehicleParams.b * 2 * m_vehicleParams.s2 * (- (u.v_y() - m_vehicleParams.b * u.yaw_rate())/u.v() ) );
        }
        // TODO :
        // if u.v = 0 ==> y_dot_dot = 0 !!!


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
        double delta_t = 0.05;   //TODO set automatically
        // partial derivative of x.x() w.r.t. x.x()
        this->F( S::X, S::X ) = 1;
        // partial derivative of x.x() w.r.t. x.x_dot()
        this->F( S::X, S::X_DOT ) = delta_t;
        // partial derivative of x.x() w.r.t. x.theta()
        this->F( S::X_DOT, S::THETA ) = -std::sin( x.theta() ) * u.v() - lateral_acceleration * std::cos(x.theta())* delta_t;

        // partial derivative of x.y() w.r.t. x.y()
        this->F( S::Y, S::Y ) = 1;
        // partial derivative of x.y() w.r.t. x.y_dot()
        this->F( S::Y, S::Y_DOT ) = delta_t;
        // partial derivative of x.y() w.r.t. x.theta()
        this->F( S::Y_DOT, S::THETA ) = std::cos( x.theta() ) * u.v() - lateral_acceleration * std::sin(x.theta()) * delta_t;

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
        //                       delta_t^2/2 delta_t]; --- x - x_dot
        //Qxyz = sigma_q(1)^2 * [delta_t^5/20  delta_t^4/8   delta_t^3/6;
        //                       delta_t^4/8   delta_t^3/3   delta_t^2/2
        //                       delta_t^3/6   delta_t^2/2   delta_t]; --- x - x_dot - x_dotdot

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

    }
};
} // combinedTruckModel
} // Sensation
} // application
} // system
} // opendlv
#endif
