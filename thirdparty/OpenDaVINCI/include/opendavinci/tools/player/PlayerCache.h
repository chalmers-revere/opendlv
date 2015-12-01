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

#ifndef OPENDAVINCI_TOOLS_PLAYER_PLAYERCACHE_H_
#define OPENDAVINCI_TOOLS_PLAYER_PLAYERCACHE_H_

#include <deque>
#include <iostream>
#include <map>

#include "core/SharedPointer.h"
#include "core/base/Mutex.h"
#include "core/base/FIFOQueue.h"
#include "core/base/LIFOQueue.h"
#include "core/base/Service.h"
#include "core/data/Container.h"
#include "core/wrapper/SharedMemory.h"

namespace tools {
    namespace player {

        using namespace std;

        /**
         * This class caches containers from previously recorded file..
         */
        class PlayerCache : public core::base::Service {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                PlayerCache(const PlayerCache &/*obj*/);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                PlayerCache& operator=(const PlayerCache &/*obj*/);

            public:
                /**
                 * Constructor.
                 *
                 * @param size Number of elements to be cached from file.
                 * @param sizeMemorySegments Number of elements to be cached from file.
                 * @oaram autoRewind True if restart filling the queue.
                 * @param in Input stream to read data from.
                 * @param inSharedMemoryFile Input stream to read data from the shared memory dump.
                 */
                PlayerCache(const uint32_t size, const uint32_t sizeMemorySegments, const bool &autoRewind, core::SharedPointer<istream> in, core::SharedPointer<istream> inSharedMemoryFile);

                virtual ~PlayerCache();

                /**
                 * This method returns the next available container or
                 * and empty container if the queue is empty.
                 *
                 * @return Container.
                 */
                core::data::Container getNextContainer();

                /**
                 * This method returns the number of entries in the queue.
                 *
                 * @return Number of entries in the queue.
                 */
                uint32_t getNumberOfEntries() const;

                /**
                 * This method rewinds the input streams.
                 */
                void rewindInputStreams();

                /**
                 * This method clears the queue and rewinds the
                 * input streams.
                 */
                void clearQueueRewindInputStreams();

                /**
                 * This method is called to put the next shared data or shared
                 * image element into the respective shared memory.
                 *
                 * @param c Container with meta data describing the raw memory data.
                 */
                void copyMemoryToSharedMemory(core::data::Container &c);

                /**
                 * This method is used to fill the internal cache. It must
                 * be called regularly in the case that player is NOT running
                 * in threading mode (in that case, PlayerCache runs in
                 * background and does the job for you).
                 */
                void updateCache();

            private:
                virtual void beforeStop();

                virtual void run();

                /**
                 * This internal method is used to fill the internal cache
                 * without blocking. It is called by updateCache() that
                 * locks the cache's mutex.
                 */
                void updateCacheInternal();

                /**
                 * This method actually fills the cache.
                 *
                 * @return true if the buffer could be filled with one more element.
                 */
                bool fillCache();

                /**
                 * This method puts the raw memory data into the output buffer.
                 *
                 * @param c Container with meta data describing the raw memory data.
                 */
                void putRawMemoryDataIntoBuffer(core::data::Container &c);

            private:
                uint32_t m_cacheSize;
                const bool m_autoRewind;
                core::SharedPointer<istream> m_in;
                core::SharedPointer<istream> m_inSharedMemoryFile;

                core::base::FIFOQueue m_queue;
                core::base::LIFOQueue m_recBuffer;
                core::base::LIFOQueue m_memBuffer;

                map<uint32_t, char*> m_mapOfMemories;

                core::base::FIFOQueue m_bufferIn;
                core::base::FIFOQueue m_bufferOut;

                map<string, core::SharedPointer<core::wrapper::SharedMemory> > m_sharedPointers;

                core::base::Mutex m_modifyCacheMutex;
        };

    } // player
} // tools

#endif /*OPENDAVINCI_TOOLS_PLAYER_PLAYERCACHE_H_*/
