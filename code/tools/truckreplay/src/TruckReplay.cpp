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

        double failure_probability = 0.00;   //this means 2% of possible failures
        double fail = 1;
        int counter_failure_lenght = 0;
        int MAX_NUMBER_OF_FAILURES = 40; // at 20hz it corresponds to 2secs
        while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

            //odcore::data::Container getPropulsionShaftVehicleSpeedData = getKeyValueDataStore().get(opendlv::proxy::reverefh16::Propulsion::ID());
            //opendlv::proxy::reverefh16::Propulsion propulsionShaftVehicleSpeed  = getPropulsionShaftVehicleSpeedData.getData<opendlv::proxy::reverefh16::Propulsion>();

            //odcore::data::Container getRoadwheelangleData = getKeyValueDataStore().get(opendlv::proxy::reverefh16::Steering::ID());
            //opendlv::proxy::reverefh16::Steering roadwheelangle = getRoadwheelangleData.getData<opendlv::proxy::reverefh16::Steering>();

            //odcore::data::Container gpsData = getKeyValueDataStore().get(opendlv::proxy::GpsReading::ID());
            //opendlv::proxy::GpsReading gpsCoordinate = gpsData.getData<opendlv::proxy::GpsReading>();

            if (fail > failure_probability)   // if we are not failing
            {
            opendlv::proxy::reverefh16::Propulsion propulsionShaftVehicleSpeedData;
            opendlv::proxy::reverefh16::Steering roadwheelangleData;
            {
                m_fileU.getline(buffer, MAX_LINE_LENGTH);
                string fromU(buffer);
                stringstream sstrFromU(fromU);
                double U_time = 0;
                double U_longitudinalVelocity = 0;
                double U_steeringAngle = 0;
                sstrFromU >> U_time >> U_longitudinalVelocity >> U_steeringAngle ;
                propulsionShaftVehicleSpeedData.setPropulsionShaftVehicleSpeed( U_longitudinalVelocity );
                roadwheelangleData.setRoadwheelangle(U_steeringAngle);
            }

            opendlv::proxy::GpsReading gpsData;
            {

                m_fileZ.getline(buffer, MAX_LINE_LENGTH);
                string fromZ(buffer);
                stringstream sstrFromZ(fromZ);
                double Z_time = 0;
                double Z_lat = 0;
                double Z_long = 0;
                double Z_yaw = 0;
                double Z_yawRate = 0;
                double Z_long_acc = 0;
                double Z_lat_acc = 0;
                sstrFromZ >> Z_time >> Z_lat >> Z_long >> Z_yaw >> Z_yawRate >> Z_long_acc >> Z_lat_acc;

                gpsData.setTimestamp(Z_time);
                gpsData.setLatitude(Z_lat);
                gpsData.setLongitude(Z_long);
                gpsData.setAltitude(Z_yawRate);  //this is just a trick I use for test purposes ! TODO: take it out !
                gpsData.setNorthHeading(Z_yaw);
                gpsData.setHasHeading(true);

                // Distribute data to other modules.
                Container c1(propulsionShaftVehicleSpeedData);
                getConference().send(c1);

                Container c2(roadwheelangleData);
                getConference().send(c2);

                Container c3(gpsData);
                getConference().send(c3);


                fail = ((double) rand() / (RAND_MAX));

            }
            }
            else  // otherwise it will just get a line from the file without filling the container
            {
                opendlv::proxy::reverefh16::Propulsion propulsionShaftVehicleSpeedData;
                opendlv::proxy::reverefh16::Steering roadwheelangleData;
                {
                    m_fileU.getline(buffer, MAX_LINE_LENGTH);
                    string fromU(buffer);
                    stringstream sstrFromU(fromU);
                    double U_time = 0;
                    double U_longitudinalVelocity = 0;
                    double U_steeringAngle = 0;
                    sstrFromU >> U_time >> U_longitudinalVelocity >> U_steeringAngle ;
                    propulsionShaftVehicleSpeedData.setPropulsionShaftVehicleSpeed( U_longitudinalVelocity );
                    roadwheelangleData.setRoadwheelangle(U_steeringAngle);
                }
                //send only the containers refering to internal can signals
                Container c1(propulsionShaftVehicleSpeedData);
                getConference().send(c1);

                Container c2(roadwheelangleData);
                getConference().send(c2);


                m_fileZ.getline(buffer, MAX_LINE_LENGTH);


                if (counter_failure_lenght < MAX_NUMBER_OF_FAILURES) counter_failure_lenght++;
                else{counter_failure_lenght = 0;
                fail = ((double) rand() / (RAND_MAX));
                }
           }


        }

        return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
    }

} // revere
