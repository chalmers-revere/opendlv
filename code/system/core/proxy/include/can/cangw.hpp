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

#ifndef PROXY_CANGW_HPP_
#define PROXY_CANGW_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "odcantools/CANDevice.h"
#include "odcantools/GenericCANMessageListener.h"
#include "automotivedata/generated/automotive/GenericCANMessage.h"

namespace opendlv {
namespace proxy {
namespace can {

/**
 * This class provides...
 */

class CANGW : 
    public odcore::base::module::TimeTriggeredConferenceClientModule,
    public automotive::odcantools::GenericCANMessageListener {
  public:
    CANGW(int32_t const &, char **);
    CANGW(CANGW const &) = delete;
    CANGW &operator=(CANGW const &) = delete;
    virtual ~CANGW();

    virtual void nextGenericCANMessage(const automotive::GenericCANMessage &gcm);

  private:
    void setUp();
    void tearDown();
    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  private:
    std::unique_ptr<automotive::odcantools::CANDevice> m_device;
};

} // can
} // proxy
} // opendlv

#endif
