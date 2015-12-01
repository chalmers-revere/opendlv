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

#ifndef OPENDAVINCI_CORE_PLATFORM_H_
#define OPENDAVINCI_CORE_PLATFORM_H_

#include "core/native.h"

#include "platform/PortableEndian.h"

/**************************************************************************************/

#ifndef WIN32
    // POSIX types.
	#include <stdint.h>

    // POSIX network.
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/select.h>
	#include <sys/types.h>

    // POSIX IPC.
	#include <pthread.h>
	#include <semaphore.h>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <sys/stat.h>

    // Other POSIX stuff.
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/time.h>

	// Use regular unlink.
	#define UNLINK unlink

#elif WIN32

	// Unlink causes an error under Windows.
	#define UNLINK _unlink
	
	#include <basetsd.h>
	#include <Winsock2.h>
	#include <ws2tcpip.h>
#endif

/**************************************************************************************/

// C++ wrapping headers for C headers.
#include <cassert>
#include <cerrno>
#include <cmath>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// C++ headers.
#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

// Basic header for OpenDaVINCI macros.
#include "core/macros.h"

// Type convention software-wide.
namespace core {
    enum TYPE_ {
        BOOL_=0,
        INTEGERS_START,
        UINT8_T,  // same as unsigned char
        INT8_T,
        UINT16_T, // same as unsigned short
        INT16_T,  // same as short
        UINT32_T,
        INT32_T,
        UINT64_T, // same as unsigned long
        INT64_T,  // same as long
        INTEGERS_END,
        CHAR_,
        UCHAR_,
        FLOAT_,
        DOUBLE_,
        /////////////////////////
        // non-primitive types //
        /////////////////////////
        NON_PRIMITIVE_START=50,
        STRING_,
        DATA_,
        SERIALIZABLE_,
        NON_PRIMITIVE_END,
        //////////////////
        // delta values //
        //////////////////
        RESERVED_=127,
        UINT8_T_R,
        INT8_T_R,
        UINT16_T_R,
        INT16_T_R,
        UINT32_T_R,
        INT32_T_R,
        UINT64_T_R,
        INT64_T_R,
        ///////////////////
        UINT8_UINT16=150,
        UINT8_UINT16_R,
        UINT8_INT16,
        UINT8_INT16_R,
        UINT8_UINT32,
        UINT8_UINT32_R,
        UINT8_INT32,
        UINT8_INT32_R,
        UINT8_UINT64,
        UINT8_UINT64_R,
        UINT8_INT64,
        UINT8_INT64_R,
        UINT8_FLOAT,
        UINT8_FLOAT_R,
        UINT8_DOUBLE,
        UINT8_DOUBLE_R,
        ////
        INT8_INT16,
        INT8_INT16_R,
        INT8_INT32,
        INT8_INT32_R,
        INT8_INT64,
        INT8_INT64_R,
        INT8_FLOAT,
        INT8_FLOAT_R,
        INT8_DOUBLE,
        INT8_DOUBLE_R,
        ////
        UINT16_UINT32,
        UINT16_UINT32_R,
        UINT16_INT32,
        UINT16_INT32_R,
        UINT16_UINT64,
        UINT16_UINT64_R,
        UINT16_INT64,
        UINT16_INT64_R,
        UINT16_FLOAT,
        UINT16_FLOAT_R,
        UINT16_DOUBLE,
        UINT16_DOUBLE_R,
        ////
        INT16_INT32,
        INT16_INT32_R,
        INT16_INT64,
        INT16_INT64_R,
        INT16_FLOAT,
        INT16_FLOAT_R,
        INT16_DOUBLE,
        INT16_DOUBLE_R,
        ////
        UINT32_UINT64,
        UINT32_UINT64_R,
        UINT32_INT64,
        UINT32_INT64_R,
        UINT32_DOUBLE,
        UINT32_DOUBLE_R,
        ////
        INT32_INT64,
        INT32_INT64_R,
        INT32_DOUBLE,
        INT32_DOUBLE_R,
        ////
        FLOAT_DOUBLE,
        FLOAT_DOUBLE_R,
        ////
        BOOL_FALSE,
        BOOL_TRUE,
        ////
        NO_OP=255
    };
}

#endif // OPENDAVINCI_CORE_PLATFORM_H_
