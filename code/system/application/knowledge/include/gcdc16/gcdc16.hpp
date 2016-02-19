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

#ifndef GCDC16_GCDC16_HPP_
#define GCDC16_GCDC16_HPP_

#include <memory>

#include "core/base/module/DataTriggeredConferenceClientModule.h"
#include "core/data/Container.h"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {

/**
 * This class provides...
 */
class Gcdc16 : 
    public core::base::module::DataTriggeredConferenceClientModule {
  public:
    Gcdc16(int32_t const &, char **);
    Gcdc16(Gcdc16 const &) = delete;
    Gcdc16 &operator=(Gcdc16 const &) = delete;
    virtual ~Gcdc16();
    virtual void nextContainer(core::data::Container &);

  private:
    void setUp();
    void tearDown();
};

} // gcdc16
} // knowledge
} // opendlv

#endif
