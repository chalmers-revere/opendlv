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

#ifndef TRACKVEHICLE_TRACKVEHICLE_HPP_
#define TRACKVEHICLE_TRACKVEHICLE_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
 

namespace opendlv {
namespace perception {
namespace trackvehicle {

/**
 * This class provides a module with capability of keeping track of vehicle in a given series of images
 */
class TrackVehicle
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  TrackVehicle(int32_t const &, char **);
  TrackVehicle(TrackVehicle const &) = delete;
  TrackVehicle &operator=(TrackVehicle const &) = delete;
  virtual ~TrackVehicle();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  bool m_initialised;
  bool m_debug;
};

} // trackvehicle
} // perception
} // opendlv

#endif
