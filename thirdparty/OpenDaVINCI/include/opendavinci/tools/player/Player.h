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

#ifndef OPENDAVINCI_TOOLS_PLAYER_PLAYER_H_
#define OPENDAVINCI_TOOLS_PLAYER_PLAYER_H_

#include <iostream>

#include "core/SharedPointer.h"
#include "core/data/Container.h"
#include "core/io/URL.h"

#include "tools/player/PlayerCache.h"

namespace tools {
    namespace player {

        using namespace std;

        /**
         * This class can be used to replay previously recorded
         * data using a conference for distribution.
         */
        class Player {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Player(const Player &/*obj*/);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Player& operator=(const Player &/*obj*/);

            public:
                /**
                 * Constructor.
                 *
                 * @param url Resource to play.
                 * @param autoRewind True if the file should be rewind at EOF.
                 * @param memorySegmentSize Size of the memory segment to be used for buffering.
                 * @param numberOfMemorySegments Number of memory segments to be used for buffering.
                 * @param threading If set to true, player will load new containers from the file in background.
                 */
                Player(const core::io::URL &url, const bool &autoRewind, const uint32_t &memorySegmentSize, const uint32_t &numberOfMemorySegments, const bool &threading);

                virtual ~Player();

                /**
                 * This method returns the next container to be replayed.
                 *
                 * @return Next container to be replayed.
                 */
                core::data::Container getNextContainerToBeSent();

                /**
                 * This method returns the delay to be waited before the next container should be delivered.
                 *
                 * @return delay to the next container in real time microseconds (us).
                 */
                uint32_t getDelay() const;

                /**
                 * This method restarts the player.
                 */
                void rewind();

                /**
                 * This method returns true if there is more data to replay.
                 *
                 * @return true if there is more data to replay.
                 */
                bool hasMoreData() const;

            private:
                bool m_threading;
                bool m_autoRewind;

                core::SharedPointer<istream> m_inFile;
                core::SharedPointer<istream> m_inSharedMemoryFile;

                auto_ptr<PlayerCache> m_playerCache;

                // The "actual" container contains the data to be sent, ...
                core::data::Container m_actual;
                // ... whereas the "successor" container contains the data that follows the actual one.
                core::data::Container m_successor;

                // This flag indicates if new data has to be read from the stream.
                bool m_successorProcessed;

                // This flag indicates, that we have to seek to the beginning of the stream and read the "actual" container.
                bool m_seekToTheBeginning;

                // This flag indicates that there is no more data to cache.
                bool m_noMoreData;

                uint32_t m_delay;
        };

    } // player
} // tools

#endif /*OPENDAVINCI_TOOLS_PLAYER_PLAYER_H_*/
