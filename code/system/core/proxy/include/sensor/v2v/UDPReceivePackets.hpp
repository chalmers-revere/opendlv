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

#ifndef SENSOR_V2V_UDPRECEIVEPACKETS_HPP_
#define SENSOR_V2V_UDPRECEIVEPACKETS_HPP_

#include <opendavinci/odcore/io/PacketListener.h>

// This class will handle packets received via a UDP socket.
class UDPReceivePackets : public odcore::io::PacketListener {

    // Your class needs to implement the method void void nextPacket(const odcore::io::Packet &p).
    virtual void nextPacket(const odcore::io::Packet &p);
};

#endif