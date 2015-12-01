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

#ifndef OPENDAVINCI_TOOLS_RECORDER_SHAREDDATALISTENER_H_
#define OPENDAVINCI_TOOLS_RECORDER_SHAREDDATALISTENER_H_

#include <map>
#include <string>

#include "core/SharedPointer.h"
#include "core/base/module/TimeTriggeredConferenceClientModule.h"
#include "core/base/FIFOQueue.h"
#include "core/data/Container.h"
#include "core/data/SerializableData.h"
#include "core/wrapper/SharedMemory.h"

#include "tools/recorder/SharedDataWriter.h"

#include "GeneratedHeaders_CoreData.h"

namespace tools {
    namespace recorder {

        using namespace std;

        /**
         * This class encapsulates a listener for SharedData containers.
         */
        class SharedDataListener : public core::base::AbstractDataStore {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                SharedDataListener(const SharedDataListener &/*obj*/);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                SharedDataListener& operator=(const SharedDataListener &/*obj*/);

            public:
                /**
                 * Constructor.
                 *
                 * @param out Stream to write data to.
                 * @param memorySegmentSize Size of one memory segment.
                 * @param numberOfMemorySegments Number of available memory segments.
                 * @param threading Cf. constructor of Recorder.
                 */
                SharedDataListener(core::SharedPointer<ostream> out, const uint32_t &memorySegmentSize, const uint32_t &numberOfMemorySegments, const bool &threading);

                virtual ~SharedDataListener();

                virtual void add(const core::data::Container &container);

                virtual void clear();

                virtual uint32_t getSize() const;

                virtual bool isEmpty() const;

            private:
                /**
                 * This method copies the data pointed to by SharedData
                 * or SharedImage to the next available MemorySegment.
                 *
                 * @param name Name of SharedPointer to be used.
                 * @param header Container that contains the meta-data for this shared memory segment which shall be used as header in the file.
                 * @return true if the copy succeeded.
                 */
                bool copySharedMemoryToMemorySegment(const string &name, const core::data::Container &header);

            private:
                bool m_threading;
                auto_ptr<SharedDataWriter> m_sharedDataWriter;
                map<string, coredata::SharedData> m_mapOfAvailableSharedData;
                map<string, coredata::image::SharedImage> m_mapOfAvailableSharedImages;

                map<uint32_t, char*> m_mapOfMemories;

                core::base::FIFOQueue m_bufferIn;
                core::base::FIFOQueue m_bufferOut;

                uint32_t m_droppedSharedMemories;

                map<string, core::SharedPointer<core::wrapper::SharedMemory> > m_sharedPointers;

                core::SharedPointer<ostream> m_out;
        };

    } // recorder
} // tools

#endif /*OPENDAVINCI_TOOLS_RECORDER_SHAREDDATALISTENER_H_*/
