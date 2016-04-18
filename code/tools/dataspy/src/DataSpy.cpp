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
        if (c.getDataType() == opendlv::system::actuator::Commands::ID()) {
            opendlv::system::actuator::Commands commands = c.getData<opendlv::system::actuator::Commands>();
            cout << "[dataspy] " << commands.LongName() << ": " << commands.toString() << endl;
            return;
        }
        if (c.getDataType() == opendlv::system::sensor::TruckLocation::ID()) {
            opendlv::system::sensor::TruckLocation truckLocation = c.getData<opendlv::system::sensor::TruckLocation>();
            cout << "[dataspy] " << truckLocation.LongName() << ": " << truckLocation.toString() << endl;
            return;
        }

        cout << "[dataspy] Received container of type " << c.getDataType() << endl;
    }

} // revere
