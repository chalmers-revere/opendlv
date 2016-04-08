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

#ifndef REMOVELOOMINGFRONT_REMOVELOOMINGFRONT_HPP_
#define REMOVELOOMINGFRONT_REMOVELOOMINGFRONT_HPP_

#include <memory>

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/data/Container.h"

namespace opendlv {
namespace action {
namespace removeloomingfront {

/**
 * This class provides...
 */
class RemoveLoomingFront
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  RemoveLoomingFront(int32_t const &, char **);
  RemoveLoomingFront(RemoveLoomingFront const &) = delete;
  RemoveLoomingFront &operator=(RemoveLoomingFront const &) = delete;
  virtual ~RemoveLoomingFront();
  virtual void nextContainer(odcore::data::Container &);

 private:
  void setUp();
  void tearDown();
};

} // removeloomingfront
} // action
} // opendlv

#endif
