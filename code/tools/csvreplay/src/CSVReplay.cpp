/**
 * csvreplay - Tool to replay GPS recordings from a CSV file.
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

#include <iomanip>
#include <iostream>

#include <core/data/Container.h>
#include <core/strings/StringToolbox.h>
#include <GeneratedHeaders_AutomotiveData.h>

#include <hesperia/data/environment/Point3.h>
#include <hesperia/data/environment/EgoState.h>

#include "CSVReplay.h"

namespace revere {

    using namespace std;
    using namespace core::base;
    using namespace core::data;
    using namespace core::strings;
    using namespace automotive;
    using namespace hesperia::data::environment;

    CSVReplay::CSVReplay(const int32_t &argc, char **argv) :
        TimeTriggeredConferenceClientModule(argc, argv, "csvreplay"),
        m_reference() {}

    CSVReplay::~CSVReplay() {}

    void CSVReplay::setUp() {
        const double LAT = getKeyValueConfiguration().getValue<double>("global.reference.WGS84.latitude");
        const double LON = getKeyValueConfiguration().getValue<double>("global.reference.WGS84.longitude");

        WGS84Coordinate ref;
        if (!(LAT < 0) && !(LON < 0) ) {
            // NORTH/WEST
            ref = WGS84Coordinate(LAT, WGS84Coordinate::NORTH, LON, WGS84Coordinate::WEST);
        }
        else if (!(LAT < 0) && (LON < 0) ) {
            // NORTH/EAST
            ref = WGS84Coordinate(LAT, WGS84Coordinate::NORTH, LON * -1.0, WGS84Coordinate::EAST);
        }
        else if ((LAT < 0) && !(LON < 0) ) {
            // SOUTH/WEST
            ref = WGS84Coordinate(LAT * -1.0, WGS84Coordinate::SOUTH, LON, WGS84Coordinate::WEST);
        }
        else if ((LAT < 0) && (LON < 0) ) {
            // SOUTH/EAST
            ref = WGS84Coordinate(LAT * -1.0, WGS84Coordinate::SOUTH, LON * -1.0, WGS84Coordinate::EAST);
        }
        else {
            cerr << "[csvreplay] Invalid specification of global.reference.WGS84.latitude and global.reference.WGS84.longitude." << endl;
            cerr << "[csvreplay] Expected values like global.reference.WGS84.latitude = 57.687745843 and global.reference.WGS84.longitude = -11.98219965283333." << endl;
            ref = WGS84Coordinate(57.70485804, WGS84Coordinate::NORTH, 11.93831921, WGS84Coordinate::EAST);
        }
        m_reference = ref;
        CLOG1 << "[csvreplay] Reference frame located at " << m_reference.toString() << endl;
    }

    void CSVReplay::tearDown() {}

    coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode CSVReplay::body() {
        // Structure of a GPS recording: 
        //"Date (UTC+1,00)","Time (UTC+1,00)",TimeFromStart (s),PosLat (deg),PosLon (deg),PosAlt (m),Distance (m),VelNorth (km/h),VelEast (km/h),VelDown (km/h),VelForward (km/h),VelLateral (km/h),Speed2D (km/h),AccelX (m/s<B2>),AccelY (m/s<B2>),AccelZ (m/s<B2>),AccelForward (m/s<B2>),AccelLateral (m/s<B2>),AccelDown (m/s<B2>),AngleHeading (deg),AnglePitch (deg),AngleRoll (deg),AngleSlip (deg),AngleTrack (deg),Curvature (1/m),AngleRateX (deg/s),AngleRateY (deg/s),AngleRateZ (deg/s),AngleRateForward (deg/s),AngleRateLateral (deg/s),AngleRateDown (deg/s),AngleGradient (deg)

        // Example:
        // 2015-12-18,31:43.1,0,57.7108173,11.94329264,36.553,0,11.874,11.144,0.077,16.283,-0.211,16.284,-0.453,-0.144,-9.34,-0.505,0.073,-9.338,43.927,0.321,1.331,-0.742,43.184,0.0022,0.721,-0.822,0.59,0.725,-0.836,0.567,
        const uint32_t MAX_LINE_LENGTH = 2000;
        char buffer[MAX_LINE_LENGTH];
        while (getModuleStateAndWaitForRemainingTimeInTimeslice() == coredata::dmcp::ModuleStateMessage::RUNNING) {
            // Skip some lines to fasten playback.
            for(int i = 0; i < 10; i++) {
                // Read next line from STDIN.
                cin.getline(buffer, MAX_LINE_LENGTH);
            }

            // Tokenize the read line.
            vector<string> tokens = StringToolbox::split(string(buffer), ',');

            // Do we have a valid line?
            if (tokens.size() == 32) {
                // Read latitude.
                stringstream _latInDEG;
                double latInDEG = 0;
                _latInDEG << tokens[3]; _latInDEG >> latInDEG;

                // Read longitude.
                stringstream _lonInDEG;
                double lonInDEG = 0;
                _lonInDEG << tokens[4]; _lonInDEG >> lonInDEG;

                // Create a WGS84 coordinate from given latitude and longitude.
                WGS84Coordinate coordinate(latInDEG, WGS84Coordinate::NORTH, lonInDEG, WGS84Coordinate::EAST);
                // TODO: Determine North/South & West/East settings.
//                if (!(latInDEG < 0) && !(lonInDEG < 0) ) {
//                    // NORTH/WEST
//                    coordinate = WGS84Coordinate(latInDEG, WGS84Coordinate::NORTH, lonInDEG, WGS84Coordinate::WEST);
//                }
//                else if (!(latInDEG < 0) && (lonInDEG < 0) ) {
//                    // NORTH/EAST
//                    coordinate = WGS84Coordinate(latInDEG, WGS84Coordinate::NORTH, lonInDEG * -1.0, WGS84Coordinate::EAST);
//                }
//                else if ((latInDEG < 0) && !(lonInDEG < 0) ) {
//                    // SOUTH/WEST
//                    coordinate = WGS84Coordinate(latInDEG * -1.0, WGS84Coordinate::SOUTH, lonInDEG, WGS84Coordinate::WEST);
//                }
//                else if ((latInDEG < 0) && (lonInDEG < 0) ) {
//                    // SOUTH/EAST
//                    coordinate = WGS84Coordinate(latInDEG * -1.0, WGS84Coordinate::SOUTH, lonInDEG * -1.0, WGS84Coordinate::EAST);
//                }

                // Distribute data.
                Container c(Container::WGS84COORDINATE, coordinate);
                getConference().send(c);

                // Calculate cartesian coordinate from WGS84 coordinate using specified reference frame.
                Point3 cartesianCoordinate = m_reference.transform(coordinate);

                // Adjust precision for replay.
                CLOG1 << std::fixed << std::setprecision(10);
                CLOG1 << "latInDEG = " << latInDEG << " "
                      << "lonInDEG = " << lonInDEG << " "
                      << "cartesianCoordinate " << cartesianCoordinate.toString()
                      << endl;

                // Create an EgoState from the data.
                EgoState es;
                es.setPosition(cartesianCoordinate);

                // Distribute data.
                Container c2(Container::EGOSTATE, es);
                getConference().send(c2);
            }
        }

        return coredata::dmcp::ModuleExitCodeMessage::OKAY;
    }

} // revere
