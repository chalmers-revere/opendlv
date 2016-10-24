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

#ifndef IVRULE_IVRULE_HPP_
#define IVRULE_IVRULE_HPP_

#include <memory>
#include <ctype.h>
#include <cstring>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace knowledge {
namespace ivrule {

/**
 * This class provides knowledge for specific vehicle following.
 */
class Ivrule : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Ivrule(int32_t const &, char **);
  Ivrule(Ivrule const &) = delete;
  Ivrule &operator=(Ivrule const &) = delete;
  virtual ~Ivrule();
  virtual void nextContainer(odcore::data::Container &);

 private:
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
  void setUp();
  void tearDown();
  void ReadEnvironment(opendlv::perception::Environment &);
  void FindMio(std::vector<opendlv::perception::Object> &);

  bool m_initialised;
  float m_mioAngleRange;
  float m_mioDistanceRange;
  int32_t m_memoryDuration;
  float m_desiredAngularSize;
  float m_desiredOpticalFlow;

  odcore::data::TimeStamp m_mioValidUntil;
  opendlv::perception::Object m_mio;
  float m_speed;
};

} // ivrule
} // knowledge
} // opendlv

#endif
