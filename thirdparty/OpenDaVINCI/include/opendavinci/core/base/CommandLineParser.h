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

#ifndef OPENDAVINCI_CORE_BASE_COMMANDLINEPARSER_H_
#define OPENDAVINCI_CORE_BASE_COMMANDLINEPARSER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/CommandLineArgument.h"
#include "core/exceptions/Exceptions.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * The command line parser parses the command line used
         * to start a module. Possible command line arguments can
         * be set before parsing and accessed later.
         */
        class OPENDAVINCI_API CommandLineParser {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                CommandLineParser(const CommandLineParser&);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                CommandLineParser& operator=(const CommandLineParser &);

            public:
                CommandLineParser();

                virtual ~CommandLineParser();

                /**
                 * Adds a new argument of the form "--argument=..." to
                 * the command line parser.
                 *
                 * @param argument Argument to be added to the parser.
                 */
                void addCommandLineArgument(const string& argument);

                /**
                 * Parses the command line. The first argument is skipped
                 * because it contains the path to the executable.
                 *
                 * @param argc Number of arguments.
                 * @param argv Arguments.
                 */
                void parse(const int32_t &argc, char **argv);

                /**
                 * Returns a CommandLineArgument containing information about
                 * the specified argument.
                 *
                 * @param argument Command line argument to be queried.
                 * @return Value for the queried argument.
                 */
                const CommandLineArgument getCommandLineArgument(const string& argument) throw (exceptions::CommandLineParserException);

                /**
                 * Returns a list of CommandLineArgument containing information about
                 * the specified argument.
                 *
                 * @param argument Command line argument to be queried.
                 * @return List of values for the queried argument.
                 */
                const vector<CommandLineArgument> getCommandLineArguments(const string& argument);

            protected:
                vector<string> m_listOfArgumentsToParse;
                map<string, vector<CommandLineArgument> > m_values;
        };
    } // base
} // core::base

#endif // OPENDAVINCI_CORE_BASE_COMMANDLINEPARSER_H_
