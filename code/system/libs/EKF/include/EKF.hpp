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

#ifndef EFK_HPP
#define EKF_HPP

// standard libraries
#include <iostream>
#include <cmath>
//#include <Eigen/Geometry>


using namespace std;

/**  Simple class for EKF - Extended Kalman Filter
  *
  *  <b>Usage:</b><br>
  *		- Write me
  *
  *  <b>About the algorithm:</b><br>
  *		- Write me
  *
  *
  * <b>Changes history</b>
  *		- MAR/2016: Creation (MB).
  *  \ingroup __
  */
namespace opendlv {
namespace system {
namespace libs {
namespace kalman{
class EKF
{
public:

	EKF(); //!< Constructor

//~EKF(); //!< Destructor ---- maybe not necessary for now, let the compiler to do it



	void set_something(double something);

	//matrix compute_covariamce_P(vector X_hat, vector U_k);

	double get_something() { return _something; };

	double _something;

//private :

};
} //kalman
} //libs
} //system
} //opendlv

#endif /*EKF_HPP */
