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

#ifndef OPENDLV_SYSTEM_LIBS_KALMAN_MEASUREMENTMODEL_HPP_
#define OPENDLV_SYSTEM_LIBS_KALMAN_MEASUREMENTMODEL_HPP_

#include <type_traits>

#include "StandardBase.hpp"


namespace opendlv {
namespace system {
namespace libs {
namespace kalman{



    /**
     * @brief Abstract base class of all measurement models
     *
     * @param StateType The vector-type of the system state (usually some type derived from Kalman::Vector)
     * @param MeasurementType The vector-type of the measurement (usually some type derived from Kalman::Vector)
     * @param CovarianceBase The class template used for covariance storage (must be either StandardBase or SquareRootBase)
     */
    template<class StateType, class MeasurementType, template<class> class CovarianceBase = opendlv::system::libs::kalman::StandardBase>
    class MeasurementModel : public CovarianceBase<MeasurementType>
    {
        static_assert( StateType::length       > 0, "State vector must contain at least 1 element");
        static_assert( MeasurementType::length > 0, "Measurement vector must contain at least 1 element");
        static_assert( std::is_same<typename StateType::Scalar, typename MeasurementType::Scalar>::value, "State and Measurement scalar types must be identical" );
    public:
        //! System state type
        typedef StateType State;

        //! Measurement vector type
        typedef MeasurementType Measurement;

    public:
        /**
         * Measurement Model Function h
         *
         * Predicts the estimated measurement value given the current state estimate x
         */
        virtual Measurement h(const State& x) const = 0;

    protected:
        MeasurementModel() {}
    };
} //kalman
} //libs
} //system
} //opendlv

#endif  /*  OPENDLV_SYSTEM_LIBS_KALMAN_MEASUREMENTMODEL_HPP_  */
