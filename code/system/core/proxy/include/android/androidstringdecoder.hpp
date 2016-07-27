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

#ifndef ANDROID_ANDROIDSTRINGDECODER_HPP_
#define ANDROID_ANDROIDSTRINGDECODER_HPP_

#include <memory>

#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>
#include <opendavinci/odcore/io/ConnectionListener.h>
#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/tcp/TCPAcceptorListener.h>
#include <opendavinci/odcore/io/tcp/TCPConnection.h>

namespace opendlv {
namespace proxy {
namespace android {

/**
 * This class decodes the received string.
 */
class AndroidStringDecoder : 
    public odcore::io::ConnectionListener,
    public odcore::io::StringListener,
    public odcore::io::tcp::TCPAcceptorListener {
 public:
  AndroidStringDecoder(odcore::io::conference::ContainerConference &);
  AndroidStringDecoder(AndroidStringDecoder const &) = delete;
  AndroidStringDecoder &operator=(AndroidStringDecoder const &) = delete;
  virtual ~AndroidStringDecoder();

  virtual void nextString(const std::string &s);

  virtual void onNewConnection(std::shared_ptr<odcore::io::tcp::TCPConnection> connection);

  virtual void handleConnectionError();

 private:
  odcore::io::conference::ContainerConference &m_conference;
  std::shared_ptr<odcore::io::tcp::TCPConnection> m_connection;
};

} // android
} // proxy
} // opendlv

#endif
