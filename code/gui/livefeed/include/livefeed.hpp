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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef GUI_LIVEFEED_LIVEFEED_HPP
#define GUI_LIVEFEED_LIVEFEED_HPP

#include <opendavinci/odcore/base/Mutex.h>
#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>

namespace opendlv {
namespace gui {

class LiveFeed : public odcore::base::module::DataTriggeredConferenceClientModule {
   public:
    LiveFeed(int32_t const &, char **);
    LiveFeed(LiveFeed const &) = delete;
    LiveFeed &operator=(LiveFeed const &) = delete;
    virtual ~LiveFeed();

   private:
    void nextContainer(odcore::data::Container &);
    virtual void setUp();
    virtual void tearDown();
    virtual odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
};

}
}

#endif
