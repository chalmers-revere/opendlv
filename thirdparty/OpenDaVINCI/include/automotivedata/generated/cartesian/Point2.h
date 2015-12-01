/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef CARTESIAN_POINT2_H
#define CARTESIAN_POINT2_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace cartesian {
	using namespace std;
	
	class Point2 : public core::data::SerializableData, public core::base::Visitable {
		public:
			Point2();
	
			Point2(
			const float *val0
			);
	
			virtual ~Point2();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			Point2(const Point2 &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			Point2& operator=(const Point2 &obj);
	
		public:
			/**
			 * @return p.
			 */
			float* getP();
			
			/**
			 * This method gets the size of the fixed size array p.
			 *
			 * @return Size of the fixed size array p.
			 */
			uint32_t getSize_P() const;
	
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
			float* m_p;
	
	};
} // cartesian

#endif /*CARTESIAN_POINT2_H*/
