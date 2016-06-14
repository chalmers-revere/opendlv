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

#ifndef LIDAR_LIDAR_HPP_
#define LIDAR_LIDAR_HPP_

#include <memory>
#include <list>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/wrapper/SerialPort.h"
#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "lidar/lidarstringdecoder.hpp"

namespace opendlv {
namespace proxy {
namespace lidar {

/**
 * This class decodes data from SICK LMS 200.
 */
class Lidar : public odcore::base::module::TimeTriggeredConferenceClientModule 
{
 public:
  Lidar(int32_t const &, char **);
  Lidar(Lidar const &) = delete;
  Lidar &operator=(Lidar const &) = delete;
  virtual ~Lidar();

  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  virtual void setUp();
  virtual void tearDown();
  void Status();
  void StartScan();
  void StopScan();
  void SettingsMode();
  void SetCentimeterMode();

  std::shared_ptr<odcore::wrapper::SerialPort> m_sick;
  std::unique_ptr<LidarStringDecoder> m_lidarStringDecoder;
};

} // lidar
} // proxy
} // opendlv

#endif
