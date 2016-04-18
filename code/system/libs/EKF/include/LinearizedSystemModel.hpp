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

#ifndef OPENDLV_SYSTEM_LIBS_KALMAN_LINEARIZEDSYSTEMMODEL_HPP_
#define OPENDLV_SYSTEM_LIBS_KALMAN_LINEARIZEDSYSTEMMODEL_HPP_

#include "SystemModel.hpp"


namespace opendlv {
namespace system {
namespace libs {
namespace kalman{


    template<class StateType>
    class ExtendedKalmanFilter;
    template<class StateType>
    class SquareRootExtendedKalmanFilter;

    /**
     * @brief Abstract base class of all linearized (first order taylor expansion) system models
     *
     * @param StateType The vector-type of the system state (usually some type derived from Kalman::Vector)
     * @param ControlType The vector-type of the control input (usually some type derived from Kalman::Vector)
     * @param CovarianceBase The class template used for covariance storage (must be either StandardBase or SquareRootBase)
     */
    template<class StateType, class ControlType = Vector<typename StateType::Scalar, 0>, template<class> class CovarianceBase = StandardBase >
    class LinearizedSystemModel : public SystemModel<StateType, ControlType, CovarianceBase>
    {
        friend class ExtendedKalmanFilter<StateType>;
        friend class SquareRootExtendedKalmanFilter<StateType>;
    public:
        //! System model base
        typedef SystemModel<StateType, ControlType, CovarianceBase> Base;

        //! System state type
        using typename Base::State;

        //! System control input type
        using typename Base::Control;

    protected:
        //! System model Jacobian
        Jacobian<State, State> F;
        //! System model noise Jacobian
        Jacobian<State, State> W;

        /**
         * Callback function for state-dependent update of Jacobi-matrices F and W before each update step
         */
         virtual void updateJacobians( const State& , const Control&  )
        {
            // No update by default
        }
    protected:
        LinearizedSystemModel()
        {
            F.setIdentity();
            W.setIdentity();
        }
        ~LinearizedSystemModel() {}
    };
} //kalman
} //libs
} //system
} //opendlv
#endif   /*  OPENDLV_SYSTEM_LIBS_KALMAN_LINEARIZEDSYSTEMMODEL_HPP_   */
