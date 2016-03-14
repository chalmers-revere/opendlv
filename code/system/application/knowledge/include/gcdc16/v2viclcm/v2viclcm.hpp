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

#ifndef GCDC16_V2VICLCM_V2VICLCM_HPP_
#define GCDC16_V2VICLCM_V2VICLCM_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace v2viclcm {

/**
 * This class provides...
 */
class V2vIclcm
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  V2vIclcm(int32_t const &, char **);
  V2vIclcm(V2vIclcm const &) = delete;
  V2vIclcm &operator=(V2vIclcm const &) = delete;
  virtual ~V2vIclcm();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
};

} // v2viclcm
} // gcdc16
} // knowledge
} // opendlv

#endif
