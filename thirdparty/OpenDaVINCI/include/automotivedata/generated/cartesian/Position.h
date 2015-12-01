/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef CARTESIAN_POSITION_H
#define CARTESIAN_POSITION_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "generated/cartesian/Point2.h"
#include "generated/cartesian/Point2.h"

namespace cartesian {
	using namespace std;
	
	class Position : public core::data::SerializableData, public core::base::Visitable {
		public:
			Position();
	
			Position(
			const cartesian::Point2 &val0, 
			const cartesian::Point2 &val1
			);
	
			virtual ~Position();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			Position(const Position &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			Position& operator=(const Position &obj);
	
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
			 * @return rotation.
			 */
			cartesian::Point2 getRotation() const;
			
			/**
			 * This method sets rotation.
			 *
			 * @param val Value for rotation.
			 */
			void setRotation(const cartesian::Point2 &val);
	
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
			cartesian::Point2 m_rotation;
	
	};
} // cartesian

#endif /*CARTESIAN_POSITION_H*/
