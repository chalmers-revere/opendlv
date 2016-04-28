/**
 * dataspy - Tool to dump container content to stdout.
 * Copyright (C) 2016 Christian Berger
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/strings/StringToolbox.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "DataSpy.hpp"

namespace revere {

    using namespace std;
    using namespace odcore::base;
    using namespace odcore::data;

    DataSpy::DataSpy(const int32_t &argc, char **argv) :
        DataTriggeredConferenceClientModule(argc, argv, "dataspy")
    {}

    DataSpy::~DataSpy() {}

    void DataSpy::setUp() {}

    void DataSpy::tearDown() {}

    void DataSpy::nextContainer(odcore::data::Container &c) {
        if (c.getDataType() == opendlv::proxy::reverefh16::Propulsion::ID()) {
            opendlv::proxy::reverefh16::Propulsion data = c.getData<opendlv::proxy::reverefh16::Propulsion>();
            cout << "[dataspy] " << data.LongName() << ": " << data.toString() << endl;
            return;
        }
        if (c.getDataType() == opendlv::proxy::reverefh16::Steering::ID()) {
            opendlv::proxy::reverefh16::Steering data = c.getData<opendlv::proxy::reverefh16::Steering>();
            cout << "[dataspy] " << data.LongName() << ": " << data.toString() << endl;
            return;
        }
        if (c.getDataType() == opendlv::proxy::GpsReading::ID()) {
            opendlv::proxy::GpsReading data = c.getData<opendlv::proxy::GpsReading>();
            cout << "[dataspy] " << data.LongName() << ": " << data.toString() << endl;
            return;
        }


        cout << "[dataspy] Received container of type " << c.getDataType() << endl;
    }

} // revere
