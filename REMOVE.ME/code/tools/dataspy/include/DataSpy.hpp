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

#ifndef DATASPY_H_
#define DATASPY_H_

#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"

namespace revere {

    using namespace std;

    /**
     * This class dumps any received container to stdout.
     */
    class DataSpy : public odcore::base::module::DataTriggeredConferenceClientModule {
        private:
            /**
             * "Forbidden" copy constructor. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the copy constructor.
             *
             * @param obj Reference to an object of this class.
             */
            DataSpy(const DataSpy &/*obj*/);

            /**
             * "Forbidden" assignment operator. Goal: The compiler should warn
             * already at compile time for unwanted bugs caused by any misuse
             * of the assignment operator.
             *
             * @param obj Reference to an object of this class.
             * @return Reference to this instance.
             */
            DataSpy& operator=(const DataSpy &/*obj*/);

        public:
            /**
             * Constructor.
             *
             * @param argc Number of command line arguments.
             * @param argv Command line arguments.
             */
            DataSpy(const int32_t &argc, char **argv);

            virtual ~DataSpy();

            void nextContainer(odcore::data::Container &c);

        private:
            virtual void setUp();

            virtual void tearDown();
    };

} // revere

#endif /*DATASPY_H_*/
