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

#ifndef OPENDAVINCI_CORE_BASE_BUFFEREDLIFOQUEUE_H_
#define OPENDAVINCI_CORE_BASE_BUFFEREDLIFOQUEUE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/LIFOQueue.h"
#include "core/exceptions/Exceptions.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This interface encapsulates all methods necessary for a buffered LIFO.
         * A buffered LIFO allows indexed access to the elements without removing
         * them.
         */
        class OPENDAVINCI_API BufferedLIFOQueue : public LIFOQueue {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                BufferedLIFOQueue(const BufferedLIFOQueue &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                BufferedLIFOQueue& operator=(const BufferedLIFOQueue &);

            public:
                /**
                 * Constructor.
                 *
                 * @param bufferSize Maximum size for this buffered lifo.
                 */
                BufferedLIFOQueue(const uint32_t &bufferSize);

                virtual ~BufferedLIFOQueue();

                virtual void push(const data::Container &container);

                /**
                 * This method returns the index of the last element.
                 *
                 * @return Index of the last element or -1 if the LIFO is empty.
                 */
                int32_t getIndexOfLastElement() const;

                /**
                 * This method returns the elements at the current index.
                 *
                 * @param index Index of the element to be retrieved.
                 * @return Element at the given index.
                 * @throws ArrayIndexOutOfBoundsException if the index is invalid.
                 */
                const data::Container getElementAt(const uint32_t &index) const throw (exceptions::ArrayIndexOutOfBoundsException);

            private:
                uint32_t m_bufferSize;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_BUFFEREDLIFOQUEUE_H_*/
