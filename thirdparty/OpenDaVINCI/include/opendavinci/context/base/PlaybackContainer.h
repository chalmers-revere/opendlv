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

#ifndef CONTEXT_BASE_PLAYBACKCONTAINER_H_
#define CONTEXT_BASE_PLAYBACKCONTAINER_H_

#include <string>

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "context/base/SystemFeedbackComponent.h"
#include "tools/player/Player.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class can be used to playback all recorded Containers.
         */
        class OPENDAVINCI_API PlaybackContainer : public SystemFeedbackComponent {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
				PlaybackContainer(const PlaybackContainer&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
				PlaybackContainer& operator=(const PlaybackContainer&);

            public:
                /**
                 * Constructor.
                 *
                 * @param freq Frequency to be used.
                 * @param url URL of the file to be used for storing containers.
                 */
				PlaybackContainer(const float &freq, const string &urlFileName, const uint32_t &memorySegmentSize, const uint32_t &numberOfSegments);

				virtual ~PlaybackContainer();

                virtual void setup();

                virtual void tearDown();

                virtual void step(const core::wrapper::Time &t, SendContainerToSystemsUnderTest &sender);

            private:
                virtual float getFrequency() const;

            private:
                float m_freq;
                string m_urlFileName;
                uint32_t m_memorySegmentSize;
                uint32_t m_numberOfSegments;
                auto_ptr<tools::player::Player> m_player;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_PLAYBACKCONTAINER_H_*/
