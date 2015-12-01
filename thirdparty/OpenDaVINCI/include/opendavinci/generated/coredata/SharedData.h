/*
 * This software is open source. Please see COPYING and AUTHORS for further information.
 *
 * This file is auto-generated. DO NOT CHANGE AS YOUR CHANGES MIGHT BE OVERWRITTEN!
 */

#ifndef COREDATA_SHAREDDATA_H
#define COREDATA_SHAREDDATA_H

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"


#include "core/base/Visitable.h"
#include "core/data/SerializableData.h"


namespace coredata {
	using namespace std;
	
	class SharedData : public core::data::SerializableData, public core::base::Visitable {
		public:
			SharedData();
	
			SharedData(
			const std::string &val0, 
			const uint32_t &val1
			);
	
			virtual ~SharedData();
	
			/**
			 * Copy constructor.
			 *
			 * @param obj Reference to an object of this class.
			 */
			SharedData(const SharedData &obj);
	
			/**
			 * Assignment operator.
			 *
			 * @param obj Reference to an object of this class.
			 * @return Reference to this instance.
			 */
			SharedData& operator=(const SharedData &obj);
	
		public:
			/**
			 * @return name.
			 */
			std::string getName() const;
			
			/**
			 * This method sets name.
			 *
			 * @param val Value for name.
			 */
			void setName(const std::string &val);
		public:
			/**
			 * @return size.
			 */
			uint32_t getSize() const;
			
			/**
			 * This method sets size.
			 *
			 * @param val Value for size.
			 */
			void setSize(const uint32_t &val);
	
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
			std::string m_name;
		private:
			uint32_t m_size;
	
	};
} // coredata

#endif /*COREDATA_SHAREDDATA_H*/
