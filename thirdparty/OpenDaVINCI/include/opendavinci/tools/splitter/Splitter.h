/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2015 Christian Berger
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

#ifndef OPENDAVINCI_TOOLS_SPLITTER_SPLITTER_H_
#define OPENDAVINCI_TOOLS_SPLITTER_SPLITTER_H_

#include <string>

namespace tools {
    namespace splitter {

        using namespace std;

        /**
         * This class can be used to split a given file.
         */
        class Splitter {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Splitter(const Splitter &/*obj*/);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Splitter& operator=(const Splitter &/*obj*/);

            public:
                Splitter();

                virtual ~Splitter();

                /**
                 * This method processes the given source file and recodes it.
                 *
                 * @param source Recording file to be recoded.
                 * @param destination Output file name.
                 * @param memorySegmentSize Size of one memory segment to be used in recorder and player.
                 */
                void process(const string &source, const string &destination, const uint32_t &memorySegmentSize);

                /**
                 * This method processes the given source file and splits it between
                 * and including start and end.
                 *
                 * @param source Recording file to be split.
                 * @param memorySegmentSize Size of one memory segment to be used in recorder and player.
                 * @param start Start container to be split.
                 * @param end End container (including) in the splitting.
                 */
                void process(const string &source, const uint32_t &memorySegmentSize, const uint32_t &start, const uint32_t &end);

                /**
                 * This method processes the given source file and splits it between
                 * and including start and end.
                 *
                 * @param source Recording file to be split.
                 * @param destination Output file name.
                 * @param memorySegmentSize Size of one memory segment to be used in recorder and player.
                 * @param start Start container to be split.
                 * @param end End container (including) in the splitting.
                 */
                void process(const string &source, const string &destination, const uint32_t &memorySegmentSize, const uint32_t &start, const uint32_t &end);
        };

    } // splitter
} // tools

#endif /*OPENDAVINCI_TOOLS_SPLITTER_SPLITTER_H_*/
