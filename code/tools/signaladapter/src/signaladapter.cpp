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

#include <bitset>
#include <iostream>
#include <vector>
#include <experimental/filesystem>
#include <limits.h>

#include <dlfcn.h>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>
#include <opendavinci/odcore/base/KeyValueConfiguration.h>
#include <opendavinci/odcore/strings/StringToolbox.h>
#include <opendavinci/odcore/io/udp/UDPFactory.h>
#include "opendavinci/odcore/reflection/Message.h"

#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData.h"
#include "odvdopendlvdata/GeneratedHeaders_ODVDOpenDLVData_Helper.h"
#include "opendavinci/GeneratedHeaders_OpenDaVINCI_Helper.h"

#include "samplebuffer.hpp"
#include "samplevisitor.hpp"
#include "signaladapter.hpp"
#include "signalstringlistener.hpp"

namespace opendlv {
namespace tools {
namespace signaladapter {

HelperEntry::HelperEntry() : 
    m_library(""),
    m_dynamicObjectHandle(nullptr),
    m_helper(nullptr)
{
}

HelperEntry::HelperEntry(HelperEntry const &a_obj) :
    m_library(a_obj.m_library),
    m_dynamicObjectHandle(a_obj.m_dynamicObjectHandle),
    m_helper(a_obj.m_helper)
{
}

HelperEntry& HelperEntry::operator=(HelperEntry const &a_obj)
{
  m_library = a_obj.m_library;
  m_dynamicObjectHandle = a_obj.m_dynamicObjectHandle;
  m_helper = a_obj.m_helper;
  return *this;
}

HelperEntry::~HelperEntry()
{
}

SignalAdapter::SignalAdapter(int32_t const &a_argc, char **a_argv)
    : odcore::base::module::DataTriggeredConferenceClientModule(
      a_argc, a_argv, "tools-signaladapter"),
      m_udpReceivers(),
      m_udpSenders(),
      m_signalStringListener(),
      m_listOfLibrariesToLoad(),
      m_listOfHelpers(),
      m_debug()
{
}

SignalAdapter::~SignalAdapter()
{
  UnloadSharedLibraries();
}

std::vector<std::string> SignalAdapter::GetListOfLibrariesToLoad(std::vector<std::string> const &a_paths) 
{
  std::vector<std::string> librariesToLoad;
  for (auto pathToSearch : a_paths) {
    try {
      for (auto &pathElement : std::experimental::filesystem::recursive_directory_iterator(pathToSearch)) {
        std::stringstream sstr;
        sstr << pathElement;
        std::string entry = sstr.str();
        if (entry.find("libodvd") != string::npos) {
          if (entry.find(".so") != string::npos) {
            std::vector<string> path = odcore::strings::StringToolbox::split(entry, '/');
            if (path.size() > 0) {
              std::string lib = path[path.size() - 1];
              if (lib.size() > 0) {
                lib = lib.substr(0, lib.size() - 1);
                librariesToLoad.push_back(lib);
              }
            }
          }
        }
      }
    } catch(...) {
    }
  }
  return librariesToLoad;
}

void SignalAdapter::FindAndLoadSharedLibraries()
{
  for (auto libraryToLoad : m_listOfLibrariesToLoad) {
    HelperEntry e;

    std::cout << "Opening '" + libraryToLoad + "'..." << std::endl;
    e.m_dynamicObjectHandle = dlopen(libraryToLoad.c_str(), RTLD_LAZY);

    if (!e.m_dynamicObjectHandle) {
      std::cerr << "Cannot open library '" + libraryToLoad + "': " << dlerror() << std::endl;
    } else {
      typedef odcore::reflection::Helper *createHelper_t();

      // reset errors
      dlerror();
      createHelper_t* getHelper = (createHelper_t*) dlsym(e.m_dynamicObjectHandle, "newHelper");
      char const *dlsym_error = dlerror();
      if (dlsym_error) {
        std::cerr << "Cannot load symbol 'newHelper' from '" + libraryToLoad + "': " << dlsym_error << std::endl;
        dlclose(e.m_dynamicObjectHandle);
      } else {
        e.m_helper = getHelper();
        e.m_library = libraryToLoad;
        m_listOfHelpers.push_back(e);
      }
    }
  }
}

void SignalAdapter::UnloadSharedLibraries() 
{
  for (auto e : m_listOfHelpers) {
    // Type to refer to the destroy method inside the shared library.
    typedef void deleteHelper_t(odcore::reflection::Helper *);

    // Reset error messages from dynamically loading shared object.
    dlerror();
    deleteHelper_t* delHelper = (deleteHelper_t*) dlsym(e.m_dynamicObjectHandle, "deleteHelper");
    char const *dlsym_error = dlerror();
    if (dlsym_error) {
      std::cerr << "Cannot load symbol 'deleteHelper': " << dlsym_error << std::endl;
    } else {
      std::cout << "Closing link to '" + e.m_library + "'" << std::endl;
      delHelper(e.m_helper);
    }
    dlclose(e.m_dynamicObjectHandle);
  }
}

void SignalAdapter::setUp()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  m_debug = (kv.getValue<int32_t>("tools-signaladapter.debug")==1);

