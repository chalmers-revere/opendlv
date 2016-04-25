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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef GEOLOCATION_GEOLOCATION_HPP_
#define GEOLOCATION_GEOLOCATION_HPP_

#include <memory>
    
#include "kalman/ExtendedKalmanFilter.hpp"

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendlv/data/environment/WGS84Coordinate.h>

#include "geolocation/kinematicmodel.hpp"

namespace opendlv {
namespace sensation {
namespace geolocation {

/**
 * This class provides...
 */
class Geolocation
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Geolocation(int32_t const &, char **);
  Geolocation(Geolocation const &) = delete;
  Geolocation &operator=(Geolocation const &) = delete;
  virtual ~Geolocation();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();


   Kalman::ExtendedKalmanFilter<opendlv::sensation::geolocation::State<double>> 
       m_ekf;
};

} // geolocation
} // sensation
} // opendlv

#endif
