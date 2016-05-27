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

#ifndef LEDSTRIP_LEDSTRIP_HPP_
#define LEDSTRIP_LEDSTRIP_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include <opendavinci/odcore/wrapper/SerialPort.h>
#include <opendavinci/odcore/wrapper/SerialPortFactory.h>
namespace opendlv {
namespace proxy {
namespace ledstrip {

class Device;

/**
 * This class provides...
 */
class Ledstrip
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  Ledstrip(int32_t const &, char **);
  Ledstrip(Ledstrip const &) = delete;
  Ledstrip &operator=(Ledstrip const &) = delete;
  virtual ~Ledstrip();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();


  std::unique_ptr<Device> m_device;
};

} // ledstrip
} // proxy
} // opendlv

#endif
