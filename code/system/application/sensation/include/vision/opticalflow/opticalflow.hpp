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

#ifndef VISION_OPTICALFLOW_OPTICALFLOW_HPP_
#define VISION_OPTICALFLOW_OPTICALFLOW_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace sensation {
namespace vision {
namespace opticalflow {

/**
 * This class provides...
 */
class OpticalFlow
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  OpticalFlow(int32_t const &, char **);
  OpticalFlow(OpticalFlow const &) = delete;
  OpticalFlow &operator=(OpticalFlow const &) = delete;
  virtual ~OpticalFlow();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
};

} // opticalflow
} // vision
} // sensation
} // opendlv

#endif
