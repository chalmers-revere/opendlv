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

#ifndef GUI_LIVEFEED_LIVEFEEDAPP_HPP
#define GUI_LIVEFEED_LIVEFEEDAPP_HPP

#include <Wt/WApplication>
#include <Wt/WLineEdit>
#include <Wt/WText>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/base/Mutex.h>

namespace opendlv {
namespace gui {

class LiveFeedApp : public Wt::WApplication {
   public:
    LiveFeedApp(Wt::WEnvironment const &);
    LiveFeedApp(LiveFeedApp const &) = delete;
    LiveFeedApp &operator=(LiveFeedApp const &) = delete;
    virtual ~LiveFeedApp();
    void AddContainer(odcore::data::Container &);

   private:
    void Greet();

    odcore::data::Container m_container;
    odcore::base::Mutex m_mutex;
    Wt::WLineEdit *m_nameEdit;
    Wt::WText *m_greeting;
    bool m_isInitialized;
};

}
}

#endif
