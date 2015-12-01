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

#ifndef OPENDAVINCI_CORE_DATA_CONTAINER_H_
#define OPENDAVINCI_CORE_DATA_CONTAINER_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/base/Serializable.h"
#include "core/data/SerializableData.h"
#include "core/data/TimeStamp.h"

namespace core {
    namespace data {

        using namespace std;

        /**
         * Container for all interchangeable data.
         */
        class OPENDAVINCI_API Container : public core::base::Serializable {
            public:
                enum DATATYPE {
                    UNDEFINEDDATA                =  1,
                    CONFIGURATION                =  2,
                    DMCP_DISCOVER                =  3,
                    DMCP_CONNECTION_REQUEST      =  4,
                    DMCP_CONFIGURATION_REQUEST   =  5,
                    DMCP_MODULESTATEMESSAGE      =  6,
                    DMCP_MODULEEXITCODEMESSAGE   =  7,
                    MODULESTATISTICS             =  8,
                    RUNTIMESTATISTIC             =  9,
                    PLAYER_COMMAND               = 10,
                    RECORDER_COMMAND             = 11,
                    TIMESTAMP                    = 12,
                    SHARED_DATA                  = 13,
                    SHARED_IMAGE                 = 14,
                    POSITION                     = 15,
                    COMPRESSED_IMAGE             = 16,

                    CONTOUREDOBJECT              = 30,
                    CONTOUREDOBJECTS             = 31,
                    EGOSTATE                     = 32,
                    FORCECONTROL                 = 33,
                    LMS291MESSAGE                = 34,
                    OBSTACLE                     = 35,
                    OTHERVEHICLESTATE            = 36,
                    POINTSHAPEDOBJECT            = 37,
                    ROUTE                        = 38,
                    VEHICLEDATA                  = 39,
                    DRAW_LINE                    = 40,
                    VEHICLECONTROL               = 41,

                    GENERIC_CAN_MESSAGE          = 71,
                    WHEELSPEED                   = 75,

                    LOG_MESSAGE                  = 100,
                    DMCP_PULSE_MESSAGE           = 101,
                    DMCP_PULSE_ACK_MESSAGE       = 102,
                    DMCP_PULSE_ACK_CONTAINERS_MESSAGE = 103,

                    USER_BUTTON                  = 999,
                    USER_DATA_0                  = 1000,
                    USER_DATA_1                  = 1001,
                    USER_DATA_2                  = 1002,
                    USER_DATA_3                  = 1003,
                    USER_DATA_4                  = 1004,
                    USER_DATA_5                  = 1005,
                    USER_DATA_6                  = 1006,
                    USER_DATA_7                  = 1007,
                    USER_DATA_8                  = 1008,
                    USER_DATA_9                  = 1009
                };

            public:
                Container();

                /**
                 * Constructor.
                 *
                 * @param dataType Type of data inside this container.
                 * @param serializableData Data to be serialized.
                 */
                Container(const DATATYPE &dataType, const SerializableData &serializableData);

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                Container(const Container &obj);

                virtual ~Container();

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                Container& operator=(const Container &obj);

                /**
                 * This method returns the data type inside this container.
                 *
                 * @return Data type of the data contained in this container.
                 */
                DATATYPE getDataType() const;

                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);

                /**
                 * This method returns a usable object:
                 *
                 * @code
                 * Container c;
                 * ...
                 * T t = c.getData<T>();
                 * @endcode
                 *
                 * @return Usable object.
                 */
                template<class T>
                inline T getData() {
                    T containerData;
                    // Read from beginning.
                    m_serializedData.seekg(ios::beg);
                    m_serializedData >> containerData;
                    return containerData;
                }

                /**
                 * This method returns the time stamp when this
                 * container was sent.
                 *
                 * @return Time stamp when this container was sent.
                 */
                const TimeStamp getSentTimeStamp() const;

                /**
                 * This method sets the time stamp when this
                 * container was sent.
                 *
                 * @param sentTimeStamp Time stamp when this container was sent.
                 */
                void setSentTimeStamp(const TimeStamp &sentTimeStamp);

                /**
                 * This method returns the time stamp when this
                 * container was received.
                 *
                 * @return Time stamp when this container was received.
                 */
                const TimeStamp getReceivedTimeStamp() const;

                /**
                 * This method sets the time stamp when this
                 * container was received.
                 *
                 * @param receivedTimeStamp Time stamp when this container was received.
                 */
                void setReceivedTimeStamp(const TimeStamp &receivedTimeStamp);

                /**
                 * This method returns the data type as String.
                 *
                 * @return The data type.
                 */
                const string toString() const;

            private:
                DATATYPE m_dataType;
                stringstream m_serializedData;

                TimeStamp m_sent;
                TimeStamp m_received;
        };

    }
} // core::data

#endif /*OPENDAVINCI_CORE_DATA_CONTAINER_H_*/
