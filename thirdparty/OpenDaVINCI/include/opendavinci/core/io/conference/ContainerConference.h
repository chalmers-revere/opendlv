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

#ifndef OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCE_H_
#define OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Mutex.h"
#include "core/data/Container.h"
#include "core/io/conference/ContainerObserver.h"

namespace core {
    namespace io {
        namespace conference {

            using namespace std;

            class ContainerConferenceFactory;

            /**
             * This class provides an interface for a ContainerConference.
             * A ContainerConference is either a UDPMultiCastContainerConference
             * of a pure virtual conference as needed by simulation.
             */
            class OPENDAVINCI_API ContainerConference : public ContainerObserver {
                private:
                    friend class ContainerConferenceFactory;

                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    ContainerConference(const ContainerConference &);

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    ContainerConference& operator=(const ContainerConference &);

                protected:
                    ContainerConference();

                public:
                    virtual ~ContainerConference();

                    virtual void setContainerListener(ContainerListener *cl);

                    /**
                     * This methods returns the registered ContainerListener.
                     *
                     * @return registered container listener or NULL.
                     */
                    ContainerListener* getContainerListener();

                    /**
                     * This methods sends a container to this conference.
                     *
                     * @param container Container to be sent.
                     */
                    virtual void send(core::data::Container &container) const = 0;

                protected:
                    /**
                     * This method can be called from any subclass to distribute
                     * a newly received container.
                     *
                     * @param c Container to be distributed.
                     */
                    void receive(core::data::Container &c);

                    /**
                     * This method returns true if a ContainerListener is registered.
                     *
                     * @return true, iff a ContainerListener is registered.
                     */
                    bool hasContainerListener() const;

                private:
                    mutable base::Mutex m_containerListenerMutex;
                    ContainerListener *m_containerListener;
            };

        }
    }
} // core::io::conference

#endif /*OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERCONFERENCE_H_*/
