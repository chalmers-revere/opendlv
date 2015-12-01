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

#ifndef OPENDAVINCI_CORE_MACROS_H_
#define OPENDAVINCI_CORE_MACROS_H_

/* This macro eases the declaration of exceptions. */
#define OPENDAVINCI_CORE_DECLARE_EXCEPTION(ExceptionName) class OPENDAVINCI_API ExceptionName : public Exceptions { public: ExceptionName(const string &exceptionMessage, const string &fileName, const uint32_t &lineNumber) : Exceptions(exceptionMessage, fileName, lineNumber) {}; const string getExceptionName() const { return "" #ExceptionName ""; } };

/* This macro eases the usage of exceptions. */
#define OPENDAVINCI_CORE_THROW_EXCEPTION(ExceptionClass, ExceptionMessage) do { throw core::exceptions::ExceptionClass(ExceptionMessage, "", 0); } while (false)

/* This macro eases the usage of freeing a pointer. */
#define OPENDAVINCI_CORE_FREE_POINTER(ptr) do { if (ptr != NULL) { free(ptr); }; ptr = NULL; } while (false)

/* This macro eases the usage of deleting a pointer. */
#define OPENDAVINCI_CORE_DELETE_POINTER(ptr) do { if (ptr != NULL) { delete(ptr); }; ptr = NULL; } while (false)

/* This macro eases the usage of deleting an array. */
#define OPENDAVINCI_CORE_DELETE_ARRAY(ptr) do { if (ptr != NULL) { delete [] (ptr); }; ptr = NULL; } while (false)

/* This macro eases the usage of logging. */
#define CLOG if (core::base::module::AbstractCIDModule::isVerbose()) std::clog
#define CLOG1 if (core::base::module::AbstractCIDModule::getVerbosity() == 1) std::clog
#define CLOG2 if ((core::base::module::AbstractCIDModule::getVerbosity() <= 2) && (core::base::module::AbstractCIDModule::getVerbosity() >= 1)) std::clog
#define CLOG3 if ((core::base::module::AbstractCIDModule::getVerbosity() <= 3) && (core::base::module::AbstractCIDModule::getVerbosity() >= 1)) std::clog

#endif /*OPENDAVINCI_CORE_MACROS_H_*/
