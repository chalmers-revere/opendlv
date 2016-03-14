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

#ifndef COMMUNICATE_COMMUNICATE_HPP_
#define COMMUNICATE_COMMUNICATE_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace action {
namespace communicate {

/**
 * This class provides...
 */
class Communicate
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Communicate(int32_t const &, char **);
  Communicate(Communicate const &) = delete;
  Communicate &operator=(Communicate const &) = delete;
  virtual ~Communicate();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  void setUp();
  void tearDown();
};

} // communicate
} // action
} // opendlv

#endif
