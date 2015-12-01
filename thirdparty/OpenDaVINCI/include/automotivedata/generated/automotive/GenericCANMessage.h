/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef AUTOMOTIVE_GENERICCANMESSAGE_H
#define AUTOMOTIVE_GENERICCANMESSAGE_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"

#include "core/data/TimeStamp.h"

namespace automotive {
	using namespace std;
	
	class GenericCANMessage : public core::data::SerializableData, public core::base::Visitable {
		public:
			GenericCANMessage();
	
			GenericCANMessage(
			const core::data::TimeStamp &val0, 
			const uint64_t &val1, 
			const uint8_t &val2, 
			const uint64_t &val3
			);
	
			virtual ~GenericCANMessage();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			GenericCANMessage(const GenericCANMessage &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			GenericCANMessage& operator=(const GenericCANMessage &obj);
	
		public:
			/**
			 * @return driverTimeStamp.
			 */
			core::data::TimeStamp getDriverTimeStamp() const;
			
			/**
			 * This method sets driverTimeStamp.
			 *
			 * @param val Value for driverTimeStamp.
			 */
			void setDriverTimeStamp(const core::data::TimeStamp &val);
		public:
			/**
			 * @return identifier.
			 */
			uint64_t getIdentifier() const;
			
			/**
			 * This method sets identifier.
			 *
			 * @param val Value for identifier.
			 */
			void setIdentifier(const uint64_t &val);
		public:
			/**
			 * @return length.
			 */
			uint8_t getLength() const;
			
			/**
			 * This method sets length.
			 *
			 * @param val Value for length.
			 */
			void setLength(const uint8_t &val);
		public:
			/**
			 * @return data.
			 */
			uint64_t getData() const;
			
			/**
			 * This method sets data.
			 *
			 * @param val Value for data.
			 */
			void setData(const uint64_t &val);
	
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
			core::data::TimeStamp m_driverTimeStamp;
		private:
			uint64_t m_identifier;
		private:
			uint8_t m_length;
		private:
			uint64_t m_data;
	
	};
} // automotive

#endif /*AUTOMOTIVE_GENERICCANMESSAGE_H*/
