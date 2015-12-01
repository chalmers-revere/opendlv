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

#ifndef OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERLISTENER_H_
#define OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERLISTENER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/data/Container.h"

namespace core {
    namespace io {
        namespace conference {

            using namespace std;

            /**
             * This class provides an interface for getting informed
             * about new containers by an observer.
             */
            class OPENDAVINCI_API ContainerListener {
                public:
                    virtual ~ContainerListener();

                    /**
                     * This method is called whenever a new container occurs.
                     *
                     * @param c Container that has been occured.
                     */
                    virtual void nextContainer(core::data::Container &c) = 0;
            };

        }
    }
} // core::io::conference

#endif /*OPENDAVINCI_CORE_IO_CONFERENCE_CONTAINERLISTENER_H_*/
