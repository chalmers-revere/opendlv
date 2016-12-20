/**
 * Copyright (C) 2016 Chalmers Revere
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

#ifndef SIGNALADAPTER_SIGNALSENDER_HPP_
#define SIGNALADAPTER_SIGNALSENDER_HPP_

#include <map>
#include <memory>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/io/udp/UDPSender.h>
#include <opendavinci/odcore/reflection/Helper.h>

namespace opendlv {
namespace tools {
namespace signaladapter {

class HelperEntry {
 public:
  HelperEntry();
  HelperEntry(HelperEntry const &);
  HelperEntry &operator=(HelperEntry const &);
  virtual ~HelperEntry();

  std::string m_library;
  void *m_dynamicObjectHandle;
  odcore::reflection::Helper *m_helper;
};

class SignalSender {
 public:
  SignalSender(std::string const &, std::string const &, bool);
  SignalSender(SignalSender const &) = delete;
  SignalSender &operator=(SignalSender const &) = delete;
  virtual ~SignalSender();

  void AddContainer(odcore::data::Container &);
  virtual void SendMessage(odcore::reflection::Message &) = 0;

 protected:
  std::vector<int32_t> m_messageIds;
  bool m_debug;

 private:
  void FindAndLoadSharedLibraries();
  void UnloadSharedLibraries();
  std::vector<std::string> GetListOfLibrariesToLoad(std::vector<std::string> const &);

  std::vector<std::string> m_listOfLibrariesToLoad;
  std::vector<HelperEntry> m_listOfHelpers;
};

} // signaladapter
} // tools
} // opendlv

#endif
