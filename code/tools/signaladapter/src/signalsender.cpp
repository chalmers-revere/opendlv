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

#include <bitset>
#include <iostream>
#include <vector>
#include <dirent.h>
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

#include "signalsender.hpp"

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

SignalSender::SignalSender(std::string const &a_messageIds,
    std::string const &a_senderStamps,
    std::string const &a_libraryPath, bool a_debug)
  : m_messageIds(),
    m_senderStamps(),
    m_debug(a_debug),
    m_listOfLibrariesToLoad(),
    m_listOfHelpers()
{
  std::cout << "Trying to find libodvd*.so files in: " << a_libraryPath << std::endl;

  std::vector<std::string> const paths = odcore::strings::StringToolbox::split(a_libraryPath, ',');
  m_listOfLibrariesToLoad = GetListOfLibrariesToLoad(paths);

  FindAndLoadSharedLibraries();

  std::vector<std::string> messageIdStrings = 
    odcore::strings::StringToolbox::split(a_messageIds, ',');
  std::vector<std::string> senderStampStrings = 
    odcore::strings::StringToolbox::split(a_senderStamps, ',');
  for (uint16_t i = 0; i < messageIdStrings.size(); i++) {
    int32_t messageId = std::stoi(messageIdStrings[i]);
    int32_t senderStamp = std::stoi(senderStampStrings[i]);
    m_messageIds.push_back(messageId);
    m_senderStamps.push_back(senderStamp);
  }
}

SignalSender::~SignalSender()
{
  UnloadSharedLibraries();
}

std::vector<std::string> SignalSender::GetListOfLibrariesToLoad(std::vector<std::string> const &a_paths) 
{
  std::vector<std::string> librariesToLoad;
  for (auto pathToSearch : a_paths) {

 //   try {
      DIR *dir;
      struct dirent *ent;
      if ((dir = opendir(pathToSearch.c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
          bool isOdvdLib = false;
          
          std::string pathElement = ent->d_name;

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
                  isOdvdLib = true;
                }
              }
            }
          }
        
          if (m_debug) {
            std::cout << "Checking if file " << pathElement << " is ODVD library: ";
            if (isOdvdLib) {
              std::cout << "YES" << std::endl;
            } else {
              std::cout << "NO" << std::endl;
            }
          }
        
        }
        closedir(dir);
      }
 //   } catch(...) {
 //     if (m_debug) {
 //       std::cout << "NO" << std::endl;
 //     }
 //   }
  }
  return librariesToLoad;
}

void SignalSender::FindAndLoadSharedLibraries()
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

void SignalSender::UnloadSharedLibraries() 
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

void SignalSender::AddContainer(odcore::data::Container &a_container)
{
  uint32_t messageId = a_container.getDataType();
  uint32_t senderStamp = a_container.getSenderStamp();
  
  bool isServed = false;
  for (uint16_t i = 0; i < m_messageIds.size(); i++) {
    if (m_messageIds[i] == messageId && m_senderStamps[i] == senderStamp) {
      isServed = true;
      break;
    }
  }

  if (isServed) {
    bool successfullyMapped = false;
    odcore::reflection::Message msg;

    if (!successfullyMapped) {
      msg = GeneratedHeaders_OpenDaVINCI_Helper::__map(a_container,
          successfullyMapped);
    }

    if (!successfullyMapped) {
      msg = GeneratedHeaders_ODVDOpenDLVData_Helper::__map(a_container,
          successfullyMapped);
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
      AddMappedMessage(msg, senderStamp);
    }
  }
}

} // signaladapter
} // tools
} // opendlv
