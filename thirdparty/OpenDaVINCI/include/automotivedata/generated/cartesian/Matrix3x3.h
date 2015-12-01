/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef CARTESIAN_MATRIX3X3_H
#define CARTESIAN_MATRIX3X3_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace cartesian {
	using namespace std;
	
	class Matrix3x3 : public core::data::SerializableData, public core::base::Visitable {
		public:
			Matrix3x3();
	
			Matrix3x3(
			const double &val0, 
			const double &val1, 
			const double &val2, 
			const double &val3, 
			const double &val4, 
			const double &val5, 
			const double &val6, 
			const double &val7, 
			const double &val8
			);
	
			virtual ~Matrix3x3();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			Matrix3x3(const Matrix3x3 &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			Matrix3x3& operator=(const Matrix3x3 &obj);
	
		public:
			/**
			 * @return XX.
			 */
			double getXX() const;
			
			/**
			 * This method sets XX.
			 *
			 * @param val Value for XX.
			 */
			void setXX(const double &val);
		public:
			/**
			 * @return XY.
			 */
			double getXY() const;
			
			/**
			 * This method sets XY.
			 *
			 * @param val Value for XY.
			 */
			void setXY(const double &val);
		public:
			/**
			 * @return XZ.
			 */
			double getXZ() const;
			
			/**
			 * This method sets XZ.
			 *
			 * @param val Value for XZ.
			 */
			void setXZ(const double &val);
		public:
			/**
			 * @return YX.
			 */
			double getYX() const;
			
			/**
			 * This method sets YX.
			 *
			 * @param val Value for YX.
			 */
			void setYX(const double &val);
		public:
			/**
			 * @return YY.
			 */
			double getYY() const;
			
			/**
			 * This method sets YY.
			 *
			 * @param val Value for YY.
			 */
			void setYY(const double &val);
		public:
			/**
			 * @return YZ.
			 */
			double getYZ() const;
			
			/**
			 * This method sets YZ.
			 *
			 * @param val Value for YZ.
			 */
			void setYZ(const double &val);
		public:
			/**
			 * @return ZX.
			 */
			double getZX() const;
			
			/**
			 * This method sets ZX.
			 *
			 * @param val Value for ZX.
			 */
			void setZX(const double &val);
		public:
			/**
			 * @return ZY.
			 */
			double getZY() const;
			
			/**
			 * This method sets ZY.
			 *
			 * @param val Value for ZY.
			 */
			void setZY(const double &val);
		public:
			/**
			 * @return ZZ.
			 */
			double getZZ() const;
			
			/**
			 * This method sets ZZ.
			 *
			 * @param val Value for ZZ.
			 */
			void setZZ(const double &val);
	
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
			double m_XX;
		private:
			double m_XY;
		private:
			double m_XZ;
		private:
			double m_YX;
		private:
			double m_YY;
		private:
			double m_YZ;
		private:
			double m_ZX;
		private:
			double m_ZY;
		private:
			double m_ZZ;
	
	};
} // cartesian

#endif /*CARTESIAN_MATRIX3X3_H*/
