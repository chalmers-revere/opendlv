/**
 * vboreplay - Tool to replay from a VBO file.
 * Copyright (C) 2015 Chalmers ReVeRe
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

#include <iomanip>
#include <iostream>

#include <core/data/Container.h>
#include <core/strings/StringToolbox.h>
#include <GeneratedHeaders_AutomotiveData.h>

#include <hesperia/data/environment/Point3.h>
#include <hesperia/data/environment/EgoState.h>

#include "VBOReplay.h"

namespace revere {

    using namespace std;
    using namespace core::base;
    using namespace core::data;
    using namespace core::strings;
    using namespace automotive;
    using namespace hesperia::data::environment;

    VBOReplay::VBOReplay(const int32_t &argc, char **argv) :
        TimeTriggeredConferenceClientModule(argc, argv, "vboreplay"),
        m_reference() {}

    VBOReplay::~VBOReplay() {}

    void VBOReplay::setUp() {
        const string LAT = getKeyValueConfiguration().getValue<string>("global.reference.WGS84.latitude");
        const string LON = getKeyValueConfiguration().getValue<string>("global.reference.WGS84.longitude");

        stringstream sstrLAT(LAT);
        double vLAT = 0; sstrLAT >> vLAT;
        string sLAT; sstrLAT >> sLAT >> sLAT;

        stringstream sstrLON(LON);
        double vLON = 0; sstrLON >> vLON;
        string sLON; sstrLON >> sLON >> sLON;

        WGS84Coordinate ref;
        if (StringToolbox::equalsIgnoreCase(sLAT, "_north") && StringToolbox::equalsIgnoreCase(sLON, "_west")) {
            ref = WGS84Coordinate(vLAT, WGS84Coordinate::NORTH, vLON, WGS84Coordinate::WEST);
        }
        else if (StringToolbox::equalsIgnoreCase(sLAT, "_north") && StringToolbox::equalsIgnoreCase(sLON, "_east")) {
            ref = WGS84Coordinate(vLAT, WGS84Coordinate::NORTH, vLON, WGS84Coordinate::EAST);
        }
        else if (StringToolbox::equalsIgnoreCase(sLAT, "_south") && StringToolbox::equalsIgnoreCase(sLON, "_west")) {
            ref = WGS84Coordinate(vLAT, WGS84Coordinate::SOUTH, vLON, WGS84Coordinate::WEST);
        }
        else if (StringToolbox::equalsIgnoreCase(sLAT, "_south") && StringToolbox::equalsIgnoreCase(sLON, "_east")) {
            ref = WGS84Coordinate(vLAT, WGS84Coordinate::SOUTH, vLON, WGS84Coordinate::EAST);
        }
        else {
            cerr << "[vboxreplay] Invalid specification of global.reference.WGS84.latitude and global.reference.WGS84.longitude." << endl;
            cerr << "[vboxreplay] Expected values like global.reference.WGS84.latitude = 57.687745843_NORTH and global.reference.WGS84.longitude = 11.98219965283333_EAST." << endl;
        }
        m_reference = ref;

        clog << "[vboxreplay] Reference frame located at " << m_reference.toString() << endl;
    }

    void VBOReplay::tearDown() {}

    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode VBOReplay::body() {
        // Structure of a VBO recording: sats time lat long velocity heading height vert-vel Longacc Latacc VB3i_AD1 Glonass_Sats GPS_Sats IMU_Kalman_Filter_Status Solution_Type Velocity_Quality event-1 RMS_HPOS RMS_VPOS RMS_HVEL RMS_VVEL _lat _long _velocity _heading _height _vert-vel POSCov_xx POSCov_yy POSCov_zz VELCov_xx VELCov_yy VELCov_zz T1 True_Head Slip_Angle  Pitch_Ang.  Lat._Vel. Yaw_Rate Roll_Angle  Lng._Vel. Slip_COG Slip_FL Slip_FR Slip_RL Slip_RR True_Head2  YawRate X_Accel Y_Accel Temp PitchRate RollRate Z_Accel Head_imu Pitch_imu Roll_imu
        // Example:
        // 008 120043.630 +3461.26475058 -0718.93197917 005.324 327.16 +0091.88 +0000.41 +0000.13 +0000.06 +4.773634E+00 000 008 +00309 +00001 000.393 0.000000 +2.824536E+00 +3.817176E+00 +6.489671E-02 +8.770368E-02 +3461.26475058 -0718.93197917 005.324 327.16 +0091.88 +0000.41 +0.000000E+00 +0.000000E+00 +0.000000E+00 +0.000000E+00 +0.000000E+00 +0.000000E+00        -0.0002998672 +3.268784E+02 -2.857361E-01 -4.464000E+00 -2.625371E-02 +4.602051E+00 +0.000000E+00 +5.264349E+00 -2.857361E-01 -2.857361E-01 -2.857361E-01 -2.857361E-01 -2.857361E-01 +3.267798E+02 -1.155747E+01 +1.000893E-01 +9.715526E-02 +2.075000E+01 +3.347323E-01 +7.384524E-02 +9.896591E-01 +0.000000E+00 +0.000000E+00 +0.000000E+00
        const uint32_t MAX_LINE_LENGTH = 2000;
        char buffer[MAX_LINE_LENGTH];
        while (getModuleStateAndWaitForRemainingTimeInTimeslice() == coredata::dmcp::ModuleStateMessage::RUNNING) {
            // Skip some lines to fasten playback.
            for(int i = 0; i < 15; i++) {
                // Read next line from STDIN.
                cin.getline(buffer, MAX_LINE_LENGTH);
            }

            // Tokenize the read line.
            vector<string> tokens = StringToolbox::split(string(buffer), ' ');

            // Do we have a valid line?
            if ( (tokens.size() >= 30) &&
                 (!StringToolbox::equalsIgnoreCase(tokens[3], "sats")) ) { // Ignore header specification.
                // Read number of satellites.
                stringstream _numberOfSatellites;
                uint64_t numberOfSatellites = 0;
                _numberOfSatellites << tokens[0]; _numberOfSatellites >> numberOfSatellites;

                // Read time stamp.
                stringstream _timeStamp;
                double timeStamp = 0;
                _timeStamp << tokens[1]; _timeStamp >> timeStamp;

                // Read latitude in minutes.
                stringstream _latInMin;
                double latInMin = 0;
                _latInMin << tokens[2]; _latInMin >> latInMin;
                double latInDEG = latInMin / 60.0;

                // Read longitude in minutes.
                stringstream _lonInMin;
                double lonInMin = 0;
                _lonInMin << tokens[3]; _lonInMin >> lonInMin;
                double lonInDEG = lonInMin / 60.0;

                // Create a WGS84 coordinate from given latitude and longitude.
                WGS84Coordinate coordinate;
                if (!(latInDEG < 0) && !(lonInDEG < 0) ) {
                    // NORTH/WEST
                    coordinate = WGS84Coordinate(latInDEG, WGS84Coordinate::NORTH, lonInDEG, WGS84Coordinate::WEST);
                }
                else if (!(latInDEG < 0) && (lonInDEG < 0) ) {
                    // NORTH/EAST
                    coordinate = WGS84Coordinate(latInDEG, WGS84Coordinate::NORTH, lonInDEG * -1.0, WGS84Coordinate::EAST);
                }
                else if ((latInDEG < 0) && !(lonInDEG < 0) ) {
                    // SOUTH/WEST
                    coordinate = WGS84Coordinate(latInDEG * -1.0, WGS84Coordinate::SOUTH, lonInDEG, WGS84Coordinate::WEST);
                }
                else if ((latInDEG < 0) && (lonInDEG < 0) ) {
                    // SOUTH/EAST
                    coordinate = WGS84Coordinate(latInDEG * -1.0, WGS84Coordinate::SOUTH, lonInDEG * -1.0, WGS84Coordinate::EAST);
                }

                // Distribute data.
                Container c(Container::USER_DATA_0, coordinate);
                getConference().send(c);

                // Calculate cartesian coordinate from WGS84 coordinate using specified reference frame.
                Point3 cartesianCoordinate = m_reference.transform(coordinate);

                // Read heading in DEG.
                stringstream _headingInDEG;
                double headingInDEG = 0;
                _headingInDEG << tokens[4]; _headingInDEG >> headingInDEG;
                double headingInRAD = cartesian::Constants::DEG2RAD * headingInDEG;

                // Adjust precision for replay.
                CLOG1 << std::fixed << std::setprecision(10);
                CLOG1 << "numberOfSatellites = " << numberOfSatellites << " "
                      << "timeStamp = " << timeStamp << " "
                      << "latInMin = " << latInMin << " "
                      << "lonInMin = " << lonInMin << " "
                      << "latInDEG = " << latInDEG << " "
                      << "lonInDEG = " << lonInDEG << " "
                      << "headingInDEG = " << headingInDEG << " "
                      << "headingInRAD = " << headingInRAD << " "
                      << "cartesianCoordinate " << cartesianCoordinate.toString()
                      << endl;

                // Create orientation.
                Point3 orientation(1, 0, 0);
                orientation.rotateZ(90.0 * cartesian::Constants::DEG2RAD + headingInRAD);
                orientation.normalize();

                // Create an EgoState from the data.
                EgoState es;
                es.setPosition(cartesianCoordinate);
                es.setRotation(orientation);

                // Distribute data.
                Container c2(Container::EGOSTATE, es);
                getConference().send(c2);
            }
        }

        return coredata::dmcp::ModuleExitCodeMessage::OKAY;
    }

} // revere
