/**
 * Copyright (C) 2016 Chalmers REVERE
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

#ifndef SIGNALINJECTOR_HPP_
#define SIGNALINJECTOR_HPP_


#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

#include <fstream>
#include <vector>
#include <utility>

namespace opendlv {
namespace tools {
namespace signalinjector {


class Signalinjector
: public odcore::base::module::TimeTriggeredConferenceClientModule{
 public:
  Signalinjector(int32_t const &, char **);
  Signalinjector(Signalinjector const &) = delete;
  Signalinjector &operator=(Signalinjector const &) = delete;
  virtual ~Signalinjector();

 private:
  void setUp();
  void tearDown();

  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  void ImportData(std::vector<std::pair<float,float>> &, std::string);

  bool m_initialized;
  bool m_record;
  std::ofstream m_log;
  std::vector<std::string> m_fileNames;
  int32_t m_testNumber;
  std::vector<std::pair<float,float>> m_brake;
  std::vector<std::pair<float,float>> m_steering;
  std::vector<std::pair<float,float>> m_throttle;
};

} // signalinjector
} // tools
} // opendlv

#endif
