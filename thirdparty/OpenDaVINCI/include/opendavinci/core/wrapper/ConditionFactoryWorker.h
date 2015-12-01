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

#ifndef OPENDAVINCI_CORE_WRAPPER_CONDITIONFACTORYWORKER_H_
#define OPENDAVINCI_CORE_WRAPPER_CONDITIONFACTORYWORKER_H_

#include "core/SharedPointer.h"
#include "core/wrapper/SystemLibraryProducts.h"

namespace core {
    namespace wrapper {

        using namespace std;

        /**
         * This template class provides factory methods to the
         * ConditionFactory. The factory methods' implementations
         * for different products have to be defined in specializations
         * of the ConditionFactoryWorker template class.
         *
         * @See ConditionFactory, ConditionFactoryWorker, SystemLibraryProducts,
         *      WIN32ConditionFactoryWorker, POSIXConditionFactoryWorker
         */
        template <SystemLibraryProducts product>
        class OPENDAVINCI_API ConditionFactoryWorker {
            public:
                /**
                 * This method returns the condition.
                 */
                static SharedPointer<Condition> createCondition();
        };

    }
} // core::wrapper

#endif /*OPENDAVINCI_CORE_WRAPPER_CONDITIONFACTORYWORKER_H_*/