  std::string const searchPath = kv.getValue<std::string>("tools-signaladapter.directoriesForSharedLibaries");
  std::cout << "Trying to find libodvd*.so files in: " << searchPath << std::endl;

  std::vector<std::string> const paths = odcore::strings::StringToolbox::split(searchPath, ',');
  m_listOfLibrariesToLoad = GetListOfLibrariesToLoad(paths);

  FindAndLoadSharedLibraries();

  SetUpReceivers();
  SetUpSenders();

}

void SignalAdapter::SetUpReceivers()
{
  std::string address = "0.0.0.0";

  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const messageIds = 
      kv.getValue<std::string>("tools-signaladapter.receiver.messages");
  std::string const ports =
      kv.getValue<std::string>("tools-signaladapter.receiver.ports");
  std::vector<std::string> messageIdStrings = 
      odcore::strings::StringToolbox::split(messageIds, ',');
  std::vector<std::string> portStrings = 
      odcore::strings::StringToolbox::split(ports, ',');
  if (messageIdStrings.size() != portStrings.size()) {
    // TODO: How to properly handle errors in OpenDLV?
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  m_signalStringListener = std::unique_ptr<SignalStringListener>(new SignalStringListener(getConference(), m_debug));

  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    uint16_t port = std::stoi(portStrings[i]);
    auto udpReceiver = odcore::io::udp::UDPFactory::createUDPReceiver(address, port);
    std::cout << "Receiving message '" << messageId << "' on port " << port << std::endl;

    udpReceiver->setStringListener(m_signalStringListener.get());
    udpReceiver->start();

    m_udpReceivers[messageId] = udpReceiver;
  }
}

void SignalAdapter::SetUpSenders()
{
  odcore::base::KeyValueConfiguration kv = getKeyValueConfiguration();
  std::string const address =
      kv.getValue<std::string>("tools-signaladapter.sender.address");

  std::string const messageIds = 
      kv.getValue<std::string>("tools-signaladapter.sender.messages");
  std::string const ports =
      kv.getValue<std::string>("tools-signaladapter.sender.ports");
  std::vector<std::string> messageIdStrings = 
      odcore::strings::StringToolbox::split(messageIds, ',');
  std::vector<std::string> portStrings = 
      odcore::strings::StringToolbox::split(ports, ',');
  if (messageIdStrings.size() != portStrings.size()) {
    // TODO: How to properly handle errors in OpenDLV?
    std::cerr << "Number of output messages and ports mismatch." << std::endl; 
  }
  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    uint16_t port = std::stoi(portStrings[i]);
    auto udpSender = odcore::io::udp::UDPFactory::createUDPSender(address, port);
    std::cout << "Sending message '" << messageId << "' to " << address << ":" << port << std::endl;

    m_udpSenders[messageId] = udpSender;
  }
}

void SignalAdapter::tearDown()
{
  for (auto receiver : m_udpReceivers) {
    receiver.second->stop();
  }
}


void SignalAdapter::nextContainer(odcore::data::Container &a_container)
{
  int32_t messageId = a_container.getDataType();
  bool is_served = m_udpSenders.count(messageId);
  if(m_debug) {
    std::cout << "Received container, ID:" << messageId << ", is_served:" << is_served << "." << std::endl;
  }

  if (is_served) {
    bool successfullyMapped = false;
    odcore::reflection::Message msg;

    if (!successfullyMapped) {
      msg = GeneratedHeaders_OpenDaVINCI_Helper::__map(a_container, successfullyMapped);
    }

    if (!successfullyMapped) {
      msg = GeneratedHeaders_ODVDOpenDLVData_Helper::__map(a_container, successfullyMapped);
    }

    if (!successfullyMapped) {
      for (auto e : m_listOfHelpers) {
        msg = e.m_helper->map(a_container, successfullyMapped);
        if (successfullyMapped) {
          break;
        }
      }
    }

    if (successfullyMapped) {
      std::shared_ptr<SampleBuffer> sampleBuffer(new SampleBuffer);
      SampleVisitor sampleVisitor(sampleBuffer);
      msg.accept(sampleVisitor);

      std::string data = sampleBuffer->GetDataString();
      if(m_debug) {
        std::cout << "Sending container message ID: " << messageId << ", size:" << sampleBuffer->GetSize() << "." << std::endl;
        std::cout << "Message: " << msg.toString() << std::endl;
        std::cout << "Sent message in bits:"; 
        for(std::size_t i = 0; i < data.size(); i++) {
          std::cout << std::bitset<CHAR_BIT>(data[i]) << " ";
        }
        std::cout << std::endl;
      }
      m_udpSenders[messageId]->send(data);
    }
  }
}

} // signaladapter
} // tools
} // opendlv
