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

#ifndef CAN_CAN_HPP_
#define CAN_CAN_HPP_

#include <fstream>
#include <map>
#include <memory>

#include <opendavinci/odcore/base/FIFOQueue.h>
#include <opendavinci/odcore/base/Mutex.h>
#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include <opendavinci/odcore/reflection/CSVFromVisitableVisitor.h>
#include <odcantools/GenericCANMessageListener.h>
#include <reverefh16mapping/GeneratedHeaders_reverefh16mapping.h>

namespace automotive {
class GenericCANMessage;
}
namespace automotive {
namespace odcantools {
class CANDevice;
}
}
namespace odtools {
namespace recorder {
class Recorder;
}
}

namespace opendlv {
namespace proxy {
namespace can {

class CanMessageDataStore;

/**
 * This class opens a PEAK CAN device to record CAN messages and transform
 * them to high-level messages.
 */
class Can : public odcore::base::module::TimeTriggeredConferenceClientModule,
              public automotive::odcantools::GenericCANMessageListener {
 public:
  Can(int32_t const &, char **);
  Can(Can const &) = delete;
  Can &operator=(Can const &) = delete;
  virtual ~Can();

  virtual void nextGenericCANMessage(const automotive::GenericCANMessage &gcm);

 private:
  void setUp();
  void tearDown();
  odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

 private:
  odcore::base::FIFOQueue m_fifoGenericCanMessages;
  std::unique_ptr<odtools::recorder::Recorder> m_recorderGenericCanMessages;

  odcore::base::FIFOQueue m_fifoMappedCanMessages;
  std::unique_ptr<odtools::recorder::Recorder> m_recorderMappedCanMessages;

  std::shared_ptr<automotive::odcantools::CANDevice> m_device;
  std::unique_ptr<CanMessageDataStore> m_canMessageDataStore;

  canmapping::CanMapping m_revereFh16CanMessageMapping;

  odcore::data::TimeStamp m_startOfRecording;
  std::shared_ptr<std::fstream> m_ASCfile;
  std::map<uint32_t, std::shared_ptr<std::fstream> > m_mapOfCSVFiles;
  std::map<uint32_t, std::shared_ptr<odcore::reflection::CSVFromVisitableVisitor> > m_mapOfCSVVisitors;
};

} // can
} // proxy
} // opendlv

#endif
