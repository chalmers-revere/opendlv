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

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>

#include "opendavinci/odcore/base/KeyValueConfiguration.h"
#include "opendavinci/odcore/data/Container.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "toggeler/toggeler.hpp"
#include "toggeler/device.hpp"

namespace opendlv {
namespace proxy {
namespace toggeler {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Toggeler::Toggeler(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "proxy-toggeler")
    , m_device()
{
}

Toggeler::~Toggeler()
{
}

void Toggeler::nextContainer(odcore::data::Container &)
{
}

void Toggeler::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();

  std::string const type =
  kv.getValue<std::string>("proxy-toggeler.type");
  /*  std::string const port = kv.getValue<std::string>(
   *      "proxy-toggeler.port");
  */
  if (type.compare("serial") == 0) {
    //      m_device = std::unique_ptr<Device>(new SerialDevice());
  }

  if (m_device.get() == nullptr) {
    std::cerr << "[proxy-toggeler] No valid device driver defined."
              << std::endl;
  }
}

void Toggeler::tearDown()
{
}

} // toggeler
} // proxy
} // opendlv
