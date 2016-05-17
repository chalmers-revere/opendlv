/**
 * Copyright (C) 2016 Christian Berger
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

#ifndef DIAGNOSTICS_HEALTH_HPP_
#define DIAGNOSTICS_HEALTH_HPP_

#include <string>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

namespace opendlv {
namespace diagnostics {
namespace health {

/**
 * This class regularly checks system health.
 */
class Health : public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Health(int32_t const &, char **);
  Health(Health const &) = delete;
  Health &operator=(Health const &) = delete;
  virtual ~Health();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();

 private:
  std::string m_healthScript;
};

} // health
} // diagnostics
} // opendlv

#endif
