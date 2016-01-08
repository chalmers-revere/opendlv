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

#ifndef CSVREPLAY_H_
#define CSVREPLAY_H_

#include "core/base/module/TimeTriggeredConferenceClientModule.h"
#include "hesperia/data/environment/WGS84Coordinate.h"

namespace revere {

    using namespace std;

    /**
     * This class plays back GPS recordings data from a CSV file.
     */
    class CSVReplay : public core::base::module::TimeTriggeredConferenceClientModule {
        private:
            /**
             * "Forbidden" copy constructor. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the copy constructor.
             *
             * @param obj Reference to an object of this class.
             */
            CSVReplay(const CSVReplay &/*obj*/);

            /**
             * "Forbidden" assignment operator. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the assignment operator.
             *
             * @param obj Reference to an object of this class.
             * @return Reference to this instance.
             */
            CSVReplay& operator=(const CSVReplay &/*obj*/);

        public:
            /**
             * Constructor.
             *
             * @param argc Number of command line arguments.
             * @param argv Command line arguments.
             */
            CSVReplay(const int32_t &argc, char **argv);

            virtual ~CSVReplay();

            coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

        private:
            virtual void setUp();

            virtual void tearDown();

        private:
            hesperia::data::environment::WGS84Coordinate m_reference;
    };

} // revere

#endif /*CSVREPLAY_H_*/
