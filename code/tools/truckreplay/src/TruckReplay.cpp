/**
 * truckreplay - Tool to replay data recordings captured on the truck.
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
#include <sstream>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/strings/StringToolbox.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "TruckReplay.hpp"

namespace revere {

    using namespace std;
    using namespace odcore::base;
    using namespace odcore::data;
    using namespace odcore::strings;

    TruckReplay::TruckReplay(const int32_t &argc, char **argv) :
        TimeTriggeredConferenceClientModule(argc, argv, "truckreplay"),
        m_fileU(),
        m_fileZ()
    {
        // Parse command line arguments.
        if (argc == 5) {
            clog << argv[0] << " opening " << argv[3] << " and " << argv[4] << endl;
            m_fileU.open(argv[3]);
            m_fileZ.open(argv[4]);

            if (!m_fileU.good() && !m_fileZ.good()) {
                cerr << "Could not open specified files. You need to run the tools with " << argv[0] << " --cid=111 --freq=10 U_k_VSE_TEST.txt Z_k_VSE_test.txt for example." << endl;
                ::exit(-1);
            }
        }
        else {
            cerr << "No truck replay files specified. You need to run the tools with " << argv[0] << " --cid=111 --freq=10 U_k_VSE_TEST.txt Z_k_VSE_test.txt for example." << endl;
            ::exit(-1);
        }
    }

    TruckReplay::~TruckReplay() {}

    void TruckReplay::setUp() {}

    void TruckReplay::tearDown() {}

    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TruckReplay::body() {
        const uint32_t MAX_LINE_LENGTH = 2000;
        char buffer[MAX_LINE_LENGTH];

        while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
            opendlv::system::actuator::Commands commands;
            {
                m_fileU.getline(buffer, MAX_LINE_LENGTH);
                string fromU(buffer);
                stringstream sstrFromU(fromU);
                double U_time = 0;
                double U_longitudinalVelocity = 0;
                double U_steeringAngle = 0;
                sstrFromU >> U_time >> U_longitudinalVelocity >> U_steeringAngle;
                commands = opendlv::system::actuator::Commands(U_longitudinalVelocity, U_steeringAngle);
            }

            opendlv::system::sensor::TruckLocation truckLocation;
            {
                m_fileZ.getline(buffer, MAX_LINE_LENGTH);
                string fromZ(buffer);
                stringstream sstrFromZ(fromZ);
                double Z_time = 0;
                double Z_x = 0;
                double Z_y = 0;
                double Z_yaw = 0;
                double Z_yawRate = 0;
                double Z_long_acc = 0;
                double Z_lat_acc = 0;
                sstrFromZ >> Z_time >> Z_x >> Z_y >> Z_yaw >> Z_yawRate >> Z_long_acc >> Z_lat_acc;
                truckLocation = opendlv::system::sensor::TruckLocation(Z_x, Z_y, Z_yaw, Z_yawRate, Z_long_acc, Z_lat_acc);
            }

            // Distribute data to other modules.
            Container c1(commands);
            getConference().send(c1);

            Container c2(truckLocation);
            getConference().send(c2);
        }

        return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
    }

} // revere
