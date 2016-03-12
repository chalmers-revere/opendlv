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

#ifndef OPENDLV_SYSTEM_LIBS_KALMAN_EXTENDEDKALMANFILTER_HPP_
#define OPENDLV_SYSTEM_LIBS_KALMAN_EXTENDEDKALMANFILTER_HPP_

// standard libraries
#include <iostream>
#include <cmath>
//#include <Eigen/Geometry>
#include <opendavinci/Eigen/Dense>
#include "KalmanFilterBase.hpp"

#include "StandardFilterBase.hpp"
#include "LinearizedSystemModel.hpp"
#include "LinearizedMeasurementModel.hpp"

using namespace std;


namespace opendlv {
namespace system {
namespace libs {
namespace kalman{


/**  @brief Simple class for EKF - Extended Kalman Filter
  *
  *
  * This implementation is based upon [An Introduction to the Kalman Filter](https://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf)
  * by Greg Welch and Gary Bishop.
  *
  * @param StateType The vector-type of the system state (usually some type derived from Kalman::Vector)
  *
  *
  *  <b>Usage:</b><br>
  *		- Write me
  *
  *  <b>About the algorithm:</b><br>
  *		- Write me
  *
  *
  * <b>Changes history</b>
  *		- MAR/2016: Creation (-).
  *  \ingroup __
  */
    template<class StateType>
    class ExtendedKalmanFilter : public KalmanFilterBase<StateType>,
                                 public StandardFilterBase<StateType>
    {
    public:
        //! Kalman Filter base type
        typedef KalmanFilterBase<StateType> KalmanBase;
        //! Standard Filter base type
        typedef StandardFilterBase<StateType> StandardBase;

        //! Numeric Scalar Type inherited from base
        using typename KalmanBase::T;

        //! State Type inherited from base
        using typename KalmanBase::State;

        //! Linearized Measurement Model Type
        template<class Measurement, template<class> class CovarianceBase>
        using MeasurementModelType = LinearizedMeasurementModel<State, Measurement, CovarianceBase>;

        //! Linearized System Model Type
        template<class Control, template<class> class CovarianceBase>
        using SystemModelType = LinearizedSystemModel<State, Control, CovarianceBase>;

    protected:
        //! Kalman Gain Matrix Type
        template<class Measurement>
        using KalmanGain = kalman::KalmanGain<State, Measurement>;

    protected:
        //! State Estimate
        using KalmanBase::x;
        //! State Covariance Matrix
        using StandardBase::P;

    public:
        /**
         * @brief Constructor
         */
        ExtendedKalmanFilter()
        {
            // Setup state and covariance
            P.setIdentity();
        }

        /**
         * @brief Perform filter prediction step using system model and no control input (i.e. \f$ u = 0 \f$)
         *
         * @param [in] s The System model
         * @return The updated state estimate
         */
        template<class Control, template<class> class CovarianceBase>
        const State& predict( SystemModelType<Control, CovarianceBase>& s )
        {
            // predict state (without control)
            Control u;
            u.setZero();
            return predict( s, u );
        }

        /**
         * @brief Perform filter prediction step using control input \f$u\f$ and corresponding system model
         *
         * @param [in] s The System model
         * @param [in] u The Control input vector
         * @return The updated state estimate
         */
        template<class Control, template<class> class CovarianceBase>
        const State& predict( SystemModelType<Control, CovarianceBase>& s, const Control& u )
        {
            s.updateJacobians( x, u );

            // predict state

            x = s.f(x, u);

            // predict covariance
            P  = ( s.F * P * s.F.transpose() ) + ( s.W * s.getCovariance() * s.W.transpose() );
            //TODO: MB - add a support to set the noise covariance matrixes Q and R 

            // return state prediction
            return this->getState();
        }

        /**
         * @brief Perform filter update step using measurement \f$z\f$ and corresponding measurement model
         *
         * @param [in] m The Measurement model
         * @param [in] z The measurement vector
         * @return The updated state estimate
         */
        template<class Measurement, template<class> class CovarianceBase>
        const State& update( MeasurementModelType<Measurement, CovarianceBase>& m, const Measurement& z )
        {
            m.updateJacobians( x );

            // COMPUTE KALMAN GAIN
            // compute innovation covariance
            Covariance<Measurement> S = ( m.H * P * m.H.transpose() ) + ( m.V * m.getCovariance() * m.V.transpose() );

            // compute kalman gain
            KalmanGain<Measurement> K = P * m.H.transpose() * S.inverse();

            // UPDATE STATE ESTIMATE AND COVARIANCE
            // Update state using computed kalman gain and innovation
             x += K * ( z - m.h( x ) );

            // Update covariance
            P -= K * m.H * P;

            // return updated state estimate
            return this->getState();
        }
    };

} //kalman
} //libs
} //system
} //opendlv

#endif /*  OPENDLV_SYSTEM_LIBS_KALMAN_EXTENDEDKALMANFILTER_HPP_  */
