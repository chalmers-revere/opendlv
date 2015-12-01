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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PROXY_HPP_
#define PROXY_HPP_

#include "core/base/module/DataTriggeredConferenceClientModule.h"
#include "core/data/Container.h"
#include "tools/recorder/Recorder.h"

namespace opendlv {
namespace system {

class Camera;

/**
 * This class provides...
 */
class Proxy : public core::base::module::DataTriggeredConferenceClientModule {
  public:
    Proxy(int32_t const &, char **);
    Proxy(Proxy const &) = delete;
    Proxy &operator=(Proxy const &) = delete;
    virtual ~Proxy();
    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body();
    void nextContainer(core::data::Container &);

  private:
    void distribute(core::data::Container c);
    void setUp();
    void tearDown();

    std::unique_ptr<tools::recorder::Recorder> m_recorder;
    std::unique_ptr<Camera> m_camera;
};

} // system
} // opendlv

#endif
