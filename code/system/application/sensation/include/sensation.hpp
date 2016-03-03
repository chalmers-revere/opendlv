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

#ifndef SENSATION_HPP_
#define SENSATION_HPP_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

//include headers for the EKF

#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic push
# endif
# pragma GCC diagnostic ignored "-Weffc++"
# pragma GCC diagnostic ignored "-Wshadow"
#endif
    #include "ExtendedKalmanFilter.hpp"
    #include "Types.hpp"
    #include "truckModel.hpp"
#ifndef WIN32
# if !defined(__OpenBSD__) && !defined(__NetBSD__)
#  pragma GCC diagnostic pop
# endif
#endif


namespace opendlv {
namespace system {
namespace application {




/**
 * This class provides...
 */
class Sensation : public odcore::base::module::TimeTriggeredConferenceClientModule {
  public:
    Sensation(int32_t const &, char **);
    Sensation(Sensation const &) = delete;
    Sensation &operator=(Sensation const &) = delete;
    virtual ~Sensation();
    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  private:
    virtual void setUp();
    virtual void tearDown();



};

} // application
} // system
} // opendlv

#endif
