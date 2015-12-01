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

#ifndef OPENDAVINCI_CORE_DATA_TIMESTAMP_H_
#define OPENDAVINCI_CORE_DATA_TIMESTAMP_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

#include "core/data/SerializableData.h"

namespace core {
    namespace data {

        using namespace std;

        /**
         * This class can be used for time computations.
         */
        class OPENDAVINCI_API TimeStamp : public SerializableData {
            private:
                enum CUMULATIVE_DAYS {
                    January = 31,   // 31
                    February = 59,  // 28
                    March = 90,     // 31
                    April = 120,    // 30
                    May = 151,      // 31
                    June = 181,     // 30
                    July = 212,     // 31
                    August = 243,   // 31
                    September = 273,// 30
                    October = 304,  // 31
                    November = 334, // 30
                    December = 365  // 31
                };

            public:
                TimeStamp();

                /**
                 * Constructor.
                 *
                 * @param seconds Seconds.
                 * @param microseconds Microseconds.
                 */
                TimeStamp(const int32_t &seconds, const int32_t &microseconds);

                virtual ~TimeStamp();

                /**
                 * Constructor.
                 *
                 * @param ddmmyyyyhhmmss DDMMYYYYHHMMSS.
                 */
                TimeStamp(const string &ddmmyyyyhhmmss);

                /**
                 * Copy constructor.
                 *
                 * @param obj Reference to an object of this class.
                 */
                TimeStamp(const TimeStamp &obj);

                /**
                 * Assignment operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                TimeStamp& operator=(const TimeStamp &obj);

                /**
                 * Add operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                TimeStamp operator+(const TimeStamp& t) const;

                /**
                 * Subtract operator.
                 *
                 * @param obj Reference to an object of this class.
                 * @return Reference to this instance.
                 */
                TimeStamp operator-(const TimeStamp& t) const;

                bool operator==(const TimeStamp& t) const;
                bool operator!=(const TimeStamp& t) const;
                bool operator<(const TimeStamp& t) const;
                bool operator>(const TimeStamp& t) const;
                bool operator<=(const TimeStamp& t) const;
                bool operator>=(const TimeStamp& t) const;

                /**
                 * This method converts the specified time into
                 * microseconds.
                 *
                 * @return This time converted into microseconds.
                 */
                long toMicroseconds() const;

                /**
                 * This method returns the fractional microseconds
                 * to the next full second.
                 *
                 * @return microseconds.
                 */
                int32_t getFractionalMicroseconds() const;

                /**
                 * This method returns the seconds.
                 *
                 * @return seconds
                 */
                int32_t getSeconds() const;

                /**
                 * This method returns the hour for today
                 * since 01.01.1970.
                 *
                 * @return Hour for today.
                 */
                uint32_t getHour() const;

                /**
                 * This method returns the minute for today
                 * since 01.01.1970.
                 *
                 * @return Minute for today.
                 */
                uint32_t getMinute() const;

                /**
                 * This method returns the seconds for today
                 * since 01.01.1970.
                 *
                 * @return Seconds for today.
                 */
                uint32_t getSecond() const;

                /**
                 * This method returns the day for today
                 * since 01.01.1970.
                 *
                 * @return Day for today.
                 */
                uint32_t getDay() const;

                /**
                 * This method returns the minute for today
                 * since 01.01.1970.
                 *
                 * @return Month for today.
                 */
                uint32_t getMonth() const;

                /**
                 * This method returns the seconds for today
                 * since 01.01.1970.
                 *
                 * @return Year for today.
                 */
                uint32_t getYear() const;

                /**
                 * This method returns this time in the following format:
                 * YYYY-MM-DD HH:MM:SS
                 *
                 * @return Time in the given specified format.
                 */
                const string getYYYYMMDD_HHMMSS() const;

                /**
                 * This method returns this time in the following format:
                 * YYYY-MM-DD HH:MM:SS.ms
                 *
                 * @return Time in the given specified format.
                 */
                const string getYYYYMMDD_HHMMSSms() const;

                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);

                virtual const string toString() const;

            private:
                int32_t m_seconds;
                int32_t m_microseconds;

                /**
                 * This method returns true if the given year is
                 * a leap year.
                 *
                 * @param year Year.
                 * @return true iff year is a leap year.
                 */
                bool isLeapYear(const uint32_t &year) const;
        };

    }
} // core::data

#endif /*OPENDAVINCI_CORE_DATA_TIMESTAMP_H_*/
