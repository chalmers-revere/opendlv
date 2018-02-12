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

#ifndef LOGIC_SENSATION_GEOLOCATIONTEST_GEOLOCATIONTEST_HPP
#define LOGIC_SENSATION_GEOLOCATIONTEST_GEOLOCATIONTEST_HPP

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendavinci/odcore/wrapper/Eigen.h>
#include <opendavinci/odcore/base/Lock.h>
#include "opendlv/data/environment/Point3.h"
#include "opendlv/data/environment/WGS84Coordinate.h"

#include "odvdopendlvstandardmessageset/GeneratedHeaders_ODVDOpenDLVStandardMessageSet.h"

namespace opendlv {
namespace logic {
namespace sensation {

class Geolocationtest : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Geolocationtest(int32_t const &, char **);
  Geolocationtest(Geolocationtest const &) = delete;
  Geolocationtest &operator=(Geolocationtest const &) = delete;
  virtual ~Geolocationtest();
  virtual void nextContainer(odcore::data::Container &);
  

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();
  void getDataFromFile();
  //odcore::base::Mutex m_egoStateMutex;
  vector<double> m_times = {};
  vector<double> m_lats = {};
  vector<double> m_lons = {};
  vector<double> m_groundspeeds= {};
  vector<double> m_ax = {};
  vector<double> m_ay = {};
  vector<double> m_yawrate = {};
  vector<double> m_heading = {};
  vector<double> m_racktravel = {};
  int m_iterator = 0;
  

  //void nextContainer(odcore::data::Container &);
};

}
}
}

#endif
