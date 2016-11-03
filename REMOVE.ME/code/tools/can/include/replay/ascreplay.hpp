/**
 * Copyright (C) 2016 Chalmers REVERE
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

#ifndef ASCREPLAY_ASCREPLAY_HPP_
#define ASCREPLAY_ASCREPLAY_HPP_

#include <string>
#include <vector>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <reverefh16mapping/GeneratedHeaders_reverefh16mapping.h>

namespace opendlv {
namespace tools {
namespace can {
namespace replay {

/**
 * This class replays CAN messages from a .ASC recording piped to this tool over
 * commandline.
 */
class AscReplay
: public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  AscReplay(int32_t const &, char **);
  AscReplay(AscReplay const &) = delete;
  AscReplay &operator=(AscReplay const &) = delete;
  virtual ~AscReplay();

  vector<odcore::data::Container> getMessages(
  const std::string &nextLineFromAsc);

 private:
  void setUp();
  void tearDown();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  canmapping::CanMapping m_revereFh16CanMessageMapping;
};

} // replay
} // can
} // tools
} // opendlv

#endif
