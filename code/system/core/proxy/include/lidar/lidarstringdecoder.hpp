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

#ifndef LIDAR_LIDARSTRINGDECODER_HPP_
#define LIDAR_LIDARSTRINGDECODER_HPP_
 
#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/conference/ContainerConference.h>
#include <opendavinci/odcore/base/Mutex.h>

namespace opendlv {
namespace proxy {
namespace lidar {

/**
 * This class decodes the received string.
 */
class LidarStringDecoder : public odcore::io::StringListener {
 public:
  LidarStringDecoder(odcore::io::conference::ContainerConference &, double, double, double);
  LidarStringDecoder(LidarStringDecoder const &) = delete;
  LidarStringDecoder &operator=(LidarStringDecoder const &) = delete;
  virtual ~LidarStringDecoder();

  virtual void nextString(const std::string &);
  opendlv::proxy::EchoReading GetLatestReading();
  bool IsCentimeterMode() const;
  bool IsRunning() const;
  bool IsSettingsMode() const;
  void NotSettingsMode();
  void NotCentimeterMode();

 private:
  bool CheckForCentimeterResponse() ;
  bool CheckForMeasurementHeader() ;
  bool CheckForSettingsResponse() ;
  bool CheckForStartResponse() ;
  void ConvertToDistances();

  odcore::io::conference::ContainerConference &m_conference;
  bool m_firstHeader;
  bool m_startConfirmed;
  bool m_settingsMode;
  bool m_centimeterMode;
  bool m_settingsDone;
  uint32_t m_counter;
  uint32_t m_bufferSize;
  //std::vector<opendlv::model::Direction> m_directions;
  //std::vector<double> m_radii;
  odcore::base::Mutex m_latestReadingMutex;
  opendlv::proxy::EchoReading m_latestReading; 
  double m_position[3];
  unsigned char m_measurements[1000];
  unsigned char m_startResponse[10];
  unsigned char m_measurementHeader[7];
  unsigned char m_buffer[44];
  unsigned char m_centimeterResponse[44];
};

} // gps
} // proxy
} // opendlv

#endif
