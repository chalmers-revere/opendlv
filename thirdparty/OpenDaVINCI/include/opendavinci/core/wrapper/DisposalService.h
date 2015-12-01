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

#ifndef OPENDAVINCI_CORE_WRAPPER_DISPOSALSERVICE_H_
#define OPENDAVINCI_CORE_WRAPPER_DISPOSALSERVICE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/SharedPointer.h"
#include "core/wrapper/Condition.h"
#include "core/wrapper/Disposable.h"
#include "core/wrapper/Mutex.h"
#include "core/wrapper/Runnable.h"
#include "core/wrapper/Thread.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * Disposal service for cleaning up.
         */
        class OPENDAVINCI_API DisposalService {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                DisposalService(const DisposalService &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                DisposalService& operator=(const DisposalService &);

            protected:
                DisposalService();

            public:
                virtual ~DisposalService();

                /**
                 * Singleton getter.
                 *
                 * @return Instance of the concrete factory.
                 */
                static DisposalService& getInstance();

                /**
                 * This method cleans up finally all existing
                 * instances and should be called inside atexit().
                 */
                void cleanUpFinally();

                /**
                 * This method adds a pointer to get deleted next time
                 * the regular cleaner is running.
                 *
                 * @param d Disposable to delete.
                 */
                void addDisposableForRegularRemoval(Disposable **d);

                /**
                 * This method adds a pointer to get deleted when
                 * closing the program.
                 *
                 * @param d Disposable to delete.
                 */
                void addDisposableForFinalRemoval(Disposable **d);

                /**
                 * This method invokes an immediate cleanup pass.
                 */
                void cleanUpImmediately();

            private:
                static auto_ptr<Mutex> m_singletonMutex;
                static DisposalService *m_singleton;

                SharedPointer<Condition> m_queueCondition;
                auto_ptr<Mutex> m_finalRemovalMutex;
                auto_ptr<Mutex> m_queueMutex;
                deque<Disposable**> m_queueForRegularRemoval;
                deque<Disposable**> m_queueForFinalRemoval;

                auto_ptr<Thread> m_thread;

                /**
                 * This class is responsible for cleaning up the queue.
                 */
                class QueueCleaner : public Runnable {
                    private:
                        /**
                         * "Forbidden" copy constructor. Goal: The compiler should warn
                         * already at compile time for unwanted bugs caused by any misuse
                         * of the copy constructor.
                         */
                        QueueCleaner(const QueueCleaner &);

                        /**
                         * "Forbidden" assignment operator. Goal: The compiler should warn
                         * already at compile time for unwanted bugs caused by any misuse
                         * of the assignment operator.
                         */
                        QueueCleaner& operator=(const QueueCleaner &);

                    public:
                        /**
                         * Constructor.
                         *
                         * @param condition Condition for getting informed about new entries in the queue.
                         * @param finalRemovalMutex Mutex for locking the final removal.
                         * @param mutex Mutex for locking the queue.
                         * @param queue Queue with entries.
                         */
                        QueueCleaner(SharedPointer<Condition> condition, Mutex &finalRemovalMutex, Mutex &mutex, deque<Disposable**> &queue);

                        virtual ~QueueCleaner();

                        /**
                         * This method controls the state of this thread.
                         *
                         * @param b True iff this thread is running.
                         */
                        void setRunning(const bool &b);

                    private:
                        SharedPointer<Condition> m_queueCondition;
                        Mutex &m_finalRemovalMutex;
                        Mutex &m_queueMutex;
                        deque<Disposable**> &m_queue;

                        auto_ptr<Mutex> m_threadStateMutex;
                        bool m_threadState;

                        virtual void run();

                        virtual bool isRunning();
                } *m_queueCleaner;
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_DISPOSALSERVICE_H_*/
