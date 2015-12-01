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

#ifndef CONTEXT_BASE_COMMANDLINEINTERFACE_H_
#define CONTEXT_BASE_COMMANDLINEINTERFACE_H_

// native.h must be included as first header file for definition of _WIN32_WINNT.
#include "core/native.h"
#include "context/base/RuntimeControlInterface.h"

namespace context {
    namespace base {

        using namespace std;

        /**
         * This class provides access to the RuntimeControl using the CLI.
         */
        class OPENDAVINCI_API CommandLineInterface : public RuntimeControlInterface {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                CommandLineInterface(const CommandLineInterface&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                CommandLineInterface& operator=(const CommandLineInterface&);

            public:
                CommandLineInterface();

                virtual ~CommandLineInterface();

                /**
                 * This method parses the commandline to get necessary information.
                 *
                 * @param argc Number of command line arguments.
                 * @param argv Commandline arguments.
                 * @return true if the arguments provided by the commandline could be successfully parsed.
                 */
                bool parse(const int32_t &argc, char **argv);

                virtual const core::base::KeyValueConfiguration getConfiguration() const;

                virtual const string getMulticastGroup() const;

                virtual uint32_t getCID() const;

                virtual bool isVerbose() const;

                virtual bool isSupercomponent() const;

            private:
                core::base::KeyValueConfiguration m_configuration;
                string m_multicastGroup;
                uint32_t m_CID;
                bool m_isVerbose;
                bool m_isSupercomponent;
        };

    }
} // context::base

#endif /*CONTEXT_BASE_COMMANDLINEINTERFACE_H_*/
