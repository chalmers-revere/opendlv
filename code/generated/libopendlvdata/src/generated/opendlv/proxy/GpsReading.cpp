/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#include <memory>

#include "opendavinci/odcore/base/Hash.h"
#include "opendavinci/odcore/base/Deserializer.h"
#include "opendavinci/odcore/base/SerializationFactory.h"
#include "opendavinci/odcore/base/Serializer.h"


#include "opendlvdata/generated/opendlv/proxy/GpsReading.h"

namespace opendlv {
	namespace proxy {
			using namespace std;
			using namespace odcore::base;
		
		
			GpsReading::GpsReading() :
			    SerializableData(), Visitable()
				, m_timestamp(0.0)
				, m_latitude(0.0)
				, m_longitude(0.0)
				, m_altitude(0)
				, m_northHeading(0)
				, m_speed(0)
				, m_latitudeDirection(0)
				, m_longitudeDirection(0)
				, m_satelliteCount(0)
				, m_hasHeading(false)
				, m_hasRtk(false)
			{
			}
		
			GpsReading::GpsReading(
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
			) :
			    SerializableData(), Visitable()
				, m_timestamp(val0)
				, m_latitude(val1)
				, m_longitude(val2)
				, m_altitude(val3)
				, m_northHeading(val4)
				, m_speed(val5)
				, m_latitudeDirection(val6)
				, m_longitudeDirection(val7)
				, m_satelliteCount(val8)
				, m_hasHeading(val9)
				, m_hasRtk(val10)
			{
			}
		
			GpsReading::GpsReading(const GpsReading &obj) :
			    SerializableData(), Visitable()
				, m_timestamp(obj.m_timestamp)
				, m_latitude(obj.m_latitude)
				, m_longitude(obj.m_longitude)
				, m_altitude(obj.m_altitude)
				, m_northHeading(obj.m_northHeading)
				, m_speed(obj.m_speed)
				, m_latitudeDirection(obj.m_latitudeDirection)
				, m_longitudeDirection(obj.m_longitudeDirection)
				, m_satelliteCount(obj.m_satelliteCount)
				, m_hasHeading(obj.m_hasHeading)
				, m_hasRtk(obj.m_hasRtk)
			{
			}
			
			GpsReading::~GpsReading() {
			}
		
			GpsReading& GpsReading::operator=(const GpsReading &obj) {
				m_timestamp = obj.m_timestamp;
				m_latitude = obj.m_latitude;
				m_longitude = obj.m_longitude;
				m_altitude = obj.m_altitude;
				m_northHeading = obj.m_northHeading;
				m_speed = obj.m_speed;
				m_latitudeDirection = obj.m_latitudeDirection;
				m_longitudeDirection = obj.m_longitudeDirection;
				m_satelliteCount = obj.m_satelliteCount;
				m_hasHeading = obj.m_hasHeading;
				m_hasRtk = obj.m_hasRtk;
				return (*this);
			}
		
			int32_t GpsReading::ID() {
				return 161;
			}
		
			const string GpsReading::ShortName() {
				return "GpsReading";
			}
		
			const string GpsReading::LongName() {
				return "opendlv.proxy.GpsReading";
			}
		
			int32_t GpsReading::getID() const {
				return GpsReading::ID();
			}
		
			const string GpsReading::getShortName() const {
				return GpsReading::ShortName();
			}
		
			const string GpsReading::getLongName() const {
				return GpsReading::LongName();
			}
		
			double GpsReading::getTimestamp() const {
				return m_timestamp;
			}
			
			void GpsReading::setTimestamp(const double &val) {
				m_timestamp = val;
			}
			double GpsReading::getLatitude() const {
				return m_latitude;
			}
			
			void GpsReading::setLatitude(const double &val) {
				m_latitude = val;
			}
			double GpsReading::getLongitude() const {
				return m_longitude;
			}
			
			void GpsReading::setLongitude(const double &val) {
				m_longitude = val;
			}
			float GpsReading::getAltitude() const {
				return m_altitude;
			}
			
			void GpsReading::setAltitude(const float &val) {
				m_altitude = val;
			}
			float GpsReading::getNorthHeading() const {
				return m_northHeading;
			}
			
			void GpsReading::setNorthHeading(const float &val) {
				m_northHeading = val;
			}
			float GpsReading::getSpeed() const {
				return m_speed;
			}
			
			void GpsReading::setSpeed(const float &val) {
				m_speed = val;
			}
			uint8_t GpsReading::getLatitudeDirection() const {
				return m_latitudeDirection;
			}
			
			void GpsReading::setLatitudeDirection(const uint8_t &val) {
				m_latitudeDirection = val;
			}
			uint8_t GpsReading::getLongitudeDirection() const {
				return m_longitudeDirection;
			}
			
			void GpsReading::setLongitudeDirection(const uint8_t &val) {
				m_longitudeDirection = val;
			}
			uint8_t GpsReading::getSatelliteCount() const {
				return m_satelliteCount;
			}
			
			void GpsReading::setSatelliteCount(const uint8_t &val) {
				m_satelliteCount = val;
			}
			bool GpsReading::getHasHeading() const {
				return m_hasHeading;
			}
			
			void GpsReading::setHasHeading(const bool &val) {
				m_hasHeading = val;
			}
			bool GpsReading::getHasRtk() const {
				return m_hasRtk;
			}
			
			void GpsReading::setHasRtk(const bool &val) {
				m_hasRtk = val;
			}
		
