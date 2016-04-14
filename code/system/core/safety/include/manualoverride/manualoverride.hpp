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

#ifndef MANUALOVERRIDE_MANUALOVERRIDE_HPP_
#define MANUALOVERRIDE_MANUALOVERRIDE_HPP_

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"

namespace opendlv {
namespace safety {
namespace manualoverride {

/**
 * This class provides...
 */
class ManualOverride
: public odcore::base::module::DataTriggeredConferenceClientModule {
 public:
  ManualOverride(int32_t const &, char **);
  ManualOverride(ManualOverride const &) = delete;
  ManualOverride &operator=(ManualOverride const &) = delete;
  virtual ~ManualOverride();
  virtual void nextContainer(odcore::data::Container &);

 private:
  virtual void setUp();
  virtual void tearDown();

  double m_accelerationPedalPositionThreshold;
  double m_brakePedalPositionThreshold;
  double m_torsionBarTorqueThreshold;
};

} // manualoverride
} // safety
} // opendlv

#endif
