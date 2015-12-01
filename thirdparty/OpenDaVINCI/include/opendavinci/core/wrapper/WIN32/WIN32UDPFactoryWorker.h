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

#ifndef OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32UDPFACTORYWORKER_H_
#define OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32UDPFACTORYWORKER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/module/AbstractCIDModule.h"
#include "core/wrapper/NetworkLibraryProducts.h"
#include "core/wrapper/UDPFactoryWorker.h"
#include "core/wrapper/WIN32/WIN32UDPReceiver.h"
#include "core/wrapper/WIN32/WIN32UDPSender.h"

namespace core {
    namespace wrapper {

        using namespace std;

        template <> class OPENDAVINCI_API UDPFactoryWorker<NetworkLibraryWin32> {
            public:
                static core::io::udp::UDPSender* createUDPSender(const string &address, const uint32_t &port) {
                    return new WIN32Impl::WIN32UDPSender(address, port);
                };

                static core::io::udp::UDPReceiver* createUDPReceiver(const string &address, const uint32_t &port) {
                    bool isMulticast = false;
                    string::size_type posFirstDot = address.find(".");
                    if (posFirstDot != string::npos) {
                        stringstream numericalValueStream(address.substr(0, posFirstDot));
                        uint32_t numericalValue = 0;
                        numericalValueStream >> numericalValue;
                        isMulticast = ( (numericalValue >= 224) && (numericalValue <= 239) );
                        CLOG3 << "Creating " << (isMulticast ? "multicast" : "regular") << " UDP receiver at " << address << ":" << port << "." << endl;
                    }
                    return new WIN32Impl::WIN32UDPReceiver(address, port, isMulticast);
                };
        };
    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_WIN32IMPL_WIN32UDPFACTORYWORKER_H_*/