			void GpsReading::accept(odcore::base::Visitor &v) {
				v.beginVisit();
				v.visit(CRC32 < CharList<'t', CharList<'i', CharList<'m', CharList<'e', CharList<'s', CharList<'t', CharList<'a', CharList<'m', CharList<'p', NullType> > > > > > > > >  >::RESULT, 1, "GpsReading.timestamp", "timestamp", m_timestamp);
				v.visit(CRC32 < CharList<'l', CharList<'a', CharList<'t', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', NullType> > > > > > > >  >::RESULT, 2, "GpsReading.latitude", "latitude", m_latitude);
				v.visit(CRC32 < CharList<'l', CharList<'o', CharList<'n', CharList<'g', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', NullType> > > > > > > > >  >::RESULT, 3, "GpsReading.longitude", "longitude", m_longitude);
				v.visit(CRC32 < CharList<'a', CharList<'l', CharList<'t', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', NullType> > > > > > > >  >::RESULT, 4, "GpsReading.altitude", "altitude", m_altitude);
				v.visit(CRC32 < CharList<'n', CharList<'o', CharList<'r', CharList<'t', CharList<'h', CharList<'H', CharList<'e', CharList<'a', CharList<'d', CharList<'i', CharList<'n', CharList<'g', NullType> > > > > > > > > > > >  >::RESULT, 5, "GpsReading.northHeading", "northHeading", m_northHeading);
				v.visit(CRC32 < CharList<'s', CharList<'p', CharList<'e', CharList<'e', CharList<'d', NullType> > > > >  >::RESULT, 6, "GpsReading.speed", "speed", m_speed);
				v.visit(CRC32 < CharList<'l', CharList<'a', CharList<'t', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', CharList<'D', CharList<'i', CharList<'r', CharList<'e', CharList<'c', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > > > > > > >  >::RESULT, 7, "GpsReading.latitudeDirection", "latitudeDirection", m_latitudeDirection);
				v.visit(CRC32 < CharList<'l', CharList<'o', CharList<'n', CharList<'g', CharList<'i', CharList<'t', CharList<'u', CharList<'d', CharList<'e', CharList<'D', CharList<'i', CharList<'r', CharList<'e', CharList<'c', CharList<'t', CharList<'i', CharList<'o', CharList<'n', NullType> > > > > > > > > > > > > > > > > >  >::RESULT, 8, "GpsReading.longitudeDirection", "longitudeDirection", m_longitudeDirection);
				v.visit(CRC32 < CharList<'s', CharList<'a', CharList<'t', CharList<'e', CharList<'l', CharList<'l', CharList<'i', CharList<'t', CharList<'e', CharList<'C', CharList<'o', CharList<'u', CharList<'n', CharList<'t', NullType> > > > > > > > > > > > > >  >::RESULT, 9, "GpsReading.satelliteCount", "satelliteCount", m_satelliteCount);
				v.visit(CRC32 < CharList<'h', CharList<'a', CharList<'s', CharList<'H', CharList<'e', CharList<'a', CharList<'d', CharList<'i', CharList<'n', CharList<'g', NullType> > > > > > > > > >  >::RESULT, 10, "GpsReading.hasHeading", "hasHeading", m_hasHeading);
				v.visit(CRC32 < CharList<'h', CharList<'a', CharList<'s', CharList<'R', CharList<'t', CharList<'k', NullType> > > > > >  >::RESULT, 11, "GpsReading.hasRtk", "hasRtk", m_hasRtk);
				v.endVisit();
			}
		
			const string GpsReading::toString() const {
				stringstream s;
		
		
				s << "Timestamp: " << getTimestamp() << " ";
				s << "Latitude: " << getLatitude() << " ";
				s << "Longitude: " << getLongitude() << " ";
				s << "Altitude: " << getAltitude() << " ";
				s << "NorthHeading: " << getNorthHeading() << " ";
				s << "Speed: " << getSpeed() << " ";
				s << "LatitudeDirection: " << getLatitudeDirection() << " ";
				s << "LongitudeDirection: " << getLongitudeDirection() << " ";
				s << "SatelliteCount: " << getSatelliteCount() << " ";
				s << "HasHeading: " << getHasHeading() << " ";
				s << "HasRtk: " << getHasRtk() << " ";
		
				return s.str();
			}
		
			ostream& GpsReading::operator<<(ostream &out) const {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Serializer> s = sf.getSerializer(out);
		
				s->write(1,
						m_timestamp);
				s->write(2,
						m_latitude);
				s->write(3,
						m_longitude);
				s->write(4,
						m_altitude);
				s->write(5,
						m_northHeading);
				s->write(6,
						m_speed);
				s->write(7,
						m_latitudeDirection);
				s->write(8,
						m_longitudeDirection);
				s->write(9,
						m_satelliteCount);
				s->write(10,
						m_hasHeading);
				s->write(11,
						m_hasRtk);
				return out;
			}
		
			istream& GpsReading::operator>>(istream &in) {
		
				SerializationFactory& sf = SerializationFactory::getInstance();
		
				std::shared_ptr<Deserializer> d = sf.getDeserializer(in);
		
				d->read(1,
						m_timestamp);
				d->read(2,
						m_latitude);
				d->read(3,
						m_longitude);
				d->read(4,
						m_altitude);
				d->read(5,
						m_northHeading);
				d->read(6,
						m_speed);
				d->read(7,
						m_latitudeDirection);
				d->read(8,
						m_longitudeDirection);
				d->read(9,
						m_satelliteCount);
				d->read(10,
						m_hasHeading);
				d->read(11,
						m_hasRtk);
				return in;
			}
	} // proxy
} // opendlv
