/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This implementation is inspired by Yonat Sharon's
 * http://ootips.org/yonat/4dev/counted_ptr.h 
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

#ifndef OPENDAVINCI_CORE_SHAREDPOINTER_H_
#define OPENDAVINCI_CORE_SHAREDPOINTER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/wrapper/Mutex.h"
#include "core/wrapper/MutexFactory.h"

namespace core {

    using namespace std;

    /**
     * This class can be used to share pointers to dynamically allocated memory
     * that should be released automagically. Contrary to auto_ptr from STL,
     * it can be used in STL containers. It can be used as following:
     *
     * SharedPointer<TestData> p1(new TestData());
     * p1->val = 10;
     *
     * vector<SharedPointer<TestData> > listOfPtrs;
     * SharedPointer<TestData> p2(new TestData());
     * listOfPtrs.push_back(p2);
     * ...
     */
    template<class T>
    class SharedPointer {
        public:
            /**
             * Constructor.
             *
             * @param pointer Pointer to the type T.
             */
            explicit SharedPointer(T *pointer = NULL) :
                m_countablePointer(NULL) {
                if (pointer != NULL) {
                    m_countablePointer = new CountablePointer(pointer);
                }
            }

            ~SharedPointer() {
                release();
            }

            /**
             * This method returns true if this shared pointer
             * contains a valid (i.e. != NULL) pointer.
             *
             * @return true if (*this).operator->() != NULL.
             */
            bool isValid() const {
                return ( (m_countablePointer != NULL) && (m_countablePointer->m_pointer != NULL) );
            }

            /**
             * Copy constructor. If this instance gets copied the counter
             * will be incremented.
             *
             * @param obj Reference to an instance of this class.
             */
            SharedPointer(const SharedPointer &obj) throw() :
                m_countablePointer(NULL) {
                acquire(obj.m_countablePointer);
            }

            /**
             * Assignment operator. If this instance gets assigned,
             * its old referring instance is released and the new
             * one given by obj is acquired.
             *
             * @param obj Object of this class.
             * @return Reference to itself.
             */
            SharedPointer& operator=(const SharedPointer &obj) {
                if (this != &obj) {
                    // Release reference to old pointer.
                    release();

                    // Acquire a counted reference to the new one.
                    acquire(obj.m_countablePointer);
                }
                return (*this);
            }

            /**
             * Copy constructor for derived types. If this instance
             * gets copied the counter will be incremented.
             *
             * @param obj Reference to an instance of this class.
             */
            template <typename derivedClass> friend class counted_ptr;
            template <class derivedClass>
            SharedPointer(const SharedPointer<derivedClass>& obj) throw() :
                m_countablePointer(NULL) {
                acquire(reinterpret_cast<SharedPointer<T>*>( const_cast<SharedPointer<derivedClass>*>(&obj) )->m_countablePointer);
            }

            /**
             * Assignment operator for derived types. If this instance
             * gets assigned, its old referring instance is released
             * and the new one given by obj is acquired.
             *
             * @param obj Object of this class.
             * @return Reference to itself.
             */
            template <class derivedClass> SharedPointer& operator=(const SharedPointer<derivedClass>& obj) {
                if (this != &obj) {
                    release();
                    acquire(reinterpret_cast<SharedPointer<T>*>( const_cast<SharedPointer<derivedClass>*>(&obj) )->m_countablePointer);
                }
                return (*this);
            }

            /**
             * This method passes access to the actual pointer.
             *
             * @return Access to the value pointed by the contained pointer.
             */
            T& operator*() const throw() {
                return (*(m_countablePointer->m_pointer));
            }

            /**
             * This method passes access to the actual pointer.
             *
             * @return Access to the contained pointer.
             */
            T* operator->() const throw() {
                return (m_countablePointer->m_pointer);
            }

            /**
             * This method decrements the counter to the given pointer.
             * If no more references exist, the contained pointer gets freed.
             */
            void release() {
                if (m_countablePointer != NULL) {
                    if (m_countablePointer->dec() == 0) {
                        OPENDAVINCI_CORE_DELETE_POINTER(m_countablePointer);
                    }
                    // Reset own reference.
                    m_countablePointer = NULL;
                }
            }

        private:
            /**
             * This class is a simple data structure for keeping
             * and counting references to a given pointer.
             */
            class CountablePointer {
                private:
                    /** Do not allow copy constructor and assignment operator. */
                    CountablePointer(const CountablePointer&);
                    CountablePointer& operator=(const CountablePointer &);

                public:
                    /**
                     * Constructor. The references counter gets initialized by 1.
                     *
                     * @param pointer Pointer.
                     */
                    CountablePointer(T *pointer = NULL) :
                        m_pointer(pointer),
                        m_counter(1),
                        m_counterMutex(NULL) {
                        m_counterMutex = core::wrapper::MutexFactory::createMutex();
                    }

                    ~CountablePointer() {
                        OPENDAVINCI_CORE_DELETE_POINTER(m_pointer);
                        OPENDAVINCI_CORE_DELETE_POINTER(m_counterMutex);
                    }

                    /**
                     * Increment usage counter.
                     */
                    void inc() {
                        m_counterMutex->lock();
                        ++m_counter;
                        m_counterMutex->unlock();
                    }

                    /**
                     * Decrement usage counter.
                     */
                    uint32_t dec() {
                        uint32_t retVal = 0;
                        m_counterMutex->lock();
                            retVal = --m_counter;
                        m_counterMutex->unlock();
                        return retVal;
                    }

                public:
                    T* m_pointer;

                private:
                    uint32_t m_counter;
                    core::wrapper::Mutex* m_counterMutex;
            };

            // Pointer to the previously defined data structure.
            CountablePointer *m_countablePointer;

            /**
             * This method acquires a given pointer.
             *
             * @param countablePointer CountablePointer describing the references to the contained pointer.
             */
            void acquire(CountablePointer *countablePointer) throw() {
                m_countablePointer = countablePointer;
                if (m_countablePointer != NULL) {
                    m_countablePointer->inc();
                }
            }
    };
}

#endif /*OPENDAVINCI_CORE_SHAREDPOINTER_H_*/
