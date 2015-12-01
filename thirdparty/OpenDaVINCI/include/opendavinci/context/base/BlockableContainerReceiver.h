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

#ifndef CONTEXT_BASE_BLOCKABLECONTAINERRECEIVER_H_
#define CONTEXT_BASE_BLOCKABLECONTAINERRECEIVER_H_

#include <vector>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "core/base/BufferedFIFOQueue.h"
#include "core/io/conference/ContainerListener.h"
#include "context/base/BlockableContainerListener.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class realizes a blockable container receiver which can
         * blocked from sending. This class is used for every registered
         * system under test.
         */
        class OPENDAVINCI_API BlockableContainerReceiver : public BlockableContainerListener {
            private:
                enum THREAD_YIELDING {
                    YIELDING_TIME = 100,
                };

            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                BlockableContainerReceiver(const BlockableContainerReceiver&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                BlockableContainerReceiver& operator=(const BlockableContainerReceiver&);

            public:
                /**
                 * Constructor.
                 *
                 * @param cl ContainerListener for finally receiving Containers from the System Under Test.
                 */
                BlockableContainerReceiver(core::io::conference::ContainerListener &cl);

                virtual ~BlockableContainerReceiver();

                // This method is called by ControlledContainerConference to send c from an app to all SystemParts.
                virtual void nextContainer(core::data::Container &c);

            private:
                // This ContainerListener receives the containers sent from the System Under Test to which this BlockableContainerReceiver belongs to all SystemParts and all other Systems Under Test.
                core::io::conference::ContainerListener &m_dispatcherForContainersSentFromSystemUnderTest;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_BLOCKABLECONTAINERRECEIVER_H_*/
