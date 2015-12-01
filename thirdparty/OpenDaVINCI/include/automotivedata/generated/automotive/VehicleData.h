/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_VEHICLEDATA_H
#define AUTOMOTIVE_VEHICLEDATA_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "generated/cartesian/Point2.h"
#include "generated/cartesian/Point2.h"

namespace automotive {
	using namespace std;
	
	class VehicleData : public core::data::SerializableData, public core::base::Visitable {
		public:
			VehicleData();
	
			VehicleData(
			const cartesian::Point2 &val0, 
			const cartesian::Point2 &val1, 
			const double &val2, 
			const double &val3, 
			const double &val4, 
			const double &val5, 
			const double &val6, 
			const double &val7, 
			const double &val8
			);
	
			virtual ~VehicleData();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			VehicleData(const VehicleData &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			VehicleData& operator=(const VehicleData &obj);
	
		public:
			/**
			 * @return position.
			 */
			cartesian::Point2 getPosition() const;
			
			/**
			 * This method sets position.
			 *
			 * @param val Value for position.
			 */
			void setPosition(const cartesian::Point2 &val);
		public:
			/**
			 * @return velocity.
			 */
			cartesian::Point2 getVelocity() const;
			
			/**
			 * This method sets velocity.
			 *
			 * @param val Value for velocity.
			 */
			void setVelocity(const cartesian::Point2 &val);
		public:
			/**
			 * @return heading.
			 */
			double getHeading() const;
			
			/**
			 * This method sets heading.
			 *
			 * @param val Value for heading.
			 */
			void setHeading(const double &val);
		public:
			/**
			 * @return absTraveledPath.
			 */
			double getAbsTraveledPath() const;
			
			/**
			 * This method sets absTraveledPath.
			 *
			 * @param val Value for absTraveledPath.
			 */
			void setAbsTraveledPath(const double &val);
		public:
			/**
			 * @return relTraveledPath.
			 */
			double getRelTraveledPath() const;
			
			/**
			 * This method sets relTraveledPath.
			 *
			 * @param val Value for relTraveledPath.
			 */
			void setRelTraveledPath(const double &val);
		public:
			/**
			 * @return speed.
			 */
			double getSpeed() const;
			
			/**
			 * This method sets speed.
			 *
			 * @param val Value for speed.
			 */
			void setSpeed(const double &val);
		public:
			/**
			 * @return v_log.
			 */
			double getV_log() const;
			
			/**
			 * This method sets v_log.
			 *
			 * @param val Value for v_log.
			 */
			void setV_log(const double &val);
		public:
			/**
			 * @return v_batt.
			 */
			double getV_batt() const;
			
			/**
			 * This method sets v_batt.
			 *
			 * @param val Value for v_batt.
			 */
			void setV_batt(const double &val);
		public:
			/**
			 * @return temp.
			 */
			double getTemp() const;
			
			/**
			 * This method sets temp.
			 *
			 * @param val Value for temp.
			 */
			void setTemp(const double &val);
	
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
	
		public:
			virtual void accept(core::base::Visitor &v);
	
			virtual ostream& operator<<(ostream &out) const;
			virtual istream& operator>>(istream &in);
	
			virtual const string toString() const;
	
		private:
			cartesian::Point2 m_position;
		private:
			cartesian::Point2 m_velocity;
		private:
			double m_heading;
		private:
			double m_absTraveledPath;
		private:
			double m_relTraveledPath;
		private:
			double m_speed;
		private:
			double m_v_log;
		private:
			double m_v_batt;
		private:
			double m_temp;
	
	};
} // automotive

#endif /*AUTOMOTIVE_VEHICLEDATA_H*/
