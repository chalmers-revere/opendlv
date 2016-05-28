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

#ifndef GCDC2016_GCDC2016_HPP_
#define GCDC2016_GCDC2016_HPP_

#include <memory>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/data/Container.h>

namespace opendlv {
namespace tools {
namespace simulation {
namespace gcdc2016 {

class Vehicle;

class Gcdc2016 :
    public odcore::base::module::TimeTriggeredConferenceClientModule {
 public:
  Gcdc2016(int32_t const &, char **);
  Gcdc2016(Gcdc2016 const &) = delete;
  Gcdc2016 &operator=(Gcdc2016 const &) = delete;
  virtual ~Gcdc2016();

  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();

  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  std::unique_ptr<Vehicle> m_vehicle;
};

} // lanekeeping
} // simulation
} // tools
} // opendlv

#endif
