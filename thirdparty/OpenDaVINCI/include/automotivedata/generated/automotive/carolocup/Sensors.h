/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_CAROLOCUP_SENSORS_H
#define AUTOMOTIVE_CAROLOCUP_SENSORS_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace automotive {
	namespace carolocup {
		using namespace std;
		
		class Sensors : public core::data::SerializableData, public core::base::Visitable {
			public:
				Sensors();
		
				Sensors(
				const uint32_t &val0, 
				const uint32_t &val1, 
				const uint32_t &val2, 
				const uint32_t &val3, 
				const uint32_t &val4, 
				const uint32_t &val5, 
				const uint32_t &val6, 
				const uint32_t &val7
				);
		
				virtual ~Sensors();
		
				/**
				 * Copy constructor.
				 *
				 * @param obj Reference to an object of this class.
				 */
				Sensors(const Sensors &obj);
		
				/**
				 * Assignment operator.
				 *
				 * @param obj Reference to an object of this class.
				 * @return Reference to this instance.
				 */
				Sensors& operator=(const Sensors &obj);
		
			public:
				/**
				 * @return usFront.
				 */
				uint32_t getUsFront() const;
				
				/**
				 * This method sets usFront.
				 *
				 * @param val Value for usFront.
				 */
				void setUsFront(const uint32_t &val);
			public:
				/**
				 * @return usRear.
				 */
				uint32_t getUsRear() const;
				
				/**
				 * This method sets usRear.
				 *
				 * @param val Value for usRear.
				 */
				void setUsRear(const uint32_t &val);
			public:
				/**
				 * @return irFrontRight.
				 */
				uint32_t getIrFrontRight() const;
				
				/**
				 * This method sets irFrontRight.
				 *
				 * @param val Value for irFrontRight.
				 */
				void setIrFrontRight(const uint32_t &val);
			public:
				/**
				 * @return irRearRight.
				 */
				uint32_t getIrRearRight() const;
				
				/**
				 * This method sets irRearRight.
				 *
				 * @param val Value for irRearRight.
				 */
				void setIrRearRight(const uint32_t &val);
			public:
				/**
				 * @return irBackLeft.
				 */
				uint32_t getIrBackLeft() const;
				
				/**
				 * This method sets irBackLeft.
				 *
				 * @param val Value for irBackLeft.
				 */
				void setIrBackLeft(const uint32_t &val);
			public:
				/**
				 * @return irBackRight.
				 */
				uint32_t getIrBackRight() const;
				
				/**
				 * This method sets irBackRight.
				 *
				 * @param val Value for irBackRight.
				 */
				void setIrBackRight(const uint32_t &val);
			public:
				/**
				 * @return wheelFrontLeft.
				 */
				uint32_t getWheelFrontLeft() const;
				
				/**
				 * This method sets wheelFrontLeft.
				 *
				 * @param val Value for wheelFrontLeft.
				 */
				void setWheelFrontLeft(const uint32_t &val);
			public:
				/**
				 * @return wheelRearRight.
				 */
				uint32_t getWheelRearRight() const;
				
				/**
				 * This method sets wheelRearRight.
				 *
				 * @param val Value for wheelRearRight.
				 */
				void setWheelRearRight(const uint32_t &val);
		
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
				uint32_t m_usFront;
			private:
				uint32_t m_usRear;
			private:
				uint32_t m_irFrontRight;
			private:
				uint32_t m_irRearRight;
			private:
				uint32_t m_irBackLeft;
			private:
				uint32_t m_irBackRight;
			private:
				uint32_t m_wheelFrontLeft;
			private:
				uint32_t m_wheelRearRight;
		
		};
	} // carolocup
} // automotive

#endif /*AUTOMOTIVE_CAROLOCUP_SENSORS_H*/
