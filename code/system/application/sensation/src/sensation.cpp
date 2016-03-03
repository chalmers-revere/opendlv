/**
 * Copyright (C) 2015 Chalmers REVERE
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
#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

#include "sensation.hpp"


namespace opendlv {
namespace system {
namespace application {




/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Sensation::Sensation(int32_t const &a_argc, char **a_argv) :
    TimeTriggeredConferenceClientModule(a_argc, a_argv, "sensation")
{

}

void Sensation::initializeEKF()
{
       std::cout << "Hello world - I am the sensation module" << std::endl;


   opendlv::system::application::truckKinematicModel::State<float> x;
   x.setZero();

   opendlv::system::application::truckKinematicModel::Control<float> u;


    opendlv::system::application::truckKinematicModel::SystemModel<float> sys;

    std::cout << " x " << x << " u " << u << std::endl;


   opendlv::system::libs::kalman::ExtendedKalmanFilter< opendlv::system::application::truckKinematicModel::State<float> > m_ekf;



}

Sensation::~Sensation()
{
}

void Sensation::setUp()
{
  // This method will be call automatically _before_ running body().
}

void Sensation::tearDown()
{
  // This method will be call automatically _after_ return from body().
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode Sensation::body() {
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        odcore::data::Container c1 = getKeyValueDataStore().get(opendlv::system::actuator::Commands::ID());
        opendlv::system::actuator::Commands commands = c1.getData<opendlv::system::actuator::Commands>();

        odcore::data::Container c2 = getKeyValueDataStore().get(opendlv::system::sensor::TruckLocation::ID());
        opendlv::system::sensor::TruckLocation truckLocation = c2.getData<opendlv::system::sensor::TruckLocation>();

        cout << getName() << ": " << commands.toString() << ", " << truckLocation.toString() << endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // application
} // system
} // opendlv




      /*
        cout << "[perception] received container of type " << c.getDataType() <<
                                               " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() <<
                                           " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << endl;

        if (c.getDataType() == Container::USER_DATA_2) {
            revere::generic::ExampleMessageForPerception payload = c.getData<revere::generic::ExampleMessageForPerception>();
            cout << "[perception]: " << payload.toString() << endl;

            // Example: Create message revere.generic.ExampleMessageForSafety.
            revere::generic::ExampleMessageForDecision nextMessage;
            nextMessage.setData1(5.2345);

            Container c2(Container::USER_DATA_3, nextMessage);
            getConference().send(c2);
        }
        */
