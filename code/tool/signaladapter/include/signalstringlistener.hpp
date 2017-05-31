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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef TOOL_SIGNALADAPTER_SIGNALSTRINGLISTENER_HPP
#define TOOL_SIGNALADAPTER_SIGNALSTRINGLISTENER_HPP

#include <sstream>

#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>

namespace opendlv {
namespace tool {

/**
 * This class decodes data from open udp port.
 */
class SignalStringListener : public odcore::io::StringListener {
   private:
    SignalStringListener(SignalStringListener const &) = delete;
    SignalStringListener &operator=(SignalStringListener const &) = delete;

   public:
    SignalStringListener(odcore::io::conference::ContainerConference &, bool &);
    virtual ~SignalStringListener();

    virtual void nextString(const std::string &);

   private:
    odcore::io::conference::ContainerConference &m_conference;
    bool &m_debug;
};

}
}

#endif
