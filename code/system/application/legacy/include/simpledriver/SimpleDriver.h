/**
 * simpledriver is an example application to demonstrate how to 
 *              generate driving commands from an application realized
 *              with OpenDaVINCI
 * Copyright (C) 2015 Christian Berger
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

#ifndef SIMPLEDRIVER_H_
#define SIMPLEDRIVER_H_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

namespace opendlv {
namespace legacy {

using namespace std;

/**
 * This class is a skeleton to send driving commands to Hesperia-light's vehicle driving dynamics simulation.
 */
class SimpleDriver : public odcore::base::module::TimeTriggeredConferenceClientModule {
   private:
    /**
     * "Forbidden" copy constructor. Goal: The compiler should warn
     * already at compile time for unwanted bugs caused by any misuse
     * of the copy constructor.
     *
     * @param obj Reference to an object of this class.
     */
    SimpleDriver(const SimpleDriver & /*obj*/);

    /**
     * "Forbidden" assignment operator. Goal: The compiler should warn
     * already at compile time for unwanted bugs caused by any misuse
     * of the assignment operator.
     *
     * @param obj Reference to an object of this class.
     * @return Reference to this instance.
     */
    SimpleDriver &operator=(const SimpleDriver & /*obj*/);

   public:
    /**
     * Constructor.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     */
    SimpleDriver(const int32_t &argc, char **argv);

    virtual ~SimpleDriver();

    odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

   private:
    virtual void setUp();

    virtual void tearDown();
};
}
} // opendlv::legacy

#endif /*SIMPLEDRIVER_H_*/
