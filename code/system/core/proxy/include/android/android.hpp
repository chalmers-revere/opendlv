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

#ifndef ANDROID_ANDROID_HPP_
#define ANDROID_ANDROID_HPP_

#include <memory>

#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/tcp/TCPAcceptor.h>

#include "android/androidstringdecoder.hpp"

namespace opendlv {
namespace proxy {
namespace android {

class Device;

/**
 * This class provides...
 */
class Android : public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  Android(int32_t const &, char **);
  Android(Android const &) = delete;
  Android &operator=(Android const &) = delete;
  virtual ~Android();

  virtual void nextContainer(odcore::data::Container &c);

 private:
  void setUp();
  void tearDown();
  
 private:
  std::shared_ptr<odcore::io::tcp::TCPAcceptor> m_tcpacceptor;
  std::unique_ptr<AndroidStringDecoder> m_androidStringDecoder;
};

} // android
} // proxy
} // opendlv

#endif
