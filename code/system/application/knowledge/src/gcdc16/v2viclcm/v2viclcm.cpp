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

#include "opendavinci/odcore/data/Container.h"
#include "opendavinci/odcore/data/TimeStamp.h"

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "linguistics/v2vcam/buffer.hpp"
#include "gcdc16/v2viclcm/v2viclcm.hpp"

namespace opendlv {
namespace knowledge {
namespace gcdc16 {
namespace v2viclcm {

/**
  * Constructor.
  *
  * @param a_argc Number of command line arguments.
  * @param a_argv Command line arguments.
  */
V2vIclcm::V2vIclcm(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(
      a_argc, a_argv, "knowledge-gcdc16-v2viclcm")
{
}

V2vIclcm::~V2vIclcm()
{
}

/**
 * Receives .
 * Sends .
 */
void V2vIclcm::nextContainer(odcore::data::Container &c)
{
  if(c.getDataType() == opendlv::sensation::Voice::ID())
  {
    opendlv::sensation::Voice message = c.getData<opendlv::sensation::Voice>();
    // std::cout<<message.getType()<<std::endl;
    if(strcmp(message.getType().c_str(),"iclcm") == 0)
    {
      std::string dataString = message.getData();
      std::vector<unsigned char> data(dataString.begin(), dataString.end());
    
      std::shared_ptr<Buffer const> buffer(new Buffer(data));
      // std::cout<< buffer->GetSize()<<std::endl;
      std::shared_ptr<Buffer::Iterator> inIterator = buffer->GetIterator();
      //Long and little endian reverser
      inIterator->ItReversed();
      
      //Print the information 
      // std::vector<unsigned char> const bytes = v;
      // std::stringstream ss;
      // for (uint i = 0; i < bytes.size(); i++) {
      //     ss << std::to_string(bytes.at(i));
      //     ss << "|";
      // }
      // std::cout<<ss.str()<<std::endl;


      unsigned char messageId = inIterator->ReadByte();
      int32_t stationId = inIterator->ReadInteger();

      unsigned char containerMask = inIterator->ReadByte();
      int32_t rearAxleLocation = inIterator->ReadInteger();

      int32_t controllerType = inIterator->ReadInteger();
      int32_t responseTimeConstant = inIterator->ReadInteger();
      int32_t responseTimeDelay = inIterator->ReadInteger();
      int32_t targetLongAcc = inIterator->ReadInteger();

      int32_t timeHeadway = inIterator->ReadInteger();
      int32_t cruiseSpeed = inIterator->ReadInteger();
      unsigned char lowFrequencyMask = inIterator->ReadByte();
      int32_t participantsReady = inIterator->ReadInteger();

      int32_t startPlatoon = inIterator->ReadInteger();
      int32_t endOfScenario = inIterator->ReadInteger();
      int32_t mioId = inIterator->ReadInteger();
      int32_t mioRange = inIterator->ReadInteger();

      int32_t mioBearing = inIterator->ReadInteger();
      int32_t mioRangeRate = inIterator->ReadInteger();
      int32_t lane = inIterator->ReadInteger();
      unsigned char forwardId = inIterator->ReadByte();

      int32_t backwardId = inIterator->ReadInteger();
      int32_t mergeRequest = inIterator->ReadInteger();
      int32_t safeToMerge = inIterator->ReadInteger();
      int32_t flag = inIterator->ReadInteger();

      int32_t flagTail = inIterator->ReadInteger();
      int32_t flagHead = inIterator->ReadInteger();
      int32_t platoonId = inIterator->ReadInteger();
      int32_t distanceTravelledCz = inIterator->ReadInteger();
      
      int32_t intention = inIterator->ReadInteger();
      int32_t counter = inIterator->ReadInteger();


      std::string output = "*** Iclcm object ***\n";
  
      output += "Message Id: " + std::to_string(messageId) + "\n";
      output += "Container mask: " + std::to_string(containerMask) + "\n";
      output += "low frequency mask: " + std::to_string(lowFrequencyMask) + "\n";
      output += "StationId: " + std::to_string(stationId) + "\n";
      output += "Rear axle location: " + std::to_string(rearAxleLocation) + "\n";
      output += "Controller type: " + std::to_string(controllerType) + "\n";
      output += "Response time constant: " + std::to_string(responseTimeConstant) + "\n";
      output += "Response time delay: " + std::to_string(responseTimeDelay) + "\n";
      output += "Target long acc: " + std::to_string(targetLongAcc) + "\n";
      output += "Time headway: " + std::to_string(timeHeadway) + "\n";
      output += "Cruise speed: " + std::to_string(cruiseSpeed) + "\n";
      output += "Participants ready: " + std::to_string(participantsReady) + "\n";
      output += "Start platoon: " + std::to_string(startPlatoon) + "\n";
      output += "End of scenario: " + std::to_string(endOfScenario) + "\n";
      output += "Mio Id: " + std::to_string(mioId) + "\n";
      output += "Mio range: " + std::to_string(mioRange) + "\n";
      output += "Mio bearing" + std::to_string(mioBearing) + "\n";
      output += "Mio range rate: " + std::to_string(mioRangeRate) + "\n";
      output += "Lane: " + std::to_string(lane) + "\n";
      output += "Forward Id: " + std::to_string(forwardId) + "\n";
      output += "Backward Id: " + std::to_string(backwardId) + "\n";
      output += "Merge request: " + std::to_string(mergeRequest) + "\n";
      output += "Safe to merge: " + std::to_string(safeToMerge) + "\n";
      output += "Flag: " + std::to_string(flag) + "\n";
      output += "Flag tail: " + std::to_string(flagTail) + "\n";
      output += "Flag head: " + std::to_string(flagHead) + "\n";
      output += "Platoon Id: " + std::to_string(platoonId) + "\n";
      output += "Distance travelled cz: " + std::to_string(distanceTravelledCz) + "\n";
      output += "Intention: " + std::to_string(intention) + "\n";
      output += "Counter: " + std::to_string(counter) + "\n";

      std::cout<<output<<std::endl;
    }
  }
}

void V2vIclcm::setUp()
{
}

void V2vIclcm::tearDown()
{
}

} // v2viclcm
} // gcdc16
} // knowledge
} // opendlv
