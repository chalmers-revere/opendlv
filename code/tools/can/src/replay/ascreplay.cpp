/**
 * Copyright (C) 2016 Chalmers REVERE
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

#include <iostream>
#include <string>

#include <opendavinci/odcore/strings/StringToolbox.h>
#include <automotivedata/generated/automotive/GenericCANMessage.h>

#include "opendlvdata/GeneratedHeaders_opendlvdata.h"

#include "replay/ascreplay.hpp"

namespace opendlv {
namespace tools {
namespace can {
namespace replay {

AscReplay::AscReplay(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::TimeTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-can-ascreplay")
    , m_revereFh16CanMessageMapping()
{
}

AscReplay::~AscReplay()
{
}

void AscReplay::setUp()
{
}

void AscReplay::tearDown()
{
}

vector<odcore::data::Container> AscReplay::getMessages(
const std::string &nextLineFromASC)
{
  using namespace std;
  using namespace odcore::base;
  using namespace odcore::data;
  using namespace odcore::strings;
  using namespace automotive;

  vector<odcore::data::Container> result;

  // Structure of an ASC entry: 'Timestamp Channel  ID             Rx   d Length
  // 00 11 22 33 44 55 66 77'

  // Tokenize the read line.
  vector<string> tokens = StringToolbox::split(string(nextLineFromASC), ' ');

  // Do we have a valid line?
  if ((tokens.size() >= 7) &&
  (StringToolbox::equalsIgnoreCase(tokens[3], "rx"))) {
    // Read CAN identifier.
    stringstream _identifier;
    uint64_t identifier = 0;
    _identifier << tokens[2];
    _identifier >> hex >> identifier;

    // Read payload length (1-8).
    stringstream _length;
    uint16_t length = 0;
    _length << tokens[5];
    _length >> dec >> length;

    // Read payload.
    uint64_t data = 0;
    uint32_t i = 0;
    // Read bytes into data.
    for (; i < length; i++) {
      // Put next data byte into stringstream.
      stringstream _data;
      _data << tokens[6 + i];

      // Read next data byte.
      uint16_t value = 0;
      _data >> hex >> value;
      data |= (static_cast<uint64_t>(value) << ((length - 1 - i) * 8));
    }


    // Create GenericCANMessage from parsed data.
    GenericCANMessage gcm;
    gcm.setIdentifier(identifier);
    gcm.setLength(length);
    gcm.setData(data);

    cout << gcm.toString() << ", decoded: " << result.size() << endl;

    result = m_revereFh16CanMessageMapping.mapNext(gcm);
  }
  return result;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode AscReplay::body()
{
  using namespace std;

  char buffer[100];
  while (getModuleStateAndWaitForRemainingTimeInTimeslice() ==
  odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    // Read next line from STDIN.
    cin.getline(buffer, 100);

    vector<odcore::data::Container> result = getMessages(buffer);

    if (result.size() > 0) {
      auto it = result.begin();
      while (it != result.end()) {
        odcore::data::Container c = (*it);

        if (c.getDataType() == opendlv::proxy::reverefh16::Steering::ID()) {
          opendlv::proxy::reverefh16::Steering s =
          c.getData<opendlv::proxy::reverefh16::Steering>();
          cout << s.toString() << endl;
        }
        if (c.getDataType() == opendlv::proxy::reverefh16::VehicleState::ID()) {
          opendlv::proxy::reverefh16::VehicleState v =
          c.getData<opendlv::proxy::reverefh16::VehicleState>();
          cout << v.toString() << endl;
        }

        // Distribute data.
        getConference().send(c);

        it++;
      }
    }
  }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

} // replay
} // can
} // tools
} // opendlv
