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

#ifndef OPENDAVINCI_CORE_BASE_MODULE_MASTERMODULE_H_
#define OPENDAVINCI_CORE_BASE_MODULE_MASTERMODULE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/module/AbstractCIDModule.h"
#include "core/exceptions/Exceptions.h"

namespace core {
    namespace base {
        namespace module {

            using namespace std;

            /**
             * This class is the base for every master module. A master module
             * masters the configuration for every module as well as the the
             * overall state machines. For using this class one has to override
             * the method "virtual int32_t body();":
             *
             * @code
             * class MyModule: public MasterModule
             * {
             *     public:
             *         MyModule(const int32_t argc, char **argv) :
             *           MasterModule(argc, argv)
             *         {}
             *
             *        int32_t body() {
             *            // Do something.
             *            return OKAY;
             *        }
             * };
             *
             * // Regular main method.
             * int32_t main(int32_t argc, char **argv) {
             *     MyModule myModule(argc, argv);
             *
             *     return myModule.runModule();
             * }
             * @endcode
             */
            class OPENDAVINCI_API MasterModule : public AbstractCIDModule {
                private:
                    /**
                     * "Forbidden" copy constructor. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the copy constructor.
                     */
                    MasterModule(const MasterModule& );

                    /**
                     * "Forbidden" assignment operator. Goal: The compiler should warn
                     * already at compile time for unwanted bugs caused by any misuse
                     * of the assignment operator.
                     */
                    MasterModule& operator=(const MasterModule& );

                public:
                    /**
                     * Constructor.
                     *
                     * @param argc Number of command line arguments.
                     * @param argv Command line arguments.
                     * @throw InvalidArgumentException if the signal handler could not be registered.
                     */
                    MasterModule(const int32_t &argc, char **argv) throw (exceptions::InvalidArgumentException);

                    virtual ~MasterModule();

                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode runModule();

                protected:
                    virtual coredata::dmcp::ModuleExitCodeMessage::ModuleExitCode body() = 0;
            };

        }
    }
} // core::base::module

#endif /*OPENDAVINCI_CORE_BASE_MODULE_MASTERMODULE_H_*/
