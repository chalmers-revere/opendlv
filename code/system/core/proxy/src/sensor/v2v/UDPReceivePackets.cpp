/**
 * OpenDaVINCI - Tutorial.
 * Copyright (C) 2015 Christian Berger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <opendavinci/odcore/io/Packet.h>

#include "sensor/v2v/UDPReceivePackets.hpp"
#include "sensor/v2v/buffer.hpp"

void UDPReceivePackets::nextPacket(const odcore::io::Packet &p) {
    std::cout << "Received a packet from " << p.getSender() << ", "
         << "with " << p.getData().length() << " bytes containing '"
         << p.getData() << "'" << std::endl;
    // TODO: Forward the packet to other layer alternatively packing up data.
    const std::string packetString = p.getData();
    std::vector<unsigned char> data(packetString.begin(), packetString.end());
    std::shared_ptr<Buffer const> buffer(new Buffer(data));
    std::shared_ptr<Buffer::Iterator> iterator = buffer->GetIterator();
    //Long and little endian reverser
    iterator->ItReversed();

    char receivedMessageID = iterator->ReadByte();
    signed int receivedStationID = iterator->ReadInteger();
    std::cout << "MessageID: "<< std::to_string(receivedMessageID) << " StationId: " << receivedStationID  <<std::endl;

}

