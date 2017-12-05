/**
 * Copyright (C) 2015 Christian Berger
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

#ifndef LOGIC_LEGACY_SIMPLEDRIVER_SIMPLEDRIVER_HPP
#define LOGIC_LEGACY_SIMPLEDRIVER_SIMPLEDRIVER_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/Mutex.h"
#include "opendlv/data/environment/EgoState.h"
#include "opendlv/data/environment/Point3.h"
#include "opendlv/data/environment/WGS84Coordinate.h"

namespace opendlv {
namespace logic {
namespace legacy {

class SimpleDriver : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  SimpleDriver(const int32_t &argc, char **argv);
  SimpleDriver(SimpleDriver const &) = delete;
  SimpleDriver &operator=(SimpleDriver const &) = delete;
  virtual ~SimpleDriver();

  virtual void nextContainer(odcore::data::Container &c);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  virtual void setUp();
  virtual void tearDown();
       
  bool m_receveivedFirstWGS84Position;
  opendlv::data::environment::WGS84Coordinate WGS84Reference;
  opendlv::data::environment::Point3 m_oldPosition;
  opendlv::data::environment::Point3 m_oldPositionForDirection;

  odcore::base::Mutex m_egoStateMutex;
  opendlv::data::environment::EgoState m_egoState;

  odcore::base::Mutex m_currentSpeedMutex;
  double m_currentSpeed;

  double m_speedErrorSum;
};

}
}
}

#endif
