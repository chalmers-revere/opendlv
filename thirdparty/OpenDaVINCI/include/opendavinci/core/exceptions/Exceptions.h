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

#ifndef OPENDAVINCI_CORE_EXCEPTIONS_EXCEPTIONS_H_
#define OPENDAVINCI_CORE_EXCEPTIONS_EXCEPTIONS_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace exceptions {

        using namespace std;

        /**
         * This is the base class for all exceptions.
         * New exceptions are defined as follows:
         *
         * @code
         * OPENDAVINCI_CORE_DECLARE_EXCEPTION(IAMTHEREASONException);
         * @endcode
         */
        class OPENDAVINCI_API Exceptions: public exception {

            private:
                /**
                 * Forbidden default constructor.
                 */
                Exceptions():
                        m_message("undefined exception"),
                        m_fileName("undefined file"),
                        m_lineNumber(0),
                        m_whatMessage(NULL) {};

            public:
                /**
                 * Constructor for the exception class.
                 *
                 * The OPENDAVINCI_CORE_THROW_EXCEPTION macro uses this signature for adding file name
                 * and line number. Every exception derived from this class has to use
                 * the same signature:
                 *
                 * @code
                 * class MyException : public exceptions::Exceptions {
                 *     public:
                 *         MyException(const string &exceptionMessage, const string &fileName, const uint32_t &lineNumber) :
                 *             Exceptions(exceptionMessage, fileName, lineNumber) {};
                 * };
                 * @endcode
                 *
                 * @param exceptionMessage Message for this exception.
                 * @param fileName File name of the file where this exception occured.
                 * @param lineNumber Line number.
                 */
                Exceptions(const string &exceptionMessage, const string &fileName, const uint32_t &lineNumber);

                /**
                 * Copy constructor.
                 *
                 * @param obj Another exception.
                 */
                Exceptions(const Exceptions &obj);

                virtual ~Exceptions() throw ();

                /**
                 * Assignment operator.
                 *
                 * @param obj Another exception.
                 * @return Reference to this instance.
                 */
                Exceptions& operator=(const Exceptions &obj);

                /**
                 * This method re-implements the same interface as the standard C++ exception
                 * for overriding the that method.
                 *
                 * @return The exception's message.
                 */
                const char* what() const throw ();

                /**
                 * This method returns a string representation.
                 *
                 * @return String representation.
                 */
                const string toString() const;

                /**
                 * This method returns the name of the Exception.
                 *
                 * @return name of the exception.
                 */
                virtual const string getExceptionName() const = 0;

                /**
                 * This method returns this exception's message.
                 *
                 * @return This exception's message.
                 */
                const string getMessage() const;

                /**
                 * This method returns the file name.
                 *
                 * @return file name.
                 */
                const string getFileName() const;

                /**
                 * This method returns the line number.
                 *
                 * @return line number.
                 */
                uint32_t getLineNumber() const;

            private:
                string m_message;
                string m_fileName;
                uint32_t m_lineNumber;
                mutable char *m_whatMessage; // This attribute is mutable for allocation memory in the what()-message.
        };

        /* Exception declarations. */
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ArrayIndexOutOfBoundsException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(CommandLineParserException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ConferenceException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ConfigurationFileNotFoundException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ConnectException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ConnectionException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ConnectionAcceptorException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(InvalidArgumentException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(IOException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ModulesNotRespondingException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(NoDatabaseAvailableException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(RealtimeExecutionException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(RealtimeSetupException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(SingletonException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(SocketException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(TransformationException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ThreadControlException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ThreadException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ToManyTestSenderStarted);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(UnimplementedException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(ValueForKeyNotFoundException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(DMCPServerAlreadyStartedException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(DMCPServerNotFoundException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(DMCPServerConnectionException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(DMCPClientConfigurationException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(SCNXScenarioFileNotFoundException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(SCNScenarioVisitorException);
        OPENDAVINCI_CORE_DECLARE_EXCEPTION(SITSituationVisitorException);

    }
} // core::exceptions

#endif /*OPENDAVINCI_CORE_EXCEPTIONS_EXCEPTIONS_H_*/
