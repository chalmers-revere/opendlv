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




#ifndef OPENDLV_SYSTEM_LIBS_KALMAN_MATRIX_HPP_
#define OPENDLV_SYSTEM_LIBS_KALMAN_MATRIX_HPP_

#include <cmath>

#include <opendavinci/Eigen/Dense>



#define KALMAN_VECTOR(NAME, T, N)                                                       \
    static constexpr int length = N;                                                    \
    typedef opendlv::system::libs::kalman::Vector<T, N> Base;                           \
                                                                                        \
    NAME(void) : opendlv::system::libs::kalman::Vector<T, N>() {}                       \
                                                                                        \
    template<typename OtherDerived>                                                     \
    NAME(const Eigen::MatrixBase<OtherDerived>& other) : opendlv::system::libs::kalman::Vector<T, N>(other) {} \
                                                                                        \
    template<typename OtherDerived>                                                     \
    NAME& operator= (const Eigen::MatrixBase <OtherDerived>& other)                     \
    {                                                                                   \
        this->Base::operator=(other);                                                   \
        return *this;                                                                   \
    }




namespace opendlv {
namespace system {
namespace libs {
namespace kalman {


    /**
     * @class Kalman::Matrix
     * @brief Template type for matrices
     * @param T The numeric scalar type
     * @param rows The number of rows
     * @param cols The number of columns
     */
    template<typename T, int rows, int cols>
    using Matrix = Eigen::Matrix<T, rows, cols>;

    /**
     * @brief Template type for vectors
     * @param T The numeric scalar type
     * @param N The vector dimension
     */
    template<typename T, int N>
    class Vector : public Matrix<T, N, 1>
    {
    public:
        //! Vector length
        static constexpr int length = N;

        //! Matrix base type
        typedef Matrix<T, N, 1> Base;

        Vector(void) : Matrix<T, N, 1>() {}


        /**
         * @brief Copy constructor
         */
        template<typename OtherDerived>
        Vector(const Eigen::MatrixBase<OtherDerived>& other)
            : Matrix<T, N, 1>(other)
        { }
        /**
         * @brief Copy assignment constructor
         */
        template<typename OtherDerived>
        Vector& operator= (const Eigen::MatrixBase <OtherDerived>& other)
        {
            this->Base::operator=(other);
            return *this;
        }
    };

    /**
     * @brief Cholesky square root decomposition of a symmetric positive-definite matrix
     * @param _MatrixType The matrix type
     * @param _UpLo Square root form (Eigen::Lower or Eigen::Upper)
     */
    template<typename _MatrixType, int _UpLo = Eigen::Lower>
    class Cholesky : public Eigen::LLT< _MatrixType, _UpLo >
    {
    public:
        Cholesky() : Eigen::LLT< _MatrixType, _UpLo >() {}

        /**
         * @brief Construct cholesky square root decomposition from matrix
         * @param m The matrix to be decomposed
         */
        Cholesky(const _MatrixType& m ) : Eigen::LLT< _MatrixType, _UpLo >(m) {}

        /**
         * @brief Set decomposition to identity
         */
        Cholesky& setIdentity()
        {
            this->m_matrix.setIdentity();
            this->m_isInitialized = true;
            return *this;
        }

        /**
         * @brief Check whether the decomposed matrix is the identity matrix
         */
        bool isIdentity() const
        {
            eigen_assert(this->m_isInitialized && "LLT is not initialized.");
            return this->m_matrix.isIdentity();
        }

        /**
         * @brief Set lower triangular part of the decomposition
         * @param matrix The lower part stored in a full matrix
         */
        template<typename Derived>
        Cholesky& setL(const Eigen::MatrixBase <Derived>& matrix)
        {
            this->m_matrix = matrix.template triangularView<Eigen::Lower>();
            this->m_isInitialized = true;
            return *this;
        }

        /**
         * @brief Set upper triangular part of the decomposition
         * @param matrix The upper part stored in a full matrix
         */
        template<typename Derived>
        Cholesky& setU(const Eigen::MatrixBase <Derived>& matrix)
        {
            this->m_matrix = matrix.template triangularView<Eigen::Upper>().adjoint();
            this->m_isInitialized = true;
            return *this;
        }
    };
} //kalman
} //libs
} //system
} //opendlv

#endif   /* OPENDLV_SYSTEM_LIBS_KALMAN_MATRIX_HPP_ */
