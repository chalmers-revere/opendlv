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

#ifndef LOGIC_KNOWLEDGE_IDENTITY_IDENTITY_HPP_
#define LOGIC_KNOWLEDGE_IDENTITY_IDENTITY_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

 
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

namespace opendlv {
namespace logic {
namespace knowledge {

class Identity
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Identity(int32_t const &, char **);
  Identity(Identity const &) = delete;
  Identity &operator=(Identity const &) = delete;
  virtual ~Identity();
  // virtual void nextContainer(odcore::data::Container &);
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();
  void SendContainer(opendlv::knowledge::Insight  &);

  uint32_t m_stationId;
  uint32_t m_stationType;
  double m_vehicleLength;
  double m_vehicleWidth;
  uint32_t m_vehicleRole;
  double m_rearAxleLocation;
};

}
}
}

#endif
