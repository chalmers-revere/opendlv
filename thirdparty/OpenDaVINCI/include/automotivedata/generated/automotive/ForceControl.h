/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_FORCECONTROL_H
#define AUTOMOTIVE_FORCECONTROL_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	using namespace std;
	
	class ForceControl : public core::data::SerializableData, public core::base::Visitable {
		public:
			ForceControl();
	
			ForceControl(
			const double &val0, 
			const double &val1, 
			const double &val2, 
			const bool &val3, 
			const bool &val4, 
			const bool &val5
			);
	
			virtual ~ForceControl();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			ForceControl(const ForceControl &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			ForceControl& operator=(const ForceControl &obj);
	
		public:
			/**
			 * @return accelerationForce.
			 */
			double getAccelerationForce() const;
			
			/**
			 * This method sets accelerationForce.
			 *
			 * @param val Value for accelerationForce.
			 */
			void setAccelerationForce(const double &val);
		public:
			/**
			 * @return brakeForce.
			 */
			double getBrakeForce() const;
			
			/**
			 * This method sets brakeForce.
			 *
			 * @param val Value for brakeForce.
			 */
			void setBrakeForce(const double &val);
		public:
			/**
			 * @return steeringForce.
			 */
			double getSteeringForce() const;
			
			/**
			 * This method sets steeringForce.
			 *
			 * @param val Value for steeringForce.
			 */
			void setSteeringForce(const double &val);
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
			double m_accelerationForce;
		private:
			double m_brakeForce;
		private:
			double m_steeringForce;
		private:
			bool m_brakeLights;
		private:
			bool m_flashingLightsLeft;
		private:
			bool m_flashingLightsRight;
	
	};
} // automotive

#endif /*AUTOMOTIVE_FORCECONTROL_H*/
