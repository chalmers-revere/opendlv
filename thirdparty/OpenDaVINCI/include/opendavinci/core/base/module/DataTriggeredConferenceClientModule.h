/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2014 - 2015 Christian Berger
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

#ifndef OPENDAVINCI_BASE_DATATRIGGEREDCONFERENCECLIENTMODULE_H_
#define OPENDAVINCI_BASE_DATATRIGGEREDCONFERENCECLIENTMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/data/Container.h"
#include "core/exceptions/Exceptions.h"

#include "core/base/module/AbstractConferenceClientModule.h"

namespace core {
    namespace base {
        namespace module {

            using namespace std;

            /**
             * This class is the base for every client module that shall
             * realize functionality in a data-triggered way. It participates
             * automatically in a UDP multicast container conference and the
             * basic runtime frequency to stop this module is 1 Hz (although,
             * it will process data according to their reception). All
             * received data is forwarded to a listening method nextContainer:
             *
             * @code
             * class MyModule: public DataTriggeredConferenceClientModule
             * {
             *     public:
             *         MyModule(const int32_t argc, char **argv) :
             *           DataTriggeredConferenceClientModule(argc, argv, "MyName")
             *         {}
             *
             *        virtual void setUp()
             *        {}
             *
             *        virtual void tearDown()
             *        {}
             *
             *        virtual void nextContainer(Container &c) {
             *            // Do something...
             *
             *            Container c2(Container::TIMESTAMP, TimeStamp());
             *            getConference().send(c2);
             *        }
             * };
             *
             * // Regular main method.
             * int32_t main(int32_t argc, char **argv) {
             *     MyModule myModule(argc, argv);
             *
             *     return myModule.runModule();
             * }
             * @endcode
             */
            class OPENDAVINCI_API DataTriggeredConferenceClientModule : public AbstractConferenceClientModule {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    DataTriggeredConferenceClientModule(const DataTriggeredConferenceClientModule& );

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    DataTriggeredConferenceClientModule& operator=(const DataTriggeredConferenceClientModule&);

                public:
                    /**
                     * Constructor.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @param name Name of this module. This parameter is necessary for identifying the corresponding parts in the configuration.
                     * @throw InvalidArgumentException if the signal handler could not be registered.
                     */
                    DataTriggeredConferenceClientModule(const int32_t &argc, char **argv, const string &name) throw (core::exceptions::InvalidArgumentException);

                    virtual ~DataTriggeredConferenceClientModule();

                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

                protected:
                    virtual void setUp() = 0;

                    virtual void tearDown() = 0;

                    virtual void nextContainer(core::data::Container &c) = 0;
            };

        }
    }
} // core::base::module

#endif /*OPENDAVINCI_BASE_DATATRIGGEREDCONFERENCECLIENTMODULE_H_*/
