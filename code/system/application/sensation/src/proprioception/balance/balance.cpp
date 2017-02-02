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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <numeric>

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"

#include "proprioception/balance/balance.hpp"

namespace opendlv {
namespace sensation {
namespace proprioception {
namespace balance {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
Balance::Balance(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "sensation-proprioception-balance")
{
}

Balance::~Balance()
{
}

/**
 * Receives .
 * Sends .
 */
void Balance::nextContainer(odcore::data::Container &)//, odcore::data::Container &a_gyroContainer)
{
  /* Broken dependency
 float *accelerometerValue;
 float gyroscopeValue = 0.0;

    if (a_accContainer.getDataType() == opendlv::proxy::AccelerometerReading::ID()) {
      opendlv::proxy::AccelerometerReading accelerometerReading =
      a_accContainer.getData<opendlv::proxy::AccelerometerReading>();



      accelerometerValue = accelerometerReading.getAcceleration();

    }


    if (a_gyroContainer.getDataType() == opendlv::proxy::AccelerometerReading::ID()) {
      opendlv::proxy::GyroscopeReading GyroscopeReading =
      a_container.getData<opendlv::proxy::GyroscopeReading>();

      std::string deviceId = GyroscopeReading.getDeviceId();

      if (deviceId != getIdentifier()) {
        return;
      }

      gyroscopeValue = GyroscopeReading.getOrientation();

    }*/



// TODO - this does not work
//    auto magnetometerReadingContainer =
//        getKeyValueDataStore().get(opendlv::proxy::MagnetometerReading::ID());
//    auto magnetometerReading = gyroscopeReadingContainer.getData<opendlv::proxy::MagnetometerReading>();


    // std::cout << getName()
    //          << "\n acceleration value " << &accelerometerValue
    //          << "\n gyroscope value " << gyroscopeValue <<  std::endl;


}

double Balance::moving_average (std::vector<double> _signal)
{
double sum = std::accumulate(_signal.begin(), _signal.end(), 0.0);
return sum/_signal.size();

}

void Balance::setUp()
{
}

void Balance::tearDown()
{
}

} // orientation
} // proprioception
} // sensation
} // opendlv
