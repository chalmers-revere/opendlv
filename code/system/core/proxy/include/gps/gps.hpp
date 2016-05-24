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

#ifndef GPS_GPS_HPP_
#define GPS_GPS_HPP_

#include <memory>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/tcp/TCPConnection.h>

#include "gps/gpsstringdecoder.hpp"

namespace opendlv {
namespace proxy {
namespace gps {

class Device;

/**
 * This class provides...
 */
class Gps : public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  Gps(int32_t const &, char **);
  Gps(Gps const &) = delete;
  Gps &operator=(Gps const &) = delete;
  virtual ~Gps();

  virtual void nextContainer(odcore::data::Container &c);

 private:
  void setUp();
  void tearDown();
  
 private:
  std::unique_ptr<Device> m_device;
  std::shared_ptr<odcore::io::tcp::TCPConnection> m_trimble;
  std::unique_ptr<GpsStringDecoder> m_gpsStringDecoder;
};

} // gps
} // proxy
} // opendlv

#endif
