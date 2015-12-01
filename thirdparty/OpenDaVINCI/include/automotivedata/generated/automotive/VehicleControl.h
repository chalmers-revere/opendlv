/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_VEHICLECONTROL_H
#define AUTOMOTIVE_VEHICLECONTROL_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	using namespace std;
	
	class VehicleControl : public core::data::SerializableData, public core::base::Visitable {
		public:
			VehicleControl();
	
			VehicleControl(
			const double &val0, 
			const double &val1, 
			const double &val2, 
			const bool &val3, 
			const bool &val4, 
			const bool &val5
			);
	
			virtual ~VehicleControl();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			VehicleControl(const VehicleControl &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			VehicleControl& operator=(const VehicleControl &obj);
	
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
			 * @return acceleration.
			 */
			double getAcceleration() const;
			
			/**
			 * This method sets acceleration.
			 *
			 * @param val Value for acceleration.
			 */
			void setAcceleration(const double &val);
		public:
			/**
			 * @return steeringWheelAngle.
			 */
			double getSteeringWheelAngle() const;
			
			/**
			 * This method sets steeringWheelAngle.
			 *
			 * @param val Value for steeringWheelAngle.
			 */
			void setSteeringWheelAngle(const double &val);
		public:
			/**
			 * @return brakeLights.
			 */
			bool getBrakeLights() const;
			
			/**
			 * This method sets brakeLights.
			 *
			 * @param val Value for brakeLights.
			 */
			void setBrakeLights(const bool &val);
		public:
			/**
			 * @return flashingLightsLeft.
			 */
			bool getFlashingLightsLeft() const;
			
			/**
			 * This method sets flashingLightsLeft.
			 *
			 * @param val Value for flashingLightsLeft.
			 */
			void setFlashingLightsLeft(const bool &val);
		public:
			/**
			 * @return flashingLightsRight.
			 */
			bool getFlashingLightsRight() const;
			
			/**
			 * This method sets flashingLightsRight.
			 *
			 * @param val Value for flashingLightsRight.
			 */
			void setFlashingLightsRight(const bool &val);
	
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
			double m_speed;
		private:
			double m_acceleration;
		private:
			double m_steeringWheelAngle;
		private:
			bool m_brakeLights;
		private:
			bool m_flashingLightsLeft;
		private:
			bool m_flashingLightsRight;
	
	};
} // automotive

#endif /*AUTOMOTIVE_VEHICLECONTROL_H*/
