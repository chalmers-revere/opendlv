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

#ifndef COMBINEDOBSERVATIONMODEL_HPP_
#define COMBINEDOBSERVATIONMODEL_HPP_

#include "LinearizedMeasurementModel.hpp"

namespace opendlv {
namespace system {
namespace application {
namespace sensation {
namespace combinedTruckObservationModel
{


using namespace opendlv::system::libs;




/**
 * @brief Measurement vector
 *
 *  Vector:
 *    Z_m: measures as a vector m-by-1 -- where m is 4 in our truck:
 *           Z_m(1) = position along x in (m)
 *           Z_m(2) = position along y in (m)
 *           Z_m(3) = yaw, i.e. heading of the vehicle in (rad)
 *           Z_m(4) = yaw rate (rad/s)
 *
 * @param T Numeric scalar type
 */
template<typename T>
class combinedTruckObservationVector : public opendlv::system::libs::kalman::Vector<T, 4>
{
public:
    KALMAN_VECTOR(combinedTruckObservationVector, T, 4)

    //! position along x axis
    static constexpr size_t Z_X = 0;

    //! position along y axis
    static constexpr size_t Z_Y = 1;

    //! heading of the vehicle yaw
    static constexpr size_t Z_THETA = 2;

    //! yaw rate of the vehicle
    static constexpr size_t Z_THETA_DOT = 3;

    T Z_x()          const { return (*this)[ Z_X ]; }
    T Z_y()          const { return (*this)[ Z_Y ]; }
    T Z_theta()      const { return (*this)[ Z_THETA ]; }
    T Z_theta_dot()  const { return (*this)[ Z_THETA_DOT ]; }

    T& Z_x()          { return (*this)[ Z_X ]; }
    T& Z_y()          { return (*this)[ Z_Y ]; }
    T& Z_theta()        { return (*this)[ Z_THETA ]; }
    T& Z_theta_dot()    { return (*this)[ Z_THETA_DOT ]; }
}; // end - class truckObservationModel


/**
 * @brief Observation model measuring the truck position, orientation and rotational velocity
 *        all the measures must be in SI unit system.
 *        according to the notation in the Kalman filter theory and notes :TODO file !!!
 *
 *        Observation Equation:
 *           Z(k) = h(X(k)) + v(k)
 *                the observation Z has the dimension M-by-1
 *                v ~ N(0,R) is gaussian noise with covariance R
 *
 *        h: observation function, it takes the state variable x(k) (at the time k) and
 *        returns:
 *           1) Z(k) = h(X(k))
 *           2) its Jacobian dh/dx in X(k).
 *
 *
 * @param T Numeric scalar type
 * @param CovarianceBase Class template to determine the covariance representation
 *                       (as covariance matrix (StandardBase) or as lower-triangular
 *                       coveriace square root (SquareRootBase))
 */
template<typename T, template<class> class CovarianceBase = opendlv::system::libs::kalman::StandardBase>
class combinedTruckObservationModel :
        public opendlv::system::libs::kalman::LinearizedMeasurementModel<opendlv::system::application::sensation::combinedTruckModel::State<T>,
               combinedTruckObservationVector<T>,
               CovarianceBase>
{
public :
    //! State type shortcut definition
    typedef opendlv::system::application::sensation::combinedTruckModel::State<T> S;

    //! Measurement type shortcut definition
    typedef combinedTruckObservationVector<T> M;

    /**
     * @brief Constructor of the observation model -
     *        For now it is considered that we directly access all the quantities.
     *
     * @param _x The x-position of the truck along x direction in meters
     * @param _y The y-position of the truck along y direction in meters
     * @param _theta The heading of the truck in rad
     * @param _theta_dot The rotational velocity of the truck in rad/sec
     */
    combinedTruckObservationModel(T _x, T _y, T _theta, T _theta_rate)
    {
        // Save landmark positions
        Z_k << _x, _y, _theta, _theta_rate;

        // Setup noise jacobian. As this one is static, we can define it once
        // and do not need to update it dynamically
        this->V.setIdentity();

    }

    /**
     * @brief Definition of (possibly non-linear) measurement function
     *
     * This function maps the system state to the measurement that is expected
     * to be received from the sensor assuming the system is currently in the
     * estimated state.
     *
     * @param [in] x The system state in current time-step
     * @returns The (predicted) sensor measurement for the system state
     */
    M h(const S& _x) const
    {
        M measurement;

        // Vehicle measurement vector (x,y, theta, theta_dot)-vector
        // This uses the Eigen template method to get the first 4 elements of the vector

        // for now we are considering a linear observation model
        // moreover, we already get our measures as we expect to be
        measurement.Z_x() = _x(0);         // position along the x axis
        measurement.Z_y() = _x(1);         // position along the y axis
        measurement.Z_theta() = _x(4);     // heading of the vehicle
        measurement.Z_theta_dot() = _x(5); // yaw rate, i.e. rotational velocity of the vehicle

        return measurement;
    }

protected:
    //! Measurement vector (x,y,theta,theta_dot)-measurement
    opendlv::system::libs::kalman::Vector<T, 4> Z_k;

protected:

    /**
     * @brief Update jacobian matrices for the system state transition function using current state
     *
     * This will re-compute the (state-dependent) elements of the jacobian matrices
     * to linearize the non-linear measurement function \f$h(x)\f$ around the
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
    void updateJacobians( const S& x )
    {
        // H = dh/dx (Jacobian of measurement function w.r.t. the state)
        this->H.setZero();

        ///TODO: this function must have the state as argument but for this case it is not
        ///      necessary, to avoid the compiler to complain I just set the argument to 0
        S _x = x; //this will be valid only inside the function and it will not affect the real variable
        _x=_x;

        // Robot position as (x,y)-vector
        // This uses the Eigen template method to get the first 2 elements of the vector
        //opendlv::system::libs::kalman::Vector<T, 6> _x = x.template head<6>();

        // partial derivative of meas.d1() w.r.t. x.x()
        this->H( M::Z_X, S::X ) = 1;//_x(0);//1;//delta1[0] / d1;
        // partial derivative of meas.d1() w.r.t. x.y()
        this->H( M::Z_Y, S::Y ) = 1;//_x(2);//1;//delta1[1] / d1;

        // partial derivative of meas.d1() w.r.t. x.x()
        this->H( M::Z_THETA, S::THETA ) = 1;//_x(4);//1;//delta2[0] / d2;
        // partial derivative of meas.d1() w.r.t. x.y()
        this->H( M::Z_THETA_DOT, S::R ) = 1;//_x(5);//1;//delta2[1] / d2;
    }


};


} // observationModel
} // sensation
} // application
} // system
} // opendlv
#endif  /* OBSERVATIONMODEL_HPP_ */
