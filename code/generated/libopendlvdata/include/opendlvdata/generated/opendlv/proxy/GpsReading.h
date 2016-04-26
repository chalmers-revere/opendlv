/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef OPENDLV_PROXY_GPSREADING_H
#define OPENDLV_PROXY_GPSREADING_H

#include "opendavinci/odcore/opendavinci.h"


#include "opendavinci/odcore/base/Visitable.h"
#include "opendavinci/odcore/data/SerializableData.h"


namespace opendlv {
	namespace proxy {
		using namespace std;
		
		class GpsReading : public odcore::data::SerializableData, public odcore::base::Visitable {
			public:
				GpsReading();
		
				GpsReading(
				const double &val0, 
				const double &val1, 
				const double &val2, 
				const float &val3, 
				const float &val4, 
				const float &val5, 
				const uint8_t &val6, 
				const uint8_t &val7, 
				const uint8_t &val8, 
				const bool &val9, 
				const bool &val10
				);
		
				virtual ~GpsReading();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				GpsReading(const GpsReading &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				GpsReading& operator=(const GpsReading &obj);
		
			public:
				/**
				 * @return timestamp.
				 */
				double getTimestamp() const;
				
				/**
				 * This method sets timestamp.
				 *
				 * @param val Value for timestamp.
				 */
				void setTimestamp(const double &val);
			public:
				/**
				 * @return latitude.
				 */
				double getLatitude() const;
				
				/**
				 * This method sets latitude.
				 *
				 * @param val Value for latitude.
				 */
				void setLatitude(const double &val);
			public:
				/**
				 * @return longitude.
				 */
				double getLongitude() const;
				
				/**
				 * This method sets longitude.
				 *
				 * @param val Value for longitude.
				 */
				void setLongitude(const double &val);
			public:
				/**
				 * @return altitude.
				 */
				float getAltitude() const;
				
				/**
				 * This method sets altitude.
				 *
				 * @param val Value for altitude.
				 */
				void setAltitude(const float &val);
			public:
				/**
				 * @return northHeading.
				 */
				float getNorthHeading() const;
				
				/**
				 * This method sets northHeading.
				 *
				 * @param val Value for northHeading.
				 */
				void setNorthHeading(const float &val);
			public:
				/**
				 * @return speed.
				 */
				float getSpeed() const;
				
				/**
				 * This method sets speed.
				 *
				 * @param val Value for speed.
				 */
				void setSpeed(const float &val);
			public:
				/**
				 * @return latitudeDirection.
				 */
				uint8_t getLatitudeDirection() const;
				
				/**
				 * This method sets latitudeDirection.
				 *
				 * @param val Value for latitudeDirection.
				 */
				void setLatitudeDirection(const uint8_t &val);
			public:
				/**
				 * @return longitudeDirection.
				 */
				uint8_t getLongitudeDirection() const;
				
				/**
				 * This method sets longitudeDirection.
				 *
				 * @param val Value for longitudeDirection.
				 */
				void setLongitudeDirection(const uint8_t &val);
			public:
				/**
				 * @return satelliteCount.
				 */
				uint8_t getSatelliteCount() const;
				
				/**
				 * This method sets satelliteCount.
				 *
				 * @param val Value for satelliteCount.
				 */
				void setSatelliteCount(const uint8_t &val);
			public:
				/**
				 * @return hasHeading.
				 */
				bool getHasHeading() const;
				
				/**
				 * This method sets hasHeading.
				 *
				 * @param val Value for hasHeading.
				 */
				void setHasHeading(const bool &val);
			public:
				/**
				 * @return hasRtk.
				 */
				bool getHasRtk() const;
				
				/**
				 * This method sets hasRtk.
				 *
				 * @param val Value for hasRtk.
				 */
				void setHasRtk(const bool &val);
		
			public:
				/**
				 * This method returns the message id.
				 *
				 * @return Message id.
				 */
				static int32_t ID();
		
				/**
				 * This method returns the short message name.
				 *
				 * @return Short message name.
				 */
				static const string ShortName();
		
				/**
				 * This method returns the long message name include package/sub structure.
				 *
				 * @return Long message name.
				 */
				static const string LongName();
		
				/**
				 * This method returns the message id.
				 *
				 * @return Message id.
				 */
				virtual int32_t getID() const;
		
				/**
				 * This method returns the short message name.
				 *
				 * @return Short message name.
				 */
				virtual const string getShortName() const;
		
				/**
				 * This method returns the long message name include package/sub structure.
				 *
				 * @return Long message name.
				 */
				virtual const string getLongName() const;
		
			public:
				virtual void accept(odcore::base::Visitor &v);
		
				virtual ostream& operator<<(ostream &out) const;
				virtual istream& operator>>(istream &in);
		
				virtual const string toString() const;
		
			private:
				double m_timestamp;
			private:
				double m_latitude;
			private:
				double m_longitude;
			private:
				float m_altitude;
			private:
				float m_northHeading;
			private:
				float m_speed;
			private:
				uint8_t m_latitudeDirection;
			private:
				uint8_t m_longitudeDirection;
			private:
				uint8_t m_satelliteCount;
			private:
				bool m_hasHeading;
			private:
				bool m_hasRtk;
		
		};
	} // proxy
} // opendlv

#endif /*OPENDLV_PROXY_GPSREADING_H*/
