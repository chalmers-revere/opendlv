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

#include <opendavinci/odcore/data/Container.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>
#include <odcantools/CANDevice.h>
#include <fh16mapping/GeneratedHeaders_FH16Mapping.h>

#include "can/gw/canmessagedatastore.hpp"

namespace opendlv {
namespace proxy {
namespace can {
namespace gw {

CANMessageDataStore::CANMessageDataStore(
std::shared_ptr<automotive::odcantools::CANDevice> canDevice)
    : automotive::odcantools::MessageToCANDataStore(canDevice)
{
}

void CANMessageDataStore::add(const odcore::data::Container &container)
{
  using namespace automotive;
  if (container.getDataType() == GenericCANMessage::ID()) {
    // TODO: Use CAN mapping to transform a high-level message to a low-level
    // message.

//    GenericCANMessage gcm = const_cast<odcore::data::Container &>(container)
//                            .getData<GenericCANMessage>();
//    m_canDevice->write(gcm);
  }
}

} // gw
} // can
} // proxy
} // opendlv
