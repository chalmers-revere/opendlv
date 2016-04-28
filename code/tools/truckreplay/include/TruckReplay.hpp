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

#ifndef TRUCKREPLAY_H_
#define TRUCKREPLAY_H_

#include <fstream>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

namespace revere {

    using namespace std;

    /**
     * This class plays back data from a truck recording file.
     */
    class TruckReplay : public odcore::base::module::TimeTriggeredConferenceClientModule {
        private:
            /**
             * "Forbidden" copy constructor. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the copy constructor.
             *
             * @param obj Reference to an object of this class.
             */
            TruckReplay(const TruckReplay &/*obj*/);

            /**
             * "Forbidden" assignment operator. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the assignment operator.
             *
             * @param obj Reference to an object of this class.
             * @return Reference to this instance.
             */
            TruckReplay& operator=(const TruckReplay &/*obj*/);

        public:
            /**
             * Constructor.
             *
             * @param argc Number of command line arguments.
             * @param argv Command line arguments.
             */
            TruckReplay(const int32_t &argc, char **argv);

            virtual ~TruckReplay();

            odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

        private:
            virtual void setUp();

            virtual void tearDown();

        private:
            ifstream m_fileU;
            ifstream m_fileZ;
    };

} // revere

#endif /*TRUCKREPLAY_H_*/
