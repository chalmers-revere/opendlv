/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
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

#ifndef OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCEFACTORY_H_
#define OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCEFACTORY_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/io/conference/ContainerConference.h"

namespace core {
    namespace io {
        namespace conference {

            using namespace std;

            /**
             * This class provides ContainerConferences.
             */
            class OPENDAVINCI_API ContainerConferenceFactory {
                public:
                    enum {
                        MULTICAST_PORT = 12175 // Mariposa Rd, Victorville.
                    };

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ContainerConferenceFactory(const ContainerConferenceFactory &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ContainerConferenceFactory& operator=(const ContainerConferenceFactory &);

                protected:
                    /**
                     * The default constructor is protected to allow subclasses
                     * (e.g. in testcases).
                     */
                    ContainerConferenceFactory();

                public:
                    virtual ~ContainerConferenceFactory();

                    /**
                     * This method returns a static instance for this factory.
                     *
                     * @return Instance of this factory.
                     */
                    static ContainerConferenceFactory& getInstance();

                    /**
                     * This method returns a new ContainerConference.
                     *
                     * @param address Use address for joining.
                     * @param port Use port for joining.  If omitted, MULTICAST_PORT will be used.
                     * @return ContainerConference or NULL.
                     */
                    virtual core::SharedPointer<ContainerConference> getContainerConference(const string &address, const uint32_t &port = ContainerConferenceFactory::MULTICAST_PORT);

                protected:
                    /**
                     * This method sets the singleton pointer.
                     *
                     * @param singleton Singleton to be used.
                     */
                    static void setSingleton(ContainerConferenceFactory* singleton);

                private:
                    static base::Mutex m_singletonMutex;
                    static ContainerConferenceFactory* m_singleton;
            };

        }
    }
} // core::io::conference

#endif /*OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCEFACTORY_H_*/
