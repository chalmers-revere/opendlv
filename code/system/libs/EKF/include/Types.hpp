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



#ifndef OPENDLV_SYSTEM_LIBS_KALMAN_TYPES_HPP_
#define OPENDLV_SYSTEM_LIBS_KALMAN_TYPES_HPP_

#include "Matrix.hpp"


namespace opendlv {
namespace system {
namespace libs {
namespace kalman{


    /**
     * @class Kalman::SquareMatrix
     * @brief Template type representing a square matrix
     * @param T The numeric scalar type
     * @param N The dimensionality of the Matrix
     */
    template<typename T, int N>
    using SquareMatrix = Matrix<T, N, N>;

    /**
     * @class Kalman::Covariance
     * @brief Template type for covariance matrices
     * @param Type The vector type for which to generate a covariance (usually a state or measurement type)
     */
    template<class Type>
    using Covariance = SquareMatrix<typename Type::Scalar, Type::length>;

    /**
     * @class Kalman::CovarianceSquareRoot
     * @brief Template type for covariance square roots
     * @param Type The vector type for which to generate a covariance (usually a state or measurement type)
     */
    template<class Type>
    using CovarianceSquareRoot = Cholesky< Covariance<Type> >;

    /**
     * @class Kalman::KalmanGain
     * @brief Template type of Kalman Gain
     * @param State The system state type
     * @param Measurement The measurement type
     */
    template<class State, class Measurement>
    using KalmanGain = Matrix<typename State::Scalar, State::length, Measurement::length>;

    /**
     * @class Kalman::Jacobian
     * @brief Template type of jacobian of A w.r.t. B
     */
    template<class A, class B>
    using Jacobian = Matrix<typename A::Scalar, A::length, B::length>;

} //kalman
} //libs
} //system
} //opendlv
#endif   /*  OPENDLV_SYSTEM_LIBS_KALMAN_TYPES_HPP_  */
