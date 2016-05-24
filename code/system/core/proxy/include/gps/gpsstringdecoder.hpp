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

#ifndef GPS_GPSSTRINGDECODER_HPP_
#define GPS_GPSSTRINGDECODER_HPP_

#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>

namespace opendlv {
namespace proxy {
namespace gps {

/**
 * This class decodes the received string.
 */
class GpsStringDecoder : public odcore::io::StringListener {
 public:
  GpsStringDecoder(odcore::io::conference::ContainerConference &);
  GpsStringDecoder(GpsStringDecoder const &) = delete;
  GpsStringDecoder &operator=(GpsStringDecoder const &) = delete;
  virtual ~GpsStringDecoder();

  virtual void nextString(const std::string &s);

 private:
  odcore::io::conference::ContainerConference &m_conference;
};

} // gps
} // proxy
} // opendlv

#endif
