/**
 * Copyright (C) 2017 Chalmers Revere
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

#ifndef LOGIC_SENSATION_GEOLOCATION_GEOLOCATION_HPP
#define LOGIC_SENSATION_GEOLOCATION_GEOLOCATION_HPP

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendavinci/odcore/wrapper/Eigen.h>

//#include "opendlv/data/environment/Point3.h"
//#include "opendlv/data/environment/WGS84Coordinate.h"

//#include "odvdfh16truck/GeneratedHeaders_ODVDFH16Truck.h"
//#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
//#include "odvdtrimble/GeneratedHeaders_ODVDTrimble.h"
//#include "odvdimu/GeneratedHeaders_ODVDIMU.h"

//Include cfsd??
//#include <odvdcfsd18/GeneratedHeaders_ODVDcfsd18.h>

namespace opendlv {
namespace logic {
namespace sensation {

class Geolocation : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Geolocation(int32_t const &, char **);
  Geolocation(Geolocation const &) = delete;
  Geolocation &operator=(Geolocation const &) = delete;
  virtual ~Geolocation();
  virtual void nextContainer(odcore::data::Container &);
  

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();
  //void nextContainer(odcore::data::Container &);
};

}
}
}

#endif
