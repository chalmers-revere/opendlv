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

#ifndef OPENDAVINCI_CORE_BASE_COMMANDLINEARGUMENT_H_
#define OPENDAVINCI_CORE_BASE_COMMANDLINEARGUMENT_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class represents a command line argument parsed by
         * CommandLineParser. It provides methods to access the
         * argument's name, the argument's value (as arbitrary data type)
         * and to find out whether this argument has been specified in the
         * command line.
         */
        class OPENDAVINCI_API CommandLineArgument {
            public:
                CommandLineArgument();

                /**
                 * Constructor.
                 *
                 * @param argument Commandline argument to match.
                 * @param value Value for the given argument.
                 */
                CommandLineArgument(const string& argument, const string& value, const bool &set);

                virtual ~CommandLineArgument();

                /**
                 * Returns true if this argument was specified
                 * in the command line.
                 */
                bool isSet() const;

                /**
                 * Returns the argument's name.
                 *
                 * @return The argument's name.
                 */
                const string getArgument() const;

                /**
                 * Returns the value of the command line argument
                 * as data type T
                 */
                template<class T>
                inline T getValue() {
                    T retVal;
                    stringstream ss;
                    ss << m_value;
                    ss >> retVal;
                    return retVal;
                };

            protected:
                string m_argument;
                string m_value;
                bool m_set;
        };
    }
} // core::base

#endif // OPENDAVINCI_CORE_BASE_COMMANDLINEARGUMENT_H_
