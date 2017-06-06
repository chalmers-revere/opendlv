/**
 * Copyright (C) 2016 Chalmers Revere
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

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>

#include "livefeed.hpp"

namespace opendlv {
namespace gui {

LiveFeed::LiveFeed(const int &argc, char **argv)
    : DataTriggeredConferenceClientModule(argc, argv, "gui-livefeed")
{
}

LiveFeed::~LiveFeed() 
{
}

void LiveFeed::setUp() 
{
//  std::string setting = getKeyValueConfiguration().getValue<std::string>(
//      "gui-livefeed.setting");
}

void LiveFeed::tearDown()
{
}

void LiveFeed::nextContainer(odcore::data::Container &a_container) 
{
  (void) a_container;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode LiveFeed::body() 
{
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

}
}
